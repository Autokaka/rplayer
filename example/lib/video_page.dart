import 'package:android_dpad_detector/dpad_detector.dart';
import 'package:flutter/material.dart';
import 'package:rplayer/rplayer.dart';

class VideoTestPage extends StatefulWidget {
  @override
  _VideoTestPageState createState() => _VideoTestPageState();
}

class _VideoTestPageState extends State<VideoTestPage> {
  final player = RPlayer(
    config: RPlayerConfig()
      ..retryTimesOnDisconnect = 10
      ..retryDelayInMilliseconds = 2000
      ..preferHardcodec = 0,
  );
  // final player2 = RPlayer(
  //   config: RPlayerConfig()
  //     ..retryTimesOnDisconnect = 10
  //     ..retryDelayInMilliseconds = 2000,
  // );

  @override
  void initState() {
    super.initState();
    player.startPlayWhenReady('rtsp://admin:admin@192.168.1.102:8554/live');
    // player2.startPlayWhenReady('rtsp://admin:admin@192.168.1.104:8554/live');
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('RPlayer Video Test Page'),
      ),
      body: Center(
        child: Flex(
          direction: (MediaQuery.of(context).size.width >
                  MediaQuery.of(context).size.height)
              ? Axis.horizontal
              : Axis.vertical,
          children: [
            Expanded(
              child: DPadDetector(
                child: RPlayerView(
                  player: player,
                  width: MediaQuery.of(context).size.width,
                  height: 300,
                ),
              ),
            ),
            // Expanded(
            //   child: DPadDetector(
            //     child: RPlayerView(
            //       player: player2,
            //       width: MediaQuery.of(context).size.width,
            //       height: 300,
            //     ),
            //   ),
            // ),
          ],
        ),
      ),
    );
  }

  @override
  void dispose() {
    player.dispose();
    // player2.dispose();
    super.dispose();
  }
}
