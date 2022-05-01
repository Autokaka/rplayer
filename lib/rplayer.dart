
import 'rplayer_platform_interface.dart';

class Rplayer {
  Future<String?> getPlatformVersion() {
    return RplayerPlatform.instance.getPlatformVersion();
  }
}
