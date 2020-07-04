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
 * @file        byopen_macho.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "byopen.h"
#include <mach/mach.h>
#include <mach/machine.h>
#include <mach-o/dyld.h>
#include <mach-o/nlist.h>
#include <objc/runtime.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */
#if defined(__LP64__)
#   define NLIST                struct nlist_64
#else
#   define NLIST                struct nlist
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */
// the dynamic library context type for fake dlopen
typedef struct _by_fake_dlctx_t 
{    
    // the image index
    by_size_t                   image_index;

    // the image image_header
    struct mach_header const*   image_header;

}by_fake_dlctx_t, *by_fake_dlctx_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */

// get first cmd after image_header
static by_pointer_t by_get_first_cmd_after_header(struct mach_header const* image_header)
{
    switch (image_header->magic)
    {
        case MH_MAGIC:
        case MH_CIGAM:
            return (by_pointer_t)(image_header + 1);
        case MH_MAGIC_64:
        case MH_CIGAM_64:
            return (by_pointer_t)(((struct mach_header_64*)image_header) + 1);
        default:
            // image_header is corrupt
            return 0;
    }
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
by_pointer_t by_dlopen(by_char_t const* filename, by_int_t flag)
{
    // check
    by_assert_and_check_return_val(filename, by_null);

    /* skip the first image (master app), becasue it's symtab will be stripped after be archived.
     * so we need to keep our behavior consistent for debug/release and archived packages.
     */
    struct mach_header const* image_header = by_null;
    by_size_t const           image_count = _dyld_image_count();
    for (by_size_t image_index = 1; image_index < image_count; image_index++)
    {
        image_header = _dyld_get_image_header((uint32_t)image_index);
        if (image_header)
        {
            by_char_t const* image_name = _dyld_get_image_name(image_index);
            if (image_name && 0 == strcmp(image_name, filename))
            {
                by_fake_dlctx_ref_t dlctx = calloc(1, sizeof(by_fake_dlctx_t));
                if (dlctx)
                {
                    dlctx->image_index  = image_index;
                    dlctx->image_header = image_header;
                }
                by_trace("%s: found at %p/%d", image_name, image_header, (by_int_t)image_index);
                return (by_pointer_t)dlctx;
            }
        }
    }
    return by_null;
}
by_pointer_t by_dlsym(by_pointer_t handle, by_char_t const* symbol)
{
    // check
    by_fake_dlctx_ref_t dlctx = (by_fake_dlctx_ref_t)handle;
    by_assert_and_check_return_val(dlctx && dlctx->image_header && symbol, by_null);

    // skip '_'
    if (*symbol == '_') symbol++;

    // get command pointer
    struct mach_header const* image_header  = dlctx->image_header;
    uintptr_t cmd_ptr                       = by_get_first_cmd_after_header(image_header);
    uintptr_t image_vmaddr_slide            = (uintptr_t)_dyld_get_image_vmaddr_slide(dlctx->image_index);
    if (cmd_ptr)
    {
        uintptr_t segment_base = 0;
        for (by_size_t cmd_index = 0; cmd_index < image_header->ncmds; cmd_index++)
        {
            // get segment base address
            struct load_command const* load_cmd = (struct load_command*)cmd_ptr;
            if (load_cmd->cmd == LC_SEGMENT)
            {
                struct segment_command const* segment_cmd = (struct segment_command*)cmd_ptr;
                if (strcmp(segment_cmd->segname, SEG_LINKEDIT) == 0)
                    segment_base = (uintptr_t)(segment_cmd->vmaddr - segment_cmd->fileoff) + image_vmaddr_slide;
            }
            else if (load_cmd->cmd == LC_SEGMENT_64)
            {
                struct segment_command_64 const* segment_cmd = (struct segment_command_64*)cmd_ptr;
                if (strcmp(segment_cmd->segname, SEG_LINKEDIT) == 0)
                    segment_base = (uintptr_t)(segment_cmd->vmaddr - segment_cmd->fileoff) + image_vmaddr_slide;
            }
            else if (load_cmd->cmd == LC_SYMTAB && segment_base > 0)
            {
                struct symtab_command const* symbol_table_cmd = (struct symtab_command*)cmd_ptr;
                NLIST const*                 symbol_table = (NLIST*)(segment_base + symbol_table_cmd->symoff);
                uintptr_t                    string_table = segment_base + symbol_table_cmd->stroff;
                for (uint32_t symbol_index = 0; symbol_index < symbol_table_cmd->nsyms; symbol_index++)
                {
                    // if n_value is 0, the symbol refers to an external object.
                    if (symbol_table[symbol_index].n_value != 0)
                    {
                        // get symbol name
                        NLIST const* item          = symbol_table + symbol_index;
                        by_pointer_t dli_saddr     = (by_pointer_t)(item->n_value + image_vmaddr_slide);
                        by_char_t const* dli_sname = (by_char_t*)((intptr_t)string_table + (intptr_t)item->n_un.n_strx);
                        if (*dli_sname == '_') dli_sname++;

                        // found and skip symbols with '0x...'?
                        if (*dli_sname != '0' && !strcmp(symbol, dli_sname))
                        {
                            // thumb function? fix address
#if defined(BY_ARCH_ARM) && !defined(BY_ARCH_ARM64)
#   ifdef BY_ARCH_ARM_THUMB
                            if (item->n_desc & N_ARM_THUMB_DEF) 
                                dli_saddr = (by_pointer_t)((by_ulong_t)dli_saddr | 1);
#   else
                            if (item->n_desc & N_ARM_THUMB_DEF) 
                                dli_saddr = (by_pointer_t)((by_ulong_t)dli_saddr & ~1);
#   endif
#endif
                            // trace
                            by_trace("dlsym(%s): %p", dli_sname, dli_saddr);
                            return dli_saddr;
                        }
                    }
                }

                // reset the segment base address
                segment_base = 0;
            }
            cmd_ptr += load_cmd->cmdsize;
        }
    }
    return by_null;
}
by_int_t by_dlclose(by_pointer_t handle)
{
    // check
    by_fake_dlctx_ref_t dlctx = (by_fake_dlctx_ref_t)handle;
    by_assert_and_check_return_val(dlctx, -1);

    // free it
    free(dlctx);
    return 0;
}
