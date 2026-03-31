#pragma once

#include "c-api.h"
#include <set>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>

namespace unnufm {

	// =====================
	// Data Structures
	// =====================
	
	
	static unnufm_motion_t unnufm_motion_from_string(const std::string & type_str) {
		if (type_str == "stand") {
			return unnufm_motion::MOTION_STAND;
		}
		if (type_str == "sit") {
			return unnufm_motion::MOTION_SIT;
		}
		if (type_str == "lie") {
			return unnufm_motion::MOTION_LIE;
		}
		if (type_str == "walk") {
			return unnufm_motion::MOTION_WALK;
		}
		if (type_str == "jog") {
			return unnufm_motion::MOTION_JOG;
		}
		if (type_str == "run") {
			return unnufm_motion::MOTION_RUN;
		}
		return unnufm_motion::MOTION_NONE;
	}

	static std::string unnufm_motion_to_string(unnufm_motion type) {
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
	

	static unnufm_scene_timing unnufm_scene_timing_from_string(const std::string & type_str) {
		if (type_str == "sequential") {
			return unnufm_scene_timing::TIMING_SEQUENTIAL;
		}
		if (type_str == "simultaneous") {
			return unnufm_scene_timing::TIMING_SIMULTANEOUS;
		}
		if (type_str == "overlap") {
			return unnufm_scene_timing::TIMING_OVERLAP;
		}
		return unnufm_scene_timing::TIMING_SEQUENTIAL;
	}

	static std::string unnufm_scene_timing_to_string(unnufm_scene_timing type) {
		switch (type) {
			case TIMING_SEQUENTIAL: return "sequential";
			case TIMING_SIMULTANEOUS: return "simultaneous";
			case TIMING_OVERLAP: return "overlap";
			default:  return "sequential";
		}
	}
	
	static unnufm_sound_cue_type sound_cue_type_from_string(const std::string & type_str) {
		if (type_str == "sfx") {
			return unnufm_sound_cue_type::SOUND_SFX;
		}
		if (type_str == "music") {
			return unnufm_sound_cue_type::SOUND_MUSIC;
		}
		if (type_str == "ambient") {
			return unnufm_sound_cue_type::SOUND_AMBIENT;
		}
		if (type_str == "default") {
			return unnufm_sound_cue_type::SOUND_DEFAULT;
		}
		return unnufm_sound_cue_type::SOUND_DEFAULT;
	}

	static std::string sound_cue_type_to_string(unnufm_sound_cue_type type) {
		switch (type) {
			case SOUND_SFX: return "sfx";
			case SOUND_MUSIC: return "music";
			case SOUND_AMBIENT: return "ambient";
			case SOUND_DEFAULT: return "default";
			default:  return "default";
		}
	}


	static unnufm_camera_shot_size camera_shot_size_from_string(const std::string & size_str) {
		if (size_str == "default") {
			return unnufm_camera_shot_size::CAMERA_SIZE_DEFAULT;
		}
		if (size_str == "medium") {
			return unnufm_camera_shot_size::CAMERA_SIZE_MEDIUM;
		}
		if (size_str == "full") {
			return unnufm_camera_shot_size::CAMERA_SIZE_FULL;
		}
		if (size_str == "close_up") {
			return unnufm_camera_shot_size::CAMERA_SIZE_CLOSE_UP;
		}
		if (size_str == "wide") {
			return unnufm_camera_shot_size::CAMERA_SIZE_WIDE;
		}
		if (size_str == "cowboy") {
			return unnufm_camera_shot_size::CAMERA_SIZE_COWBOY;
		}
		if (size_str == "medium_wide") {
			return unnufm_camera_shot_size::CAMERA_SIZE_MEDIUM_WIDE;
		}
		if (size_str == "medium_close_up") {
			return unnufm_camera_shot_size::CAMERA_SIZE_MEDIUM_CLOSE_UP;
		}
		if (size_str == "extreme_close_up") {
			return unnufm_camera_shot_size::CAMERA_SIZE_EXTREME_CLOSE_UP;
		}
		if (size_str == "extreme_wide") {
			return unnufm_camera_shot_size::CAMERA_SIZE_EXTREME_WIDE;
		}
		return unnufm_camera_shot_size::CAMERA_SIZE_DEFAULT;
	}

	static std::string camera_shot_size_to_string(unnufm_camera_shot_size type) {
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

	static unnufm_camera_shot_angle camera_shot_angle_from_string(const std::string & angle_str) {
		if (angle_str == "default") {
			return unnufm_camera_shot_angle::CAMERA_ANGLE_DEFAULT;
		}
		if (angle_str == "eye_level") {
			return unnufm_camera_shot_angle::CAMERA_ANGLE_EYE_LEVEL;
		}
		if (angle_str == "shoulder_level") {
			return unnufm_camera_shot_angle::CAMERA_ANGLE_SHOULDER_LEVEL;
		}
		if (angle_str == "hip_level") {
			return unnufm_camera_shot_angle::CAMERA_ANGLE_HIP_LEVEL;
		}
		if (angle_str == "knee_level") {
			return unnufm_camera_shot_angle::CAMERA_ANGLE_KNEE_LEVEL;
		}
		if (angle_str == "ground_level") {
			return unnufm_camera_shot_angle::CAMERA_ANGLE_GROUND_LEVEL;
		}
		if (angle_str == "low") {
			return unnufm_camera_shot_angle::CAMERA_ANGLE_LOW;
		}
		if (angle_str == "high") {
			return unnufm_camera_shot_angle::CAMERA_ANGLE_HIGH;
		}
		if (angle_str == "overhead") {
			return unnufm_camera_shot_angle::CAMERA_ANGLE_OVERHEAD;
		}
		return unnufm_camera_shot_angle::CAMERA_ANGLE_DEFAULT;
	}

	static std::string camera_shot_angle_to_string(unnufm_camera_shot_angle type) {
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

	static unnufm_camera_shot_movement camera_shot_movement_from_string(const std::string & move_str) {
		if (move_str == "fixed") {
			return unnufm_camera_shot_movement::CAMERA_MOVEMENT_FIXED;
		}
		if (move_str == "pan_left") {
			return unnufm_camera_shot_movement::CAMERA_MOVEMENT_PAN_LEFT;
		}
		if (move_str == "pan_right") {
			return unnufm_camera_shot_movement::CAMERA_MOVEMENT_PAN_RIGHT;
		}
		if (move_str == "tilt_up") {
			return unnufm_camera_shot_movement::CAMERA_MOVEMENT_TILT_UP;
		}
		if (move_str == "tilt_down") {
			return unnufm_camera_shot_movement::CAMERA_MOVEMENT_TILT_DOWN;
		}
		if (move_str == "push_in") {
			return unnufm_camera_shot_movement::CAMERA_MOVEMENT_PUSH_IN;
		}
		if (move_str == "pull_out") {
			return unnufm_camera_shot_movement::CAMERA_MOVEMENT_PULL_OUT;
		}
		if (move_str == "zoom_in") {
			return unnufm_camera_shot_movement::CAMERA_MOVEMENT_ZOOM_IN;
		}
		if (move_str == "zoom_out") {
			return unnufm_camera_shot_movement::CAMERA_MOVEMENT_ZOOM_OUT;
		}
		if (move_str == "tracking") {
			return unnufm_camera_shot_movement::CAMERA_MOVEMENT_TRACKING;
		}
		if (move_str == "trucking") {
			return unnufm_camera_shot_movement::CAMERA_MOVEMENT_TRUCKING;
		}
		return unnufm_camera_shot_movement::CAMERA_MOVEMENT_FIXED;
	}

	static std::string camera_shot_movement_to_string(unnufm_camera_shot_movement type) {
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
	
	typedef struct unnufm_speaker_ref_deleter {
		void operator()(unnufm_speaker_ref_t* speaker) const {
			if (speaker) {
				if (speaker->speaker) { free(speaker->speaker); speaker->speaker = NULL; }
				free(speaker);
			}
		}
	} unnufm_speaker_ref_deleter_t;

	typedef std::unique_ptr<unnufm_speaker_ref_t, unnufm_speaker_ref_deleter_t> unnufm_speaker_ref_ptr;

	typedef struct unnufm_speaker_list_deleter {
		void operator()(unnufm_speaker_list_t* list) const {
			if (list) {
				if (list->n_speakers > 0 && list->speakers) {
					for (int i = 0; i < list->n_speakers; i++) {
						if (list->speakers[i]) unnufm_speaker_ref_ptr(list->speakers[i]).reset();
					}
					free(list->speakers); list->speakers = NULL;
				}
				free(list);
			}
		}
	} unnufm_speaker_list_deleter_t;

	typedef std::unique_ptr<unnufm_speaker_list_t, unnufm_speaker_list_deleter_t> unnufm_speaker_list_ptr;

	typedef struct sound_cue_deleter {
		void operator()(unnufm_sound_cue_t* cue) const {
			if (cue) {
				if (cue->uri) { free(cue->uri); cue->uri = NULL; }
				free(cue);
			}
		}
	} sound_cue_deleter_t;

	typedef std::unique_ptr<unnufm_sound_cue_t, sound_cue_deleter_t> sound_cue_ptr;

	typedef struct camera_cue_deleter {
		void operator()(unnufm_camera_cue_t* cue) const {
			if (cue) {
				if (cue->target) { free(cue->target); cue->target = NULL; }
				free(cue);
			}
		}
	} camera_cue_deleter_t;

	typedef std::unique_ptr<unnufm_camera_cue_t, camera_cue_deleter_t> camera_cue_ptr;

	typedef struct action_deleter {
		void operator()(unnufm_action_t* action) const {
			if (action) {
				if (action->gesture) {
					free(action->gesture); action->gesture = NULL;
				}
				if (action->target) { free(action->target); action->target = NULL; }
				free(action);
			}
		}
	} action_deleter_t;

	typedef std::unique_ptr<unnufm_action_t, action_deleter_t> action_ptr;

	typedef struct cue_line_deleter {
		void operator()(unnufm_cue_line_t* line) const {
			if (line) {
				if (line->speaker) {free(line->speaker); line->speaker = NULL; }
				if (line->text) { free(line->text); line->text = NULL; }
				if(line->n_actions > 0 && line->actions) {
					for (int i = 0; i < line->n_actions; i++) {
						if(line->actions[i]) action_ptr(line->actions[i]).reset();
					}
					free(line->actions); line->actions = NULL;
				}
				free(line);
			}
		}
	} cue_line_deleter_t;

	typedef std::unique_ptr<unnufm_cue_line_t, cue_line_deleter_t> cue_line_ptr;
	
	typedef struct audio_chunk_deleter {
		void operator()(unnufm_audio_chunk_t* chunk) const {
			if (chunk) {
				if (chunk->n_samples > 0 && chunk->samples) { free(chunk->samples); chunk->samples = NULL; }
				free(chunk);
			}
		}
	} audio_chunk_deleter_t;

	typedef std::unique_ptr<unnufm_audio_chunk_t, audio_chunk_deleter_t> audio_chunk_ptr;

	

	typedef struct audio_deleter {
		void operator()(unnufm_audio_t* audio) const {
			if (audio) {
				if (audio->n_text > 0 && audio->text) { free(audio->text); audio->text = NULL; }
				if (audio->chunk) audio_chunk_ptr(audio->chunk).reset();
				free(audio);
			}
		}
	} audio_deleter_t;

	typedef std::unique_ptr<unnufm_audio_t, audio_deleter_t> audio_ptr;

	typedef struct performance_deleter {
		void operator()(unnufm_performance_t* performance) const {
			if (performance) {
				if(performance->audio) audio_ptr(performance->audio).reset();
				// if (performance->line) cue_line_ptr(performance->line).reset();
				delete performance;
			}
		}
	} performance_deleter_t;

	typedef std::unique_ptr<unnufm_performance_t, performance_deleter_t> performance_ptr;

	typedef struct cue_deleter {
		void operator()(unnufm_cue_t* cue) const {
			if (cue) {
				if(cue->n_lines > 0 && cue->lines) {
					 for (int i = 0; i < cue->n_lines; i++) {
						if(cue->lines[i]) cue_line_ptr(cue->lines[i]).reset();
					}
					free(cue->lines); cue->lines = NULL;
				}
				if(cue->n_sounds > 0 && cue->sounds) {
					for (int i = 0; i < cue->n_sounds; i++) {
						if(cue->sounds[i]) sound_cue_ptr(cue->sounds[i]).reset();
					}
					free(cue->sounds); cue->sounds = NULL;
				}
				if (cue->n_shots > 0 && cue->shots) {
					for (int i = 0; i < cue->n_shots; i++) {
						if(cue->shots[i]) camera_cue_ptr(cue->shots[i]).reset();
					}
					free(cue->shots); cue->shots = NULL;
				}
				free(cue);
			}
		}
	} cue_deleter_t;

	typedef std::unique_ptr<unnufm_cue_t, cue_deleter_t> cue_ptr;
	
	

	typedef struct take_deleter {
		void operator()(unnufm_take_t* take) const {
			if (take) {
				if (take->n_performances > 0 && take->performances) {
					for (int i = 0; i < take->n_performances; i++) {
						if (take->performances[i]) performance_ptr(take->performances[i]).reset();
					}
					free(take->performances); take->performances = NULL;
				}
				if (take->n_sounds > 0 && take->sounds) {
					for (int i = 0; i < take->n_sounds; i++) {
						if (take->sounds[i]) sound_cue_ptr(take->sounds[i]).reset();
					}
					free(take->sounds); take->sounds = NULL;
				}
				if (take->n_shots > 0 && take->shots) {
					for (int i = 0; i < take->n_shots; i++) {
						if (take->shots[i]) camera_cue_ptr(take->shots[i]).reset();
					}
					free(take->shots); take->shots = NULL;
				}
				free(take);
			}
		}
	} take_deleter_t;

	typedef std::unique_ptr<unnufm_take_t, take_deleter_t> take_ptr;

	typedef struct scene_deleter {
		void operator()(unnufm_scene_t* scene) const {
			if (scene) {
				if (scene->n_sceneID > 0 && scene->sceneID) { free(scene->sceneID); scene->sceneID = NULL; }
				if (scene->n_location > 0 && scene->location) { free(scene->location); scene->location = NULL; }
				if (scene->n_stage > 0 && scene->stage) { free(scene->stage); scene->stage = NULL; }
				if (scene->n_jump > 0 && scene->jump) { free(scene->jump); scene->jump = NULL; }
				if(scene->n_cues > 0 && scene->cues) {
					 for (int i = 0; i < scene->n_cues; i++) {
						if(scene->cues[i]) cue_ptr(scene->cues[i]).reset();
					}
					free(scene->cues); scene->cues = NULL;
				}
				free(scene);
			}
		}
	} scene_deleter_t;

	typedef std::unique_ptr<unnufm_scene_t, scene_deleter_t> scene_ptr;

	

	typedef struct skit_deleter {
		void operator()(unnufm_skit_t* skit) const {
			if (skit) {
				if (skit->n_sceneID > 0 && skit->sceneID) { free(skit->sceneID); skit->sceneID = NULL; }
				if (skit->n_location > 0 && skit->location) { free(skit->location); skit->location = NULL; }
				if (skit->n_stage > 0 && skit->stage) { free(skit->stage); skit->stage = NULL; }
				if (skit->n_jump > 0 && skit->jump) { free(skit->jump); skit->jump = NULL; }
				if (skit->n_takes > 0 && skit->takes) {
					for (int i = 0; i < skit->n_takes; i++) {
						if (skit->takes[i]) take_ptr(skit->takes[i]).reset();
					}
					free(skit->takes); skit->takes = NULL;
				}
				free(skit);
			}
		}
	} skit_deleter_t;

	typedef std::unique_ptr<unnufm_skit_t, skit_deleter_t> skit_ptr;

	typedef struct storyboard_deleter {
		void operator()(unnufm_storyboard_t* story) const {
			if (story) {
				if (story->n_scenes > 0 && story->scenes) {
					for (int i = 0; i < story->n_scenes; i++)
					{
						if (story->scenes[i]) scene_ptr(story->scenes[i]).reset();
					}
					free(story->scenes);
				}
				free(story);
			}
		}
	} storyboard_deleter_t;

	typedef std::unique_ptr<unnufm_storyboard_t, storyboard_deleter_t> storyboard_ptr;
	
	// ---------------- Storyboard Class ----------------
	class Storyboard {

	public:
		
		Storyboard() = default;
		Storyboard(const Storyboard& obj) = default;
		Storyboard(Storyboard&& obj) noexcept = default;
		Storyboard& operator=(const Storyboard& obj) = default;
		Storyboard& operator=(Storyboard&& obj) noexcept = default;

		~Storyboard() {
			if (storyboard) {
				storyboard.reset();
			}
		}

		Storyboard(unnufm_storyboard_t* story);

		Storyboard(const char* script);

		void update(unnufm_storyboard_t* story) {
			storyboard.reset(story);
		}

		unnufm_storyboard_t* getScenes() const {
			return storyboard.get();
		}

		std::string exportToEDL() const;
		
		std::string exportToPremiereXML(float fps) const;
		
		static std::string formatSRTTime(double seconds);
		
		std::string exportToSRT() const;

		std::string firstSceneID() const;
	private:	
		storyboard_ptr storyboard;
		void parse(const char* script);
		
	};

	// =====================
	// Piper TTS
	// =====================
	class PiperTTS {
	public:
		static unnufm_audio_t* speak(unnufm_cue_line_t* line, const unnufm_speaker_list_t* voices);

		static unnufm_speaker_properties_t find_speaker(const unnufm_speaker_list_t* actors, const char* speaker);

	};

	// ---------------- Skit Grabber ----------------
	class SkitGrabber {
	public:
		static bool grab(unnufm_skit_t& skit);
	};

	// ---------------- Script Parser ----------------
	class ScriptParser {
	public:
		static unnufm_storyboard_t* toStoryboard(nlohmann::json j);
		static unnufm_storyboard_t* toStoryboard(const char* script);
		static void toStoryboardByRef(const char* script, Storyboard* story);
		static std::set<std::string> getCreditedActorsInStoryboard(Storyboard storyboard);
		static std::set<std::string> getActorsInSkit(unnufm_skit_t skit);
		static std::set<std::string> getCreditedActorsInScene(unnufm_scene_t scene);
		static std::set<std::string> getCreditedActorsInTake(unnufm_take_t take);
	};


    // Replace the default constructor for ProductionWrangler with one that takes a PiperTTS reference
    class ProductionWrangler {
    private:
					static void SendSkitToUnrealEngine(unnufm_skit_t skit);
    public:
					static unnufm_performance_t* toPerformance(const unnufm_cue_line_t* line, const unnufm_speaker_list_t* voices);
					static unnufm_take_t* toTake(const unnufm_cue_t* cue, const unnufm_speaker_list_t* voices);
					static unnufm_skit_t* toSkit(const unnufm_scene_t* scene, const unnufm_speaker_list_t* voices);
					static void perform(const unnufm_scene_t* scene, const unnufm_speaker_list_t* voices);
					static void play(const unnufm_scene_t* scene, const unnufm_speaker_list_t* voices);
    };
	// ---------------- Scene Manager ------------------
	class SceneManager {

	public:
		SceneManager() = default;

		void LoadStoryboard(const std::string& script);

		void run(const unnufm_storyboard_t* story, const unnufm_speaker_list_t* voices);

	//private:
	//	ProductionWrangler wrangler;
	};
}
