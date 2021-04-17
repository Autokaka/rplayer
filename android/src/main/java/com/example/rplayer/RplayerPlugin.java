package com.example.rplayer;

import android.util.Log;
import android.view.Surface;

import androidx.annotation.NonNull;

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

    public static FlutterExternalTexture createExternalTexture() {
        Surface surface = new Surface(_textureEntry.surfaceTexture());
        return new FlutterExternalTexture(_textureEntry.id(), surface);
    }

    @Override
    public void onAttachedToEngine(@NonNull FlutterPluginBinding binding) {
        _textureEntry = binding.getTextureRegistry().createSurfaceTexture();
    }

    @Override
    public void onDetachedFromEngine(@NonNull FlutterPluginBinding binding) {

    }
}