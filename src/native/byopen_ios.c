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
 * @file        byopen_ios.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
by_pointer_t by_dlopen(by_char_t const* filename, by_int_t flag)
{
    return by_null;
}
by_pointer_t by_dlsym(by_pointer_t handle, by_char_t const* symbol)
{
    return by_null;
}
by_int_t by_dlclose(by_pointer_t handle)
{
    return 0;
}
