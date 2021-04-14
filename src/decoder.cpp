#include "decoder.h"

extern "C" {
void* decodeThread(void* args) {
  char* url = static_cast<char*>(args);

  // 1.创建封装格式上下文
  AVFormatContext* m_AVFormatContext = avformat_alloc_context();

  // 2.打开输入文件，解封装
  if (avformat_open_input(&m_AVFormatContext, url, NULL, NULL) != 0) {
    LOGE("avformat_open_input fail.");
    return (void*)-1;
  }

  // 3.获取音视频流信息
  if (avformat_find_stream_info(m_AVFormatContext, NULL) < 0) {
    LOGE("avformat_find_stream_info fail.");
    return (void*)-1;
  }

  // 4.获取音视频流索引
  int m_StreamIndex = -1;
  for (int i = 0; i < m_AVFormatContext->nb_streams; i++) {
    if (m_AVFormatContext->streams[i]->codecpar->codec_type ==
        AVMEDIA_TYPE_VIDEO) {
      m_StreamIndex = i;
      break;
    }
  }

  if (m_StreamIndex == -1) {
    LOGE("fail to find stream index.");
    return (void*)-1;
  }
  // 5.获取解码器参数
  AVCodecParameters* codecParameters =
      m_AVFormatContext->streams[m_StreamIndex]->codecpar;

  // 6.根据 codec_id 获取解码器
  AVCodec* m_AVCodec = avcodec_find_decoder(codecParameters->codec_id);
  if (m_AVCodec == nullptr) {
    LOGE("avcodec_find_decoder fail.");
    return (void*)-1;
  }

  // 7.创建解码器上下文
  AVCodecContext* m_AVCodecContext = avcodec_alloc_context3(m_AVCodec);
  if (avcodec_parameters_to_context(m_AVCodecContext, codecParameters) != 0) {
    LOGE("avcodec_parameters_to_context fail.");
    return (void*)-1;
  }

  // 8.打开解码器
  int result = avcodec_open2(m_AVCodecContext, m_AVCodec, NULL);
  if (result < 0) {
    LOGE("avcodec_open2 fail. result=%d", result);
    return (void*)-1;
  }

  // 9.创建存储编码数据和解码数据的结构体
  AVPacket* m_Packet = av_packet_alloc();  //创建 AVPacket 存放编码数据
  AVFrame* m_Frame = av_frame_alloc();  //创建 AVFrame 存放解码后的数据

  // 10.解码循环
  while (av_read_frame(m_AVFormatContext, m_Packet) >= 0) {  //读取帧
    if (m_Packet->stream_index == m_StreamIndex) {
      if (avcodec_send_packet(m_AVCodecContext, m_Packet) != 0) {  //视频解码
        LOGE("avcodec_send_packet unexpectedly");
        return (void*)-1;
      }
      while (avcodec_receive_frame(m_AVCodecContext, m_Frame) == 0) {
        //获取到 m_Frame 解码数据，在这里进行格式转换，然后进行渲染，下一节介绍
        // ANativeWindow 渲染过程
        LOGE("receive frame...");
      }
    }
    av_packet_unref(m_Packet);  //释放 m_Packet 引用，防止内存泄漏
  }

  // 11.释放资源，解码完成
  if (m_Frame != nullptr) {
    av_frame_free(&m_Frame);
    m_Frame = nullptr;
  }

  if (m_Packet != nullptr) {
    av_packet_free(&m_Packet);
    m_Packet = nullptr;
  }

  if (m_AVCodecContext != nullptr) {
    avcodec_close(m_AVCodecContext);
    avcodec_free_context(&m_AVCodecContext);
    m_AVCodecContext = nullptr;
    m_AVCodec = nullptr;
  }

  if (m_AVFormatContext != nullptr) {
    avformat_close_input(&m_AVFormatContext);
    avformat_free_context(m_AVFormatContext);
    m_AVFormatContext = nullptr;
  }
}
}
