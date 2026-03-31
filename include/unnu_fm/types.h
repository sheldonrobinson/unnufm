#ifndef _UNNU_FM_TYPES_H
#define _UNNU_FM_TYPES_H

#ifdef __cplusplus
	#include <cstdint>
	#include <cstdbool>
#else // __cplusplus - Objective-C or other C platform
	#include <stdint.h>
	#include <stdbool.h>
#endif

#include <unnu_tts/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum unnufm_scene_timing {
	TIMING_SEQUENTIAL = 0,
	TIMING_SIMULTANEOUS = 1,
	TIMING_OVERLAP = 2
} unnufm_scene_timing_t;

typedef enum unnufm_motion {
	MOTION_NONE = 0,
	MOTION_STAND = 1,
	MOTION_SIT = 2,
	MOTION_LIE = 3,
	MOTION_WALK = 4,
	MOTION_JOG = 5,
	MOTION_RUN = 6
} unnufm_motion_t;

typedef enum unnufm_sound_cue_type {
	SOUND_DEFAULT = 0,
	SOUND_AMBIENT = 1,
	SOUND_SFX = 2,
	SOUND_MUSIC = 3
} unnufm_sound_cue_type_t;



typedef enum unnufm_camera_shot_size {
	CAMERA_SIZE_DEFAULT = 0,
	CAMERA_SIZE_MEDIUM = 1,
	CAMERA_SIZE_FULL = 2,
	CAMERA_SIZE_CLOSE_UP = 3,
	CAMERA_SIZE_WIDE = 4,
	CAMERA_SIZE_COWBOY = 5,
	CAMERA_SIZE_MEDIUM_WIDE = 6,
	CAMERA_SIZE_MEDIUM_CLOSE_UP = 7,
	CAMERA_SIZE_EXTREME_CLOSE_UP = 8,
	CAMERA_SIZE_EXTREME_WIDE = 9
} unnufm_camera_shot_size_t;


typedef enum unnufm_camera_shot_angle {
	CAMERA_ANGLE_DEFAULT = 0,
	CAMERA_ANGLE_EYE_LEVEL = 1,
	CAMERA_ANGLE_SHOULDER_LEVEL = 2,
	CAMERA_ANGLE_HIP_LEVEL = 3,
	CAMERA_ANGLE_KNEE_LEVEL = 4,
	CAMERA_ANGLE_GROUND_LEVEL = 5,
	CAMERA_ANGLE_LOW = 6,
	CAMERA_ANGLE_HIGH = 7,
	CAMERA_ANGLE_OVERHEAD = 8
} unnufm_camera_shot_angle_t;



typedef enum unnufm_camera_shot_movement {
	CAMERA_MOVEMENT_FIXED = 0,
	CAMERA_MOVEMENT_PAN_LEFT = 1,
	CAMERA_MOVEMENT_PAN_RIGHT = 2,
	CAMERA_MOVEMENT_TILT_UP = 3,
	CAMERA_MOVEMENT_TILT_DOWN = 4,
	CAMERA_MOVEMENT_PUSH_IN = 5,
	CAMERA_MOVEMENT_PULL_OUT = 6,
	CAMERA_MOVEMENT_ZOOM_IN = 7,
	CAMERA_MOVEMENT_ZOOM_OUT = 8,
	CAMERA_MOVEMENT_TRACKING = 9,
	CAMERA_MOVEMENT_TRUCKING = 10
} unnufm_camera_shot_movement_t;

typedef struct unnufm_speaker_properties {
	int32_t speaker_id;
	int8_t is_robot;
} unnufm_speaker_properties_t;

typedef struct unnufm_speaker_ref {
	char* speaker;
	int32_t n_speaker;
	unnufm_speaker_properties_t properties;
} unnufm_speaker_ref_t;

typedef struct unnufm_speaker_list {
	unnufm_speaker_ref_t** speakers;
	int32_t n_speakers;
} unnufm_speaker_list_t;

typedef struct unnufm_sound_cue {
	unnufm_sound_cue_type type = unnufm_sound_cue_type::SOUND_DEFAULT; // "music", "sfx", "ambient"
	char* uri = NULL;
	int32_t n_uri = 0;
	double time = 0.0;
} unnufm_sound_cue_t;

typedef struct unnufm_camera_cue {
	unnufm_camera_shot_size size = unnufm_camera_shot_size::CAMERA_SIZE_DEFAULT;
	unnufm_camera_shot_angle angle = unnufm_camera_shot_angle::CAMERA_ANGLE_DEFAULT;
	unnufm_camera_shot_movement movement = unnufm_camera_shot_movement::CAMERA_MOVEMENT_FIXED;
	char* target = NULL;
	int32_t n_target = 0;
	double duration = 0.0;
	double transition = 0.0;
} unnufm_camera_cue_t;

typedef struct unnufm_action {
	unnufm_motion_t motion = unnufm_motion::MOTION_NONE;
	char* gesture = NULL;
	int32_t n_gesture = 0;
	char* target = NULL;
	int32_t n_target = 0;
	double delay = 0.0;
	bool persistent = false;
} unnufm_action_t;

typedef struct unnufm_cue_line {
	char* speaker = NULL;
	int32_t n_speaker = 0;
	char* text = NULL;
	int32_t n_text = 0;
	double pause = 0.0;
	double delay = 0.0;
	unnufm_action_t** actions;
	int32_t n_actions = 0;
	EEMOTION emotion = EEMOTION::EMOTION_NEUTRAL;
	bool lipSync = true;
} unnufm_cue_line_t;

typedef struct unnufm_audio_chunk {
	float* samples;
	int32_t n_samples = 0;
	int sample_rate = 0;
} unnufm_audio_chunk_t;

typedef struct unnufm_audio {
	unnufm_audio_chunk_t* chunk;
	char* text = NULL;
	int32_t n_text = 0;
} unnufm_audio_t;

typedef struct unnufm_performance {
	unnufm_audio_t* audio;
	unnufm_cue_line_t* line;
} unnufm_performance_t;

typedef struct unnufm_cue {
	unnufm_scene_timing_t timing = unnufm_scene_timing::TIMING_SEQUENTIAL; // sequential, simultaneous, overlap, branch
	double stagger = 0.7; // NEW: default stagger for overlap
	bool dynamic = false; // NEW: allow real-time modification
	unnufm_cue_line_t** lines;
	int32_t n_lines = 0;
	unnufm_sound_cue_t** sounds;
	int32_t n_sounds = 0;
	unnufm_camera_cue_t** shots;
	int32_t n_shots = 0;
} unnufm_cue_t;

typedef struct unnufm_take {
	unnufm_scene_timing_t timing = unnufm_scene_timing::TIMING_SEQUENTIAL;; // sequential, simultaneous, overlap, branch
	double stagger = 0.7; // NEW: default stagger for overlap
	bool dynamic = false; // NEW: allow real-time modification
	unnufm_performance_t** performances;
	int32_t n_performances = 0;
	unnufm_sound_cue_t** sounds;
	int32_t n_sounds = 0;
	unnufm_camera_cue_t** shots;
	int32_t n_shots = 0;
} unnufm_take_t;

typedef struct unnufm_scene {
	char* sceneID = NULL;
	int32_t n_sceneID = 0;
	char* location = NULL;
	int32_t n_location = 0;
	char* stage = NULL;
	int32_t n_stage = 0;
	char* jump = NULL;
	int32_t n_jump = 0;
	unnufm_cue_t** cues;
	int32_t n_cues = 0;
} unnufm_scene_t;

typedef struct unnufm_skit {
	char* sceneID = NULL;
	int32_t n_sceneID = 0;
	char* location = NULL;
	int32_t n_location = 0;
	char* stage = NULL;
	int32_t n_stage = 0;
	char* jump = NULL;
	int32_t n_jump = 0;
	unnufm_take_t** takes;
	int32_t n_takes = 0;
} unnufm_skit_t;

typedef struct unnufm_storyboard {
	unnufm_scene_t** scenes;
	int32_t n_scenes;
} unnufm_storyboard_t;

#ifdef __cplusplus
}
#endif



#endif // _UNNU_FM_TYPES_H