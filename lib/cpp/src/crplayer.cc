/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/01.
 */

#include <rplayer/crplayer.h>
#include <rplayer/rplayer.h>
#include <algorithm>
#include <mutex>
#include <unordered_map>

#include "base/type_guard.h"

namespace {

std::mutex rplayer_mutex_;
std::unordered_map<int, RPlayerPtr> players_;
int ids_ = 0;

RPlayerPtr FindRPlayerById(int id) {
  auto result = players_.find(id);
  if (result == players_.end()) {
    return nullptr;
  }
  return result->second;
}

int AllocIdForRPlayer(const RPlayerPtr& rplayer) {
  std::scoped_lock lock(rplayer_mutex_);
  for (auto&& player : players_) {
    if (player.second == rplayer) {
      return player.first;
    }
  }
  ids_ += 1;
  players_[ids_] = rplayer;
  return ids_;
}

}  // namespace

#ifdef __cplusplus
extern "C" {
#endif

int RPlayer_CreateFromURL(const char* url) {
  return AllocIdForRPlayer(RPlayer::CreateFromURL(url));
}

int RPlayer_GetStatus(int id) {
  std::scoped_lock lock(rplayer_mutex_);
  auto player = FindRPlayerById(id);

  if (isNull(player)) {
    return static_cast<int>(RPlayerStatus::kUnknown);
  }
  return static_cast<int>(player->GetStatus());
}

const char* RPlayer_GetError(int id) {
  std::scoped_lock lock(rplayer_mutex_);
  auto player = FindRPlayerById(id);

  if (isNull(player)) {
    return nullptr;
  }
  return player->GetError();
}

void RPlayer_Play(int id) {
  std::scoped_lock lock(rplayer_mutex_);
  auto player = FindRPlayerById(id);

  if (isNull(player)) {
    return;
  }
  return player->Play();
}

void RPlayer_Pause(int id) {
  std::scoped_lock lock(rplayer_mutex_);
  auto player = FindRPlayerById(id);

  if (isNull(player)) {
    return;
  }
  return player->Pause();
}

void RPlayer_Release(int id) {
  RPlayer_Pause(id);

  std::scoped_lock lock(rplayer_mutex_);
  players_.erase(id);
}

#ifdef __cplusplus
}
#endif
