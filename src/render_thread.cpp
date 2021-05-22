#include "render_thread.h"

void* _render(void* args) {
  RPlayer* pPlayer = static_cast<RPlayer*>(args);

  /**
   * Create av_frame(RGBA) to store image data.
   */
  pPlayer->decoder->outFrame = av_frame_alloc();
  size_t bufferSize = av_image_get_buffer_size(
      AV_PIX_FMT_RGBA, pPlayer->decoder->codecContext->width,
      pPlayer->decoder->codecContext->height, 1);
  uint8_t* outBuffer =
      static_cast<uint8_t*>(av_malloc(static_cast<size_t>(bufferSize)));
  int imgFillResult = av_image_fill_arrays(
      pPlayer->decoder->outFrame->data, pPlayer->decoder->outFrame->linesize,
      outBuffer, AV_PIX_FMT_RGBA, pPlayer->decoder->codecContext->width,
      pPlayer->decoder->codecContext->height, 1);
  if (imgFillResult < 0) {
    pPlayer->setError("Failed on av_image_fill_arrays: %s",
                      av_err2str(imgFillResult));
    pPlayer->decoder->release();
    return nullptr;
  }
  pPlayer->decoder->swsContext =
      sws_getContext(pPlayer->decoder->codecContext->width,
                     pPlayer->decoder->codecContext->height,
                     pPlayer->decoder->codecContext->pix_fmt,
                     pPlayer->decoder->codecContext->width,
                     pPlayer->decoder->codecContext->height, AV_PIX_FMT_RGBA,
                     SWS_BICUBIC, nullptr, nullptr, nullptr);
  if (pPlayer->decoder->swsContext == nullptr) {
    pPlayer->setError("Failed to create picture context.");
    pPlayer->decoder->release();
    return nullptr;
  }

  ANativeWindow_Buffer nativeWindowBuffer;
  // Setting the display size by its pixels rather than physics size.
  // This means if your physics size is not the same as your display size,
  // The image may seem stretched or compressed in your screen.
  ANativeWindow_setBuffersGeometry(
      pPlayer->render->nativeWindow, pPlayer->decoder->codecContext->width,
      pPlayer->decoder->codecContext->height, WINDOW_FORMAT_RGBA_8888);

  /**
   * Safely draw on native window.
   * Find address of the pixel list.
   * Note that RGBA pictures only store pixels in data[0].
   * If using YUV pictures, we will have data[0], data[1], data[2]
   * to store pixels.
   */
  while (pPlayer->state != RPlayerState::STOPPED &&
         pPlayer->state != RPlayerState::ERROR) {
    if (pPlayer->state == RPlayerState::BUFFERING ||
        pPlayer->state == RPlayerState::PAUSED) {
      usleep(100000);
      continue;
    }
    if (ANativeWindow_lock(pPlayer->render->nativeWindow, &nativeWindowBuffer,
                           nullptr) < 0) {
      pPlayer->setError("Failed to lock native window.");
      pPlayer->render->release();
      return nullptr;
    }
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
      memmove(dst + i * dstStride, src + i * srcStride, srcStride);
    }

    if (ANativeWindow_unlockAndPost(pPlayer->render->nativeWindow) < 0) {
      pPlayer->setError("Failed to unlock native window.");
      pPlayer->render->release();
      return nullptr;
    }
  }

  pPlayer->render->release();
  return nullptr;
}
