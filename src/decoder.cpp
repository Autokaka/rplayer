#include "decoder.h"

void RPlayerDecoder::dispose() {
  if (swsContext != nullptr) {
    sws_freeContext(swsContext);
  }

  if (outFrame != nullptr) {
    av_frame_free(&outFrame);
  }

  if (frame != nullptr) {
    av_frame_free(&frame);
  }

  if (packet != nullptr) {
    av_packet_free(&packet);
  }

  if (codecContext != nullptr) {
    avcodec_free_context(&codecContext);
  }

  if (formatContext != nullptr) {
    avformat_close_input(&formatContext);
    avformat_free_context(formatContext);
  }

  avformat_network_deinit();

  delete this;
}
