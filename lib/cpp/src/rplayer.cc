/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/01.
 */

#include <rplayer/rplayer.h>

#include "base/logging.h"

RPlayerPtr RPlayer::CreateFromURL(const std::string& url) {
  return std::make_shared<RPlayer>(url);
}

RPlayer::RPlayer(const std::string& url) : decoder_(url) {
  decoder_.delegate = this;
  renderer_.delegate = this;
}

void RPlayer::Play() {
  PlayerLike::Play();
  decoder_.Play();
  renderer_.Play();
}

void RPlayer::Pause() {
  PlayerLike::Pause();
  decoder_.Pause();
  renderer_.Pause();
}

void RPlayer::Stop() {
  PlayerLike::Stop();
  decoder_.Stop();
  renderer_.Stop();
}

// Delegate

void RPlayer::PlayerThrowsError(const std::string& error) {
  Stop();

  std::scoped_lock lock(mutex_);
  status_ = RPlayerStatus::kFailed;
  error_ = error;
  LOG(ERROR) << error;
}

void RPlayer::PlayerStatusWillChange(RPlayerStatus status) {
  status_ = status;
}

void RPlayer::PlayerStatusDidChange(RPlayerStatus status) {}
