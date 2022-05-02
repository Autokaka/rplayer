/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/02.
 */

#include "player_like.h"

void PlayerLike::Play() {
  std::scoped_lock lock(mutex_);
  if (status_ == RPlayerStatus::kReadyToPlay) {
    is_playing_ = true;
  }
}

void PlayerLike::Pause() {
  std::scoped_lock lock(mutex_);
  is_playing_ = false;
}

void PlayerLike::Stop() {
  std::scoped_lock lock(mutex_);
  is_stopped_ = true;
}
