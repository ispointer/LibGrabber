package antik.libgrabber;

/*
 * Created by aantik
 * 3/27/2026 3:21 PM
 *
 *   ⋆    ႔ ႔
 *     ᠸ^ ^ ⸝⸝
 *       |、˜〵
 *      じしˍ,)⁐̤ᐷ
 *
 * Fox Mode 🍺
 */
public class lib {

    static {
        System.loadLibrary("grabber");
    }
    public static native String dump(String inPath, String outPath);
}
