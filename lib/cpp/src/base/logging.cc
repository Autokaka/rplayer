#include "logging.h"

namespace forge {

const LogLevel LogLevel::INFO(0);
const LogLevel LogLevel::DEBUG(1);
const LogLevel LogLevel::WARN(2);
const LogLevel LogLevel::ERROR(3);
const LogLevel LogLevel::FATAL(4);

LogLevel LogLevel::endurance(LogLevel::INFO.severity);

const std::string_view LogLevel::_levels[CAPACITY] = {"INFO", "DEBUG", "WARN",
                                                      "ERROR", "FATAL"};

std::string_view LogLevel::toString() const {
  return LogLevel::_levels[severity];
}

bool LogLevel::reachEndurance() const {
  return severity >= endurance.severity;
}

Log::Log(const LogLevel& level, const std::string_view& file, const int& line)
    : _logLevel(level) {
  stream_ << "[" << level.toString() << "](" << file << ":" << line << ") ";
}

}  // namespace forge
