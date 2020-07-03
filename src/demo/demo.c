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
 * @file        demo.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "byopen.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
by_int_t main(by_int_t argc, by_char_t** argv)
{
    by_pointer_t handle = by_dlopen(argv[1], BY_RTLD_LAZY);
    if (handle)
    {
        by_pointer_t addr = by_dlsym(handle, argv[2]);
        if (addr)
        {
            by_trace("dlopen(%s): %s -> %p", argv[1], argv[2], addr);
        }
        by_dlclose(handle);
    }
    return 0;
}
