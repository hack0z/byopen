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
 * @file        byopen_android.c
 *
 */

/* ///////////////////////////////////////////////////////////////////////
 * includes
 */
#include "byopen.h"
#include <dlfcn.h>

/* ///////////////////////////////////////////////////////////////////////
 * private implementation
 */
static by_pointer_t by_dlopen_fake(by_char_t const* filename, by_int_t flag)
{
    return by_null;
}

/* ///////////////////////////////////////////////////////////////////////
 * implementation
 */
by_pointer_t by_dlopen(by_char_t const* filename, by_int_t flag)
{
    // check
    by_assert_and_check_return_val(filename, by_null);

    // attempt to use original dlopen to load it fist
    by_pointer_t handle = dlopen(filename, flag == BY_RTLD_LAZY? RTLD_LAZY : RTLD_NOW);

    // uses the fake dlopen to load it from maps directly
    if (!handle) handle = by_dlopen_fake(filename, flag);

    // uses the fake dlopen to load it from maps directly
    if (!handle) 
    {
        // TODO load it via system call
        handle = by_dlopen_fake(filename, flag);
    }
    return handle;
}
by_pointer_t by_dlsym(by_pointer_t handle, by_char_t const* symbol)
{
    return by_null;
}
by_int_t by_dlclose(by_pointer_t handle)
{
    return 0;
}
