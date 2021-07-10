#pragma once

#include "ffi_android.h"
#include "texture/texture_android.h"

extern "C" {
#include <libavcodec/jni.h>

JNIEXPORT jint JNI_OnLoad(JavaVM*, void*);

JNIEXPORT void JNI_OnUnload(JavaVM*, void*);
}
