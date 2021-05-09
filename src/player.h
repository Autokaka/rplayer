#pragma once

#include <pthread.h>
#include <memory>
#include "config.h"
#include "decoder.h"
#include "decoder_thread.h"
#include "render_thread.h"
#include "texture/texture_android.h"
#include "util/log.h"

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
  char msg[1024];
  int state = RPlayerState::INIT;

  TextureAndroid* render = new TextureAndroid();
  pthread_t renderThread;
  RPlayerDecoder* decoder = RPlayerDecoder::createInstance();
  pthread_t decodeThread;
  RPlayerConfig* config = RPlayerConfig::createInstance();

 public:
  static RPlayer* createInstance();
  void dispose();

  int createDecodeThread(char*);
  void releaseDecodeThread();
  int createRenderThread();
  void releaseRenderThread();

  int getHeight();
  int getWidth();

 public:
  void setBuffering();
  void setPlaying();
  void setPaused();
  void setStopped();
  void setError(const char*, ...);
  void resetState();
};
