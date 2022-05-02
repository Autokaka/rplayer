/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/02.
 */

#pragma once

#include <mutex>
#include <string>

#include "status.h"

class PlayerLikeDelegate {
 public:
  virtual void PlayerThrowsError(const std::string& error) = 0;
  virtual void PlayerStatusWillChange(RPlayerStatus status) = 0;
  virtual void PlayerStatusDidChange(RPlayerStatus status) = 0;
};

class PlayerLike {
 public:
  PlayerLikeDelegate* delegate = nullptr;

  virtual RPlayerStatus GetStatus() const final { return status_; }
  virtual const char* GetError() const final { return error_.c_str(); }

  virtual void Play();
  virtual void Pause();

  virtual void Stop();

 protected:
  std::mutex mutex_;

  bool is_playing_ = false;
  bool is_stopped_ = false;

  RPlayerStatus status_ = RPlayerStatus::kUnknown;
  std::string error_;
};
