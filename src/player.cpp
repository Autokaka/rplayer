#include "player.h"

RPlayer* RPlayer::createInstance() {
  RPlayer* pPlayer = new RPlayer();
  pPlayer->state = RPlayerState::INIT;
  return pPlayer;
}

int RPlayer::createDecodeThread(char* urlIn) {
  if (decodeThread != 0) {
    resetState();
  }

  url = urlIn;
  int createResult =
      pthread_create(&decodeThread, nullptr, _decode, static_cast<void*>(this));
  if (createResult == 0) {
    LOG::D("Decode thread created. PID: %ld", renderThread);
  }
  return createResult;
}

void RPlayer::releaseDecodeThread() {
  if (decodeThread == 0) {
    return LOG::D("Decode thread finished. PID: %ld", decodeThread);
  }
  if (int threadReturn = pthread_join(decodeThread, nullptr) != 0) {
    setError("Failed to detach decode thread: code %d.", threadReturn);
  }
  LOG::D("Decode thread finished. PID: %ld", decodeThread);
  decodeThread = 0;
  decoder = nullptr;
}

int RPlayer::createRenderThread() {
  if (renderThread != 0) {
    LOG::E("Render thread already exists, no need to create more.");
    return 0;
  }
  int createResult =
      pthread_create(&renderThread, nullptr, _render, static_cast<void*>(this));
  if (createResult == 0) {
    LOG::D("Render thread created. PID: %ld", renderThread);
  }
  return createResult;
}

void RPlayer::releaseRenderThread() {
  if (renderThread == 0) {
    return LOG::D("Render thread finished. PID: %ld", renderThread);
  }
  if (int threadReturn = pthread_join(renderThread, nullptr) != 0) {
    setError("Failed to detach render thread: code %d.", threadReturn);
  }
  LOG::D("Render thread finished. PID: %ld", renderThread);
  renderThread = 0;
  render = nullptr;
}

void RPlayer::dispose() {
  setStopped();

  releaseDecodeThread();

  config->release();
  config = nullptr;

  delete this;
}

int RPlayer::getHeight() {
  if (decoder == nullptr || decoder->codecContext == nullptr) {
    return 0;
  }
  return decoder->codecContext->height;
}

int RPlayer::getWidth() {
  if (decoder == nullptr || decoder->codecContext == nullptr) {
    return 0;
  }
  return decoder->codecContext->width;
}

void RPlayer::setBuffering() {
  if (state == RPlayerState::STOPPED || state == RPlayerState::ERROR) {
    LOG::E("Failed to set player state to BUFFERING: player in state %d",
           state);
    return;
  }
  state = RPlayerState::BUFFERING;
}

void RPlayer::setPlaying() {
  if (state != RPlayerState::PAUSED && state != RPlayerState::BUFFERING) {
    LOG::E("Failed to set player state to PLAYING: player in state %d", state);
    return;
  }
  state = RPlayerState::PLAYING;
}

void RPlayer::setPaused() {
  if (state != RPlayerState::PLAYING && state != RPlayerState::BUFFERING) {
    LOG::E("Failed to set player state to PAUSED: player in state %d", state);
    return;
  }
  state = RPlayerState::PAUSED;
}

void RPlayer::setStopped() {
  if (state == RPlayerState::ERROR) {
    LOG::E("Failed to set player state to STOPPED: player in state %d", state);
    return;
  }
  state = RPlayerState::STOPPED;
}

void RPlayer::setError(const char* fmt, ...) {
  if (state == RPlayerState::STOPPED) {
    LOG::E("Failed to set player state to ERROR: player in state %d", state);
    return;
  }
  state = RPlayerState::ERROR;
  va_list args;
  va_start(args, fmt);
  __android_log_vprint(ANDROID_LOG_ERROR, LOG::TAG, fmt, args);
  vsprintf(msg, fmt, args);
  va_end(args);
}

void RPlayer::resetState() {
  setStopped();
  releaseRenderThread();
  render = new TextureAndroid();
  releaseDecodeThread();
  decoder = RPlayerDecoder::createInstance();
  state = RPlayerState::INIT;
}
