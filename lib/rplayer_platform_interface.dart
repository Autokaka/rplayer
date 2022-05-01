import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'rplayer_method_channel.dart';

abstract class RplayerPlatform extends PlatformInterface {
  /// Constructs a RplayerPlatform.
  RplayerPlatform() : super(token: _token);

  static final Object _token = Object();

  static RplayerPlatform _instance = MethodChannelRplayer();

  /// The default instance of [RplayerPlatform] to use.
  ///
  /// Defaults to [MethodChannelRplayer].
  static RplayerPlatform get instance => _instance;
  
  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [RplayerPlatform] when
  /// they register themselves.
  static set instance(RplayerPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }
}
