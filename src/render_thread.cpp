#include "render_thread.h"

void* _render(void* args) {
  RPlayer* pPlayer = static_cast<RPlayer*>(args);

  ANativeWindow_Buffer nativeWindowBuffer;
  // Setting the display size by its pixels rather than physics size.
  // This means if your physics size is not the same as your display size,
  // The image may seem stretched or compressed in your screen.
  ANativeWindow_setBuffersGeometry(
      pPlayer->render->nativeWindow, pPlayer->decoder->codecContext->width,
      pPlayer->decoder->codecContext->height, WINDOW_FORMAT_RGBA_8888);

  while (pPlayer->state != RPlayerState::STOPPED &&
         pPlayer->state != RPlayerState::ERROR) {
    if (pPlayer->state == RPlayerState::BUFFERING ||
        pPlayer->state == RPlayerState::PAUSED) {
      continue;
    }

    if (int lockState = ANativeWindow_lock(pPlayer->render->nativeWindow,
                                           &nativeWindowBuffer, nullptr) < 0) {
      pPlayer->setError("Failed to lock native window: code %d", lockState);
      return nullptr;
    }

    /**
     * Safely draw on native window.
     * Find address of the pixel list.
     * Note that RGBA pictures only store pixels in data[0].
     * If using YUV pictures, we will have data[0], data[1], data[2]
     * to store pixels.
     */
    sws_scale(pPlayer->decoder->swsContext,
              static_cast<const uint8_t* const*>(pPlayer->decoder->frame->data),
              pPlayer->decoder->frame->linesize, 0,
              pPlayer->decoder->frame->height, pPlayer->decoder->outFrame->data,
              pPlayer->decoder->outFrame->linesize);
    uint8_t* src = pPlayer->decoder->outFrame->data[0];
    int srcStride = pPlayer->decoder->outFrame->linesize[0];
    uint8_t* dst = static_cast<uint8_t*>(nativeWindowBuffer.bits);
    int dstStride = nativeWindowBuffer.stride * 4;
    for (int i = 0; i < pPlayer->decoder->codecContext->height; i++) {
      memcpy(dst + i * dstStride, src + i * srcStride, srcStride);
    }

    if (int unlockState =
            ANativeWindow_unlockAndPost(pPlayer->render->nativeWindow) < 0) {
      pPlayer->setError("Failed to unlock native window: code %d", unlockState);
      return nullptr;
    }
  }

  return nullptr;
}
