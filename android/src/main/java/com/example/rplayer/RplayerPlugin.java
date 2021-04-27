package com.example.rplayer;

import android.graphics.SurfaceTexture;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.Surface;

import androidx.annotation.MainThread;
import androidx.annotation.NonNull;
import androidx.annotation.UiThread;

import io.flutter.embedding.engine.plugins.FlutterPlugin;
import io.flutter.view.TextureRegistry;

public class RplayerPlugin implements FlutterPlugin {
    static {
        try {
            System.loadLibrary("rplayer");
        } catch (UnsatisfiedLinkError e) {
            Log.e("RplayerPlugin", e.getMessage());
        }
    }

    private static TextureRegistry.SurfaceTextureEntry _textureEntry;
    private static SurfaceTexture _surfaceTexture;

    public static FlutterExternalTexture createExternalTexture() {
        if (_surfaceTexture == null) {
            _surfaceTexture = _textureEntry.surfaceTexture();
        }
        return new FlutterExternalTexture(_textureEntry.id(), new Surface(_surfaceTexture));
    }

    public static void releaseExternalTexture(FlutterExternalTexture externalTexture) {
        if (externalTexture.surface.isValid()) {
            externalTexture.surface.release();
        }
    }

    @Override
    public void onAttachedToEngine(@NonNull FlutterPluginBinding binding) {
        _textureEntry = binding.getTextureRegistry().createSurfaceTexture();
    }

    @Override
    public void onDetachedFromEngine(@NonNull FlutterPluginBinding binding) {
        if (_textureEntry != null) {
            _textureEntry.release();
            _textureEntry = null;
        }
    }
}