#pragma once

#include <android/log.h>

extern "C" {
#define LOGI(...) \
  __android_log_print(ANDROID_LOG_INFO, "RPlayer_native", __VA_ARGS__)
#define LOGE(...) \
  __android_log_print(ANDROID_LOG_ERROR, "RPlayer_native", __VA_ARGS__)
#define LOGD(...) \
  __android_log_print(ANDROID_LOG_DEBUG, "RPlayer_native", __VA_ARGS__)
#define LOGW(...) \
  __android_log_print(ANDROID_LOG_WARN, "RPlayer_native", __VA_ARGS__)
#define LOGV(...) \
  __android_log_print(ANDROID_LOG_VERBOSE, "RPlayer_native", __VA_ARGS__)
}
