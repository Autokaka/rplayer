#pragma once

#include <android/native_window_jni.h>
#include <jni.h>

#include "ffi/ffi_android.h"
#include "util/log.h"

class TextureAndroid {
 private:
  static jclass _TextureAndroidClass;
  jobject _textureAndroid;

 public:
  long long id = -1;
  ANativeWindow* nativeWindow;

 public:
  static void FFIRegister(JNIEnv*);
  static void FFIUnregister(JNIEnv*);
  TextureAndroid();
  int release();
};
