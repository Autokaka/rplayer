part of rplayer;

class RPlayerState {
  static const int INIT = 0;

  static const int STOPPED = 1;
  static const int BUFFERING = 2;
  static const int PLAYING = 3;

  static const int PAUSED = 4;
  static const int ERROR = 5;
}
