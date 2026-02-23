#pragma once

#include "c-api.h"
#include <set>
#include <nlohmann/json.hpp>

namespace unnufm {

	// =====================
	// Data Structures
	// =====================
	
	
	static unnu_motion_t unnu_motion_from_string(const std::string & type_str) {
		if (type_str == "stand") {
			return unnu_motion::MOTION_STAND;
		}
		if (type_str == "sit") {
			return unnu_motion::MOTION_SIT;
		}
		if (type_str == "lie") {
			return unnu_motion::MOTION_LIE;
		}
		if (type_str == "walk") {
			return unnu_motion::MOTION_WALK;
		}
		if (type_str == "jog") {
			return unnu_motion::MOTION_JOG;
		}
		if (type_str == "run") {
			return unnu_motion::MOTION_RUN;
		}
		return unnu_motion::MOTION_NONE;
	}

	static std::string unnu_motion_to_string(unnu_motion type) {
		switch (type) {
			case MOTION_NONE: return "";
			case MOTION_STAND: return "stand";
			case MOTION_SIT: return "sit";
			case MOTION_LIE: return "lie";
			case MOTION_WALK: return "walk";
			case MOTION_JOG: return "jog";
			case MOTION_RUN: return "run";
			default:  return "";
		}
	}
	

	static unnu_scene_timing unnu_scene_timing_from_string(const std::string & type_str) {
		if (type_str == "sequential") {
			return unnu_scene_timing::TIMING_SEQUENTIAL;
		}
		if (type_str == "simultaneous") {
			return unnu_scene_timing::TIMING_SIMULTANEOUS;
		}
		if (type_str == "overlap") {
			return unnu_scene_timing::TIMING_OVERLAP;
		}
		return unnu_scene_timing::TIMING_SEQUENTIAL;
	}

	static std::string unnu_scene_timing_to_string(unnu_scene_timing type) {
		switch (type) {
			case TIMING_SEQUENTIAL: return "sequential";
			case TIMING_SIMULTANEOUS: return "simultaneous";
			case TIMING_OVERLAP: return "overlap";
			default:  return "sequential";
		}
	}
	
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
		if (size_str == "cowboy") {
			return unnu_camera_shot_size::CAMERA_SIZE_COWBOY;
		}
		if (size_str == "medium_wide") {
			return unnu_camera_shot_size::CAMERA_SIZE_MEDIUM_WIDE;
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
			case CAMERA_SIZE_COWBOY: return "cowboy";
			case CAMERA_SIZE_MEDIUM_WIDE: return "medium_wide";
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
		if (move_str == "fixed") {
			return unnu_camera_shot_movement::CAMERA_MOVEMENT_FIXED;
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
		return unnu_camera_shot_movement::CAMERA_MOVEMENT_FIXED;
	}

	static std::string camera_shot_movement_to_string(unnu_camera_shot_movement type) {
		switch (type) {
			case CAMERA_MOVEMENT_FIXED: return "fixed";
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
			default:  return "fixed";
		}
	}

	typedef struct SoundCue {
		unnu_sound_cue_type type; // "music", "sfx", "ambient"
		std::string uri;
		double time = 0.0;
	} sound_cue_t;

	typedef struct CameraCue {
		unnu_camera_shot_size size = unnu_camera_shot_size::CAMERA_SIZE_DEFAULT;
		unnu_camera_shot_angle angle = unnu_camera_shot_angle::CAMERA_ANGLE_DEFAULT;
		unnu_camera_shot_movement movement = unnu_camera_shot_movement::CAMERA_MOVEMENT_FIXED;
		std::string target;
		double duration = 0.0;
		double transition = 0.0;
	} camera_cue_t;

	typedef struct Action {
		unnu_motion_t motion;
		std::string gesture;
		std::string target;
		double delay = 0.0;
		bool persistent = false;
	} action_t;

	typedef struct CueLine {
		std::string speaker;
		std::string text;
		double pause = 0.0;
		double delay = 0.0;
		std::vector<action_t> actions;
		EEMOTION emotion = EEMOTION::EMOTION_NEUTRAL;
		bool lipSync = true;
	} cue_line_t;
	
	typedef struct AudioChunk{
		std::vector<float> samples;
		int sample_rate;
	} audio_chunk_t;

	typedef struct Audio {
		audio_chunk_t chunk;
		std::string text;
	} audio_t;
	
	typedef struct Performance {
		audio_t audio;
		cue_line_t line;
	} performance_t;

	typedef struct Cue {
		unnu_scene_timing_t timing; // sequential, simultaneous, overlap, branch
		double stagger = 0.7; // NEW: default stagger for overlap
		bool dynamic = false; // NEW: allow real-time modification
		std::vector<cue_line_t> lines;
		std::vector<sound_cue_t> sounds;
		std::vector<camera_cue_t> shots;
	} cue_t;
	
	typedef struct Take {
		unnu_scene_timing_t timing; // sequential, simultaneous, overlap, branch
		double stagger = 0.7; // NEW: default stagger for overlap
		bool dynamic = false; // NEW: allow real-time modification
		std::vector<performance_t> performances;
		std::vector<sound_cue_t> sounds;
		std::vector<camera_cue_t> shots;
	} take_t;

	typedef struct Scene {
		std::string sceneID;
		std::string location;
		std::string stage;
		std::vector<cue_t> cues;
		std::string jump;
	} scene_t;
	
	typedef struct Skit {
		std::string sceneID;
		std::string location;
		std::string stage;
		std::vector<take_t> takes;
		std::string jump;
	} skit_t;
	
	// ---------------- Storyboard Class ----------------
	class Storyboard {

	public:
		std::vector<scene_t> scenes;
		void addScene(const scene_t& scene);

		scene_t& addSceneReturnRef(const scene_t& scene);
		
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
		audio_t speak(const cue_line_t& line);

		void setVoice(const std::string& speaker, int32_t speaker_id, bool is_robot);

		std::pair<int32_t, bool> getVoice(const std::string& speaker) const;
	};

	// ---------------- Skit Grabber ----------------
	class SkitGrabber {
	public:
		static bool grab(skit_t& skit);
	};

	// ---------------- Script Parser ----------------
	class ScriptParser {
	public:

		static Storyboard toStoryboard(const nlohmann::json& j);
		static std::set<std::string> getCreditedActorsInStoryboard(const Storyboard& storyboard);
		static std::set<std::string> getActorsInSkit(const skit_t& skit);
		static std::set<std::string> getCreditedActorsInScene(const scene_t& scene);
		static std::set<std::string> getCreditedActorsInTake(const take_t& take);
	};


	// ---------------- Dialog Manager ----------------
	class ProductionWrangler {
	private:
		PiperTTS& tts;
		std::map<std::string, scene_t> scenes;
		Storyboard storyboard;
		// Send audio file path to UE5 for Speech2Face processing
		void SendSkitToUnrealEngine(const skit_t& skit);
	public:
		
		ProductionWrangler(PiperTTS& actors) : tts(actors) {}

		ProductionWrangler(PiperTTS& actors, const std::string& script);

		ProductionWrangler(PiperTTS& actors, Storyboard& script) : tts(actors), storyboard(script) {}
				
		void load(const std::string& script);
		
		Storyboard getStoryboard() const {
			return storyboard;
		}
		
		Scene getScene(std::string& sceneId) {
			return scenes[sceneId];
		}

		Performance toPerformance(const cue_line_t& line);

		Take toTake(const cue_t& cue);

		Skit toSkit(const scene_t& scene);

		void perform(const scene_t& scene);

		std::string play(std::string sceneId);
	};
	// ---------------- Scene Manager ------------------
	class SceneManager {

	public:
		SceneManager(ProductionWrangler&  manager) : wrangler(manager) {}

		void LoadStoryboard(const std::string& script);

		void run(const std::string& startScene);

	private:
		ProductionWrangler& wrangler;
	};
}
