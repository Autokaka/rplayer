package com.example.rplayer;

import android.view.Surface;

public class FlutterExternalTexture {
    public long id;
    public Surface surface;

    public FlutterExternalTexture(long id, Surface surface) {
        this.id = id;
        this.surface = surface;
    }
}
