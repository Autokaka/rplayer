#include "player.h"
#include "decoder.h"

extern "C" {
FLUTTER_EXPORT char* getFFmpegVersion() {
  static char version[1024 * 4];
  strcat(version, "libavcodec: ");
  strcat(version, AV_STRINGIFY(LIBAVCODEC_VERSION));
  strcat(version, "\nlibavformat: ");
  strcat(version, AV_STRINGIFY(LIBAVFORMAT_VERSION));
  strcat(version, "\nlibavutil: ");
  strcat(version, AV_STRINGIFY(LIBAVUTIL_VERSION));
  strcat(version, "\nlibavfilter: ");
  strcat(version, AV_STRINGIFY(LIBAVFILTER_VERSION));
  strcat(version, "\nlibswresample: ");
  strcat(version, AV_STRINGIFY(LIBSWRESAMPLE_VERSION));
  strcat(version, "\nlibswscale: ");
  strcat(version, AV_STRINGIFY(LIBSWSCALE_VERSION));
  strcat(version, "\navcodec_configure: \n");
  strcat(version, avcodec_configuration());
  strcat(version, "\navcodec_license: ");
  strcat(version, avcodec_license());
  return version;
}

FLUTTER_EXPORT void play(char* url) {
  pthread_t playThread;
  pthread_create(&playThread, nullptr, decodeThread, (void*)url);
}
}
