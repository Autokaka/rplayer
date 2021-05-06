import 'package:android_dpad_detector/dpad_detector.dart';
import 'package:flutter/material.dart';
import 'package:rplayer_example/video_page.dart';

class EntryPage extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('RPlayer example app'),
      ),
      body: Center(
        child: DPadDetector(
          onTap: () {
            Navigator.of(context).push(
              MaterialPageRoute(
                builder: (_) => VideoTestPage(),
              ),
            );
          },
          child: Padding(
            padding: EdgeInsets.all(5),
            child: Text('Launch Video Test Page.'),
          ),
        ),
      ),
    );
  }
}
