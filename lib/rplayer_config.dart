part of rplayer;

class RPlayerConfig {
  final Pointer<Void> _pNativeConfig;

  RPlayerConfig() : _pNativeConfig = _lib.RPlayerConfig_createInstance();

  set retryTimesOnDisconnect(int value) =>
      _lib.RPlayerConfig_setRetryTimesOnDisconnect(_pNativeConfig, value);

  set retryDelayInMilliseconds(int value) =>
      _lib.RPlayerConfig_setRetryDelayInMilliseconds(_pNativeConfig, value);

  set preferHardcodec(int value) =>
      _lib.RPlayerConfig_setPreferHardcodec(_pNativeConfig, value);
}
