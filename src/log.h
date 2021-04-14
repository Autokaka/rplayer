#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, __FILE__, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, __FILE__, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, __FILE__, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, __FILE__, __VA_ARGS__)
#define LOGV(...) \
  __android_log_print(ANDROID_LOG_VERBOSE, __FILE__, __VA_ARGS__)
