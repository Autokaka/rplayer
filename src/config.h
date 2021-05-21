#pragma once

#include <string.h>

/**
 * Configuration for RPlayer.
 */
class RPlayerConfig {
 public:
  /**
   * Contains the copy of current config.
   * This pointer is used to be consumed by
   * internal player. The fields of RPlayerConfig
   * should never be changed internally except for
   * this pointer.
   */
  RPlayerConfig* _consumer;

  /**
   * Define how many times will decode
   * thread retry when stream is shut down
   * but player is still playing.
   */
  int retryTimesOnDisconnect = 0;

  /**
   * Considering some streams may need time
   * to reset themselves, this field is used
   * to set the delay time before reconnecting
   * a previous stream. Note that the value is
   * in milliseconds.
   */
  int retryDelayInMilliseconds = 0;

  /**
   * Let player use hardcodec first, if
   * fail to use hardcodec, player will
   * fallback to softcodec.
   */
  int preferHardcodec = 1;

 public:
  static RPlayerConfig* createInstance();
  void resetConsumer();
  void release();
};
