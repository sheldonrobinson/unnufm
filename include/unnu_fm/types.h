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

typedef enum unnu_scene_timing {
	TIMING_SEQUENTIAL = 0,
	TIMING_SIMULTANEOUS = 1,
	TIMING_OVERLAP = 2
} unnu_scene_timing_t;

typedef enum unnu_motion {
	MOTION_NONE = 0,
	MOTION_STAND = 1,
	MOTION_SIT = 2,
	MOTION_LIE = 3,
	MOTION_WALK = 4,
	MOTION_JOG = 5,
	MOTION_RUN = 6
} unnu_motion_t;

typedef enum unnu_sound_cue_type {
	SOUND_DEFAULT = 0,
	SOUND_AMBIENT = 1,
	SOUND_SFX = 2,
	SOUND_MUSIC = 3
} unnu_sound_cue_type_t;



typedef enum unnu_camera_shot_size {
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
} unnu_camera_shot_size_t;


typedef enum unnu_camera_shot_angle {
	CAMERA_ANGLE_DEFAULT = 0,
	CAMERA_ANGLE_EYE_LEVEL = 1,
	CAMERA_ANGLE_SHOULDER_LEVEL = 2,
	CAMERA_ANGLE_HIP_LEVEL = 3,
	CAMERA_ANGLE_KNEE_LEVEL = 4,
	CAMERA_ANGLE_GROUND_LEVEL = 5,
	CAMERA_ANGLE_LOW = 6,
	CAMERA_ANGLE_HIGH = 7,
	CAMERA_ANGLE_OVERHEAD = 8
} unnu_camera_shot_angle_t;



typedef enum unnu_camera_shot_movement {
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
} unnu_camera_shot_movement_t;

#ifdef __cplusplus
}
#endif



#endif // _UNNU_FM_TYPES_H