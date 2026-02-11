#pragma once

#include "c-api.h"
#include <unnu_tts/cxx-api.h>
#include <set>
#include <nlohmann/json.hpp>

namespace unnufm {

	// =====================
	// Data Structures
	// =====================

	static unnu_sound_cue_type sound_cue_type_from_string(const std::string & type_str) {
		if (type_str == "sfx") {
			return unnu_sound_cue_type::SOUND_SFX;
		}
		if (type_str == "music") {
			return unnu_sound_cue_type::SOUND_MUSIC;
		}
		if (type_str == "ambient") {
			return unnu_sound_cue_type::SOUND_AMBIENT;
		}
		if (type_str == "default") {
			return unnu_sound_cue_type::SOUND_DEFAULT;
		}
		return unnu_sound_cue_type::SOUND_DEFAULT;
	}

	static std::string sound_cue_type_to_string(unnu_sound_cue_type type) {
		switch (type) {
			case SOUND_SFX: return "sfx";
			case SOUND_MUSIC: return "music";
			case SOUND_AMBIENT: return "ambient";
			case SOUND_DEFAULT: return "default";
			default:  return "default";
		}
	}


	static unnu_camera_shot_size camera_shot_size_from_string(const std::string & size_str) {
		if (size_str == "default") {
			return unnu_camera_shot_size::CAMERA_SIZE_DEFAULT;
		}
		if (size_str == "medium") {
			return unnu_camera_shot_size::CAMERA_SIZE_MEDIUM;
		}
		if (size_str == "full") {
			return unnu_camera_shot_size::CAMERA_SIZE_FULL;
		}
		if (size_str == "close_up") {
			return unnu_camera_shot_size::CAMERA_SIZE_CLOSE_UP;
		}
		if (size_str == "wide") {
			return unnu_camera_shot_size::CAMERA_SIZE_WIDE;
		}
		if (size_str == "medium_full") {
			return unnu_camera_shot_size::CAMERA_SIZE_MEDIUM_FULL;
		}
		if (size_str == "medium_close_up") {
			return unnu_camera_shot_size::CAMERA_SIZE_MEDIUM_CLOSE_UP;
		}
		if (size_str == "extreme_close_up") {
			return unnu_camera_shot_size::CAMERA_SIZE_EXTREME_CLOSE_UP;
		}
		if (size_str == "extreme_wide") {
			return unnu_camera_shot_size::CAMERA_SIZE_EXTREME_WIDE;
		}
		return unnu_camera_shot_size::CAMERA_SIZE_DEFAULT;
	}

	static std::string camera_shot_size_to_string(unnu_camera_shot_size type) {
		switch (type) {
			case CAMERA_SIZE_DEFAULT: return "default";
			case CAMERA_SIZE_MEDIUM: return "medium";
			case CAMERA_SIZE_FULL: return "full";
			case CAMERA_SIZE_CLOSE_UP: return "close_up";
			case CAMERA_SIZE_WIDE: return "wide";
			case CAMERA_SIZE_MEDIUM_FULL: return "medium_full";
			case CAMERA_SIZE_MEDIUM_CLOSE_UP: return "medium_close_up";
			case CAMERA_SIZE_EXTREME_CLOSE_UP: return "extreme_close_up";
			case CAMERA_SIZE_EXTREME_WIDE: return "extreme_wide";
			default:  return "default";
		}
	}

	static unnu_camera_shot_angle camera_shot_angle_from_string(const std::string & angle_str) {
		if (angle_str == "default") {
			return unnu_camera_shot_angle::CAMERA_ANGLE_DEFAULT;
		}
		if (angle_str == "eye_level") {
			return unnu_camera_shot_angle::CAMERA_ANGLE_EYE_LEVEL;
		}
		if (angle_str == "shoulder_level") {
			return unnu_camera_shot_angle::CAMERA_ANGLE_SHOULDER_LEVEL;
		}
		if (angle_str == "hip_level") {
			return unnu_camera_shot_angle::CAMERA_ANGLE_HIP_LEVEL;
		}
		if (angle_str == "knee_level") {
			return unnu_camera_shot_angle::CAMERA_ANGLE_KNEE_LEVEL;
		}
		if (angle_str == "ground_level") {
			return unnu_camera_shot_angle::CAMERA_ANGLE_GROUND_LEVEL;
		}
		if (angle_str == "low") {
			return unnu_camera_shot_angle::CAMERA_ANGLE_LOW;
		}
		if (angle_str == "high") {
			return unnu_camera_shot_angle::CAMERA_ANGLE_HIGH;
		}
		if (angle_str == "overhead") {
			return unnu_camera_shot_angle::CAMERA_ANGLE_OVERHEAD;
		}
		return unnu_camera_shot_angle::CAMERA_ANGLE_DEFAULT;
	}

	static std::string camera_shot_angle_to_string(unnu_camera_shot_angle type) {
		switch (type) {
			case CAMERA_ANGLE_DEFAULT: return "default";
			case CAMERA_ANGLE_EYE_LEVEL: return "eye_level";
			case CAMERA_ANGLE_SHOULDER_LEVEL: return "shoulder_level";
			case CAMERA_ANGLE_HIP_LEVEL: return "hip_level";
			case CAMERA_ANGLE_KNEE_LEVEL: return "knee_level";
			case CAMERA_ANGLE_GROUND_LEVEL: return "ground_level";
			case CAMERA_ANGLE_LOW: return "low";
			case CAMERA_ANGLE_HIGH: return "high";
			case CAMERA_ANGLE_OVERHEAD: return "overhead";
			default:  return "default";
		}
	}

	static unnu_camera_shot_movement camera_shot_movement_from_string(const std::string & move_str) {
		if (move_str == "static") {
			return unnu_camera_shot_movement::CAMERA_MOVEMENT_STATIC;
		}
		if (move_str == "pan_left") {
			return unnu_camera_shot_movement::CAMERA_MOVEMENT_PAN_LEFT;
		}
		if (move_str == "pan_right") {
			return unnu_camera_shot_movement::CAMERA_MOVEMENT_PAN_RIGHT;
		}
		if (move_str == "tilt_up") {
			return unnu_camera_shot_movement::CAMERA_MOVEMENT_TILT_UP;
		}
		if (move_str == "tilt_down") {
			return unnu_camera_shot_movement::CAMERA_MOVEMENT_TILT_DOWN;
		}
		if (move_str == "push_in") {
			return unnu_camera_shot_movement::CAMERA_MOVEMENT_PUSH_IN;
		}
		if (move_str == "pull_out") {
			return unnu_camera_shot_movement::CAMERA_MOVEMENT_PULL_OUT;
		}
		if (move_str == "zoom_in") {
			return unnu_camera_shot_movement::CAMERA_MOVEMENT_ZOOM_IN;
		}
		if (move_str == "zoom_out") {
			return unnu_camera_shot_movement::CAMERA_MOVEMENT_ZOOM_OUT;
		}
		if (move_str == "tracking") {
			return unnu_camera_shot_movement::CAMERA_MOVEMENT_TRACKING;
		}
		if (move_str == "trucking") {
			return unnu_camera_shot_movement::CAMERA_MOVEMENT_TRUCKING;
		}
		return unnu_camera_shot_movement::CAMERA_MOVEMENT_STATIC;
	}

	static std::string camera_shot_movement_to_string(unnu_camera_shot_movement type) {
		switch (type) {
			case CAMERA_MOVEMENT_STATIC: return "static";
			case CAMERA_MOVEMENT_PAN_LEFT: return "pan_left";
			case CAMERA_MOVEMENT_PAN_RIGHT: return "pan_right";
			case CAMERA_MOVEMENT_TILT_UP: return "tilt_up";
			case CAMERA_MOVEMENT_TILT_DOWN: return "tilt_down";
			case CAMERA_MOVEMENT_PUSH_IN: return "push_in";
			case CAMERA_MOVEMENT_PULL_OUT: return "pull_out";
			case CAMERA_MOVEMENT_ZOOM_IN: return "zoom_in";
			case CAMERA_MOVEMENT_ZOOM_OUT: return "zoom_out";
			case CAMERA_MOVEMENT_TRACKING: return "tracking";
			case CAMERA_MOVEMENT_TRUCKING: return "trucking";
			default:  return "static";
		}
	}

	struct SoundCue {
		unnu_sound_cue_type type; // "music", "sfx", "ambient"
		std::string uri;
		double time = 0.0;
	};

	struct CameraCue {
		unnu_camera_shot_size size = unnu_camera_shot_size::CAMERA_SIZE_DEFAULT;
		unnu_camera_shot_angle angle = unnu_camera_shot_angle::CAMERA_ANGLE_DEFAULT;
		unnu_camera_shot_movement movement = unnu_camera_shot_movement::CAMERA_MOVEMENT_STATIC;
		std::string location;
		std::string target;
		double duration = 0.0;
		double time = 0.0;
	};

	struct Action {
		std::string gesture;
		double delay = 0.0;
		bool persistent = false;
	};

	typedef struct CueLine {
		std::string speaker;
		std::string text;
		double pause = 0.0;
		double delay = 0.0;
		std::vector<Action> actions;
		EEMOTION emotion = EEMOTION::EMOTION_NEUTRAL;
		bool lipSync = true;
	};
	
	struct Audio {
		unnutts::audio_sample_ptr ptr;
		std::string text;
	};
	
	struct Performance {
		Audio audio;
		CueLine line;
	};

	struct Cue {
		std::string timing; // sequential, simultaneous, overlap, branch
		double stagger = 0.7; // NEW: default stagger for overlap
		bool dynamic = false; // NEW: allow real-time modification
		std::vector<CueLine> lines;
		std::vector<SoundCue> sounds;
		std::vector<CameraCue> shots;
	};
	
	struct Take {
		std::string timing; // sequential, simultaneous, overlap, branch
		double stagger = 0.7; // NEW: default stagger for overlap
		bool dynamic = false; // NEW: allow real-time modification
		std::vector<Performance> performances;
		std::vector<SoundCue> sounds;
		std::vector<CameraCue> shots;
	};

	struct Scene {
		std::string sceneID;
		std::vector<Cue> cues;
		std::string jump;
	};
	
	struct Skit {
		std::string sceneID;
		std::vector<Take> takes;
		std::string jump;
	};
	
	// ---------------- Storyboard Class ----------------
	class Storyboard {
	private:
		

	public:
		std::vector<Scene> scenes;
		void addScene(const Scene& scene);

		Scene& addSceneReturnRef(const Scene& scene);
		
		std::string exportToEDL() const;
		
		std::string exportToPremiereXML(float fps) const;
		
		static std::string formatSRTTime(double seconds);
		
		std::string exportToSRT() const;

		std::string firstSceneID() const {
			if (scenes.empty()) return "";
			return scenes.front().sceneID;
		}
		
	};

	// =====================
	// Piper TTS
	// =====================
	class PiperTTS {
	private:
		std::map<std::string, std::pair<int32_t, bool>> voices;
	public:
		Audio speak(const CueLine& line) {
			Audio audio;
			audio.text = line.text;
			auto& speaker = getVoice(line.speaker);
			if(speaker.first >= 0) {
				if(!line.text.empty()){
					ut_audio_sample_t* result = unnu_tts(speaker.first, line.emotion, speaker.second, line.text.c_str());
					audio.ptr.reset(result);
					// if(result->num_samples > 0){
					// 	float* samples = result->samples;
					//	int n = result->num_samples;
					//	std::vector<float> v(samples, samples + n);
					//	audio.audio = v;
					// }
				}
			}
			return audio;
		}

		void setVoice(const std::string& speaker, int32_t speaker_id, bool is_robot) {
			voices[speaker] = std::make_pair(speaker_id, is_robot);
		}

		std::pair<int32_t, bool> getVoice(const std::string& speaker) const {
			auto it = voices.find(speaker);
			if (it != voices.end()) return it->second;
			return std::make_pair(-1, false);
		}
	};

	// ---------------- Skit Grabber ----------------
	class SkitGrabber {
	public:
		static bool grab(Skit& skit);
	};

	// ---------------- Script Parser ----------------
	class ScriptParser {
	public:

		static Storyboard toStoryboard(const nlohmann::json& j);
		static std::set<std::string> getCreditedActorsInStoryboard(const Storyboard& storyboard);
		static std::set<std::string> getCreditedActorsInScene(const Scene& scene);
	};


	// ---------------- Dialog Manager ----------------
	class ProductionWrangler {
	private:
		PiperTTS& tts;
		std::map<std::string, Scene> scenes;
		Storyboard storyboard;
		// Send audio file path to UE5 for Speech2Face processing
		void SendSkitToUnrealEngine(const Skit& skit);
	public:
		
		ProductionWrangler(PiperTTS& actors) : tts(actors) {}

		ProductionWrangler(PiperTTS& actors, const std::string& script)  : tts(actors) {
			load(script);
		}

		ProductionWrangler(PiperTTS& actors, Storyboard& script) : tts(actors), storyboard(script) {}

		
		void load(const std::string& script) {
			nlohmann::json j = nlohmann::json::parse(script);
			
			storyboard = ScriptParser::toStoryboard(j);
			for(const Scene& scene : storyboard.scenes){
				scenes[scene.sceneID] = scene;
			}
		}
		
		Storyboard getStoryboard() const {
			return storyboard;
		}
		
		Scene getScene(std::string& sceneId) {
			return scenes[sceneId];
		}

		Performance toPerformance(const CueLine& line);

		Take toTake(const Cue& cue);

		Skit toSkit(const Scene& scene);

		void perform(const Scene& scene);

		std::string play(std::string sceneId) {
			auto it = scenes.find(sceneId);
            if (it == scenes.end()) {
				return "";
			}
			Scene& scene = it->second;
			perform(scene);
			return scene.jump;
		}
	};
	// ---------------- Scene Manager ------------------
	class SceneManager {

	public:
		SceneManager(ProductionWrangler&  manager) : wrangler(manager) {}

		void LoadStoryboard(const std::string& script) {
			wrangler.load(script);
		}

		void run(const std::string& startScene) {
			std::string currentScene = startScene;

			while (!currentScene.empty()) {
				// go to next scene
				currentScene = wrangler.play(currentScene);
			}
		}

	private:
		ProductionWrangler& wrangler;
	};
}
