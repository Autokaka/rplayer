#pragma once

#include <jni.h>

#include "util/log.h"

#define FFI_ENV_ATTACH(VALUE_ON_FAILURE) \
  JNIEnv* env = FFIAndroid::attach();    \
  if (env == nullptr) {                  \
    return VALUE_ON_FAILURE;             \
  }

#define FFI_ENV_DETACH(VALUE_ON_FAILURE) \
  if (FFIAndroid::detach() != 0) {       \
    return VALUE_ON_FAILURE;             \
  }

class FFIAndroid {
 private:
  static JavaVM* _pJvm;

 public:
  static void FFIRegister(JavaVM*);
  static JNIEnv* attach();
  static int detach();
};
