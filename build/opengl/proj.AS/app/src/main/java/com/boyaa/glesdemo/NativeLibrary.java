package com.boyaa.glesdemo;

public class NativeLibrary {
    static
    {
        System.loadLibrary("Native");
    }
    public static native void init(int width, int height);
    public static native void step();
    public static native void uninit();
    public static native void rumCmd(String cmd);
}
