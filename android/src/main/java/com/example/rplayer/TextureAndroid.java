package com.example.rplayer;

import android.os.Handler;
import android.os.Looper;
import android.view.Surface;

import io.flutter.view.TextureRegistry;

public class TextureAndroid {
    public static TextureRegistry textureRegistry;
    public long id;
    public Surface surface;

    private static final Handler _mainHandler = new Handler(Looper.getMainLooper());
    private TextureRegistry.SurfaceTextureEntry _textureEntry;

    public TextureAndroid() {
        _mainHandler.post(new Runnable() {
            @Override
            public void run() {
                _textureEntry = textureRegistry.createSurfaceTexture();
                id = _textureEntry.id();
                surface = new Surface(_textureEntry.surfaceTexture());
            }
        });
    }

    public void release() {
        _mainHandler.post(new Runnable() {
            @Override
            public void run() {
                if (_textureEntry == null) {
                    return;
                }
                _textureEntry.release();
                _textureEntry = null;
            }
        });
    }
}
