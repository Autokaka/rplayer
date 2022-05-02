/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/02.
 */

#pragma once

#include <EGL/egl.h>

#include "base/macros.h"
#include "jni_weak_ref.h"

class GLContextManagerAndroid final {
 public:
  explicit GLContextManagerAndroid();
  ~GLContextManagerAndroid();

  bool Initialize();
  void Release();

  bool SetSurface(ANativeWindow* window);

  bool PresentRenderbuffer();
  bool MakeCurrent();
  bool MakeNoCurrent();

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

  EGLContext CreateContext(EGLDisplay display,
                           EGLConfig config,
                           EGLContext shared_context);
  void DestroyContext();
  bool MakeCurrent(EGLContext context, EGLSurface surface);

  void DestroySurface();

  DISALLOW_COPY_ASSIGN_AND_MOVE(GLContextManagerAndroid);
};
