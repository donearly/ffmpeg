package com.example.sail.ffmpegdemo;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Environment;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import java.io.File;

public class MainActivity extends AppCompatActivity {
    private static final int REQUESTCODE = 101 ;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button btn = (Button) findViewById(R.id.sample_text);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        test();
                    }
                }).start();
            }
        });
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == REQUESTCODE){
            if (permissions[0].equals(Manifest.permission.WRITE_EXTERNAL_STORAGE ) && grantResults[0] == PackageManager.PERMISSION_GRANTED){
                mDecode(Environment.getExternalStorageDirectory().getAbsolutePath()+File.separator+"input.mp4",
                        Environment.getExternalStorageDirectory().getAbsolutePath()+File.separator+"output.yuv");
            }
        }
    }

    private void test() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M){
            int check = checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE );
            if (check == PackageManager.PERMISSION_DENIED){
                requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE },REQUESTCODE);
            }else{
                mDecode(Environment.getExternalStorageDirectory().getAbsolutePath()+File.separator+"input.mp4",
                        Environment.getExternalStorageDirectory().getAbsolutePath()+File.separator+"output.yuv");
            }
        }else{
            mDecode(Environment.getExternalStorageDirectory().getAbsolutePath()+File.separator+"input.mp4",
                    Environment.getExternalStorageDirectory().getAbsolutePath()+File.separator+"output.yuv");
        }
    }
    public void mDecode(String input,String output){
        VideoUtils.decode(input, output);
    }
}
