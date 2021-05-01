#include "ffi_android.h"

JavaVM* FFIAndroid::_pJvm;

void FFIAndroid::FFIRegister(JavaVM* pJvm) {
  _pJvm = pJvm;
}

JNIEnv* FFIAndroid::attach() {
  if (_pJvm == nullptr) {
    LOG::E("Failed to attach to current JNIEnv: JVM not initialized.");
    return nullptr;
  }

  JNIEnv* env;
  int state = _pJvm->GetEnv((void**)&env, JNI_VERSION_1_6);
  if (state == JNI_EDETACHED || env == nullptr) {
    state = _pJvm->AttachCurrentThread(&env, nullptr);
    if (state < 0) {
      LOG::E("Failed to attach to current JNIEnv: code %d.", state);
      return nullptr;
    }
  }

  return env;
}

int FFIAndroid::detach() {
  if (jint state = _pJvm->DetachCurrentThread() != 0) {
    LOG::E("Failed to detach to current JNIEnv: code %d.", state);
    return static_cast<int>(state);
  }
  return 0;
}
