/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/02.
 */

#pragma once

#include <EGL/egl.h>
#include <memory>

#include "base/macros.h"
#include "jni_weak_ref.h"

class GLContextManagerAndroid final {
 public:
  using GLContextManagerAndroidPtr = std::unique_ptr<GLContextManagerAndroid>;

  static GLContextManagerAndroidPtr Create();
  explicit GLContextManagerAndroid() = default;
  ~GLContextManagerAndroid();

  void Release();

  bool SetSurface(ANativeWindow* window = nullptr);

  bool PresentRenderbuffer();

  EGLint GetSurfaceWidth() const { return surface_width_; }
  EGLint GetSurfaceHeight() const { return surface_height_; }
  bool IsSurfaceValid() const { return surface_ != EGL_NO_SURFACE; }

 private:
  EGLDisplay display_ = EGL_NO_DISPLAY;
  EGLConfig config_ = nullptr;
  EGLContext context_ = EGL_NO_CONTEXT;
  EGLSurface surface_ = EGL_NO_SURFACE;

  EGLint surface_width_ = 0;
  EGLint surface_height_ = 0;

  bool MakeCurrent(EGLContext context, EGLSurface surface);

  DISALLOW_COPY_ASSIGN_AND_MOVE(GLContextManagerAndroid);
};

using GLContextManagerAndroidPtr = std::unique_ptr<GLContextManagerAndroid>;
