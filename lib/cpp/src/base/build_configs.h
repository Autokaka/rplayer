#pragma once

#include <string_view>

namespace forge {

/////////////////////////////////////////////
// Engine ///////////////////////////////////
/////////////////////////////////////////////
static constexpr std::string_view TAG = "RPlayer";

/////////////////////////////////////////////
// Mode /////////////////////////////////////
/////////////////////////////////////////////
#ifdef NDEBUG
#define kReleaseMode true
#define kDebugMode false
#else
#define kReleaseMode false
#define kDebugMode true
#endif

/////////////////////////////////////////////
// Darwin ///////////////////////////////////
/////////////////////////////////////////////
#if __APPLE__
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE
#define kTargetIOS true
#define kTargetMacOS false
#define kTargetAndroid false
#endif
#if TARGET_OS_MAC
#define kTargetIOS false
#define kTargetMacOS true
#define kTargetAndroid false
#endif
#if TARGET_IPHONE_SIMULATOR
#define kIsSimulator true
#else
#define kIsSimulator false
#endif
#endif

/////////////////////////////////////////////
// Android //////////////////////////////////
/////////////////////////////////////////////
#if __ANDROID__
#define kTargetIOS false
#define kTargetMacOS false
#define kTargetAndroid true
#define kIsSimulator false
#endif

}  // namespace forge
