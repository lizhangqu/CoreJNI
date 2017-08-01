package io.github.lizhangqu.sample;

import java.util.List;
import java.util.Map;

/**
 * 功能介绍
 *
 * @author lizhangqu
 * @version V1.0
 * @since 2017-07-31 19:33
 */
public class Test {

    static {
        System.loadLibrary("corejni");
    }

    public static native void native_test() throws IllegalArgumentException;

    public static native String native_string();

    public static native void native_byte(byte[] bytes, java.nio.ByteBuffer byteBuffer);

    public static native List<String> native_list();

    public static native Map<String, String> native_map(Map<String, String> map);

    public static void callByNative() {
        int a = 20 / 0;
    }
}
