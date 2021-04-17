#pragma once

#include "flutter.h"
#include "log.h"
#include "decoder.h"
#include "platform.h"
#include "rplayer_dart.h"

extern "C" {
#include <libavcodec/version.h>
#include <libavfilter/version.h>
#include <libavformat/version.h>
#include <libavutil/imgutils.h>
#include <libavutil/version.h>
#include <libswresample/version.h>
#include <libswscale/version.h>
#include <pthread.h>

FLUTTER_EXPORT char* getFFmpegVersion();

FLUTTER_EXPORT long initialize(char* url);

FLUTTER_EXPORT void stop();
}
