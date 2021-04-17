#include "platform.h"

#pragma mark FlutterTextureAndroid
jclass FlutterTextureAndroid::_clazz;

void FlutterTextureAndroid::JNIRegister(JNIEnv *env) {
    jclass J_FlutterTextureAndroid = env->FindClass("com/example/rplayer/FlutterExternalTexture");
    _clazz = (jclass) env->NewGlobalRef(J_FlutterTextureAndroid);
}

FlutterTextureAndroid::FlutterTextureAndroid(JNIEnv *env, jobject obj) {
    _env = env;
    _obj = obj;

    _surface_FI = env->GetFieldID(_clazz, "surface", "Landroid/view/Surface;");
    _id_FI = env->GetFieldID(_clazz, "id", "J");
}

jobject FlutterTextureAndroid::getSurface() {
    return _env->GetObjectField(_obj, _surface_FI);
}

jlong FlutterTextureAndroid::getId() {
    return _env->GetLongField(_obj, _id_FI);
}

#pragma mark RplayerAndroid
jclass RplayerAndroid::_clazz;
jmethodID RplayerAndroid::_createExternalTexture_MID;

void RplayerAndroid::JNIRegister(JNIEnv *env) {
    jclass J_FlutterTextureAndroid = env->FindClass("com/example/rplayer/RplayerPlugin");
    _clazz = (jclass) env->NewGlobalRef(J_FlutterTextureAndroid);

    _createExternalTexture_MID = env->GetStaticMethodID(_clazz,
                                                        "createExternalTexture",
                                                        "()Lcom/example/rplayer/FlutterExternalTexture;");
}

jobject RplayerAndroid::createExternalTexture(JNIEnv *env) {
    return env->CallStaticObjectMethod(_clazz,  _createExternalTexture_MID);
}

#pragma mark Platform
extern "C" {

JNIEnv *getJNIEnv(int *attach) {
    if (pJvm == NULL) return NULL;

    *attach = 0;
    JNIEnv *env = NULL;

    int status = pJvm->GetEnv((void **) &env, JNI_VERSION_1_6);

    if (status == JNI_EDETACHED || env == NULL) {
        status = pJvm->AttachCurrentThread(&env, NULL);
        if (status < 0) {
            env = NULL;
        } else {
            *attach = 1;
        }
    }
    return env;
}

void delJNIEnv() {
    pJvm->DetachCurrentThread();
}

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *jvm, void *reserved) {
    pJvm = jvm;
    JNIEnv *env;

    if (jvm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    // register plugin and its methods
    RplayerAndroid::JNIRegister(env);
    FlutterTextureAndroid::JNIRegister(env);

    return JNI_VERSION_1_6;
}

long createWindowFromAndroid(ANativeWindow** nativeWindow) {
    int attach = 0;
    JNIEnv *env = getJNIEnv(&attach);
    if (attach != 1) {
        return -1;
    }

    jobject externalTexture = RplayerAndroid::createExternalTexture(env);
    FlutterTextureAndroid *flutterExtTexture = new FlutterTextureAndroid(env, externalTexture);
    jobject surface = flutterExtTexture->getSurface();
    jlong textureId = flutterExtTexture->getId();
    ANativeWindow *pWindow(ANativeWindow_fromSurface(env, surface));
    *nativeWindow = pWindow;
    delJNIEnv();
    return textureId;
}
}