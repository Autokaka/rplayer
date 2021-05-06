#include "decoder_thread.h"

void* _decode(void* args) {
  RPlayer* pPlayer = static_cast<RPlayer*>(args);
  pPlayer->setBuffering();

  /**
   * Init avformat and start the network components.
   */
  av_register_all();
  avformat_network_init();
  AVDictionary* options = nullptr;
  av_dict_set(&options, "buffer_size", "1024000", 0);
  av_dict_set(&options, "fflags", "nobuffer", 0);
  av_dict_set(&options, "fflags", "discardcorrupt", 0);
  av_dict_set(&options, "probesize", "32", 0);
  av_dict_set(&options, "analyzeduration", "0", 0);
  av_dict_set(&options, "packet-buffering", "0", 0);
  av_dict_set(&options, "fps", "30", 0);
  av_dict_set(&options, "preset", "ultrafast", 0);
  av_dict_set(&options, "tune", "zerolatency", 0);

  /**
   * Create format context and open video stream.
   */
  pPlayer->decoder->formatContext = avformat_alloc_context();
  if (int openResult = avformat_open_input(&(pPlayer->decoder->formatContext),
                                           pPlayer->url, nullptr, nullptr) < 0) {
    if (pPlayer->config->_consumer->retryTimesOnDisconnect > 0) {
      return _retryDecode(static_cast<void*>(pPlayer));
    }
    pPlayer->setError("Failed on avformat_open_input: %s",
                      av_err2str(openResult));
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
  if (avformat_find_stream_info(pPlayer->decoder->formatContext, NULL) < 0) {
    pPlayer->setError("Failed to read video info from stream.");
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
    return nullptr;
  }

  /**
   * Open video stream decoder.
   * Find suitable stream decoder by codec_id.
   */
  AVCodecParameters* codecParams =
      pPlayer->decoder->formatContext->streams[videoStreamIndex]->codecpar;
  AVCodec* codec = avcodec_find_decoder(codecParams->codec_id);
  if (codec == nullptr) {
    pPlayer->setError("Failed to find suitable decoder for this video stream.");
    return nullptr;
  }
  pPlayer->decoder->codecContext = avcodec_alloc_context3(codec);
  if (pPlayer->decoder->codecContext == nullptr) {
    pPlayer->setError("Failed to allocate space for decoder context.");
    return nullptr;
  }
  if (avcodec_parameters_to_context(pPlayer->decoder->codecContext,
                                    codecParams) < 0) {
    pPlayer->setError("Failed to inject params to decoder context.");
    return nullptr;
  }
  if (avcodec_open2(pPlayer->decoder->codecContext, codec, nullptr) < 0) {
    pPlayer->setError("Failed to open decoder from context.");
  }

  /**
   * Create av_frame(RGBA) to store image data.
   */
  pPlayer->decoder->outFrame = av_frame_alloc();
  size_t bufferSize = av_image_get_buffer_size(
      AV_PIX_FMT_RGBA, pPlayer->decoder->codecContext->width,
      pPlayer->decoder->codecContext->height, 1);
  uint8_t* outBuffer =
      static_cast<uint8_t*>(av_malloc(static_cast<size_t>(bufferSize)));
  if (int imgFillResult =
          av_image_fill_arrays(pPlayer->decoder->outFrame->data,
                               pPlayer->decoder->outFrame->linesize, outBuffer,
                               AV_PIX_FMT_RGBA,
                               pPlayer->decoder->codecContext->width,
                               pPlayer->decoder->codecContext->height, 1) < 0) {
    pPlayer->setError("Failed on av_image_fill_arrays: code %d", imgFillResult);
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
    return nullptr;
  }

  /**
   * Create buffer for nativeWindow to draw in later loop.
   */
  pPlayer->pTextureAndroid = new TextureAndroid();
  ANativeWindow_Buffer nativeWindowBuffer;
  // Setting the display size by its pixels rather than physics size.
  // This means if your physics size is not the same as your display size,
  // The image may seem stretched or compressed in your screen.
  ANativeWindow_setBuffersGeometry(pPlayer->pTextureAndroid->nativeWindow,
                                   pPlayer->decoder->codecContext->width,
                                   pPlayer->decoder->codecContext->height,
                                   WINDOW_FORMAT_RGBA_8888);

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
      return nullptr;
    }
    if (pPlayer->decoder->packet->stream_index != videoStreamIndex ||
        pPlayer->state == RPlayerState::PAUSED) {
      av_packet_unref(pPlayer->decoder->packet);
      continue;
    }
    if (int sendPacketState =
            avcodec_send_packet(pPlayer->decoder->codecContext,
                                pPlayer->decoder->packet) != 0) {
      LOG::E("Failed to send packet: code %d.", sendPacketState);
      av_packet_unref(pPlayer->decoder->packet);
      continue;
    }
    if (int receiveFrameState =
            avcodec_receive_frame(pPlayer->decoder->codecContext,
                                  pPlayer->decoder->frame) != 0) {
      LOG::E("Failed to receive frame: code %d.", receiveFrameState);
      av_packet_unref(pPlayer->decoder->packet);
      continue;
    }
    if (int lockState =
            ANativeWindow_lock(pPlayer->pTextureAndroid->nativeWindow,
                               &nativeWindowBuffer, NULL) < 0) {
      pPlayer->setError("Failed to lock native window: code %d", lockState);
      av_packet_unref(pPlayer->decoder->packet);
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

    if (int unlockState = ANativeWindow_unlockAndPost(
                              pPlayer->pTextureAndroid->nativeWindow) < 0) {
      pPlayer->setError("Failed to unlock native window: code %d", unlockState);
      av_packet_unref(pPlayer->decoder->packet);
      return nullptr;
    }
    av_packet_unref(pPlayer->decoder->packet);
  }

  /**
   * If loop is broken without STOPPED or ERROR flag,
   * we need to consider relinking to previous stream.
   */
  _retryDecode(static_cast<void*>(pPlayer));

  return nullptr;
}

void* _retryDecode(void* args) {
  RPlayer* pPlayer = static_cast<RPlayer*>(args);

  if (pPlayer->state == RPlayerState::STOPPED ||
      pPlayer->state == RPlayerState::ERROR ||
      pPlayer->config->_consumer->retryTimesOnDisconnect <= 0) {
    return nullptr;
  }

  usleep(pPlayer->config->retryDelayInMilliseconds * 1000);
  pPlayer->config->_consumer->retryTimesOnDisconnect--;
  sprintf(pPlayer->msg,
          "Decode thread %ld: lost connection. Retrying... (%d/%d)",
          pPlayer->pid,
          pPlayer->config->retryTimesOnDisconnect -
              pPlayer->config->_consumer->retryTimesOnDisconnect,
          pPlayer->config->retryTimesOnDisconnect);
  LOG::D(pPlayer->msg);
  pPlayer->decoder->release();
  pPlayer->decoder = new RPlayerDecoder();
  return _decode(static_cast<void*>(pPlayer));
}
