import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'rplayer_platform_interface.dart';

/// An implementation of [RplayerPlatform] that uses method channels.
class MethodChannelRplayer extends RplayerPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('rplayer');

  @override
  Future<String?> getPlatformVersion() async {
    final version = await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }
}
