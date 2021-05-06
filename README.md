# rplayer

Flutter RTSP low latency video player for Android TV only. This player is intended to use FFmpeg under the hood to decode RTSP streams and Android SurfaceView to draw in native code. If time permits, I'll try to use OpenGLES as its backend renderer and OpenSLES as its audio solution. For now I only achieved video decode part.

## Plugin Design

### Decoder

#### decode

1. Dart FFI + ffigen to bind native code with Flutter.
2. Use binded code to control decode logic.
3. Decoder thread create frame receive loop.

### Player

#### init()

1. Flutter init player. 
2. Dart FFI use native code to create a Surface on Android platform via JNI.
3. Dart FFI use native code to get Flutter Plugin TextureEntry via JNI.
4. Native code return TextureEntry id to Flutter.

#### draw()

1. Native code draw video picture on AWindow.
2. Flutter use Texture(TextureEntry id) to display video view.

## TODO:

- [x] Precompile FFmpeg.
- [x] Embed FFmpeg to Android platform.
- [x] Generate Dart FFI bindings for Flutter Plugin.
- [x] Implement decode RTSP video streams logic.
- [x] Init plugin on Android platform, return TextureEntry id to Flutter Plugin.
- [x] Draw image to Flutter External Texture.
- [x] RTSP low latency.
- [x] Full Dart RPlayer control.
- [x] Functional RPlayerView.
- [ ] Video & Audio syncing.
- [x] Make FFmpeg related shared libraries static, and link librplayer.so as a whole shared library. 
- [x] Shrink librplayer.so size to about 2MB.