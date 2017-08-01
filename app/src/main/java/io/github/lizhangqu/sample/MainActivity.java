package io.github.lizhangqu.sample;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

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

            }
        });

    }
}
