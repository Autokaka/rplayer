/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/01.
 */

#pragma once

#include <string>

#include "base/macros.h"
#include "decoder.h"
#include "renderer.h"

class RPlayer final : public PlayerLike, PlayerLikeDelegate {
 public:
  using RPlayerPtr = std::shared_ptr<RPlayer>;

  static RPlayerPtr CreateFromURL(const std::string& url);
  explicit RPlayer(const std::string& url);

  void Play() override;
  void Pause() override;
  void Stop() override;

  // Delegate

  void PlayerThrowsError(const std::string& error) override;
  void PlayerStatusWillChange(RPlayerStatus status) override;
  void PlayerStatusDidChange(RPlayerStatus status) override;

 private:
  RDecoder decoder_;
  RRenderer renderer_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(RPlayer);
};

using RPlayerPtr = RPlayer::RPlayerPtr;
