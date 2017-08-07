#include <jni.h>

int initCL();
void closeCL();
void processFrame(int tex1, int tex2, int w, int h, int mode);

extern "C"
jint Java_com_kneron_flda_NativePart_initCL(JNIEnv * env, jclass cls)
{
    return initCL();
}

extern "C"
void Java_com_kneron_flda_NativePart_closeCL(JNIEnv * env, jclass cls)
{
    closeCL();
}

extern "C"
void Java_com_kneron_flda_NativePart_processFrame(JNIEnv * env, jclass cls, jint tex1, jint tex2, jint w, jint h, jint mode)
{
    processFrame(tex1, tex2, w, h, mode);
}

/*
JNIEXPORT jint JNICALL Java_com_kneron_flda_NativePart_initCL(JNIEnv * env, jclass cls)
{
    return initCL();
}

JNIEXPORT void JNICALL Java_com_kneron_flda_NativePart_closeCL(JNIEnv * env, jclass cls)
{
    closeCL();
}

JNIEXPORT void JNICALL Java_com_kneron_flda_NativePart_processFrame(JNIEnv * env, jclass cls, jint tex1, jint tex2, jint w, jint h, jint mode)
{
    processFrame(tex1, tex2, w, h, mode);
}
*/