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
        child: TextButton(
          onPressed: () {
            Navigator.of(context).push(
              MaterialPageRoute(
                builder: (_) => VideoTestPage(),
              ),
            );
          },
          child: Text('Launch Video Test Page.'),
        ),
      ),
    );
  }
}
