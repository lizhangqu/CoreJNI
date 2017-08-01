package io.github.lizhangqu.sample;

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

    public static void callByNative() {
        int a = 20 / 0;
    }
}
