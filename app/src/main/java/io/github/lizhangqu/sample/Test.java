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

    public static native void native_test();
}
