/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/02.
 */

#include <EGL/egl.h>
#include <glstub/gl3stub.h>

#include <android/native_window.h>

#include "base/logging.h"
#include "base/type_guard.h"
#include "gl_context_manager_android.h"
#include "jni_util.h"

namespace {

static std::once_flag g_gl3stub_init_flag;
static bool g_gl3stub_init_success = false;

const EGLint kGl3ContextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
const EGLint kGl2ContextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};

EGLContext CreateContext(EGLDisplay display,
                         EGLConfig config,
                         EGLContext shared_context) {
  if (g_gl3stub_init_success) {
    EGLContext context =
        eglCreateContext(display, config, shared_context, kGl3ContextAttribs);
    if (context) {
      return context;
    }
  }
  return eglCreateContext(display, config, shared_context, kGl2ContextAttribs);
}

}  // namespace

GLContextManagerAndroidPtr GLContextManagerAndroid::Create() {
  std::call_once(g_gl3stub_init_flag,
                 []() { g_gl3stub_init_success = (gl3stubInit() == GL_TRUE); });

  auto context_manager = std::make_unique<GLContextManagerAndroid>();

  EGLDisplay display = context_manager->display_;
  display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if (display == EGL_NO_DISPLAY) {
    LOG(ERROR) << "eglGetDisplay() returned error: " << eglGetError();
    return nullptr;
  }

  if (!eglInitialize(display, nullptr, nullptr)) {
    LOG(ERROR) << "eglInitialize() returned error: " << eglGetError();
    return nullptr;
  }

  const EGLint buffer_attribs[] = {EGL_RENDERABLE_TYPE,
                                   EGL_OPENGL_ES2_BIT,
                                   EGL_SURFACE_TYPE,
                                   EGL_WINDOW_BIT,
                                   EGL_BLUE_SIZE,
                                   8,
                                   EGL_GREEN_SIZE,
                                   8,
                                   EGL_RED_SIZE,
                                   8,
                                   EGL_ALPHA_SIZE,
                                   8,
                                   EGL_NONE};

  EGLint num_configs;

  EGLConfig config = context_manager->config_;
  if (!eglChooseConfig(display, buffer_attribs, &config, 1, &num_configs)) {
    LOG(ERROR) << "eglChooseConfig() returned error: " << eglGetError();
    return nullptr;
  }

  EGLContext context = context_manager->context_;
  context = CreateContext(display, config, nullptr);
  if (isNull(context)) {
    LOG(ERROR) << "eglCreateContext() returned error: " << eglGetError();
    return nullptr;
  }

  return std::move(context_manager);
}

GLContextManagerAndroid::~GLContextManagerAndroid() {
  MakeCurrent(EGL_NO_CONTEXT, EGL_NO_SURFACE);

  if (context_ != EGL_NO_CONTEXT) {
    eglDestroyContext(display_, context_);
    context_ = EGL_NO_CONTEXT;
  }

  if (surface_ != EGL_NO_SURFACE) {
    eglDestroySurface(display_, surface_);
    surface_ = EGL_NO_SURFACE;
  }

  if (display_ != EGL_NO_DISPLAY) {
    eglTerminate(display_);
    display_ = EGL_NO_DISPLAY;
  }

  config_ = nullptr;
}

bool GLContextManagerAndroid::MakeCurrent(EGLContext context,
                                          EGLSurface surface) {
  if (!eglMakeCurrent(display_, surface, surface, context)) {
    LOG(ERROR) << "eglMakeCurrent() returned error: " << eglGetError();
    return false;
  }

  return true;
}

bool GLContextManagerAndroid::SetSurface(ANativeWindow* window) {
  if (context_ == EGL_NO_CONTEXT) {
    return false;
  }

  if (isNull(window)) {
    // Destroy current surface
    MakeCurrent(context_, EGL_NO_SURFACE);

    if (surface_ != EGL_NO_SURFACE) {
      eglDestroySurface(display_, surface_);
      surface_ = EGL_NO_SURFACE;
    }

    return true;
  }

  SetSurface();

  EGLint format;
  if (!eglGetConfigAttrib(display_, config_, EGL_NATIVE_VISUAL_ID, &format)) {
    LOG(ERROR) << "eglGetConfigAttrib() returned error: " << eglGetError();
    SetSurface();
    return false;
  }

  ANativeWindow_setBuffersGeometry(window, 0, 0, format);

  surface_ = eglCreateWindowSurface(display_, config_, window, nullptr);
  if (isNull(surface_)) {
    LOG(ERROR) << "eglCreateWindowSurface() returned error: " << eglGetError();
    SetSurface();
    return false;
  }

  if (!MakeCurrent(context_, surface_)) {
    SetSurface();
    return false;
  }

  if (!eglQuerySurface(display_, surface_, EGL_WIDTH, &surface_width_) ||
      !eglQuerySurface(display_, surface_, EGL_HEIGHT, &surface_height_)) {
    LOG(ERROR) << "eglQuerySurface() returned error: " << eglGetError();
    SetSurface();
    return false;
  }

  return true;
}

bool GLContextManagerAndroid::PresentRenderbuffer() {
  if (!IsSurfaceValid()) {
    return false;
  }

  if (!eglSwapBuffers(display_, surface_)) {
    LOG(ERROR) << "eglSwapBuffers() returned error: " << eglGetError();
    return false;
  }

  return true;
}
