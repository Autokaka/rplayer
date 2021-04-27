#include "jni_onload.h"

extern "C" {
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* jvm, void* reserved) {
    JNIEnv* env;

    if (jvm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    // register plugin and its methods
    FFIAndroid::FFIRegister(jvm);
    PluginAndroid::FFIRegister(env);
    TextureAndroid::FFIRegister(env);

    return JNI_VERSION_1_6;
}
}