#pragma once

#include <android/log.h>

class LOG {
 public:
  static const char* TAG;
  static void D(const char*, ...);
  static void E(const char*, ...);
  static void I(const char*, ...);
  static void V(const char*, ...);
  static void W(const char*, ...);
};
