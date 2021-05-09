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
