#pragma once

#include "ffi_android.h"
#include "texture/texture_android.h"
#include "plugin/plugin_android.h"

extern "C" {
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* jvm, void* reserved);
}