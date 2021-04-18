#include "decoder.h"

extern "C" {

void* decodeThread(void* args) {
  // Extract args
  RplayerDart* rplayerDart = static_cast<RplayerDart*>(args);
  char* url = rplayerDart->url;
  ANativeWindow* nativeWindow = rplayerDart->nativeWindow;

  if (url == NULL) {
    LOGE("Failed to convert url.");
    return (void*)-1;
  }

  // register all codec.
  //  av_register_all();

  // Create format context and open video stream
  AVFormatContext* formatContext = avformat_alloc_context();
  int open_state = avformat_open_input(&formatContext, url, NULL, NULL);
  if (open_state < 0) {
    char errbuf[128];
    if (av_strerror(open_state, errbuf, sizeof(errbuf)) == 0) {
      LOGE("Failed to open stream: %s.", errbuf);
    }
    return (void*)-1;
  }

  // AVFormatContext has three kinds of streams:
  // - video stream
  // - audio stream
  // - subtitle stream
  // Now we need to find the video stream in order to draw pictures
  // on the screen.
  if (avformat_find_stream_info(formatContext, NULL) < 0) {
    LOGE("Failed to read video info from stream.");
    return (void*)-1;
  }
  int video_stream_index = -1;
  LOGD("Current video streams: %d", formatContext->nb_streams);
  for (int i = 0; i < formatContext->nb_streams; i++) {
    if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
      video_stream_index = i;
      break;
    }
  }
  if (video_stream_index == -1) {
    LOGE("Failed to find video stream.");
    return (void*)-1;
  }

  // Create video stream decoder.
  // Find suitable stream decoder by codec_id.
  AVCodecParameters* codecParameters =
      formatContext->streams[video_stream_index]->codecpar;
  AVCodec* videoDecoder = avcodec_find_decoder(codecParameters->codec_id);
  if (videoDecoder == NULL) {
    LOGE("Failed to find suitable decoder for this video stream.");
    return (void*)-1;
  }
  AVCodecContext* codecContext = avcodec_alloc_context3(videoDecoder);
  if (codecContext == NULL) {
    LOGE("Failed to allocate space for decoder context.");
    return (void*)-1;
  }
  if (avcodec_parameters_to_context(codecContext, codecParameters) < 0) {
    LOGE("Failed to inject params to decoder context.");
    return (void*)-1;
  }
  if (avcodec_open2(codecContext, videoDecoder, NULL) < 0) {
    LOGE("Failed to create decoder from context.");
    return (void*)-1;
  }

  // Create packet to store image data in per frame.
  AVPixelFormat dstFormat = AV_PIX_FMT_RGBA;
  AVPacket* packet = av_packet_alloc();
  AVFrame* frame = av_frame_alloc();
  AVFrame* outFrame = av_frame_alloc();
  auto* out_buffer = (uint8_t*)av_malloc((size_t)av_image_get_buffer_size(
      dstFormat, codecContext->width, codecContext->height, 1));
  av_image_fill_arrays(outFrame->data, outFrame->linesize, out_buffer,
                       dstFormat, codecContext->width, codecContext->height, 1);
  SwsContext* swsContext = sws_getContext(
      codecContext->width, codecContext->height, codecContext->pix_fmt,
      codecContext->width, codecContext->height, dstFormat, SWS_BICUBIC, NULL,
      NULL, NULL);
  if (swsContext == NULL) {
    LOGE("Failed to create image context.");
    return (void*)-1;
  }

  // Create drawing buffers.
  ANativeWindow_Buffer outBuffer;
  // Setting the display size by its pixels rather than physics size.
  // This means if your physics size is not the same as your display size,
  // The image may seem stretched or compressed in your screen.
  ANativeWindow_setBuffersGeometry(nativeWindow, codecContext->width,
                                   codecContext->height,
                                   WINDOW_FORMAT_RGBA_8888);

  // Read each frame and draw.
  while (av_read_frame(formatContext, packet) == 0) {
    if (packet->stream_index == video_stream_index) {
      // Send raw data to decoder.
      int sendPacketState = avcodec_send_packet(codecContext, packet);
      if (sendPacketState == 0) {
        int receiveFrameState = avcodec_receive_frame(codecContext, frame);
        switch (receiveFrameState) {
          case 0: {
            // Create mutex to lock this window to ensure that we are
            // the only one to draw this window.
            ANativeWindow_lock(nativeWindow, &outBuffer, NULL);
            sws_scale(swsContext, (const uint8_t* const*)frame->data,
                      frame->linesize, 0, frame->height, outFrame->data,
                      outFrame->linesize);
            auto* dst = (uint8_t*)outBuffer.bits;
            // Find address of the pixel list.
            // Note that RGBA pictures only store pixels in data[0].
            // If using YUV pictures, we will have data[0], data[1], data[2]
            // to store pixels.
            uint8_t* src = outFrame->data[0];
            int oneLineByte = outBuffer.stride * 4;
            int srcStride = outFrame->linesize[0];
            for (int i = 0; i < codecContext->height; i++) {
              memcpy(dst + i * oneLineByte, src + i * srcStride, srcStride);
            }
            ANativeWindow_unlockAndPost(nativeWindow);
            break;
          }
          case AVERROR(EAGAIN): {
            // Failed to send data.
            LOGE("Failed to receive frame: AVERROR(EAGAIN)");
            break;
          }
          case AVERROR_EOF: {
            // Codec refreshed data but no new packet to codec.
            LOGE("Failed to receive frame: AVERROR_EOF");
            break;
          }
          case AVERROR(EINVAL): {
            // Data is invalid.
            LOGE("Failed to receive frame: AVERROR(EINVAL)");
            break;
          }
          case AVERROR(ENOMEM): {
            // Packets cannot be sent into decode queue.
            // Or codec internal error.
            LOGE("Failed to receive frame: AVERROR(ENOMEM)");
            break;
          }
          default: {
            LOGE("Failed to receive frame: unknown error.");
            break;
          }
        }
      }
    }
    av_packet_unref(packet);
  }

  // Release references.
  ANativeWindow_release(nativeWindow);
  av_frame_free(&outFrame);
  av_frame_free(&frame);
  av_packet_free(&packet);
  avcodec_free_context(&codecContext);
  avformat_close_input(&formatContext);
  avformat_free_context(formatContext);
}
}
