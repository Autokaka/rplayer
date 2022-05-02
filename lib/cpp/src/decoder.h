/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/02.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
}
#endif

#include <thread>

#include "base/macros.h"
#include "player_like.h"

class ScopedDecoderData {
 public:
  explicit ScopedDecoderData() = default;
  ~ScopedDecoderData();

  AVFormatContext* format_ctx = nullptr;
  AVCodecContext* codec_ctx = nullptr;

  SwsContext* sws_ctx = nullptr;
  AVFrame* out_frame = nullptr;

  AVFrame* decode_frame = nullptr;
  AVPacket* decode_packet = nullptr;

  DISALLOW_COPY_ASSIGN_AND_MOVE(ScopedDecoderData);
};

class RDecoder final : public PlayerLike {
 public:
  explicit RDecoder(const std::string& url);

 private:
  std::thread decode_thread_;
  ScopedDecoderData* data_;

  int OpenCodecSafely(
      const AVCodecParameters* codec_params,
      const AVCodec* codec = avcodec_find_decoder_by_name("h264_mediacodec"));

  DISALLOW_COPY_ASSIGN_AND_MOVE(RDecoder);
};
