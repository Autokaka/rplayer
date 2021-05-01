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
};
