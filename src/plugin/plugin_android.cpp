#include "plugin_android.h"

jclass PluginAndroid::_clazz;
jmethodID PluginAndroid::_createExternalTexture_MID;
jmethodID PluginAndroid::_releaseExternalTexture_MID;

void PluginAndroid::FFIRegister(JNIEnv* env) {
  jclass PluginAndroidClass =
      env->FindClass("com/example/rplayer/RplayerPlugin");
  _clazz = static_cast<jclass>(env->NewGlobalRef(PluginAndroidClass));
  _createExternalTexture_MID =
      env->GetStaticMethodID(_clazz, "createExternalTexture",
                             "()Lcom/example/rplayer/FlutterExternalTexture;");
  _releaseExternalTexture_MID =
      env->GetStaticMethodID(_clazz, "releaseExternalTexture",
                             "(Lcom/example/rplayer/FlutterExternalTexture;)V");
}

TextureAndroid* PluginAndroid::createTextureAndroid() {
  FFI_ENV_ATTACH(nullptr)

  jobject textureAndroid =
      env->CallStaticObjectMethod(_clazz, _createExternalTexture_MID);
  TextureAndroid* pTextureAndroid = new TextureAndroid(env, textureAndroid);

  /**
   * DO NOT use FFI_ENV_DETACH(nullptr) here
   * since we need to releaseTextureAndroid
   * if FFI_ENV_DETACH is failed.
   */
  if (FFIAndroid::detach() != 0) {
    releaseTextureAndroid(pTextureAndroid);
    return nullptr;
  }
  return pTextureAndroid;
}

int PluginAndroid::releaseTextureAndroid(TextureAndroid* pTextureAndroid) {
  FFI_ENV_ATTACH(-1)

  ANativeWindow_release(pTextureAndroid->nativeWindow);
  env->CallStaticVoidMethod(_clazz, _releaseExternalTexture_MID,
                            pTextureAndroid->thisObj);

  FFI_ENV_DETACH(-1)
  return 0;
}
