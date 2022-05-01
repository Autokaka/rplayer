#include "include/rplayer/rplayer_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "rplayer_plugin.h"

void RPlayerPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  rplayer::RPlayerPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
