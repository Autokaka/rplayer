#pragma once

#include <android/native_window_jni.h>

class RplayerDart {
public:
    RplayerDart(char* url, ANativeWindow* nativeWindow);
    char* url;
    ANativeWindow* nativeWindow;
};
