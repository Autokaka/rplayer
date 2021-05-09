#include "nppm.h"

NPPM::NPPM(const char* tag) {
  _tag = tag;
  _startMS = NPPM::_getCurrentTimeInMilliSeconds();
}

int NPPM::_getCurrentTimeInMilliSeconds() {
  struct timespec res {};
  clock_gettime(CLOCK_MONOTONIC, &res);
  return static_cast<int>(1000.0 * res.tv_sec +
                          static_cast<double>(res.tv_nsec) / 1e6);
}

void NPPM::report() {
  LOG::D("%s consume time: %dms", _tag,
         NPPM::_getCurrentTimeInMilliSeconds() - _startMS);
  _startMS = NPPM::_getCurrentTimeInMilliSeconds();
}
