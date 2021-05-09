#include "decoder_thread.h"

void* _decode(void* args) {
  RPlayer* pPlayer = static_cast<RPlayer*>(args);
  pPlayer->setBuffering();

  /**
   * Init avformat and start the network components.
   */
  av_register_all();
  avformat_network_init();

  /**
   * Create format context and open video stream.
   */
  AVDictionary* options = nullptr;
  pPlayer->decoder->formatContext = avformat_alloc_context();
  av_dict_set(&options, "fflags", "nobuffer", 0);
  av_dict_set(&options, "buffer_size", "1024000", 0);
  av_dict_set(&options, "stimeout", "2000000", 0);
  av_dict_set(&options, "max_delay", "5000", 0);
  int fmtOpenResult = avformat_open_input(&(pPlayer->decoder->formatContext),
                                          pPlayer->url, nullptr, &options);
  if (fmtOpenResult < 0) {
    pPlayer->setError("Failed on avformat_open_input: %s",
                      av_err2str(fmtOpenResult));
    pPlayer->decoder->release();
    return nullptr;
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
  if (avformat_find_stream_info(pPlayer->decoder->formatContext, nullptr) < 0) {
    pPlayer->setError("Failed to read video info from stream.");
    pPlayer->decoder->release();
    return nullptr;
  }
  int videoStreamIndex = -1;
  LOG::D("Current video streams: %d",
         pPlayer->decoder->formatContext->nb_streams);
  for (int i = 0; i < pPlayer->decoder->formatContext->nb_streams; i++) {
    if (pPlayer->decoder->formatContext->streams[i]->codecpar->codec_type ==
        AVMEDIA_TYPE_VIDEO) {
      videoStreamIndex = i;
      break;
    }
  }
  if (videoStreamIndex == -1) {
    pPlayer->setError("Failed to find video stream.");
    pPlayer->decoder->release();
    return nullptr;
  }

  /**
   * Open video stream decoder.
   * Find suitable stream decoder by codec_id.
   */
  AVCodecParameters* codecParams =
      pPlayer->decoder->formatContext->streams[videoStreamIndex]->codecpar;
  AVCodec* codec = avcodec_find_decoder_by_name("h264_mediacodec");
  if (codec == nullptr) {
    codec = avcodec_find_decoder(codecParams->codec_id);
    if (codec == nullptr) {
      pPlayer->setError(
          "Failed to find suitable decoder for this video stream.");
      pPlayer->decoder->release();
      return nullptr;
    }
  }
  pPlayer->decoder->codecContext = avcodec_alloc_context3(codec);
  pPlayer->decoder->codecContext->thread_count = 4;
  pPlayer->decoder->codecContext->flags |= AV_CODEC_FLAG_LOW_DELAY;
  if (pPlayer->decoder->codecContext == nullptr) {
    pPlayer->setError("Failed to allocate space for decoder context.");
    pPlayer->decoder->release();
    return nullptr;
  }
  if (avcodec_parameters_to_context(pPlayer->decoder->codecContext,
                                    codecParams) < 0) {
    pPlayer->setError("Failed to inject params to decoder context.");
    pPlayer->decoder->release();
    return nullptr;
  }

  int codecOpenResult =
      avcodec_open2(pPlayer->decoder->codecContext, codec, nullptr);
  if (codecOpenResult < 0) {
    pPlayer->setError("Failed to open decoder from context: %s",
                      av_err2str(codecOpenResult));
    pPlayer->decoder->release();
    return nullptr;
  }

  /**
   * Start looping to receive/send av_packet.
   * In packet we store picture rgba data and
   * submit it to Dart side.
   */
  pPlayer->setPlaying();
  pPlayer->decoder->frame = av_frame_alloc();
  pPlayer->decoder->packet = av_packet_alloc();
  pPlayer->config->resetConsumer();
  while (av_read_frame(pPlayer->decoder->formatContext,
                       pPlayer->decoder->packet) == 0) {
    if (pPlayer->state == RPlayerState::STOPPED ||
        pPlayer->state == RPlayerState::ERROR) {
      LOG::D("Decoder thread will be terminated: RPlayerState %d",
             pPlayer->state);
      av_packet_unref(pPlayer->decoder->packet);
      pPlayer->releaseRenderThread();
      pPlayer->decoder->release();
      return nullptr;
    }
    if (pPlayer->decoder->packet->stream_index != videoStreamIndex ||
        pPlayer->state == RPlayerState::PAUSED) {
      av_packet_unref(pPlayer->decoder->packet);
      continue;
    }
    int sendPacketState = avcodec_send_packet(pPlayer->decoder->codecContext,
                                              pPlayer->decoder->packet);
    if (sendPacketState != 0) {
      LOG::E("Failed to send packet: %s.", av_err2str(sendPacketState));
      av_packet_unref(pPlayer->decoder->packet);
      continue;
    }
    int recvFrameState = avcodec_receive_frame(pPlayer->decoder->codecContext,
                                               pPlayer->decoder->frame);
    if (recvFrameState != 0) {
      LOG::E("Failed to receive frame: %s.", av_err2str(recvFrameState));
      av_packet_unref(pPlayer->decoder->packet);
      continue;
    }
    if (pPlayer->renderThread == 0 && pPlayer->createRenderThread() != 0) {
      pPlayer->setError("Failed to create render thread.");
      pPlayer->decoder->release();
      return nullptr;
    }
    av_packet_unref(pPlayer->decoder->packet);
  }

  pPlayer->setStopped();
  pPlayer->releaseRenderThread();
  pPlayer->decoder->release();
  return nullptr;
}
