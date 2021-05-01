#include "log.h"

const char* LOG::TAG = "RPlayerNative";

void LOG::D(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  __android_log_vprint(ANDROID_LOG_DEBUG, LOG::TAG, fmt, args);
  va_end(args);
}

void LOG::E(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  __android_log_vprint(ANDROID_LOG_ERROR, LOG::TAG, fmt, args);
  va_end(args);
}

void LOG::I(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  __android_log_vprint(ANDROID_LOG_INFO, LOG::TAG, fmt, args);
  va_end(args);
}

void LOG::V(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  __android_log_vprint(ANDROID_LOG_VERBOSE, LOG::TAG, fmt, args);
  va_end(args);
}

void LOG::W(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  __android_log_vprint(ANDROID_LOG_WARN, LOG::TAG, fmt, args);
  va_end(args);
}
