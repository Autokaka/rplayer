#pragma once

#include "base/macros.h"

#include <algorithm>
#include <sstream>
#include <string_view>

namespace forge {

class LogLevel final {
 public:
  static const LogLevel INFO;
  static const LogLevel DEBUG;
  static const LogLevel WARN;
  static const LogLevel ERROR;
  static const LogLevel FATAL;
  static constexpr int CAPACITY = 5;

  static LogLevel endurance;
  const int severity;

  std::string_view toString() const;
  bool reachEndurance() const;

 private:
  explicit LogLevel(int severity)
      : severity(std::max(0, std::min(severity, CAPACITY))) {}
  ~LogLevel() = default;

  static const std::string_view _levels[CAPACITY];

  DISALLOW_COPY_ASSIGN_AND_MOVE(LogLevel);
};

class VoidStream final {
 public:
  explicit VoidStream() = default;

  void operator&(std::ostream&) {}

  DISALLOW_COPY_ASSIGN_AND_MOVE(VoidStream);
};

class Log {
 public:
  Log(const LogLevel& level, const std::string_view& file, const int& line);
  ~Log();

  std::ostringstream& stream() { return stream_; }

 protected:
  std::ostringstream stream_;
  const LogLevel& _logLevel;

  DISALLOW_COPY_ASSIGN_AND_MOVE(Log);
};

#define LOG(SEVERITY)   \
  forge::VoidStream() & \
      forge::Log(forge::LogLevel::SEVERITY, __FILE__, __LINE__).stream()

}  // namespace forge
