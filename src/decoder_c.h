#pragma once

#include "log.h"
#include "player.h"
#include "plugin/plugin_android.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
}

void* _decode(void*);
