#pragma once

#include <jni.h>

#include "scoped_java_ref.h"

namespace jni {

// Manages WeakGlobalRef lifecycle.
// This class is not thread-safe w.r.t. get() and reset(). Multiple threads may
// safely use get() concurrently, but if the user calls reset() (or of course,
// calls the destructor) they'll need to provide their own synchronization.
class JavaObjectWeakGlobalRef {
 public:
  JavaObjectWeakGlobalRef();

  JavaObjectWeakGlobalRef(const JavaObjectWeakGlobalRef& orig);

  JavaObjectWeakGlobalRef(JNIEnv* env, jobject obj);

  virtual ~JavaObjectWeakGlobalRef();

  void operator=(const JavaObjectWeakGlobalRef& rhs);

  ScopedJavaLocalRef<jobject> get(JNIEnv* env) const;

  bool is_empty() const { return obj_ == nullptr; }

  void reset();

 private:
  void Assign(const JavaObjectWeakGlobalRef& rhs);

  jweak obj_;
};

// Get the real object stored in the weak reference returned as a
// ScopedJavaLocalRef.
ScopedJavaLocalRef<jobject> GetRealObject(JNIEnv* env, jweak obj);

}  // namespace jni
