#pragma once

#include <jni.h>
#include <cassert>
#include <vector>

#include "base/macros.h"
#include "scoped_java_ref.h"

#define JNI_CHECK(exp, msg) assert(((void)msg, exp))

#define JNI_EXCEPTION_CHECK(env)               \
  JNI_CHECK(jni::ClearException(env) == false, \
            "[FATAL] JNI throws "              \
            "exception.")

namespace jni {

void InitJavaVM(JavaVM* vm);

JNIEnv* AttachCurrentThread();

void DetachFromVM();

std::string JavaStringToString(JNIEnv* env, jstring string);

ScopedJavaLocalRef<jstring> StringToJavaString(JNIEnv* env,
                                               const std::string& str);

std::vector<std::string> StringArrayToVector(JNIEnv* env, jobjectArray jargs);

ScopedJavaLocalRef<jobjectArray> VectorToStringArray(
    JNIEnv* env,
    const std::vector<std::string>& vector);

bool HasException(JNIEnv* env);

bool ClearException(JNIEnv* env);

std::string GetJavaExceptionInfo(JNIEnv* env, jthrowable java_throwable);

}  // namespace jni
