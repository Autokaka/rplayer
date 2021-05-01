#pragma once

#include "player.h"

#define DART_EXPORT __attribute__((visibility("default"))) __attribute__((used))

extern "C" {
DART_EXPORT void* RPlayer_createInstance();

DART_EXPORT int RPlayer_createDecodeThread(void*, char*);

DART_EXPORT void RPlayer_setPlaying(void*);

DART_EXPORT void RPlayer_setPaused(void*);

DART_EXPORT int RPlayer_dispose(void*);

DART_EXPORT int RPlayer_getHeight(void*);

DART_EXPORT int RPlayer_getWidth(void*);

DART_EXPORT int RPlayer_getState(void*);

DART_EXPORT long RPlayer_getTextureId(void*);
}
