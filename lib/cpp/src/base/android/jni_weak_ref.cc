#include "jni_weak_ref.h"
#include "base/logging.h"
#include "jni_util.h"

namespace jni {

JavaObjectWeakGlobalRef::JavaObjectWeakGlobalRef() : obj_(nullptr) {}

JavaObjectWeakGlobalRef::JavaObjectWeakGlobalRef(
    const JavaObjectWeakGlobalRef& orig)
    : obj_(nullptr) {
  Assign(orig);
}

JavaObjectWeakGlobalRef::JavaObjectWeakGlobalRef(JNIEnv* env, jobject obj)
    : obj_(env->NewWeakGlobalRef(obj)) {
  JNI_CHECK(obj_, "");
}

JavaObjectWeakGlobalRef::~JavaObjectWeakGlobalRef() {
  reset();
}

void JavaObjectWeakGlobalRef::operator=(const JavaObjectWeakGlobalRef& rhs) {
  Assign(rhs);
}

void JavaObjectWeakGlobalRef::reset() {
  if (obj_) {
    AttachCurrentThread()->DeleteWeakGlobalRef(obj_);
    obj_ = nullptr;
  }
}

ScopedJavaLocalRef<jobject> JavaObjectWeakGlobalRef::get(JNIEnv* env) const {
  return GetRealObject(env, obj_);
}

ScopedJavaLocalRef<jobject> GetRealObject(JNIEnv* env, jweak obj) {
  jobject real = nullptr;
  if (obj) {
    real = env->NewLocalRef(obj);
    if (!real) {
      LOG(ERROR) << "The real object has been deleted!";
    }
  }
  return ScopedJavaLocalRef<jobject>(env, real);
}

void JavaObjectWeakGlobalRef::Assign(const JavaObjectWeakGlobalRef& other) {
  if (&other == this) {
    return;
  }

  JNIEnv* env = AttachCurrentThread();
  if (obj_) {
    env->DeleteWeakGlobalRef(obj_);
  }

  obj_ = other.obj_ ? env->NewWeakGlobalRef(other.obj_) : nullptr;
  JNI_EXCEPTION_CHECK(env);
}

}  // namespace jni
