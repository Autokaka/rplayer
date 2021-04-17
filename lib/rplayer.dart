import 'package:ffi/ffi.dart';
import 'dart:ffi';

import 'package:rplayer/native_library.dart';

class RPlayer {
  static final _so = DynamicLibrary.open("librplayer.so");
  final _lib = NativeLibrary(_so);

  String get ffmpegVersion =>
      _lib.getFFmpegVersion().cast<Utf8>().toDartString();

  static void Function(int textureId) onExternalTextureCreated =
      _onExternalTextureCreatedDefault;
  static void _onExternalTextureCreatedDefault(int textureId) {}

  int initialize(String url) =>
      _lib.initialize(url.toNativeUtf8().cast<Int8>());
}
