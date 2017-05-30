package com.example.sail.ffmpegdemo;

public class VideoUtils {
    public native static void decode(String input,String output);
    static {
        System.loadLibrary("avutil");
        System.loadLibrary("avcodec");
        System.loadLibrary("avfilter");
        System.loadLibrary("swresample");
        System.loadLibrary("swscale");
        System.loadLibrary("avformat");
        System.loadLibrary("avdevice");
        System.loadLibrary("native-lib");
    }
}
