/**
 * # Reference file for Dart ffigen.
 * Real header file is in `ffi_flutter.h`.
 */

//////////////////////////////////////////
// RPlayer
//////////////////////////////////////////

/**
 * Create RPlayer instance,
 * initialize its state, and
 * return that pointer to Dart
 * side.
 */
void* RPlayer_createInstance();

/**
 * Create decode thread using FFmpeg.
 *
 * @param void* pPlayer.
 * @param char* url.
 */
int RPlayer_createDecodeThread(void*, char*);

void RPlayer_setPlaying(void*);

void RPlayer_setPaused(void*);

void RPlayer_setConfig(void*, void*);

/**
 * Release all resources used in decode thread.
 */
void RPlayer_dispose(void*);

int RPlayer_getHeight(void*);

int RPlayer_getWidth(void*);

int RPlayer_getState(void*);

char* RPlayer_getMessage(void*);

long long RPlayer_getTextureId(void*);

//////////////////////////////////////////
// RPlayerConfig
//////////////////////////////////////////

/**
 * See [src/config.h] for the definitions
 * of RPlayerConfig.
 */
void* RPlayerConfig_createInstance();

void RPlayerConfig_setRetryTimesOnDisconnect(void*, int);

void RPlayerConfig_setRetryDelayInMilliseconds(void*, int);

void RPlayerConfig_setPreferHardcodec(void*, int);
