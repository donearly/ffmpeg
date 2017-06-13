#include <jni.h>
#include <android/log.h>


extern "C" {

#include "libswscale/swscale.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"

};


#define LOGI(FORMAT,...) __android_log_print(ANDROID_LOG_INFO,"Sail",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"Sail",FORMAT,##__VA_ARGS__);
#ifdef __cplusplus
extern "C"{
#endif


JNIEXPORT void JNICALL
Java_com_example_sail_ffmpegdemo_VideoUtils_decode(JNIEnv *env, jclass type, jstring input_,
                                                   jstring output_) {
    const char *input_cstr = env->GetStringUTFChars(input_, 0);
    const char *output_cstr = env->GetStringUTFChars(output_, 0);
    LOGE("%s",input_cstr);
    //1.注册所有组件
    av_register_all();

    //封装格式上下文，统领全局的结构体，保存了视频文件封装格式的相关信息
    AVFormatContext *pFormatCtx = avformat_alloc_context();

    //2.打开输入视频文件
    //可以根据返回值判断不能打开文件的原因
    if (avformat_open_input(&pFormatCtx, input_cstr, NULL, NULL) != 0)
    {
        LOGE("%s","无法打开输入视频文件");
        return;
    }

    //3.获取视频文件信息
    if (avformat_find_stream_info(pFormatCtx,NULL) < 0)
    {
        LOGE("%s","无法获取视频文件信息");
        return;
    }

    //获取视频流的索引位置
    //遍历所有类型的流（音频流、视频流、字幕流），找到视频流
    int v_stream_idx = -1;
    int i = 0;
    //number of streams
    AVStream* stream = NULL ;
    for (; i < pFormatCtx->nb_streams; i++)
    {
        //流的类型
        if(pFormatCtx->streams[i]->codecpar->codec_type==AVMEDIA_TYPE_VIDEO)
        {
            stream = pFormatCtx->streams[i] ;
            v_stream_idx = i;
        }
    }

    if (v_stream_idx == -1)
    {
        LOGE("%s","找不到视频流\n");
        return;
    }

    //只有知道视频的编码方式，才能够根据编码方式去找到解码器
    //获取视频流中的编解码上下文
    AVCodecContext *pCodecCtx ;
    //4.根据编解码上下文中的编码id查找对应的解码
    AVCodec *pCodec = avcodec_find_decoder(stream->codecpar->codec_id);
    if (pCodec == NULL)
    {
        LOGE("%s","找不到解码器\n");
        return;
    }

    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (NULL == pCodecCtx)
    {
        LOGE("%s","AVCodecContext 内存分配失败！\n");
        return ;
    }

    if(avcodec_parameters_to_context(pCodecCtx,stream->codecpar) < 0)
    {
        LOGE("%s","avcodec_parameters_to_context 失败！\n");
        return ;
    }

    //5.打开解码器
    if (avcodec_open2(pCodecCtx,pCodec,NULL)<0)
    {
        LOGE("%s","解码器无法打开\n");
        return;
    }

    //输出视频信息
    LOGI("视频的文件格式：%s",pFormatCtx->iformat->name);
    LOGI("视频时长：%d", pFormatCtx->duration / 1000000);
    LOGI("视频的宽高：%d,%d",pCodecCtx->width,pCodecCtx->height);
    LOGI("解码器的名称：%s",pCodec->name);

    //准备读取
    //AVPacket用于存储一帧一帧的压缩数据（H264）
    //缓冲区，开辟空间
    AVPacket *packet = (AVPacket*)av_malloc(sizeof(AVPacket));

    //AVFrame用于存储解码后的像素数据(YUV)
    //内存分配
    AVFrame *pFrame = av_frame_alloc();
    //YUV420
    AVFrame *pFrameYUV = av_frame_alloc();
    //只有指定了AVFrame的像素格式、画面大小才能真正分配内存
    //缓冲区分配内存
    uint8_t *out_buffer = (uint8_t *)av_malloc(
            (size_t) av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1));
    //初始化
    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer,
                         AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1);
    //用于转码（缩放）的参数，转之前的宽高，转之后的宽高，格式等
    struct SwsContext *sws_ctx = sws_getContext(pCodecCtx->width,pCodecCtx->height,pCodecCtx->pix_fmt,
                                                pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P,
                                                SWS_BICUBIC, NULL, NULL, NULL);


    int got_picture, ret;
    FILE *fp_yuv = fopen(output_cstr, "wb+");
    int frame_count = 0;

    //6.一帧一帧的读取压缩数据
    while (av_read_frame(pFormatCtx, packet) >= 0)
    {
        //只要视频压缩数据（根据流的索引位置判断）
        if (packet->stream_index == v_stream_idx)
        {
            //7.解码一帧视频压缩数据，得到视频像素数据
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
            if (ret < 0)
            {
                LOGE("%s","解码错误");
                return;
            }

            //为0说明解码完成，非0正在解码
            if (got_picture)
            {
                //AVFrame转为像素格式YUV420，宽高
                //2 6输入、输出数据
                //3 7输入、输出画面一行的数据的大小 AVFrame 转换是一行一行转换的
                //4 输入数据第一列要转码的位置 从0开始
                //5 输入画面的高度
                sws_scale(sws_ctx, (const uint8_t *const *) pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
                          pFrameYUV->data, pFrameYUV->linesize);

                //输出到YUV文件
                //AVFrame像素帧写入文件
                //data解码后的图像像素数据（音频采样数据）
                //Y 亮度 UV 色度（压缩了） 人对亮度更加敏感
                //U V 个数是Y的1/4
                int y_size = pCodecCtx->width * pCodecCtx->height;
                fwrite(pFrameYUV->data[0], 1, y_size, fp_yuv);
                fwrite(pFrameYUV->data[1], 1, y_size / 4, fp_yuv);
                fwrite(pFrameYUV->data[2], 1, y_size / 4, fp_yuv);

                frame_count++;
                LOGI("解码第%d帧",frame_count);
            }
        }

        //释放资源
        av_packet_unref(packet);
    }
    fclose(fp_yuv);
    env->ReleaseStringUTFChars(input_, input_cstr);
    env->ReleaseStringUTFChars(output_, output_cstr);
    av_free(out_buffer);
    av_frame_free(&pFrame);
    av_frame_free(&pFrameYUV);
    avcodec_close(pCodecCtx);
    avformat_free_context(pFormatCtx);
}


#ifdef __cplusplus
};
#endif