#pragma once

#include "flutter.h"
#include "log.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <pthread.h>

void* decodeThread(void* args);
}
