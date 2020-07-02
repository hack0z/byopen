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
 * @file        prefix.h
 *
 */
#ifndef BY_PREFIX_H
#define BY_PREFIX_H

#ifdef __cplusplus
extern "C" {
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#ifdef __ANDROID__
#   include <jni.h>
#   include <android/log.h>
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// bool values
#ifdef __ANDROID__
#   define by_true                              ((by_bool_t)JNI_TRUE)
#   define by_false                             ((by_bool_t)JNI_FALSE)
#else
#   define by_true                              ((by_bool_t)1)
#   define by_false                             ((by_bool_t)0)
#endif

// null
#ifdef __cplusplus
#   define by_null                              (0)
#else
#   define by_null                              ((by_pointer_t)0)
#endif

// trace
#ifdef BY_DEBUG
#   define by_tracef(fmt, arg ...)              by_printf(fmt, ## arg)
#   define by_trace(fmt, arg ...)               by_printf(fmt "\n", ## arg)
#   define by_trace_line(fmt, arg ...)          by_printf(fmt " at func: %s, line: %d, file: %s\n", ##arg, __FUNCTION__, __LINE__, __FILE__)
#else
#   define by_tracef(...)
#   define by_trace(...)
#   define by_trace_line(...)
#endif

// check
#define by_check_return(x)                      do { if (!(x)) return ; } while (0)
#define by_check_return_val(x, v)               do { if (!(x)) return (v); } while (0)
#define by_check_goto(x, b)                     do { if (!(x)) goto b; } while (0)
#define by_check_break(x)                       { if (!(x)) break ; }
#define by_check_continue(x)                    { if (!(x)) continue ; }

// assert
#ifdef BY_DEBUG
#   define by_assert(x)                         do { if (!(x)) {by_trace_line("[assert]: expr: %s", #x); } } while(0)
#   define by_assert_return(x)                  do { if (!(x)) {by_trace_line("[assert]: expr: %s", #x); return ; } } while(0)
#   define by_assert_return_val(x, v)           do { if (!(x)) {by_trace_line("[assert]: expr: %s", #x); return (v); } } while(0)
#   define by_assert_goto(x, b)                 do { if (!(x)) {by_trace_line("[assert]: expr: %s", #x); goto b; } } while(0)
#   define by_assert_break(x)                   { if (!(x)) {by_trace_line("[assert]: expr: %s", #x); break ; } }
#   define by_assert_continue(x)                { if (!(x)) {by_trace_line("[assert]: expr: %s", #x); continue ; } }
#   define by_assert_and_check_return(x)        by_assert_return(x)
#   define by_assert_and_check_return_val(x, v) by_assert_return_val(x, v)
#   define by_assert_and_check_goto(x, b)       by_assert_goto(x, b)
#   define by_assert_and_check_break(x)         by_assert_break(x)
#   define by_assert_and_check_continue(x)      by_assert_continue(x)
#else
#   define by_assert(x)
#   define by_assert_return(x)
#   define by_assert_return_val(x, v)
#   define by_assert_goto(x, b)
#   define by_assert_break(x)
#   define by_assert_continue(x)
#   define by_assert_and_check_return(x)        by_check_return(x)
#   define by_assert_and_check_return_val(x, v) by_check_return_val(x, v)
#   define by_assert_and_check_goto(x, b)       by_check_goto(x, b)
#   define by_assert_and_check_break(x)         by_check_break(x)
#   define by_assert_and_check_continue(x)      by_check_continue(x)
#endif

// printf 
#ifdef __ANDROID__
#   define by_printf(fmt, arg ...)  __android_log_print(ANDROID_LOG_INFO, "byOpen", fmt, ## arg)
#else
#   define by_printf(fmt, arg ...)  printf(fmt, ## arg)
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// basic
typedef signed int                  by_int_t;
typedef unsigned int                by_uint_t;
typedef signed long                 by_long_t;
typedef unsigned long               by_ulong_t;
typedef by_ulong_t                  by_size_t;
typedef by_int_t                    by_bool_t;
typedef signed char                 by_int8_t;
typedef by_int8_t                   by_sint8_t;
typedef unsigned char               by_uint8_t;
typedef signed short                by_int16_t;
typedef by_int16_t                  by_sint16_t;
typedef unsigned short              by_uint16_t;
typedef by_int_t                    by_int32_t;
typedef by_int32_t                  by_sint32_t;
typedef by_uint_t                   by_uint32_t;
typedef char                        by_char_t;
typedef by_int32_t                  by_wchar_t;
typedef by_int32_t                  by_uchar_t;
typedef by_uint8_t                  by_byte_t;
typedef void                        by_void_t;
typedef by_void_t*                  by_pointer_t;
typedef by_void_t const*            by_cpointer_t;
typedef by_pointer_t                by_handle_t;
typedef signed long long            by_int64_t;
typedef unsigned long long          by_uint64_t;
typedef by_int64_t                  by_sint64_t;
typedef by_sint64_t                 by_hong_t;
typedef by_uint64_t                 by_hize_t;
typedef float                       by_float_t;
typedef double                      by_double_t;

#ifdef __cplusplus
}
#endif
#endif
