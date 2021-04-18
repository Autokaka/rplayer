#pragma once

#include "flutter.h"
#include "log.h"
#include "rplayer_dart.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <pthread.h>

void* decodeThread(void* args);
}

