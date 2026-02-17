#ifndef _UNNU_FM_H
#define _UNNU_FM_H

#ifdef UNNU_FM_SHARED
#ifdef __cplusplus
#    if defined(_WIN32) && !defined(__MINGW32__)
#        ifdef UNNU_FM_BUILD
#            define UNNU_FM_API extern "C" __declspec(dllexport)
#        else
#            define UNNU_FM_API extern "C" __declspec(dllimport)
#        endif
#    else
#        define UNNU_FM_API extern "C" __attribute__((visibility("default"))) __attribute__((used))
#    endif
#else
#    if defined(_WIN32) && !defined(__MINGW32__)
#        ifdef UNNU_FM_BUILD
#            define UNNU_FM_API __declspec(dllexport)
#        else
#            define UNNU_FM_API __declspec(dllimport)
#        endif
#    else
#        define UNNU_FM_API __attribute__((visibility("default"))) __attribute__((used))
#    endif	
#endif
#else
#define UNNU_FM_API
#endif

#include "types.h"

UNNU_FM_API void unnu_fm_process_script(const char* script);

#endif //_UNNU_FM_H