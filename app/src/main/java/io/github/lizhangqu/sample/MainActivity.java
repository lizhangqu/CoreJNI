package io.github.lizhangqu.sample;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    Test.native_test();
                } catch (IllegalArgumentException e) {
                    e.printStackTrace();
                }
                String s = Test.native_string();
                Log.e("TAG", "native_string:" + s);

                ByteBuffer byteBuffer = ByteBuffer.allocateDirect(4);
                byteBuffer.put("haha".getBytes());
                Test.native_byte("bytes".getBytes(), byteBuffer);
                List<String> strings = Test.native_list();
                Log.e("TAG", "strings:" + strings);
                Map<String, String> map = new HashMap<>();
                map.put("key1", "value1");
                map.put("key2", "value2");
                map.put("key3", "value3");
                map.put("key4", "value4");
                Test.native_map(map);
            }
        });

    }
}
