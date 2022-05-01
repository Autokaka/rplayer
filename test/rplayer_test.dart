import 'package:flutter_test/flutter_test.dart';
import 'package:rplayer/rplayer.dart';
import 'package:rplayer/rplayer_platform_interface.dart';
import 'package:rplayer/rplayer_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockRplayerPlatform 
    with MockPlatformInterfaceMixin
    implements RplayerPlatform {

  @override
  Future<String?> getPlatformVersion() => Future.value('42');
}

void main() {
  final RplayerPlatform initialPlatform = RplayerPlatform.instance;

  test('$MethodChannelRplayer is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelRplayer>());
  });

  test('getPlatformVersion', () async {
    Rplayer rplayerPlugin = Rplayer();
    MockRplayerPlatform fakePlatform = MockRplayerPlatform();
    RplayerPlatform.instance = fakePlatform;
  
    expect(await rplayerPlugin.getPlatformVersion(), '42');
  });
}
