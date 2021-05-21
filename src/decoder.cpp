#include "decoder.h"

RPlayerDecoder* RPlayerDecoder::createInstance() {
  return new RPlayerDecoder();
}

void RPlayerDecoder::release() {
  if (swsContext != nullptr) {
    sws_freeContext(swsContext);
    swsContext = nullptr;
  }

  if (outFrame != nullptr) {
    av_frame_free(&outFrame);
    outFrame = nullptr;
  }

  if (frame != nullptr) {
    av_frame_free(&frame);
    frame = nullptr;
  }

  if (packet != nullptr) {
    av_packet_free(&packet);
    packet = nullptr;
  }

  if (codecContext != nullptr) {
    avcodec_close(codecContext);
    avcodec_free_context(&codecContext);
    codecContext = nullptr;
  }

  if (formatContext != nullptr) {
    avformat_close_input(&formatContext);
    avformat_free_context(formatContext);
    formatContext = nullptr;
  }

  avformat_network_deinit();
}

int RPlayerDecoder::openCodec(AVCodecParameters* codecParams, AVCodec* codec) {
  int result = 0;
  if (codec == nullptr) {
    return -1;
  }
  codecContext = avcodec_alloc_context3(codec);
  // codecContext->err_recognition |= AV_EF_EXPLODE;
  // codecContext->skip_loop_filter = AVDiscard::AVDISCARD_NONE;
  // codecContext->skip_frame = AVDiscard::AVDISCARD_NONREF;
  codecContext->delay = 1;
  // codecContext->thread_count = 4;
  codecContext->flags |= AV_CODEC_FLAG_LOW_DELAY;
  if (codecContext == nullptr) {
    return -1;
  }
  result = avcodec_parameters_to_context(codecContext, codecParams);
  if (result < 0) {
    avcodec_free_context(&codecContext);
    return result;
  }

  result = avcodec_open2(codecContext, codec, nullptr);
  if (result < 0) {
    avcodec_free_context(&codecContext);
    return result;
  }

  return result;
}
