/*!A dlopen library that bypasses mobile system limitation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Copyright (C) 2020-present, TBOOX Open Source Group.
 *
 * @author      ruki
 * @file        SystemLoader.java
 *
 */

package dyopen.lib;

import android.os.Build;
import android.util.Log;

import java.lang.reflect.Method;

import static android.os.Build.VERSION.SDK_INT;

public class SystemLoader {

    private static final String TAG = "byOpen";

    static public boolean load(String libraryPath) {
        if (libraryPath == null) {
            return false;
        }
        try {
            System.load(libraryPath);
            return true;
        } catch (Throwable e) {
            if (SDK_INT >= Build.VERSION_CODES.P) {
                try {

                    Method forName = Class.class.getDeclaredMethod("forName", String.class);
                    Method getDeclaredMethod = Class.class.getDeclaredMethod("getDeclaredMethod", String.class, Class[].class);
                    Class<?> systemClass = (Class<?>) forName.invoke(null, "java.lang.System");
                    Method load = (Method) getDeclaredMethod.invoke(systemClass, "load", new Class[]{String.class});
                    load.invoke(systemClass, libraryPath);
                    return true;

                } catch (Throwable ex) {
                    Log.e(TAG, "load library failed:", ex);
                }
            } else {
                Log.e(TAG, "load library failed:", e);
            }
        }
        return false;
    }

    static public boolean loadLibrary(String libraryName) {
        if (libraryName == null) {
            return false;
        }
        try {
            System.loadLibrary(libraryName);
            return true;
        } catch (Throwable e) {
            if (SDK_INT >= Build.VERSION_CODES.P) {
                try {

                    Method forName = Class.class.getDeclaredMethod("forName", String.class);
                    Method getDeclaredMethod = Class.class.getDeclaredMethod("getDeclaredMethod", String.class, Class[].class);
                    Class<?> systemClass = (Class<?>) forName.invoke(null, "java.lang.System");
                    Method loadLibrary = (Method) getDeclaredMethod.invoke(systemClass, "loadLibrary", new Class[]{String.class});
                    loadLibrary.invoke(systemClass, libraryName);
                    return true;

                } catch (Throwable ex) {
                    Log.e(TAG, "load library failed:", ex);
                }
            } else {
                Log.e(TAG, "load library failed:", e);
            }
        }
        return false;
    }
}
