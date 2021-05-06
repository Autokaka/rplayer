#include "texture_android.h"

jclass TextureAndroid::_TextureAndroidClass;

void TextureAndroid::FFIRegister(JNIEnv* env) {
  jclass TextureAndroidClass =
      env->FindClass("com/example/rplayer/TextureAndroid");
  _TextureAndroidClass =
      reinterpret_cast<jclass>(env->NewGlobalRef(TextureAndroidClass));
}

void TextureAndroid::FFIUnregister(JNIEnv* env) {
  env->DeleteGlobalRef(_TextureAndroidClass);
}

TextureAndroid::TextureAndroid() {
  FFI_ENV_ATTACH()

  jmethodID createTextureAndroid =
      env->GetMethodID(_TextureAndroidClass, "<init>", "()V");
  _textureAndroid = env->NewObject(_TextureAndroidClass, createTextureAndroid);
  _textureAndroid = env->NewGlobalRef(_textureAndroid);

  jfieldID surfaceField = env->GetFieldID(_TextureAndroidClass, "surface",
                                          "Landroid/view/Surface;");
  jobject surface;
  while ((surface = env->GetObjectField(_textureAndroid, surfaceField)) == NULL)
    ;

  jfieldID idField = env->GetFieldID(_TextureAndroidClass, "id", "J");
  id = static_cast<long long>(env->GetLongField(_textureAndroid, idField));

  nativeWindow = ANativeWindow_fromSurface(env, surface);

  FFI_ENV_DETACH()
}

int TextureAndroid::release() {
  FFI_ENV_ATTACH(-1)

  ANativeWindow_release(nativeWindow);
  jmethodID release = env->GetMethodID(_TextureAndroidClass, "release", "()V");
  env->CallVoidMethod(_textureAndroid, release);
  env->DeleteGlobalRef(_textureAndroid);

  FFI_ENV_DETACH(-1)
  return 0;
}
