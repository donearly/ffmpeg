package com.example.sail.ffmpegdemo;

public class VideoUtils {
    public native static void decode(String input,String output);
    static {
        System.loadLibrary("native-lib");
    }
}
