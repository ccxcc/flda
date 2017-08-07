#include <jni.h>
#include <string>

int fl_main(const char *sp_file, const char *img_file);
int fc_main(const char *img_file);
int test_vecadd();

extern "C"
jstring
Java_com_kneron_flda_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    /*
    // Test dlib::face_landmark_test_ex
    int ret = fl_main("/data/local/tmp/shape_predictor_68_face_landmarks.dat", "/data/local/tmp/two.bmp");
    if(ret == 2)
        hello.append("\n[Got two faces] ");

    ret = fl_main("/data/local/tmp/shape_predictor_68_face_landmarks.dat", "/data/local/tmp/lena.bmp");
    if(ret == 1)
        hello.append("\n[Got Lena] ");

    // Test OpenCL with vector add
    ret = test_vecadd();
    if(ret > 0)
        hello.append("\n[Got OpenCL] ");

     */
    return env->NewStringUTF(hello.c_str());
}

extern "C"
jstring
Java_com_kneron_flda_MyGLSurfaceView_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    // Test dlib::face_landmark_test_ex
//    int ret = fl_main("/data/local/tmp/shape_predictor_68_face_landmarks.dat", "/data/local/tmp/lena.bmp");
//    if(ret == 1)
//        hello.append("\n[Got one Lena face] ");

    int ret = fc_main("/data/local/tmp/lena.bmp");
    if(ret == 0)
        hello.append("\n[No Face Detected] ");
    else
        hello.append("\n[Got Face Recog return] ");


    // Test OpenCL with vector add
    ret = test_vecadd();
    if(ret > 0)
        hello.append("\n[Got OpenCL] ");

    return env->NewStringUTF(hello.c_str());
}
