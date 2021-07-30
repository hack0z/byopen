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
 * declaration
 */
by_void_t by_jni_javavm_set(JavaVM* jvm, by_int_t jversion);

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* jvm, by_pointer_t reserved)
{
    by_jni_javavm_set(jvm, JNI_VERSION_1_4);
    return JNI_VERSION_1_4;
}

JNIEXPORT jboolean Java_byopen_sample_NativeTest_loadLibrary(JNIEnv* env, jclass jthis, jstring libraryName, jstring symbolName)
{
    by_bool_t ok = by_false;
    by_char_t const* libraryName_cstr = (*env)->GetStringUTFChars(env, libraryName, by_null);
    by_char_t const* symbolName_cstr = (*env)->GetStringUTFChars(env, symbolName, by_null);
    if (libraryName_cstr && symbolName_cstr)
    {
        by_pointer_t handle = by_dlopen(libraryName_cstr, BY_RTLD_LAZY);
        if (handle)
        {
            by_pointer_t addr = by_dlsym(handle, symbolName_cstr);
            if (addr)
            {
#if 0
                // test libcurl/curl_version()
                typedef char* (*func_t)();
                func_t func = (func_t)addr;
                by_trace("curl_version: %s", func());
#endif

                // load ok
                ok = by_true;
            }
            by_dlclose(handle);
        }
        (*env)->ReleaseStringUTFChars(env, libraryName, libraryName_cstr);
        (*env)->ReleaseStringUTFChars(env, symbolName, symbolName_cstr);
    }
    return ok;
}
JNIEXPORT jboolean Java_byopen_sample_NativeTest_validFromMaps(JNIEnv* env, jclass jthis, jstring libraryName)
{
    by_bool_t found = by_false;
    by_char_t const* libraryName_cstr = (*env)->GetStringUTFChars(env, libraryName, by_null);
    if (libraryName_cstr)
    {
        FILE* f = fopen("/proc/self/maps", "r");
        if (f)
        {
            char line[512];
            while (fgets(line, sizeof(line), f))
            {
                if (strstr(line, libraryName_cstr))
                {
                    found = by_true;
                    break;
                }
            }
            fclose(f);
        }
        (*env)->ReleaseStringUTFChars(env, libraryName, libraryName_cstr);
    }
    return found;
}
