# RPlayer

Flutter RTSP low latency video player for Android TV only. This player is intended to use FFmpeg under the hood to decode RTSP streams and Android SurfaceView to draw in native code. If time permits, I'll try to use OpenGLES as its backend renderer and OpenSLES as its audio solution. For now I only achieved video decode part.

## Minimal example

```dart
@override
void initState() {
  super.initState();
  player.startPlayWhenReady('rtsp://admin:admin@192.168.1.102:8554/live');
}

@override
Widget build(BuildContext context) {
  return Scaffold(
    appBar: AppBar(
      title: const Text('RPlayer Video Test Page'),
    ),
    body: RPlayerView(
      player: player,
      width: MediaQuery.of(context).size.width,
      height: 300,
    ),
  );
}

@override
void dispose() {
  player.dispose();
  super.dispose();
}
```

## Design

### RPlayerConfig

RPlayerConfig configures how videos will be played. You could only create a RPlayerConfig instance like this:

```dart
var rplayerConfig = RPlayerConfig()
  ..retryTimesOnDisconnect = 10
  ..retryDelayInMilliseconds = 2000
  ..preferHardcodec = 1;
```

- **retryTimesOnDisconnect**: Determine how many times the player will retry to connect when stream is closed.
- **retryDelayInMilliseconds**: Determine how long (in milliseconds) before the player make next reconnection when stream is closed.
- **preferHardcodec**: If set to `true`, the player will try to ask for a hardcodec first before playing video if ROM permits. If failed to allocate hardcodec, player will fallback to use a softcodec to play video.

### RPlayerState

RPlayerState exposes the inner state of RPlayer. The states are as follows:

- **INIT**:
- STOPPED:
- BUFFERING:
- PLAYING:

