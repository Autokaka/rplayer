package com.example.rplayer;

import android.util.Log;
import androidx.annotation.NonNull;
import io.flutter.embedding.engine.plugins.FlutterPlugin;

public class RplayerPlugin implements FlutterPlugin {
    static {
        try {
            System.loadLibrary("rplayer");
        } catch (UnsatisfiedLinkError e) {
            Log.e("RplayerPlugin", e.getMessage());
        }
    }

    @Override
    public void onAttachedToEngine(@NonNull FlutterPluginBinding binding) {
        TextureAndroid.textureRegistry = binding.getTextureRegistry();
    }

    @Override
    public void onDetachedFromEngine(@NonNull FlutterPluginBinding binding) {
    }
}