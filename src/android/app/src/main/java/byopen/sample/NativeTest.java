package byopen.sample;

public class NativeTest {
    static {
        System.loadLibrary("testjni");
    }
    public static native boolean test();
}
