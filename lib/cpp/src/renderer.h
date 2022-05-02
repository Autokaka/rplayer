/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/02.
 */

#pragma once

#include <thread>

#include "base/macros.h"
#include "player_like.h"

class RRenderer final : public PlayerLike {
 public:
  explicit RRenderer();

 private:
  std::thread render_thread_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(RRenderer);
};
