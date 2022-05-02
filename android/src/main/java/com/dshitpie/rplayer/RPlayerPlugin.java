package com.dshitpie.rplayer;

import android.util.Log;

import androidx.annotation.NonNull;

import io.flutter.embedding.engine.plugins.FlutterPlugin;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel;
import io.flutter.plugin.common.MethodChannel.MethodCallHandler;
import io.flutter.plugin.common.MethodChannel.Result;

/** RPlayerPlugin */
public class RPlayerPlugin implements FlutterPlugin, MethodCallHandler {
  static {
      try {
          System.loadLibrary("rplayer");
      } catch (UnsatisfiedLinkError e) {
          Log.e("RPlayerPlugin", e.getMessage());
      }
  }

  @Override
  public void onAttachedToEngine(@NonNull FlutterPluginBinding flutterPluginBinding) {
  }

  @Override
  public void onMethodCall(@NonNull MethodCall call, @NonNull Result result) {
  }

  @Override
  public void onDetachedFromEngine(@NonNull FlutterPluginBinding binding) {
  }
}
