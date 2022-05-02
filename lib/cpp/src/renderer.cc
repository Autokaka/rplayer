/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/02.
 */

// TODO(Autokaka) V2: Support iOS platform.
#include "renderer.h"
#include "base/android/gl_context_manager_android.h"
#include "base/type_guard.h"

RRenderer::RRenderer() {
  render_thread_ = std::thread([&]() {
    const auto gl_context_manager = GLContextManagerAndroid::Create();
    if (isNull(gl_context_manager)) {
      if (delegate) {
        delegate->PlayerThrowsError("Failed to create OpenGL context.");
      }
      return;
    }
  });
}
