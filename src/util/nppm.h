#pragma once

#include <time.h>
#include "log.h"

/**
 * Native Package Performance Manager
 */
class NPPM {
 private:
  int _startMS;
  const char* _tag;

 private:
  static int _getCurrentTimeInMilliSeconds();

 public:
  NPPM(const char*);
  void report();
};
