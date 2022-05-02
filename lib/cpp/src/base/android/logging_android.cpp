#include "logging_android.h"
#include "base/build_configs.h"

#if kTargetAndroid

#include <android/log.h>

namespace forge {

LogAndroid::~LogAndroid() {
  _stream << std::endl;
  android_LogPriority priority;
  if (_logLevel.severity == LogLevel::INFO.severity) {
    priority = ANDROID_LOG_INFO;
  } else if (_logLevel.severity == LogLevel::DEBUG.severity) {
    priority = ANDROID_LOG_DEBUG;
  } else if (_logLevel.severity == LogLevel::WARN.severity) {
    priority = ANDROID_LOG_WARN;
  } else if (_logLevel.severity == LogLevel::ERROR.severity) {
    priority = ANDROID_LOG_ERROR;
  } else if (_logLevel.severity == LogLevel::FATAL.severity) {
    priority = ANDROID_LOG_FATAL;
  }
  __android_log_write(priority, TAG.data(), _stream.str().c_str());
  if (_logLevel.severity >= LogLevel::FATAL.severity) {
    abort();
  }
}

}  // namespace forge

#endif
