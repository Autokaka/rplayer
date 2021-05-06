library rplayer;

import 'dart:async';
import 'dart:ffi';

import 'package:ffi/ffi.dart';
import 'package:flutter/material.dart';

import 'package:rplayer/native_library.dart';

part 'rplayer_config.dart';
part 'rplayer_state.dart';
part 'rplayer_view.dart';

final _lib = NativeLibrary(DynamicLibrary.open('librplayer.so'));

class RPlayer with ChangeNotifier {
  final Pointer<Void> _pNativePlayer;
  late final Timer _observer;

  RPlayer({
    RPlayerConfig? config,
  }) : _pNativePlayer = _lib.RPlayer_createInstance() {
    if (config != null) {
      setConfig(config);
    }

    _observer = Timer.periodic(Duration(seconds: 1), (_) {
      notifyListeners();
    });
  }

  int get textureId {
    return _lib.RPlayer_getTextureId(_pNativePlayer);
  }

  int get state {
    return _lib.RPlayer_getState(_pNativePlayer);
  }

  Size get size {
    return Size(
      _lib.RPlayer_getWidth(_pNativePlayer).toDouble(),
      _lib.RPlayer_getHeight(_pNativePlayer).toDouble(),
    );
  }

  int get width {
    return _lib.RPlayer_getWidth(_pNativePlayer);
  }

  int get height {
    return _lib.RPlayer_getHeight(_pNativePlayer);
  }

  String get message {
    return _lib.RPlayer_getMessage(_pNativePlayer).cast<Utf8>().toDartString();
  }

  int startPlayWhenReady(String url) {
    return _lib.RPlayer_createDecodeThread(
      _pNativePlayer,
      url.toNativeUtf8().cast<Int8>(),
    );
  }

  void continuePlay() {
    _lib.RPlayer_setPlaying(_pNativePlayer);
  }

  void pause() {
    _lib.RPlayer_setPaused(_pNativePlayer);
  }

  void setConfig(RPlayerConfig config) {
    _lib.RPlayer_setConfig(_pNativePlayer, config._pNativeConfig);
  }

  @override
  void dispose() {
    _observer.cancel();
    _lib.RPlayer_dispose(_pNativePlayer);
    super.dispose();
  }
}
