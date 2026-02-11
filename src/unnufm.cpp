#include <sstream>
#include <thread>
#include <concurrentqueue.h>
#include <nlohmann/json.hpp>
#include <unnu_tts/cxx-api.h>
#include "unnu_fm/cxx-api.h"

static moodycamel::ConcurrentQueue<unnufm::Storyboard> scenequeue;

static moodycamel::ConcurrentQueue<unnufm::Skit> skits;


void unnufm::Storyboard::addScene(const Scene& scene) {
	scenes.push_back(scene);
}
unnufm::Scene& unnufm::Storyboard::addSceneReturnRef(const Scene& scene) {
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

			if (cue.timing == "simultaneous") {
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
		Scene scene;
		scene.sceneID = sceneJson["sceneID"].get<std::string>();
		if (sceneJson.contains("jump")) scene.jump = sceneJson["jump"].get<std::string>();

		for (auto& cueJson : sceneJson["cues"]) {
			Cue cue;
			cue.timing = cueJson["timing"].get<std::string>();
			if (cueJson.contains("stagger")) cue.stagger = cueJson["stagger"].get<double>();
			if (cueJson.contains("dynamic")) cue.dynamic = cueJson["dynamic"].get<bool>();

			if (cueJson.contains("lines")) {
				for (auto& lineJson : cueJson["lines"]) {
					CueLine line;
					line.speaker = lineJson["speaker"].get<std::string>();
					if (lineJson.contains("text")) line.text = lineJson["text"].get<std::string>();
					if (lineJson.contains("pause")) line.pause = lineJson["pause"].get<double>();
					if (lineJson.contains("delay")) line.delay = lineJson["delay"].get<double>();
					if (lineJson.contains("emotion")) line.emotion = unnutts::emotion_from_string(lineJson["emotion"].get<std::string>());
					if (lineJson.contains("lip_sync")) line.lipSync = lineJson["lip_sync"].get<bool>();
					if (lineJson.contains("actions")) {
						for (auto& actionJson : lineJson["actions"]) {
							Action action;
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
					SoundCue soundCue;
					soundCue.uri = soundJson["uri"].get<std::string>();
					if (soundJson.contains("type")) soundCue.type = sound_cue_type_from_string(soundJson["type"].get<std::string>());
					if (soundJson.contains("time")) soundCue.time = soundJson["time"].get<double>();
					cue.sounds.push_back(soundCue);
				}
			}
			if (cueJson.contains("shots")) {
				for (auto& cameraJson : cueJson["shots"]) {
					CameraCue cameraCue;
					
					if (cameraJson.contains("location")) cameraCue.location = cameraJson["location"].get<std::string>();
					if (cameraJson.contains("size")) cameraCue.size = camera_shot_size_from_string(cameraJson["size"].get<std::string>());
					if (cameraJson.contains("angle")) cameraCue.angle = camera_shot_angle_from_string(cameraJson["angle"].get<std::string>());
					if (cameraJson.contains("movement")) cameraCue.movement = camera_shot_movement_from_string(cameraJson["movement"].get<std::string>());
					if (cameraJson.contains("target")) cameraCue.target = cameraJson["target"].get<std::string>();
					if (cameraJson.contains("duration")) cameraCue.duration = cameraJson["duration"].get<double>();
					if (cameraJson.contains("time")) cameraCue.time = cameraJson["time"].get<double>();
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
	for (const Scene& scene : storyboard.scenes) {
		for (const Cue& cue : scene.cues) {
			for (const CueLine& line : cue.lines) {
				if (!line.speaker.empty() && !line.text.empty()) actors.insert(line.speaker);
			}
		}
	}
	return actors;
}

std::set<std::string> unnufm::ScriptParser::getCreditedActorsInScene(const Scene& scene) {
	std::set<std::string> actors;
	for (const Cue& cue : scene.cues) {
		for (const CueLine& line : cue.lines) {
			if (!line.speaker.empty() && !line.text.empty()) actors.insert(line.speaker);
		}
	}
	return actors;
}

unnufm::Performance unnufm::ProductionWrangler::toPerformance(const CueLine& line) {
	Performance placeholder;
	placeholder.line = line;
	placeholder.audio = tts.speak(line);
	return placeholder;
}
		
unnufm::Take unnufm::ProductionWrangler::toTake(const Cue& cue) {
	Take take;
	take.timing = cue.timing;
	take.stagger = cue.stagger;
	take.dynamic = cue.dynamic;
	take.sounds = cue.sounds;
	take.shots = cue.shots;
	for(const CueLine& line :  cue.lines) {
		Performance p = toPerformance(line);
		take.performances.push_back(std::move(p));
	}
	return take;
}

unnufm::Skit unnufm::ProductionWrangler::toSkit(const Scene& scene) {
	Skit skit;
	skit.sceneID = scene.sceneID;
	for(const Cue& cue : scene.cues) {
		Take t = toTake(cue);
		skit.takes.push_back(std::move(t));
	}
	return skit;
}

void unnufm::ProductionWrangler::perform(const Scene& scene) {
	Skit s = toSkit(scene);
	SendSkitToUnrealEngine(s);
}

void unnufm::ProductionWrangler::SendSkitToUnrealEngine(const Skit& skit) {
	skits.enqueue(skit);
}

void unnu_fm_process_script(const char* script) {
	std::string script_str(script);
	
	nlohmann::json j =  nlohmann::json::parse(script_str);
	
	auto& story = unnufm::ScriptParser::toStoryboard(j);

	auto& speakers = unnufm::ScriptParser::getCreditedActorsInStoryboard(story);

	unnufm::PiperTTS tts;
	for(const auto& speaker : speakers) {
		if (speaker.empty()) continue;
		int id = -1;
		if (speaker == "Ava" || speaker == "Quinn") {
			id = ut_get_speaker_id("female"); // ensure speaker is loaded in TTS
		} else if (speaker == "Tar" || speaker == "Manny") {
			id = ut_get_speaker_id("male"); // ensure speaker is loaded in TTS
		}
		if (id < 0) {
			id = ut_get_speaker_id(speaker.c_str());
		}

		if (id >= 0) {
			// For demonstration, assign robot voices to Manny and Quinn
			if (speaker == "Manny" || speaker == "Quinn") {
				tts.setVoice(speaker, id, true);
			}
			else {
				tts.setVoice(speaker, id, false);
			}
		}
	}
	unnufm::ProductionWrangler wrangler(tts, story);
	for(const unnufm::Scene& scene : story.scenes) {
		wrangler.perform(scene);
	}
	
	std::thread th([&wrangler](){
		unnufm::SceneManager sm(wrangler);
		sm.run(wrangler.getStoryboard().firstSceneID());
	});
	
	th.detach();
}

bool unnufm::SkitGrabber::grab(Skit& skit) {
	return skits.try_dequeue(skit);
}
