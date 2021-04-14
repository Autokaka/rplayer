import 'dart:ffi';

import 'package:rplayer/native_library.dart';

class RPlayer {
  /// Since this plugin is just intended for Android
  /// platform, we don't need to care about loading
  /// logic for other platforms. If you want to extend
  /// this plugin for other platforms, plaease refer to:
  /// https://flutter.cn/docs/development/platform-integration/c-interop
  static final _so = DynamicLibrary.open("librplayer.so");
  final lib = NativeLibrary(_so);
}
