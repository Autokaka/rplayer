#pragma once

#include <android/native_window_jni.h>
#include <jni.h>

#include "ffi/ffi_android.h"
#include "log.h"

class TextureAndroid {
 private:
  static jclass _clazz;

 public:
  jobject thisObj;
  ANativeWindow* nativeWindow;

 public:
  static void FFIRegister(JNIEnv*);
  TextureAndroid(JNIEnv*, jobject);
  jlong getId();
};
