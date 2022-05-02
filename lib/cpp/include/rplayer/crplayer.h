/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/01.
 */

#define DART_EXPORT __attribute__((visibility("default"))) __attribute__((used))

#ifdef __cplusplus
extern "C" {
#endif

DART_EXPORT int RPlayer_CreateFromURL(const char* url);

DART_EXPORT int RPlayer_GetStatus(int id);

DART_EXPORT const char* RPlayer_GetError(int id);

DART_EXPORT void RPlayer_Play(int id);

DART_EXPORT void RPlayer_Pause(int id);

DART_EXPORT void RPlayer_Release(int id);

#ifdef __cplusplus
}
#endif
