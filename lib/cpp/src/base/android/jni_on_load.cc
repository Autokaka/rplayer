/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/02.
 */

#include "jni_util.h"

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
  JNIEnv* env;
  auto ret = vm->GetEnv((void**)&env, JNI_VERSION_1_4);
  JNI_CHECK(ret == JNI_OK, "");

  jni::InitJavaVM(vm);

  return JNI_VERSION_1_4;
}
