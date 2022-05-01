#ifndef FLUTTER_PLUGIN_RPLAYER_PLUGIN_H_
#define FLUTTER_PLUGIN_RPLAYER_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace rplayer {

class RplayerPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  RplayerPlugin();

  virtual ~RplayerPlugin();

  // Disallow copy and assign.
  RplayerPlugin(const RplayerPlugin&) = delete;
  RplayerPlugin& operator=(const RplayerPlugin&) = delete;

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

}  // namespace rplayer

#endif  // FLUTTER_PLUGIN_RPLAYER_PLUGIN_H_
