#include <sstream>
#include <thread>
#include <concurrentqueue.h>
#include <nlohmann/json.hpp>
#include <unnu_tts/cxx-api.h>
#include <locale.h>
#include <cctype> // for std::tolower
#include <algorithm>
#include "unnu_fm/cxx-api.h"

static moodycamel::ConcurrentQueue<unnufm_storyboard_t> stories;

static moodycamel::ConcurrentQueue<unnufm_skit_t> skits;

unnufm::Storyboard::Storyboard(const char* script) {
	parse(script);
}

unnufm::Storyboard::Storyboard(unnufm_storyboard_t* story) {
	storyboard.reset(story);
}


void unnufm::Storyboard::parse(const char* script) {
	unnufm_storyboard_t* story = unnufm::ScriptParser::toStoryboard(script);
	storyboard.reset(story);
}

std::string unnufm::Storyboard::exportToEDL() const {
	std::ostringstream out;
	
	out << "TITLE: Storyboard Export\nFCM: NON-DROP FRAME\n";

	int eventNum = 1;
	double currentTime = 0.0;

	if (storyboard->n_scenes > 0 && storyboard->scenes)
		for (int i = 0; i < storyboard->n_scenes; i++) {
		unnufm_scene_t* scene = storyboard->scenes[i];
		if (scene->n_cues > 0 && scene->cues) 
		for (int i = 0; i < scene->n_cues; i++) {
			unnufm_cue_t* cue = scene->cues[i];
			double cueStart = currentTime;
			double cueDuration = 0.0;
			if (cue->n_lines > 0 && cue->lines)
				for (int j = 0; j < cue->n_lines; j++) {
				unnufm_cue_line_t* line = cue->lines[j];
				double lineStart = cueStart + line->delay;
				double lineEnd = lineStart + 2.0 + line->pause; // assume 2s per line + pause
				cueDuration = std::max(cueDuration, lineEnd - cueStart);

				out << eventNum++ << "  AX       V     C        "
					<< "00:00:" << int(lineStart) << ":00 "
					<< "00:00:" << int(lineEnd) << ":00 "
					<< "00:00:" << int(lineStart) << ":00 "
					<< "00:00:" << int(lineEnd) << ":00\n";
				out << "* FROM CLIP NAME: " << scene->sceneID << "_" << line->speaker << "\n";
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
	if (storyboard->n_scenes > 0 && storyboard->scenes)
		for (int i = 0; i < storyboard->n_scenes; i++) {
		unnufm_scene_t* scene = storyboard->scenes[i];
		if (scene->n_cues > 0 && scene->cues)
			for (int i = 0; i < scene->n_cues; i++) {
			unnufm_cue_t* cue = scene->cues[i];
			double cueStart = currentTime;
			double cueDuration = 0.0;

			if (cue->n_lines > 0 && cue->lines)
				for (int j = 0; j < cue->n_lines; j++) {
				unnufm_cue_line_t* line = cue->lines[j];
				double lineStart = cueStart + line->delay;
				double lineEnd = lineStart + 2.0 + line->pause;
				cueDuration = std::max(cueDuration, lineEnd - cueStart);

				// Video placeholder
				out << "<track>\n";
				out << "<clipitem id=\"" << scene->sceneID << "_" << line->speaker << "\">\n";
				out << "<name>" << line->speaker << "</name>\n";
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
	if (storyboard->n_scenes > 0 && storyboard->scenes)
		for (int i = 0; i < storyboard->n_scenes; i++) {
			unnufm_scene_t* scene = storyboard->scenes[i];
		if (scene->n_cues > 0 && scene->cues)
			for (int i = 0; i < scene->n_cues; i++) {
				unnufm_cue_t* cue = scene->cues[i];
			double cueStart = currentTime;
			double cueDuration = 0.0;

			if (cue->timing == unnufm_scene_timing::TIMING_SIMULTANEOUS) {
				// All lines start together
				double start = cueStart;
				double end = start;
				std::ostringstream text;
				if (cue->n_lines > 0 && cue->lines)
					for (int j = 0; j < cue->n_lines; j++) {
					unnufm_cue_line_t* l = cue->lines[j];
					text << l->speaker << ": " << l->text;
					text << "\n";
					double lineEnd = start + 2.0 + l->pause;
					if (lineEnd > end) end = lineEnd;
				}
				out << subtitleIndex++ << "\n"
					<< formatSRTTime(start) << " --> " << formatSRTTime(end) << "\n"
					<< text.str() << "\n";
				cueDuration = end - cueStart;
			}
			else {
				// Overlap or sequential
				if (cue->n_lines > 0 && cue->lines)
					for (int j = 0; j < cue->n_lines; j++) {
					unnufm_cue_line_t* l = cue->lines[j];
					double start = cueStart + l->delay;
					double end = start + 2.0 + l->pause;
					out << subtitleIndex++ << "\n"
						<< formatSRTTime(start) << " --> " << formatSRTTime(end) << "\n"
						<< l->speaker << ": " << l->text;
					out << "\n\n";
					if (end - cueStart > cueDuration) cueDuration = end - cueStart;
				}
			}

			currentTime += cueDuration;
		}
	}
	return out.str();
}

std::string unnufm::Storyboard::firstSceneID() const {
	if (!(storyboard->n_scenes > 0 && storyboard->scenes)) return "";
	unnufm_scene_t* _scene = storyboard->scenes[0];
	return std::string(_scene->sceneID);
}

unnufm_storyboard_t* unnufm::ScriptParser::toStoryboard(const char* script) {
	// std::string script_str(script);
	nlohmann::json j = nlohmann::json::parse(script);
	// std::vector<unnufm::scene_t> _scenes(toScenes(j));

	return toStoryboard(j);
}
unnufm_storyboard_t* unnufm::ScriptParser::toStoryboard(nlohmann::json j) {
	std::vector<unnufm_scene_t*> _scenes;
	if (j.contains("scenes") && j["scenes"].is_array()) {
		for (const auto& sceneJson : j["scenes"]) {
			unnufm_scene_t* scene = (unnufm_scene_t*) malloc(sizeof(unnufm_scene_t));
			// Safe string extraction
			std::string tempSceneID = sceneJson.value("sceneID", std::string());
			int sceneIDBufferSize = tempSceneID.size(); // +1 for null terminator
			scene->sceneID = (char*) calloc(sceneIDBufferSize + 1, sizeof(char));
			std::memcpy(scene->sceneID, tempSceneID.c_str(), sceneIDBufferSize);
			scene->sceneID[sceneIDBufferSize] = '\0'; // Ensure null termination
			scene->n_sceneID = sceneIDBufferSize;
			std::string tempLocation = sceneJson.value("location", std::string());
			int locationBufferSize = tempLocation.size(); // +1 for null terminator
			scene->location = (char*) calloc(locationBufferSize + 1, sizeof(char));
			std::memcpy(scene->location, tempLocation.c_str(), locationBufferSize);
			scene->location[locationBufferSize] = '\0'; // Ensure null termination
			scene->n_location = locationBufferSize;
			std::string tempStage = sceneJson.value("stage", std::string());
			int stageBufferSize = tempStage.size(); // +1 for null terminator
			scene->stage = (char*) calloc(stageBufferSize + 1, sizeof(char));
			std::memcpy(scene->stage, tempStage.c_str(), stageBufferSize);
			scene->stage[stageBufferSize] = '\0'; // Ensure null termination
			scene->n_stage = stageBufferSize;
			std::string tempJump = sceneJson.value("jump", std::string());
			int jumpBufferSize = tempJump.size(); // +1 for null terminator
			scene->jump = (char*) calloc(jumpBufferSize + 1, sizeof(char));
			std::memcpy(scene->jump, tempJump.c_str(), jumpBufferSize);
			scene->jump[jumpBufferSize] = '\0'; // Ensure null termination
			scene->n_jump = jumpBufferSize;
			std::vector<unnufm_cue_t*> cues;
			if (sceneJson.contains("cues") && sceneJson["cues"].is_array()) {
				for (const auto& cueJson : sceneJson["cues"]) {
					unnufm_cue_t* cue = (unnufm_cue_t*) malloc(sizeof(unnufm_cue_t));
					if (cueJson.contains("timing") && cueJson["timing"].is_string()) {
						cue->timing = unnufm::unnufm_scene_timing_from_string(cueJson["timing"].get<std::string>());
					}
					cue->stagger = cueJson.value("stagger", 0.7);
					cue->dynamic = cueJson.value("dynamic", false);
					std::vector<unnufm_cue_line_t*> lines;
					if (cueJson.contains("lines") && cueJson["lines"].is_array()) {
						for (const auto& lineJson : cueJson["lines"]) {
							unnufm_cue_line_t* line = (unnufm_cue_line_t*) malloc(sizeof(unnufm_cue_line_t));
							std::string tempSpeaker = lineJson.value("speaker", std::string());
							int speakerBufferSize = tempSpeaker.size(); // +1 for null terminator
							line->speaker = (char*) calloc(speakerBufferSize + 1, sizeof(char));
							std::memcpy(line->speaker, tempSpeaker.c_str(), speakerBufferSize);
							line->speaker[speakerBufferSize] = '\0'; // Ensure null termination
							line->n_speaker = speakerBufferSize;
							std::string tempText = lineJson.value("text", std::string());
							int textBufferSize = tempText.size(); // +1 for null terminator
							line->text = (char*) calloc(textBufferSize + 1, sizeof(char));
							std::memcpy(line->text, tempText.c_str(), textBufferSize);
							line->text[textBufferSize] = '\0'; // Ensure null termination
							line->n_text = textBufferSize;
							line->pause = lineJson.value("pause", 0.0);
							line->delay = lineJson.value("delay", 0.0);
							if (lineJson.contains("emotion") && lineJson["emotion"].is_string()) {
								line->emotion = unnutts::emotion_from_string(lineJson["emotion"].get<std::string>());
							}
							else {
								line->emotion = EEMOTION::EMOTION_NEUTRAL; // default to neutral
							}
							line->lipSync = lineJson.value("lip_sync", true);
							std::vector<unnufm_action_t*> actions;
							if (lineJson.contains("actions") && lineJson["actions"].is_array()) {
								for (const auto& actionJson : lineJson["actions"]) {
									unnufm_action_t* action = (unnufm_action_t*) malloc(sizeof(unnufm_action_t));
									std::string tempTarget = actionJson.value("target", std::string());
									int targetBufferSize = tempTarget.size(); // +1 for null terminator
									action->target = (char*) calloc(targetBufferSize + 1, sizeof(char));
									std::memcpy(action->target, tempTarget.c_str(), targetBufferSize);
									action->target[targetBufferSize] = '\0'; // Ensure null termination
									action->n_target = targetBufferSize;
									std::string tempGesture = actionJson.value("gesture", std::string());
									int gestureBufferSize = tempGesture.size(); // +1 for null terminator
									action->gesture = (char*) calloc(gestureBufferSize + 1, sizeof(char));
									std::memcpy(action->gesture, tempGesture.c_str(), gestureBufferSize);
									action->gesture[gestureBufferSize] = '\0'; // Ensure null termination
									action->n_gesture = gestureBufferSize;

									action->motion = unnufm_motion_from_string(actionJson.value("motion", std::string()));
									action->persistent = actionJson.value("persistent", false);
									action->delay = actionJson.value("delay", 0.0);
									actions.push_back(action);
								}
								
							}
							int n_actions = actions.size();
							if (n_actions > 0) {
								line->actions = (unnufm_action_t**)calloc(n_actions, sizeof(unnufm_action_t*));
								std::memcpy(line->actions, actions.data(), n_actions * sizeof(unnufm_action_t*));
							}
							line->n_actions = n_actions;
							lines.push_back(line);
						}
					}
					int n_lines = lines.size();
					if (n_lines > 0) {
						cue->lines = (unnufm_cue_line_t**)calloc(n_lines, sizeof(unnufm_cue_line_t*));
						std::memcpy(cue->lines, lines.data(), n_lines * sizeof(unnufm_cue_line_t*));
					}
					cue->n_lines = n_lines;
					std::vector<unnufm_sound_cue_t*> sounds;
					if (cueJson.contains("sounds") && cueJson["sounds"].is_array()) {
						
						for (const auto& soundJson : cueJson["sounds"]) {
							unnufm_sound_cue_t* soundCue = (unnufm_sound_cue_t*) malloc(sizeof(unnufm_sound_cue_t));
							std::string tempUri = soundJson.value("uri", std::string());
							int urlBufferSize = tempUri.size(); // +1 for null terminator
							soundCue->uri = (char*) calloc(urlBufferSize + 1, sizeof(char));
							std::memcpy(soundCue->uri, tempUri.c_str(), urlBufferSize);
							soundCue->uri[urlBufferSize] = '\0'; // Ensure null termination
							soundCue->n_uri = urlBufferSize;
							soundCue->type = sound_cue_type_from_string(soundJson.value("type", std::string()));
							
							soundCue->time = soundJson.value("time", 0.0);
							sounds.push_back(soundCue);
						}
						
						// std::memcpy(cue->sounds, , sounds.size() * sizeof(unnufm_sound_cue_t*));
					}
					int n_sounds = sounds.size();
					if (n_sounds > 0) {
						cue->sounds = (unnufm_sound_cue_t**)calloc(n_sounds, sizeof(unnufm_sound_cue_t*));
						std::memcpy(cue->sounds, sounds.data(), n_sounds * sizeof(unnufm_sound_cue_t*));
					}
					cue->n_sounds = n_sounds;

					std::vector<unnufm_camera_cue_t*> shots;
					if (cueJson.contains("shots") && cueJson["shots"].is_array()) {
						
						for (const auto& cameraJson : cueJson["shots"]) {
							unnufm_camera_cue_t* cameraCue = (unnufm_camera_cue_t*) malloc(sizeof(unnufm_camera_cue_t));
							std::string tempCamTarget = cameraJson.value("target", std::string());
							int camTargetBufferSize = tempCamTarget.size(); // +1 for null terminator
							cameraCue->target = (char*)calloc(camTargetBufferSize + 1, sizeof(char));
							std::memcpy(cameraCue->target, tempCamTarget.c_str(), camTargetBufferSize);
							cameraCue->target[camTargetBufferSize] = '\0'; // Ensure null termination
							cameraCue->n_target = camTargetBufferSize;
							if (cameraJson.contains("size") && cameraJson["size"].is_string()) {
								cameraCue->size = camera_shot_size_from_string(cameraJson["size"].get<std::string>());
							}
							if (cameraJson.contains("angle") && cameraJson["angle"].is_string()) {
								cameraCue->angle = camera_shot_angle_from_string(cameraJson["angle"].get<std::string>());
							}
							if (cameraJson.contains("movement") && cameraJson["movement"].is_string()) {
								cameraCue->movement = camera_shot_movement_from_string(cameraJson["movement"].get<std::string>());
							}
							cameraCue->duration = cameraJson.value("duration", 0.0);
							cameraCue->transition = cameraJson.value("transition", 0.0);
							shots.push_back(cameraCue);
						}
						
					}
					int n_shots = shots.size();
					if (n_shots > 0) {
						cue->shots = (unnufm_camera_cue_t**)calloc(n_shots, sizeof(unnufm_camera_cue_t*));
						std::memcpy(cue->shots, shots.data(), n_shots * sizeof(unnufm_camera_cue_t*));
					}
					cue->n_shots = n_shots;
					cues.push_back(cue);
				}
				
			}
			int n_cues = cues.size();
			if (n_cues > 0) {
				scene->cues = (unnufm_cue_t**)calloc(n_cues, sizeof(unnufm_cue_t*));
				std::memcpy(scene->cues, cues.data(), n_cues * sizeof(unnufm_cue_t*));
			}
			scene->n_cues = n_cues;
			_scenes.push_back(scene);
		}
	}
	unnufm_storyboard_t* storyboard = (unnufm_storyboard_t*)malloc(sizeof(unnufm_storyboard_t));
	int n_scenes = _scenes.size();
	if (n_scenes > 0) {
		storyboard->scenes = (unnufm_scene_t**) calloc(n_scenes, sizeof(unnufm_scene_t*));
		std::memcpy(storyboard->scenes, _scenes.data(), n_scenes * sizeof(unnufm_scene_t*));
	}
	storyboard->n_scenes = n_scenes;
	return storyboard;
}

std::set<std::string> unnufm::ScriptParser::getCreditedActorsInStoryboard(Storyboard storyboard) {
	std::set<std::string> actors;
	unnufm_storyboard_t* story = storyboard.getScenes();
	if (story->n_scenes > 0 && story->scenes)
		for (int i = 0; i < story->n_scenes; i++) {
		unnufm_scene_t* scene = story->scenes[i];
		if (scene->n_cues > 0 && scene->cues)
			for (int i = 0; i < scene->n_cues; i++) {
				unnufm_cue_t* cue = scene->cues[i];
				if (cue->n_lines > 0 && cue->lines)
					for (int j = 0; j < cue->n_lines; j++) {
					unnufm_cue_line_t* line = cue->lines[j];
					if (line->speaker && line->text && strlen(line->text) > 0) actors.insert(std::string(line->speaker));
			}
		}
	}
	return actors;
}

std::set<std::string> unnufm::ScriptParser::getActorsInSkit(unnufm_skit_t skit)
{
	std::set<std::string> actors;
	if (skit.n_takes > 0 && skit.takes)
		for (int i = 0; i < skit.n_takes; i++) {
			unnufm_take_t* take = skit.takes[i];
			if (take->n_performances > 0 && take->performances)
				for (int j = 0; j < take->n_performances; j++) {
					unnufm_performance_t* performance = take->performances[j];
					if (performance->line && performance->line->speaker && strlen(performance->line->speaker) > 0) actors.insert(std::string(performance->line->speaker));
		}
	}
	return actors;
}

std::set<std::string>  unnufm::ScriptParser::getCreditedActorsInTake(unnufm_take_t take)
{
	std::set<std::string> actors;
	if (take.n_performances > 0 && take.performances)
		for (int j = 0; j < take.n_performances; j++) {
			unnufm_performance_t* performance = take.performances[j];
			if (performance->line && performance->line->speaker && strlen(performance->line->speaker) > 0 && performance->audio->chunk->n_samples > 0) actors.insert(std::string(performance->line->speaker));
	}
	return actors;
}

std::set<std::string> unnufm::ScriptParser::getCreditedActorsInScene(unnufm_scene_t scene) {
	std::set<std::string> actors;
	if (scene.n_cues > 0 && scene.cues)
		for (int i = 0; i < scene.n_cues; i++) {
			unnufm_cue_t* cue = scene.cues[i];
			if (cue->n_lines > 0 && cue->lines)
				for (int j = 0; j < cue->n_lines; j++) {
					unnufm_cue_line_t* line = cue->lines[j];
					if (line->speaker && strlen(line->speaker) && line->text && strlen(line->text) > 0) actors.insert(std::string(line->speaker));
		}
	}
	return actors;
}

unnufm_audio_t* unnufm::PiperTTS::speak(unnufm_cue_line_t* line, const unnufm_speaker_list_t* voices) {
	unnufm_audio_t* audio = (unnufm_audio_t*) malloc(sizeof(unnufm_audio_t));
	int textSize = strlen(line->text);
	audio->text = (char*) calloc(textSize+1, sizeof(char));
	std::memcpy(audio->text, line->text, textSize);
	audio->text[textSize] = '\0'; // Ensure null termination
	audio->n_text = textSize;
	auto& speaker = unnufm::PiperTTS::find_speaker(voices, line->speaker);
	if (speaker.speaker_id >= 0) {
		if (strlen(line->text) > 0) {
			ut_audio_sample_t* result = unnu_tts(speaker.speaker_id, line->emotion, speaker.is_robot != 0, line->text);
			if (result->num_samples > 0) {
				audio->chunk = (unnufm_audio_chunk_t*) malloc(sizeof(unnufm_audio_chunk_t));
				audio->chunk->sample_rate = result->sample_rate;
				audio->chunk->n_samples = result->num_samples;
				audio->chunk->samples = (float*) calloc(result->num_samples, sizeof(float));
				std::memcpy(audio->chunk->samples, result->samples, result->num_samples * sizeof(float));
			}
			ut_audio_sample_free(result);
		}
	}
	return audio;
}


bool unnufm::SkitGrabber::grab(unnufm_skit_t& skit) {
	return skits.try_dequeue(skit);
}

//void unnufm::ProductionWrangler::load(const std::string& script) {
//	nlohmann::json j = nlohmann::json::parse(script);
//	storyboard = ScriptParser::toStoryboard(j);
//	load(storyboard);
//	
//}
//
//void unnufm::ProductionWrangler::load(const unnufm_storyboard_t* storyboard) {
//	if (storyboard->n_scenes > 0 && storyboard->scenes)
//		for (int i = 0; i < storyboard->n_scenes; i++) {
//			unnufm_scene_t* scene = storyboard->scenes[i];
//			scenes[scene->sceneID] = scene;
//		}
//}

void unnufm::ProductionWrangler::play(const unnufm_scene_t* scene, const unnufm_speaker_list_t* voices) {
	//auto it = scenes.find(sceneId);
	//if (it == scenes.end()) {
	//	return "";
	//}
	//unnufm_scene_t* scene = it->second;
	perform(scene, voices);
}

unnufm_performance_t* unnufm::ProductionWrangler::toPerformance(const unnufm_cue_line_t* line, const unnufm_speaker_list_t* voices) {
	unnufm_performance_t* placeholder = (unnufm_performance_t*) malloc(sizeof(unnufm_performance_t));
	placeholder->line = (unnufm_cue_line_t*) malloc(sizeof(unnufm_cue_line_t));
	placeholder->line->actions =   line->actions;
	placeholder->line->delay = line->delay;
	placeholder->line->emotion = line->emotion;	
	placeholder->line->lipSync = line->lipSync;
	int speakerBufferSize = strlen(line->speaker); // +1 for null terminator
	placeholder->line->speaker = (char*) calloc(speakerBufferSize + 1, sizeof(char));
	std::memcpy(placeholder->line->speaker, line->speaker, speakerBufferSize);
	placeholder->line->speaker[speakerBufferSize] = '\0'; //
	placeholder->line->n_speaker = speakerBufferSize;
	int textBufferSize = strlen(line->text); // +1 for null terminator
	placeholder->line->text = (char*)calloc(textBufferSize + 1, sizeof(char));
	std::memcpy(placeholder->line->text, line->text, textBufferSize);
	placeholder->line->text[textBufferSize] = '\0'; // Ensure null termination
	placeholder->line->n_text = textBufferSize;
	placeholder->audio = unnufm::PiperTTS::speak(placeholder->line, voices);
	return placeholder;
}
		
unnufm_take_t* unnufm::ProductionWrangler::toTake(const unnufm_cue_t* cue, const unnufm_speaker_list_t* voices) {
	unnufm_take_t* take =(unnufm_take_t*) malloc(sizeof(unnufm_take_t));
	take->timing = cue->timing;
	take->stagger = cue->stagger;
	take->dynamic = cue->dynamic;
	take->sounds = cue->sounds;
	take->shots = cue->shots;
	if (cue->n_lines > 0 && cue->lines) {
		std::vector<unnufm_performance_t*> performances;
		for (int i = 0; i < cue->n_lines; i++) {
			unnufm_cue_line_t* line = cue->lines[i];
			unnufm_performance_t* p = toPerformance(line, voices);
			performances.push_back(std::move(p));
		}
		int n_performances = performances.size();
		if (n_performances > 0) {
			take->performances = (unnufm_performance_t**)calloc(n_performances, sizeof(unnufm_performance_t*));
			std::memcpy(take->performances, performances.data(), n_performances * sizeof(unnufm_performance_t*));
		}
		take->n_performances = n_performances;
	}
	return take;
}

unnufm_skit_t* unnufm::ProductionWrangler::toSkit(const unnufm_scene_t* scene, const unnufm_speaker_list_t* voices) {
	unnufm_skit_t* skit = (unnufm_skit_t*) malloc(sizeof(unnufm_skit_t));
	int sceneIDBufferSize = strlen(scene->sceneID); // +1 for null terminator
	skit->sceneID = (char*) calloc(sceneIDBufferSize + 1, sizeof(char));
	std::memcpy(skit->sceneID, scene->sceneID, sceneIDBufferSize);
	skit->sceneID[sceneIDBufferSize] = '\0'; // Ensure null termination
	skit->n_sceneID = sceneIDBufferSize;
	int locationBufferSize = strlen(scene->location); // +1 for null terminator
	skit->location = (char*) calloc(locationBufferSize + 1, sizeof(char));
	std::memcpy(skit->location, scene->location, locationBufferSize);
	skit->location[locationBufferSize] = '\0'; // Ensure null termination
	skit->n_location = locationBufferSize;
	int stageBufferSize = strlen(scene->stage); // +1 for null terminator
	skit->stage = (char*) calloc(stageBufferSize + 1, sizeof(char));
	std::memcpy(skit->stage, scene->stage, stageBufferSize);
	skit->stage[stageBufferSize] = '\0'; // Ensure null termination
	skit->n_stage = stageBufferSize;
	int jumpBufferSize = strlen(scene->jump); // +1 for null terminator
	skit->jump = (char*) calloc(jumpBufferSize + 1, sizeof(char));
	std::memcpy(skit->jump, scene->jump, jumpBufferSize);
	skit->jump[jumpBufferSize] = '\0'; // Ensure null termination
	skit->n_jump = jumpBufferSize;
	if (scene->n_cues > 0 && scene->cues) {
		std::vector<unnufm_take_t*> takes;
		for (int i = 0; i < scene->n_cues; i++) {
			unnufm_cue_t* cue = scene->cues[i];
			unnufm_take_t* t = toTake(cue, voices);
			takes.push_back(std::move(t));
		}
		int n_takes = takes.size();
		if (n_takes > 0) {
			skit->takes = (unnufm_take_t**)calloc(n_takes, sizeof(unnufm_take_t*));
			std::memcpy(skit->takes, takes.data(), n_takes * sizeof(unnufm_take_t*));
		}
		skit->n_takes = n_takes;
	}
	return skit;
}

void unnufm::ProductionWrangler::perform(const unnufm_scene_t* scene, const unnufm_speaker_list_t* voices) {
	unnufm_skit_t* s = toSkit(scene, voices);
	SendSkitToUnrealEngine(*s);
}

void unnufm::ProductionWrangler::SendSkitToUnrealEngine(unnufm_skit_t skit) {
	skits.enqueue(skit);
}

//void unnufm::SceneManager::LoadStoryboard(const std::string& script) {
//	wrangler.load(script);
//}

void unnufm::SceneManager::run(const unnufm_storyboard_t* story, const unnufm_speaker_list_t* voices) {
	if (story->n_scenes > 0 && story->scenes)
		for (int i = 0; i < story->n_scenes; i++) {
		// go to next scene
		unnufm::ProductionWrangler::play(story->scenes[i], voices);
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

static std::string g_unnufm_fm_male_string("male");
static std::string g_unnufm_fm_female_string("female");

unnufm_speaker_properties_t unnufm::PiperTTS::find_speaker(const unnufm_speaker_list_t* actors, const char* speaker) {
	std::string actor(speaker);
	if (actors && actors->n_speakers > 0 && actors->speakers) {
		for (int i = 0; i < actors->n_speakers; i++) {
			std::string name(actors->speakers[i]->speaker);
			if (equals_ignore_case(actor, name)) {
				return actors->speakers[i]->properties;
			}
		}
	}
	unnufm_speaker_properties_t props;
	props.is_robot = 0;
	props.speaker_id = -1;
	return props;
}


void unnufm_fm_process_script(const char* script) {
	std::string script_str(script);
	
	nlohmann::json j =  nlohmann::json::parse(script_str);
	
	unnufm_storyboard_t* story = unnufm::ScriptParser::toStoryboard(j);


	auto& speakers = unnufm::ScriptParser::getCreditedActorsInStoryboard(story);
	std::map<std::string, unnufm_speaker_properties_t> voices;
	// unnufm::PiperTTS* tts = new unnufm::PiperTTS();
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
				if(genderCode == 2 || starts_with_ci(speaker, g_unnufm_fm_male_string))
				{
					std::string maleVoce = std::string("male.") + locale_code;
					id = ut_get_speaker_id(maleVoce.c_str());
					if(id < 0)
					{
						maleVoce = g_unnufm_fm_male_string + "." + langcode;
						id = ut_get_speaker_id(maleVoce.c_str());
						if(id < 0)
						{
							id = ut_get_speaker_id(g_unnufm_fm_male_string.c_str());
						}
						
					}
				} else if(genderCode == 1 || starts_with_ci(speaker, g_unnufm_fm_female_string)){
					std::string femaleVoce = std::string("female.") + locale_code;
					id = ut_get_speaker_id(femaleVoce.c_str());
					if(id < 0)
					{
						femaleVoce = g_unnufm_fm_female_string + "." + langcode;
						id = ut_get_speaker_id(femaleVoce.c_str());
						if(id < 0)
						{
							id = ut_get_speaker_id(g_unnufm_fm_female_string.c_str());
						}
						
					}
				}
			}
		}
		
		

		if (id >= 0) {
			// For demonstration, assign robot voices to Manny and Quinn
			if (lookup == "manny" || lookup == "quinn") {
				unnufm_speaker_properties_t props;
				props.is_robot = 1;
				props.speaker_id = id;
				voices[speaker]= props;
			}
			else {
				unnufm_speaker_properties_t props;
				props.is_robot = 0;
				props.speaker_id = id;
				voices[speaker] = props;
			}
		}
	}

	unnufm_speaker_list_t* list = (unnufm_speaker_list_t*) malloc(sizeof(unnufm_speaker_list_t));
	int n_voices = voices.size();
	list->speakers = (unnufm_speaker_ref_t**)calloc(n_voices, sizeof(unnufm_speaker_ref_t*));
	list->n_speakers = n_voices;
	int i = 0;
	for (const auto& pair : voices) {
		int bufsize = pair.first.size();
		list->speakers[i] = (unnufm_speaker_ref_t*) malloc(sizeof(unnufm_speaker_ref_t));
		list->speakers[i]->speaker = (char*)calloc(bufsize, sizeof(char));
		std::memcpy(list->speakers[i]->speaker, pair.first.c_str(), bufsize);
		list->speakers[i]->speaker[bufsize] = '\0';
		list->speakers[i]->n_speaker = bufsize;
		list->speakers[i]->properties = pair.second;
		i++;
	}

	std::thread th([story, list](){
		unnufm::SceneManager sm;
		// unnufm_storyboard_t*  storyboard = wrangler.getStoryboard();
		if(story->n_scenes > 0 && story->scenes)
		sm.run(story, list);
		unnufm::unnufm_speaker_list_ptr(list).reset();
	});
	
	th.detach();
}


