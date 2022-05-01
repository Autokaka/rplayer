import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'rplayer_method_channel.dart';

abstract class RPlayerPlatform extends PlatformInterface {
  /// Constructs a RPlayerPlatform.
  RPlayerPlatform() : super(token: _token);

  static final Object _token = Object();

  static RPlayerPlatform _instance = MethodChannelRPlayer();

  /// The default instance of [RPlayerPlatform] to use.
  ///
  /// Defaults to [MethodChannelRPlayer].
  static RPlayerPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [RPlayerPlatform] when
  /// they register themselves.
  static set instance(RPlayerPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }
}
