#pragma once

#include <jni.h>
#include <android/native_window_jni.h>

#include "flutter.h"
#include "log.h"

#pragma mark FlutterTextureAndroid
class FlutterTextureAndroid {
public:
    static void JNIRegister(JNIEnv* env);

    FlutterTextureAndroid(JNIEnv* env, jobject obj);

    jobject getSurface();
    jlong getId();

private:
    static jclass _clazz;

    JNIEnv* _env;
    jobject _obj;

    jfieldID _surface_FI;
    jfieldID _id_FI;
};

#pragma mark RplayerAndroid
class RplayerAndroid {
public:
    static void JNIRegister(JNIEnv* env);
    static jobject createExternalTexture(JNIEnv* env);

    RplayerAndroid();

private:
    static jclass _clazz;

    static jmethodID _createExternalTexture_MID;
};

#pragma mark Platform
extern "C" {

static JavaVM *pJvm;

JNIEnv *getJNIEnv(int *attach);
void delJNIEnv();
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *jvm, void *reserved);

long createWindowFromAndroid(ANativeWindow** nativeWindow);

}
