import 'dart:ffi';
import 'dart:io';

import 'package:ffi/ffi.dart';

import "rplayer_ffi.g.dart";

enum RPlayerStatus { kUnknown, kReadyToPlay, kFailed }

class RPlayer {
  static final _lib = NativeLibrary(Platform.isAndroid
      ? DynamicLibrary.open('librplayer.so')
      : DynamicLibrary.process());

  static RPlayer createFromURL(String url) {
    return RPlayer._(_lib.RPlayer_CreateFromURL(url.toNativeUtf8().cast()));
  }

  RPlayer._(this._id);

  final int _id;

  RPlayerStatus get status {
    return _lib.RPlayer_GetStatus(_id) as RPlayerStatus;
  }

  String get error {
    return _lib.RPlayer_GetError(_id).toString();
  }

  void play() {
    return _lib.RPlayer_Play(_id);
  }

  void pause() {
    return _lib.RPlayer_Pause(_id);
  }

  void dispose() {
    return _lib.RPlayer_Release(_id);
  }
}
