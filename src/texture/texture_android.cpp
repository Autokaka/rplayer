#include "texture_android.h"

jclass TextureAndroid::_clazz;

void TextureAndroid::FFIRegister(JNIEnv* env) {
  jclass TextureAndroidClass =
      env->FindClass("com/example/rplayer/FlutterExternalTexture");
  _clazz = static_cast<jclass>(env->NewGlobalRef(TextureAndroidClass));
}

TextureAndroid::TextureAndroid(JNIEnv* env, jobject obj) {
  thisObj = env->NewGlobalRef(obj);

  jfieldID surface_FID =
      env->GetFieldID(_clazz, "surface", "Landroid/view/Surface;");
  jobject surface = env->GetObjectField(thisObj, surface_FID);
  nativeWindow = ANativeWindow_fromSurface(env, surface);
}

jlong TextureAndroid::getId() {
  FFI_ENV_ATTACH(-1)

  jfieldID id_FID = env->GetFieldID(_clazz, "id", "J");
  jlong id = env->GetLongField(thisObj, id_FID);

  FFI_ENV_DETACH(-1)
  return id;
}
