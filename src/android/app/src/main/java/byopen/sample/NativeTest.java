package byopen.sample;

public class NativeTest {
    static {
        System.loadLibrary("testjni");
    }
    public static native boolean loadLibrary(String libraryName, String symbolName);
    public static native boolean validFromMaps(String libraryName);

}
