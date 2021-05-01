#pragma once

/**
 * Configuration for RPlayer.
 */
class RPlayerConfig {
 public:
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
};
