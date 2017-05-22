#include <jni.h>
#include <android/log.h>
#ifdef __cplusplus
extern "C" {
#endif
#include "include/decode_video.h"

#ifdef __cplusplus
};
#endif

#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"native_lib",FORMAT,##__VA_ARGS__);
#ifdef __cplusplus
extern "C"{
#endif


JNIEXPORT void JNICALL
Java_com_example_sail_ffmpegdemo_AideoUtils_decode(JNIEnv *env, jclass type, jstring input_,
                                                   jstring output_) {
    const char *input = env->GetStringUTFChars(input_, 0);
    const char *output = env->GetStringUTFChars(output_, 0);

    if(0 != decode(input,output))
    {
        LOGE("%s","视频解码失败");
        return;
    }
    env->ReleaseStringUTFChars(input_, input);
    env->ReleaseStringUTFChars(output_, output);
}


#ifdef __cplusplus
};
#endif