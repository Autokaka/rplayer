#pragma once

#include "ffi_android.h"
#include "texture/texture_android.h"

extern "C" {
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM*, void*);

JNIEXPORT void JNI_OnUnload(JavaVM*, void*);
}
