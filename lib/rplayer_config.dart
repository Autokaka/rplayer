part of rplayer;

class RPlayerConfig {
  final Pointer<Void> _pNativeConfig;

  RPlayerConfig() : _pNativeConfig = _lib.RplayerConfig_createInstance();

  set retryTimesOnDisconnect(int value) =>
      _lib.RplayerConfig_setRetryTimesOnDisconnect(_pNativeConfig, value);
}
