import 'package:flutter/material.dart';
import 'package:rplayer/rplayer.dart';

class VideoTestPage extends StatefulWidget {
  @override
  _VideoTestPageState createState() => _VideoTestPageState();
}

class _VideoTestPageState extends State<VideoTestPage> {
  final player = RPlayer();

  @override
  void initState() {
    super.initState();
    player.startPlayWhenReady("rtsp://admin:admin@192.168.1.101:8554/live");
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('RPlayer Video Test Page'),
      ),
      body: RPlayerView(player),
    );
  }
}
