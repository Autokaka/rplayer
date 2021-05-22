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
  av_dict_set(&options, "buffer_size", "4096000", 0);
  av_dict_set(&options, "stimeout", "2000000", 0);
  av_dict_set(&options, "max_delay", "5000", 0);
  // av_dict_set(&options, "rtsp_transport", "tcp", 0);
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
  LOG::D("Stream info: current streams: %d",
         pPlayer->decoder->formatContext->nb_streams);
  videoStreamIndex = av_find_best_stream(
      pPlayer->decoder->formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
  if (videoStreamIndex < 0) {
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
  int openCodecResult =
      pPlayer->config->preferHardcodec
          ? pPlayer->decoder->openCodec(
                codecParams, avcodec_find_decoder_by_name("h264_mediacodec"))
          : -1;
  if (openCodecResult != 0) {
    openCodecResult = pPlayer->decoder->openCodec(
        codecParams, avcodec_find_decoder(codecParams->codec_id));
    if (openCodecResult != 0) {
      pPlayer->setError("Failed to open codec: %s",
                        av_err2str(openCodecResult));
      pPlayer->decoder->release();
      return nullptr;
    }
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
      break;
    }
    if (pPlayer->decoder->packet->stream_index != videoStreamIndex) {
      av_packet_unref(pPlayer->decoder->packet);
      continue;
    }
    // int sendPacketState = avcodec_send_packet(pPlayer->decoder->codecContext,
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
    int decodeResult = avcodec_decode_video2(pPlayer->decoder->codecContext,
                                             pPlayer->decoder->frame, gop,
                                             pPlayer->decoder->packet);
    if (decodeResult < 0) {
      LOG::E("Failed on avcodec_decode_video2: %s", av_err2str(decodeResult));
      avcodec_flush_buffers(pPlayer->decoder->codecContext);
      continue;
    }
    if (pPlayer->renderThread == 0 && pPlayer->createRenderThread() != 0) {
      pPlayer->setError("Failed to create render thread.");
      break;
    }
  }

  av_packet_unref(pPlayer->decoder->packet);
  pPlayer->setStopped();
  pPlayer->releaseRenderThread();
  pPlayer->decoder->release();
  return nullptr;
}
