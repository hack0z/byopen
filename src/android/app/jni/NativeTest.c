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
 * @file        NativeTest.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "byopen.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static by_void_t jni_clearException(JNIEnv* env, by_bool_t report)
{
    jthrowable e = report? (*env)->ExceptionOccurred(env) : by_null;
    (*env)->ExceptionClear(env);
    if (e)
    {
        jclass clazz = (*env)->GetObjectClass(env, e);
        jmethodID printStackTrace_id = (*env)->GetMethodID(env, clazz, "printStackTrace", "()V");
        if (!(*env)->ExceptionCheck(env) && printStackTrace_id)
            (*env)->CallVoidMethod(env, e, printStackTrace_id);
        if ((*env)->ExceptionCheck(env))
            (*env)->ExceptionClear(env);
    }
}
static jobject jni_Class_getDeclaredMethod(JNIEnv* env)
{
    // check
    by_assert_and_check_return_val(env, by_null);

    // push
    if ((*env)->PushLocalFrame(env, 10) < 0) return by_null;

    // get unreachable memory info
    jboolean check = by_false;
    jobject  getDeclaredMethod_method = by_null;
    do
    {
        // get class
        jclass clazz = (*env)->FindClass(env, "java/lang/Class");
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && clazz);

        // get string class
        jclass string_clazz = (*env)->FindClass(env, "java/lang/String");
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && string_clazz);

        // get class/array class
        jclass classarray_clazz = (*env)->FindClass(env, "[Ljava/lang/Class;");
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && classarray_clazz);

        // get getDeclaredMethod id
        jmethodID getDeclaredMethod_id = (*env)->GetMethodID(env, clazz, "getDeclaredMethod", "(Ljava/lang/String;[Ljava/lang/Class;)Ljava/lang/reflect/Method;");
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && getDeclaredMethod_id);

        // get getDeclaredMethod name
        jstring getDeclaredMethod_name = (*env)->NewStringUTF(env, "getDeclaredMethod");
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && getDeclaredMethod_name);

        // get getDeclaredMethod args
        jobjectArray getDeclaredMethod_args = (*env)->NewObjectArray(env, 2, clazz, by_null);
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && getDeclaredMethod_args);

        (*env)->SetObjectArrayElement(env, getDeclaredMethod_args, 0, string_clazz);
        (*env)->SetObjectArrayElement(env, getDeclaredMethod_args, 1, classarray_clazz);

        // Method getDeclaredMethod = Class.class.getDeclaredMethod("getDeclaredMethod", String.class, Class[].class);
        getDeclaredMethod_method = (jobject)(*env)->CallObjectMethod(env, clazz, getDeclaredMethod_id, getDeclaredMethod_name, getDeclaredMethod_args);
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && getDeclaredMethod_method);

    } while (0);

    // exception? clear it
    if (check)
    {
        getDeclaredMethod_method = by_null;
        jni_clearException(env, by_true);
    }
    return (jstring)(*env)->PopLocalFrame(env, getDeclaredMethod_method);
}

/* System.load(libraryPath)
 *
 * @code
    Method forName = Class.class.getDeclaredMethod("forName", String.class);
    Method getDeclaredMethod = Class.class.getDeclaredMethod("getDeclaredMethod", String.class, Class[].class);
    Class<?> systemClass = (Class<?>)forName.invoke(null, "java.lang.System");
    Method load = (Method)getDeclaredMethod.invoke(systemClass, "load", new Class[]{String.class});
    load.invoke(systemClass, libraryPath);
 * @endcode
 *
 * System.loadLibrary(libraryName)
 *
 * @code
    Method forName = Class.class.getDeclaredMethod("forName", String.class);
    Method getDeclaredMethod = Class.class.getDeclaredMethod("getDeclaredMethod", String.class, Class[].class);
    Class<?> systemClass = (Class<?>)forName.invoke(null, "java.lang.System");
    Method loadLibrary = (Method)getDeclaredMethod.invoke(systemClass, "loadLibrary", new Class[]{String.class});
    loadLibrary.invoke(systemClass, libraryName);
 * @endcode
 */
static by_bool_t jni_System_load_or_loadLibrary(JNIEnv* env, by_char_t const* loadName, by_char_t const* libraryPath)
{
    // check
    by_assert_and_check_return_val(env && loadName && libraryPath, by_false);

    // push
    if ((*env)->PushLocalFrame(env, 20) < 0) return by_false;

    // do load 
    jboolean check = by_false;
    do
    {
        // get getDeclaredMethod method
        jobject getDeclaredMethod_method = jni_Class_getDeclaredMethod(env);
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && getDeclaredMethod_method);

        // get class
        jclass clazz = (*env)->FindClass(env, "java/lang/Class");
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && clazz);

        // get object class
        jclass object_clazz = (*env)->FindClass(env, "java/lang/Object");
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && object_clazz);

        // get string class
        jclass string_clazz = (*env)->FindClass(env, "java/lang/String");
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && string_clazz);

        // get system class
        jclass system_clazz = (*env)->FindClass(env, "java/lang/System");
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && system_clazz);

        // get method class
        jclass method_clazz = (*env)->FindClass(env, "java/lang/reflect/Method");
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && method_clazz);

        // get getDeclaredMethod_method.invoke id
        jmethodID invoke_id = (*env)->GetMethodID(env, method_clazz, "invoke", "(Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;");
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && invoke_id);

        // get load name
        jstring load_name = (*env)->NewStringUTF(env, loadName);
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && load_name);

        // get invoke args
        jobjectArray invoke_args = (*env)->NewObjectArray(env, 2, object_clazz, by_null);
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && invoke_args);

        // get load args
        jobjectArray load_args = (*env)->NewObjectArray(env, 1, clazz, string_clazz);
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && load_args);

        (*env)->SetObjectArrayElement(env, invoke_args, 0, load_name);
        (*env)->SetObjectArrayElement(env, invoke_args, 1, load_args);

        // Method load = (Method)getDeclaredMethod.invoke(systemClass, "load", new Class[]{String.class}); 
        jobject load_method = (jobject)(*env)->CallObjectMethod(env, getDeclaredMethod_method, invoke_id, system_clazz, invoke_args);
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && load_method);

        // load.invoke(systemClass, libraryPath)
        jstring libraryPath_jstr = (*env)->NewStringUTF(env, libraryPath);
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && libraryPath_jstr);
        
        invoke_args = (*env)->NewObjectArray(env, 1, object_clazz, libraryPath_jstr);
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)) && invoke_args);

        (*env)->CallObjectMethod(env, load_method, invoke_id, system_clazz, invoke_args);
        by_assert_and_check_break(!(check = (*env)->ExceptionCheck(env)));

    } while (0);

    // exception? clear it
    if (check) jni_clearException(env, by_true);
    (*env)->PopLocalFrame(env, by_null);
    return !check;
}

// System.load(libraryPath)
static by_bool_t jni_System_load(JNIEnv* env, by_char_t const* libraryPath)
{
    return jni_System_load_or_loadLibrary(env, "load", libraryPath);
}

// System.loadLibrary(libraryName)
static by_bool_t jni_System_loadLibrary(JNIEnv* env, by_char_t const* libraryName)
{
    return jni_System_load_or_loadLibrary(env, "loadLibrary", libraryName);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
JNIEXPORT jboolean Java_byopen_sample_NativeTest_test(JNIEnv* env, jclass jthis)
{
    by_trace("jni_System_load %d", jni_System_load(env, "/system/lib64/libchrome.so"));
    by_trace("jni_System_loadLibrary %d", jni_System_loadLibrary(env, "chrome"));
    return by_true;
}
