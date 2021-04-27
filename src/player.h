#pragma once

#include <pthread.h>
#include <memory>
#include "decoder.h"
#include "decoder_c.h"
#include "log.h"
#include "texture/texture_android.h"

class RPlayerState {
 public:
  static const int INIT = 0;

  static const int STOPPED = 1;
  static const int BUFFERING = 2;
  static const int PLAYING = 3;

  static const int PAUSED = 4;
  static const int ERROR = 5;
};

class RPlayer {
 public:
  char* url;
  int state = RPlayerState::INIT;
  TextureAndroid* pTextureAndroid;
  RPlayerDecoder* decoder;

 private:
  pthread_t _pid;

 public:
  static RPlayer* createInstance();
  int createDecodeThread(char*);
  int dispose();
  int getHeight();
  int getWidth();

 public:
  void setBuffering();
  void setPlaying();
  void setPaused();
  void setError(const char*, ...);
  void setStopped();
};
