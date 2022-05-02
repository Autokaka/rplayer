#include "scoped_java_ref.h"
#include "jni_util.h"

namespace jni {

static const int kDefaultLocalFrameCapacity = 16;

ScopedJavaLocalFrame::ScopedJavaLocalFrame(JNIEnv* env) : env_(env) {
  int failed = env_->PushLocalFrame(kDefaultLocalFrameCapacity);
  JNI_CHECK(!failed, "");
}

ScopedJavaLocalFrame::ScopedJavaLocalFrame(JNIEnv* env, int capacity)
    : env_(env) {
  int failed = env_->PushLocalFrame(capacity);
  JNI_CHECK(!failed, "");
}

ScopedJavaLocalFrame::~ScopedJavaLocalFrame() {
  env_->PopLocalFrame(nullptr);
}

JavaRef<jobject>::JavaRef() : obj_(nullptr) {}

JavaRef<jobject>::JavaRef(JNIEnv* env, jobject obj) : obj_(obj) {
  if (obj) {
    JNI_CHECK(env && env->GetObjectRefType(obj) == JNILocalRefType, "");
  }
}

JavaRef<jobject>::~JavaRef() = default;

JNIEnv* JavaRef<jobject>::SetNewLocalRef(JNIEnv* env, jobject obj) {
  if (!env) {
    env = AttachCurrentThread();
  } else {
    JNI_CHECK(env == AttachCurrentThread(), "");  // Is |env| on correct thread.
  }
  if (obj) {
    obj = env->NewLocalRef(obj);
  }
  if (obj_) {
    env->DeleteLocalRef(obj_);
  }
  obj_ = obj;
  return env;
}

void JavaRef<jobject>::SetNewGlobalRef(JNIEnv* env, jobject obj) {
  if (!env) {
    env = AttachCurrentThread();
  } else {
    JNI_CHECK(env == AttachCurrentThread(), "");  // Is |env| on correct thread.
  }
  if (obj) {
    obj = env->NewGlobalRef(obj);
  }
  if (obj_) {
    env->DeleteGlobalRef(obj_);
  }
  obj_ = obj;
}

void JavaRef<jobject>::ResetLocalRef(JNIEnv* env) {
  if (obj_) {
    JNI_CHECK(env == AttachCurrentThread(), "");  // Is |env| on correct thread.
    env->DeleteLocalRef(obj_);
    obj_ = nullptr;
  }
}

void JavaRef<jobject>::ResetGlobalRef() {
  if (obj_) {
    AttachCurrentThread()->DeleteGlobalRef(obj_);
    obj_ = nullptr;
  }
}

jobject JavaRef<jobject>::ReleaseInternal() {
  jobject obj = obj_;
  obj_ = nullptr;
  return obj;
}

}  // namespace jni
