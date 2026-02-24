#include <sstream>
#include <thread>
#include <concurrentqueue.h>
#include <nlohmann/json.hpp>
#include <unnu_tts/cxx-api.h>
#include <locale.h>
#include <cctype> // for std::tolower
#include <algorithm>
#include "unnu_fm/cxx-api.h"

static moodycamel::ConcurrentQueue<unnufm::Storyboard> scenequeue;

static moodycamel::ConcurrentQueue<unnufm::skit_t> skits;


void unnufm::Storyboard::addScene(const scene_t& scene) {
	scenes.push_back(scene);
}
unnufm::scene_t& unnufm::Storyboard::addSceneReturnRef(const scene_t& scene) {
	scenes.push_back(scene);
	return scenes.back();
}

std::string unnufm::Storyboard::exportToEDL() const {
	std::ostringstream out;
	
	out << "TITLE: Storyboard Export\nFCM: NON-DROP FRAME\n";

	int eventNum = 1;
	double currentTime = 0.0;

	for (const auto& scene : scenes) {
		for (const auto& cue : scene.cues) {
			double cueStart = currentTime;
			double cueDuration = 0.0;

			for (const auto& line : cue.lines) {
				double lineStart = cueStart + line.delay;
				double lineEnd = lineStart + 2.0 + line.pause; // assume 2s per line + pause
				cueDuration = std::max(cueDuration, lineEnd - cueStart);

				out << eventNum++ << "  AX       V     C        "
					<< "00:00:" << int(lineStart) << ":00 "
					<< "00:00:" << int(lineEnd) << ":00 "
					<< "00:00:" << int(lineStart) << ":00 "
					<< "00:00:" << int(lineEnd) << ":00\n";
				out << "* FROM CLIP NAME: " << scene.sceneID << "_" << line.speaker << "\n";
			}

			currentTime += cueDuration;
		}
	}
	return out.str();
}

std::string unnufm::Storyboard::exportToPremiereXML(float fps) const {
	std::ostringstream out;

	out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	out << "<xmeml version=\"4\">\n<sequence>\n<name>Storyboard Export</name>\n<media>\n<video>\n";

	int trackIndex = 1;
	double currentTime = 0.0;

	for (const auto& scene : scenes) {
		for (const auto& cue : scene.cues) {
			double cueStart = currentTime;
			double cueDuration = 0.0;

			for (const auto& line : cue.lines) {
				double lineStart = cueStart + line.delay;
				double lineEnd = lineStart + 2.0 + line.pause;
				cueDuration = std::max(cueDuration, lineEnd - cueStart);

				// Video placeholder
				out << "<track>\n";
				out << "<clipitem id=\"" << scene.sceneID << "_" << line.speaker << "\">\n";
				out << "<name>" << line.speaker << "</name>\n";
				out << "<start>" << int(lineStart * fps) << "</start>\n"; // 25fps
				out << "<end>" << int(lineEnd * fps) << "</end>\n";
				out << "<in>0</in>\n<out>" << int((lineEnd - lineStart) * fps) << "</out>\n";
				out << "</clipitem>\n</track>\n";
			}

			currentTime += cueDuration;
			trackIndex++;
		}
	}

	out << "</video>\n</media>\n</sequence>\n</xmeml>\n";
	return out.str();
}

std::string unnufm::Storyboard::formatSRTTime(double seconds) {
	int hours = int(seconds / 3600);
	seconds -= hours * 3600;
	int minutes = int(seconds / 60);
	seconds -= minutes * 60;
	int secs = int(seconds);
	int millis = int((seconds - secs) * 1000);

	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(2) << hours << ":"
		<< std::setw(2) << minutes << ":"
		<< std::setw(2) << secs << ","
		<< std::setw(3) << millis;
	return oss.str();
}

std::string unnufm::Storyboard::exportToSRT() const {
	std::ostringstream out;

	int subtitleIndex = 1;
	double currentTime = 0.0;

	for (const auto& scene : scenes) {
		for (const auto& cue : scene.cues) {
			double cueStart = currentTime;
			double cueDuration = 0.0;

			if (cue.timing == unnu_scene_timing::TIMING_SIMULTANEOUS) {
				// All lines start together
				double start = cueStart;
				double end = start;
				std::ostringstream text;
				for (const auto& l : cue.lines) {
					text << l.speaker << ": " << l.text;
					text << "\n";
					double lineEnd = start + 2.0 + l.pause;
					if (lineEnd > end) end = lineEnd;
				}
				out << subtitleIndex++ << "\n"
					<< formatSRTTime(start) << " --> " << formatSRTTime(end) << "\n"
					<< text.str() << "\n";
				cueDuration = end - cueStart;
			}
			else {
				// Overlap or sequential
				for (const auto& l : cue.lines) {
					double start = cueStart + l.delay;
					double end = start + 2.0 + l.pause;
					out << subtitleIndex++ << "\n"
						<< formatSRTTime(start) << " --> " << formatSRTTime(end) << "\n"
						<< l.speaker << ": " << l.text;
					out << "\n\n";
					if (end - cueStart > cueDuration) cueDuration = end - cueStart;
				}
			}

			currentTime += cueDuration;
		}
	}
	return out.str();
}

unnufm::Storyboard unnufm::ScriptParser::toStoryboard(const nlohmann::json& j) {
	Storyboard sb;

	for (auto& sceneJson : j["scenes"]) {
		scene_t scene;
		scene.sceneID = sceneJson["sceneID"].get<std::string>();
		if (sceneJson.contains("location")) scene.location = sceneJson["location"].get<std::string>();
		if (sceneJson.contains("stage")) scene.stage = sceneJson["stage"].get<std::string>();
		if (sceneJson.contains("jump")) scene.jump = sceneJson["jump"].get<std::string>();

		for (auto& cueJson : sceneJson["cues"]) {
			cue_t cue;
			cue.timing = unnufm::unnu_scene_timing_from_string(cueJson["timing"].get<std::string>());
			if (cueJson.contains("stagger")) cue.stagger = cueJson["stagger"].get<double>();
			if (cueJson.contains("dynamic")) cue.dynamic = cueJson["dynamic"].get<bool>();

			if (cueJson.contains("lines")) {
				for (auto& lineJson : cueJson["lines"]) {
					cue_line_t line;
					line.speaker = lineJson["speaker"].get<std::string>();
					if (lineJson.contains("text")) line.text = lineJson["text"].get<std::string>();
					if (lineJson.contains("pause")) line.pause = lineJson["pause"].get<double>();
					if (lineJson.contains("delay")) line.delay = lineJson["delay"].get<double>();
					if (lineJson.contains("emotion")) line.emotion = unnutts::emotion_from_string(lineJson["emotion"].get<std::string>());
					if (lineJson.contains("lip_sync")) line.lipSync = lineJson["lip_sync"].get<bool>();
					if (lineJson.contains("actions")) {
						for (auto& actionJson : lineJson["actions"]) {
							action_t action;
							action.gesture = actionJson["gesture"].get<std::string>();
							if (actionJson.contains("persistent")) action.persistent = actionJson["persistent"].get<bool>();
							if (actionJson.contains("delay")) action.delay = actionJson["delay"].get<double>();
							line.actions.push_back(action);
						}
					}
					cue.lines.push_back(line);
				}
			}
			
			if (cueJson.contains("sounds")) {
				for (auto& soundJson : cueJson["sounds"]) {
					sound_cue_t soundCue;
					soundCue.uri = soundJson["uri"].get<std::string>();
					if (soundJson.contains("type")) soundCue.type = sound_cue_type_from_string(soundJson["type"].get<std::string>());
					if (soundJson.contains("time")) soundCue.time = soundJson["time"].get<double>();
					cue.sounds.push_back(soundCue);
				}
			}
			if (cueJson.contains("shots")) {
				for (auto& cameraJson : cueJson["shots"]) {
					camera_cue_t cameraCue;
					
					if (cameraJson.contains("size")) cameraCue.size = camera_shot_size_from_string(cameraJson["size"].get<std::string>());
					if (cameraJson.contains("angle")) cameraCue.angle = camera_shot_angle_from_string(cameraJson["angle"].get<std::string>());
					if (cameraJson.contains("movement")) cameraCue.movement = camera_shot_movement_from_string(cameraJson["movement"].get<std::string>());
					if (cameraJson.contains("target")) cameraCue.target = cameraJson["target"].get<std::string>();
					if (cameraJson.contains("duration")) cameraCue.duration = cameraJson["duration"].get<double>();
					if (cameraJson.contains("transition")) cameraCue.transition = cameraJson["transition"].get<double>();
					cue.shots.push_back(cameraCue);
				}
			}
			scene.cues.push_back(cue);
		}

		sb.scenes.push_back(scene);
	}
	return sb;
}

std::set<std::string> unnufm::ScriptParser::getCreditedActorsInStoryboard(const Storyboard& storyboard) {
	std::set<std::string> actors;
	for (const scene_t& scene : storyboard.scenes) {
		for (const cue_t& cue : scene.cues) {
			for (const cue_line_t& line : cue.lines) {
				if (!line.speaker.empty() && !line.text.empty()) actors.insert(line.speaker);
			}
		}
	}
	return actors;
}

std::set<std::string> unnufm::ScriptParser::getActorsInSkit(const skit_t& skit)
{
	std::set<std::string> actors;
	for (const take_t& take : skit.takes) {
		for (const performance_t& performance : take.performances) {
			if (!performance.line.speaker.empty()) actors.insert(performance.line.speaker);
		}
	}
	return actors;
}

std::set<std::string>  unnufm::ScriptParser::getCreditedActorsInTake(const take_t& take)
{
	std::set<std::string> actors;
	for (const performance_t& performance : take.performances)
	{
			if (!performance.line.speaker.empty() && !performance.audio.chunk.samples.empty()) actors.insert(performance.line.speaker);
	}
	return actors;
}

std::set<std::string> unnufm::ScriptParser::getCreditedActorsInScene(const scene_t& scene) {
	std::set<std::string> actors;
	for (const cue_t& cue : scene.cues) {
		for (const cue_line_t& line : cue.lines) {
			if (!line.speaker.empty() && !line.text.empty()) actors.insert(line.speaker);
		}
	}
	return actors;
}

unnufm::audio_t unnufm::PiperTTS::speak(const cue_line_t& line) {
	audio_t audio;
	audio.text = line.text;
	auto& speaker = getVoice(line.speaker);
	if (speaker.first >= 0) {
		if (!line.text.empty()) {
			ut_audio_sample_t* result = unnu_tts(speaker.first, line.emotion, speaker.second, line.text.c_str());
			if (result->num_samples > 0) {
				audio.chunk.sample_rate = result->sample_rate;
				audio.chunk.samples = std::vector<float>(result->samples, result->samples + result->num_samples);
			}
			ut_audio_sample_free(result);
		}
	}
	return audio;
}

void unnufm::PiperTTS::setVoice(const std::string& speaker, int32_t speaker_id, bool is_robot) {
	voices[speaker] = std::make_pair(speaker_id, is_robot);
}

std::pair<int32_t, bool> unnufm::PiperTTS::getVoice(const std::string& speaker) const {
	auto it = voices.find(speaker);
	if (it != voices.end()) return it->second;
	return std::make_pair(-1, false);
}

bool unnufm::SkitGrabber::grab(skit_t& skit) {
	return skits.try_dequeue(skit);
}



unnufm::ProductionWrangler::ProductionWrangler(PiperTTS& actors, const std::string& script) : tts(actors) {
	load(script);
}

void unnufm::ProductionWrangler::load(const std::string& script) {
	nlohmann::json j = nlohmann::json::parse(script);

	storyboard = ScriptParser::toStoryboard(j);
	for (const scene_t& scene : storyboard.scenes) {
		scenes[scene.sceneID] = scene;
	}
}

std::string unnufm::ProductionWrangler::play(std::string sceneId) {
	auto it = scenes.find(sceneId);
	if (it == scenes.end()) {
		return "";
	}
	scene_t& scene = it->second;
	perform(scene);
	return scene.jump;
}

unnufm::performance_t unnufm::ProductionWrangler::toPerformance(const cue_line_t& line) {
	performance_t placeholder;
	placeholder.line = line;
	placeholder.audio = tts.speak(line);
	return placeholder;
}
		
unnufm::take_t unnufm::ProductionWrangler::toTake(const cue_t& cue) {
	take_t take;
	take.timing = cue.timing;
	take.stagger = cue.stagger;
	take.dynamic = cue.dynamic;
	take.sounds = cue.sounds;
	take.shots = cue.shots;
	for(const cue_line_t& line :  cue.lines) {
		performance_t p = toPerformance(line);
		take.performances.push_back(std::move(p));
	}
	return take;
}

unnufm::skit_t unnufm::ProductionWrangler::toSkit(const scene_t& scene) {
	skit_t skit;
	skit.sceneID = scene.sceneID;
	skit.location = scene.location;
	skit.stage = scene.stage;
	for(const cue_t& cue : scene.cues) {
		take_t t = toTake(cue);
		skit.takes.push_back(std::move(t));
	}
	return skit;
}

void unnufm::ProductionWrangler::perform(const scene_t& scene) {
	skit_t s = toSkit(scene);
	SendSkitToUnrealEngine(s);
}

void unnufm::ProductionWrangler::SendSkitToUnrealEngine(const skit_t& skit) {
	skits.enqueue(skit);
}

void unnufm::SceneManager::LoadStoryboard(const std::string& script) {
	wrangler.load(script);
}

void unnufm::SceneManager::run(const std::string& startScene) {
	std::string currentScene = startScene;

	while (!currentScene.empty()) {
		// go to next scene
		currentScene = wrangler.play(currentScene);
	}
}

bool starts_with(const std::string& str, const std::string& prefix) {
    return str.size() >= prefix.size() &&
           str.compare(0, prefix.size(), prefix) == 0;
}

// Case-insensitive starts_with
bool starts_with_ci(const std::string& str, const std::string& prefix) {
    if (prefix.size() > str.size()) return false;

    for (size_t i = 0; i < prefix.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(str[i])) !=
            std::tolower(static_cast<unsigned char>(prefix[i]))) {
            return false;
        }
    }
    return true;
}

// Returns true if strings are equal ignoring case
bool equals_ignore_case(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;

    for (size_t i = 0; i < a.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i]))) {
            return false;
        }
    }
    return true;
}

std::string to_lower_case(const std::string& str) {
	std::string result = str;
	std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
	return std::tolower(c);
	});
	return result;
}

static std::string g_unnu_fm_male_string("male");
static std::string g_unnu_fm_female_string("female");

void unnu_fm_process_script(const char* script) {
	std::string script_str(script);
	
	nlohmann::json j =  nlohmann::json::parse(script_str);
	
	auto& story = unnufm::ScriptParser::toStoryboard(j);

	auto& speakers = unnufm::ScriptParser::getCreditedActorsInStoryboard(story);

	unnufm::PiperTTS tts;
	for(const auto& speaker : speakers) {
		if (speaker.empty()) continue;
		std::string lookup = to_lower_case(speaker);
		int id = ut_get_speaker_id(lookup.c_str());
		if(id < 0){
			const char *locale_str = setlocale(LC_ALL, "");
			int genderCode = 0;
			if (lookup == "ava" || lookup == "quinn") {
				genderCode = 1; // ensure speaker is loaded in TTS
			} else if (lookup == "tar" || lookup == "manny") {
				genderCode = 2; // ensure speaker is loaded in TTS
			}
			if (locale_str) 
			{
				std::string langcode(locale_str, 2);
				std::string locale_code = langcode + "-" + std::string(locale_str + 3,2);
				if(genderCode == 2 || starts_with_ci(speaker, g_unnu_fm_male_string))
				{
					std::string maleVoce = std::string("male.") + locale_code;
					id = ut_get_speaker_id(maleVoce.c_str());
					if(id < 0)
					{
						maleVoce = g_unnu_fm_male_string + "." + langcode;
						id = ut_get_speaker_id(maleVoce.c_str());
						if(id < 0)
						{
							id = ut_get_speaker_id(g_unnu_fm_male_string.c_str());
						}
						
					}
				} else if(genderCode == 1 || starts_with_ci(speaker, g_unnu_fm_female_string)){
					std::string femaleVoce = std::string("female.") + locale_code;
					id = ut_get_speaker_id(femaleVoce.c_str());
					if(id < 0)
					{
						femaleVoce = g_unnu_fm_female_string + "." + langcode;
						id = ut_get_speaker_id(femaleVoce.c_str());
						if(id < 0)
						{
							id = ut_get_speaker_id(g_unnu_fm_female_string.c_str());
						}
						
					}
				}
			}
		}
		
		

		if (id >= 0) {
			// For demonstration, assign robot voices to Manny and Quinn
			if (lookup == "manny" || lookup == "quinn") {
				tts.setVoice(speaker, id, true);
			}
			else {
				tts.setVoice(speaker, id, false);
			}
		}
	}
	unnufm::ProductionWrangler wrangler(tts, story);
	for(const unnufm::scene_t& scene : story.scenes) {
		wrangler.perform(scene);
	}
	
	std::thread th([&wrangler](){
		unnufm::SceneManager sm(wrangler);
		sm.run(wrangler.getStoryboard().firstSceneID());
	});
	
	th.detach();
}


