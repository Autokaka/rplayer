#include "jni_loader.h"

extern "C" {
JNIEXPORT jint JNI_OnLoad(JavaVM* jvm, void* reserved) {
  JNIEnv* env;

  if (jvm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
    return -1;
  }

  // register plugin and its methods
  av_jni_set_java_vm(jvm, nullptr);
  FFIAndroid::FFIRegister(jvm);
  TextureAndroid::FFIRegister(env);

  return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM* jvm, void* reserved) {
  FFI_ENV_ATTACH()

  TextureAndroid::FFIUnregister(env);

  FFI_ENV_DETACH()
}
}
