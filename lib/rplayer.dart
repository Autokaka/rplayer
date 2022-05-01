import 'rplayer_platform_interface.dart';

class RPlayer {
  Future<String?> getPlatformVersion() {
    return RPlayerPlatform.instance.getPlatformVersion();
  }
}
