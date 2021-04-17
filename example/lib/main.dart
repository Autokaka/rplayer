import 'package:flutter/material.dart';
import 'package:rplayer/rplayer.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  final player = RPlayer();
  int textureId = -1;

  @override
  void initState() {
    super.initState();
    textureId = player.initialize("rtsp://admin:admin@192.168.1.4:8554/live");
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('RPlayer example app'),
        ),
        body: textureId == -1
            ? Center(
                child: Text("Loading stream..."),
              )
            : SizedBox(
                width: 500,
                height: 500,
                child: Texture(textureId: textureId),
              ),
      ),
    );
  }
}
