/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/02.
 */

#include "decoder.h"
#include "base/logging.h"
#include "base/type_guard.h"

ScopedDecoderData::~ScopedDecoderData() {
  if (sws_ctx) {
    sws_freeContext(sws_ctx);
    sws_ctx = nullptr;
  }

  if (out_frame) {
    av_frame_free(&out_frame);
    out_frame = nullptr;
  }

  if (decode_frame) {
    av_frame_free(&decode_frame);
    decode_frame = nullptr;
  }

  if (decode_packet) {
    av_packet_free(&decode_packet);
    decode_packet = nullptr;
  }

  if (codec_ctx) {
    avcodec_close(codec_ctx);
    avcodec_free_context(&codec_ctx);
    codec_ctx = nullptr;
  }

  if (format_ctx) {
    avformat_close_input(&format_ctx);
    avformat_free_context(format_ctx);
    format_ctx = nullptr;
  }

  avformat_network_deinit();
}

RDecoder::RDecoder(const std::string& url) {
  decode_thread_ = std::thread([&]() {
    const auto data = std::make_unique<ScopedDecoderData>();
    data_ = data.get();

    /**
     * Init avformat and start the network components.
     */
    av_register_all();
    avformat_network_init();

    /**
     * Create format context and open video stream.
     */
    AVDictionary* options = nullptr;
    data->format_ctx = avformat_alloc_context();
    av_dict_set(&options, "fflags", "nobuffer", 0);
    av_dict_set(&options, "buffer_size", "4096000", 0);
    av_dict_set(&options, "stimeout", "2000000", 0);
    av_dict_set(&options, "max_delay", "5000", 0);
    // av_dict_set(&options, "rtsp_transport", "tcp", 0);
    int result =
        avformat_open_input(&data->format_ctx, url.c_str(), nullptr, &options);
    if (result < 0) {
      if (delegate) {
        delegate->PlayerThrowsError("Failed on avformat_open_input: " +
                                    std::string(av_err2str(result)));
      }
      return;
    }

    /**
     * Find video stream from formatContext.
     *
     * AVFormatContext has three kinds of streams:
     * - video stream
     * - audio stream
     * - subtitle stream
     *
     * Now we need to find the video stream in order to draw pictures
     * on the screen.
     */
    av_dict_set(&options, "probesize", "32", 0);
    av_dict_set(&options, "analyzeduration", "0", 0);
    if (avformat_find_stream_info(data->format_ctx, nullptr) < 0) {
      if (delegate) {
        delegate->PlayerThrowsError("Failed to read video info from stream.");
      }
      return;
    }
    int video_stream_index = -1;
    LOG(INFO) << "Stream info: current streams:"
              << data->format_ctx->nb_streams;
    video_stream_index = av_find_best_stream(
        data->format_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (video_stream_index < 0) {
      if (delegate) {
        delegate->PlayerThrowsError("Failed to find video stream.");
      }
      return;
    }

    /**
     * Open video stream decoder.
     * Find suitable stream decoder by codec_id.
     */
    AVCodecParameters* codec_params =
        data->format_ctx->streams[video_stream_index]->codecpar;
    result = OpenCodecSafely(codec_params);
    if (result != 0) {
      if (delegate) {
        delegate->PlayerThrowsError("Failed to open codec: %s" +
                                    std::string(av_err2str(result)));
      }
      return;
    }

    /**
     * Start looping to receive/send av_packet.
     * In packet we store picture rgba data and
     * submit it to Dart side.
     */
    if (delegate) {
      delegate->PlayerStatusWillChange(RPlayerStatus::kReadyToPlay);
      delegate->PlayerStatusDidChange(RPlayerStatus::kReadyToPlay);
    }
    data->decode_frame = av_frame_alloc();
    data->decode_packet = av_packet_alloc();
    while (av_read_frame(data->format_ctx, data->decode_packet) == 0) {
      std::scoped_lock lock(mutex_);

      if (is_stopped_) {
        LOG(DEBUG) << "Decoder thread will be terminated: RPlayer.is_stopped_";
        break;
      }

      if (!is_playing_ ||
          data->decode_packet->stream_index != video_stream_index) {
        av_packet_unref(data->decode_packet);
        continue;
      }

      // int sendPacketState =
      // avcodec_send_packet(pPlayer->decoder->codecContext,
      //                                           pPlayer->decoder->packet);
      // av_packet_unref(pPlayer->decoder->packet);
      // if (sendPacketState != 0) {
      //   LOG::E("Failed to send packet: %s.", av_err2str(sendPacketState));
      //   continue;
      // }
      // int recvFrameState =
      // avcodec_receive_frame(pPlayer->decoder->codecContext,
      //                                            pPlayer->decoder->frame);
      // if (recvFrameState != 0) {
      //   LOG::E("Failed to receive frame: %s.", av_err2str(recvFrameState));
      //   continue;
      // }
      int gop[1] = {0};
      result = avcodec_decode_video2(data->codec_ctx, data->decode_frame, gop,
                                     data->decode_packet);
      if (result < 0) {
        // LOG::E("Failed on avcodec_decode_video2: %s",
        // av_err2str(decodeResult));
        avcodec_flush_buffers(data->codec_ctx);
        continue;
      }
    }
  });
}

int RDecoder::OpenCodecSafely(const AVCodecParameters* codec_params,
                              const AVCodec* codec) {
  int result = 0;

  if (isNull(codec)) {
    return -1;
  }

  data_->codec_ctx = avcodec_alloc_context3(codec);
  // codecContext->err_recognition |= AV_EF_EXPLODE;
  // codecContext->skip_loop_filter = AVDiscard::AVDISCARD_NONE;
  // codecContext->skip_frame = AVDiscard::AVDISCARD_NONREF;
  data_->codec_ctx->delay = 1;
  // codecContext->thread_count = 4;
  data_->codec_ctx->flags |= AV_CODEC_FLAG_LOW_DELAY;
  if (isNull(data_->codec_ctx)) {
    return -1;
  }

  result = avcodec_parameters_to_context(data_->codec_ctx, codec_params);
  if (result < 0) {
    return result;
  }

  result = avcodec_open2(data_->codec_ctx, codec, nullptr);
  if (result < 0) {
    return result;
  }

  return result;
}
