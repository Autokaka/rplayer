#pragma once

#include "ffi/ffi_android.h"
#include "texture/texture_android.h"

class PluginAndroid {
 private:
  static jclass _clazz;
  static jmethodID _createExternalTexture_MID;
  static jmethodID _releaseExternalTexture_MID;

 public:
  static void FFIRegister(JNIEnv*);
  static TextureAndroid* createTextureAndroid();
  static int releaseTextureAndroid(TextureAndroid*);
};
