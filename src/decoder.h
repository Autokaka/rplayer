#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

class RPlayerDecoder {
 public:
  AVFormatContext* formatContext;
  AVCodecContext* codecContext;

  SwsContext* swsContext;
  AVFrame* outFrame;

  AVFrame* frame;
  AVPacket* packet;

 public:
  static RPlayerDecoder* createInstance();
  void release();
  int openCodec(AVCodecParameters*, AVCodec*);
};
