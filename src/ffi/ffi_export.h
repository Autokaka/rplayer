/**
 * # Reference file for Dart ffigen.
 * Real header file is in `ffi_flutter.h`.
 */

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

/**
 * Release all resources used in decode thread.
 */
int RPlayer_dispose(void*);

int RPlayer_getHeight(void*);

int RPlayer_getWidth(void*);

int RPlayer_getState(void*);

long RPlayer_getTextureId(void*);
