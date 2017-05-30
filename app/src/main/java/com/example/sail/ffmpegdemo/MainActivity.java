package com.example.sail.ffmpegdemo;

import android.annotation.SuppressLint;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button btn = (Button) findViewById(R.id.sample_text);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                System.out.print("clicked \n");
             //   mDecode();
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        mDecode();
                    }
                }).start();
            }
        });
    }

    @SuppressLint("SdCardPath")
    public void mDecode(){
      //  @SuppressLint("SdCardPath") String input = new File("/sdcard/input.mp4").getAbsolutePath();
     //   String output = new File(Environment.getExternalStorageDirectory(),"/sdcard/output_1280x720_yuv420p.yuv").getAbsolutePath();
        VideoUtils.decode("/sdcard/input.mp4", "/sdcard/output_1280x720_yuv420p.yuv");
    }
}
