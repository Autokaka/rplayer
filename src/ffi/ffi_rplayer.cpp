#include "ffi_rplayer.h"

extern "C" {
//////////////////////////////////////////
// RPlayer
//////////////////////////////////////////

DART_EXPORT void* RPlayer_createInstance() {
  return static_cast<void*>(RPlayer::createInstance());
}

DART_EXPORT int RPlayer_createDecodeThread(void* p, char* url) {
  RPlayer* pPlayer = static_cast<RPlayer*>(p);
  return pPlayer->createDecodeThread(url);
}

DART_EXPORT void RPlayer_setPlaying(void* p) {
  RPlayer* pPlayer = static_cast<RPlayer*>(p);
  pPlayer->setPlaying();
}

DART_EXPORT void RPlayer_setPaused(void* p) {
  RPlayer* pPlayer = static_cast<RPlayer*>(p);
  pPlayer->setPaused();
}

DART_EXPORT void RPlayer_setConfig(void* p, void* c) {
  RPlayer* pPlayer = static_cast<RPlayer*>(p);
  pPlayer->config = static_cast<RPlayerConfig*>(c);
}

DART_EXPORT void RPlayer_dispose(void* p) {
  RPlayer* pPlayer = static_cast<RPlayer*>(p);
  pPlayer->dispose();
}

DART_EXPORT int RPlayer_getHeight(void* p) {
  RPlayer* pPlayer = static_cast<RPlayer*>(p);
  return pPlayer->getHeight();
}

DART_EXPORT int RPlayer_getWidth(void* p) {
  RPlayer* pPlayer = static_cast<RPlayer*>(p);
  return pPlayer->getWidth();
}

DART_EXPORT int RPlayer_getState(void* p) {
  RPlayer* pPlayer = static_cast<RPlayer*>(p);
  return pPlayer->state;
}

DART_EXPORT char* RPlayer_getMessage(void* p) {
  RPlayer* pPlayer = static_cast<RPlayer*>(p);
  return pPlayer->msg;
}

DART_EXPORT long long RPlayer_getTextureId(void* p) {
  RPlayer* pPlayer = static_cast<RPlayer*>(p);
  if (pPlayer->render == nullptr) {
    return -1;
  }
  return pPlayer->render->id;
}

//////////////////////////////////////////
// RPlayerConfig
//////////////////////////////////////////

DART_EXPORT void* RPlayerConfig_createInstance() {
  return static_cast<void*>(RPlayerConfig::createInstance());
}

DART_EXPORT void RPlayerConfig_setRetryTimesOnDisconnect(void* c, int value) {
  RPlayerConfig* pConfig = static_cast<RPlayerConfig*>(c);
  pConfig->retryTimesOnDisconnect = value;
}

DART_EXPORT void RPlayerConfig_setRetryDelayInMilliseconds(void* c, int value) {
  RPlayerConfig* pConfig = static_cast<RPlayerConfig*>(c);
  pConfig->retryDelayInMilliseconds = value;
}

DART_EXPORT void RPlayerConfig_setPreferHardcodec(void* c, int value) {
  RPlayerConfig* pConfig = static_cast<RPlayerConfig*>(c);
  pConfig->preferHardcodec = value;
}
}
