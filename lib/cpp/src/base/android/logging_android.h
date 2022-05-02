#pragma once

#include "base/logging.h"

namespace forge {

class LogAndroid final : public Log {
 public:
  ~LogAndroid() override;
};

}  // namespace forge
