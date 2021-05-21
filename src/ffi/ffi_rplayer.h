#pragma once

#include "config.h"
#include "player.h"

#define DART_EXPORT __attribute__((visibility("default"))) __attribute__((used))

extern "C" {
//////////////////////////////////////////
// RPlayer
//////////////////////////////////////////

DART_EXPORT void* RPlayer_createInstance();

DART_EXPORT int RPlayer_createDecodeThread(void*, char*);

DART_EXPORT void RPlayer_setPlaying(void*);

DART_EXPORT void RPlayer_setPaused(void*);

DART_EXPORT void RPlayer_setConfig(void*, void*);

DART_EXPORT void RPlayer_dispose(void*);

DART_EXPORT int RPlayer_getHeight(void*);

DART_EXPORT int RPlayer_getWidth(void*);

DART_EXPORT int RPlayer_getState(void*);

DART_EXPORT char* RPlayer_getMessage(void*);

DART_EXPORT long long RPlayer_getTextureId(void*);

//////////////////////////////////////////
// RPlayerConfig
//////////////////////////////////////////

DART_EXPORT void* RPlayerConfig_createInstance();

DART_EXPORT void RPlayerConfig_setRetryTimesOnDisconnect(void*, int);

DART_EXPORT void RPlayerConfig_setRetryDelayInMilliseconds(void*, int);

DART_EXPORT void RPlayerConfig_setPreferHardcodec(void*, int);
}
