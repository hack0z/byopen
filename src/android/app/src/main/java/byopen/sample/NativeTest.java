package byopen.sample;

public class NativeTest {
    static {
        System.loadLibrary("testjni");
    }
    public static native boolean test();
    public static native boolean validFromMaps(String libraryName);

}
