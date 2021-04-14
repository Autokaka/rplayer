import 'package:flutter/material.dart';
import 'package:rplayer/rplayer.dart';
import 'package:ffi/ffi.dart';
import 'dart:ffi';

void main() {
  runApp(MyApp());
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  final player = RPlayer();

  @override
  void initState() {
    super.initState();
    player.lib.play("rtsp://admin:admin@192.168.123.68:8554/live"
        .toNativeUtf8()
        .cast<Int8>());
  }

  @override
  Widget build(BuildContext context) {
    final ffmpegVersion =
        player.lib.getFFmpegVersion().cast<Utf8>().toDartString();

    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('RPlayer example app'),
        ),
        body: Center(
          child: Text(ffmpegVersion),
        ),
      ),
    );
  }
}
