
// SPDX-License-Identifier: GPL-2.0
/*
 * MiniMiku Linux
 * include/minimiku/extype.h
 *
 * MiniMiku Expanded Types
 */

#ifndef _MINIMIKU_EXTYPE_H
#define _MINIMIKU_EXTYPE_H

#include <linux/types.h>

/* ======= 8-bit types ======= */

typedef s8      mm_s8;
typedef u8      mm_u8;

/* ======= 16-bit types ======= */

typedef s16     mm_s16;
typedef u16     mm_u16;

/* ======= 32-bit types ======= */

typedef s32     mm_s32;
typedef u32     mm_u32;

/* ======= 64-bit types ======= */

typedef s64     mm_s64;
typedef u64     mm_u64;

/* ======= Size and address types ======= */

typedef size_t  mm_size;
typedef ssize_t mm_ssize;
typedef phys_addr_t mm_phys_addr;
typedef dma_addr_t  mm_dma_addr;

/* ======= Boolean type ======= */

typedef bool    mm_bool;

#define MM_TRUE  true
#define MM_FALSE false

/* ======= Character types ======= */

typedef char          mm_char;
typedef unsigned char mm_uchar;

/* ======= Common numeric aliases ======= */

typedef mm_u8  mm_byte;
typedef mm_u16 mm_word;
typedef mm_u32 mm_dword;
typedef mm_u64 mm_qword;

#endif /* _MINIMIKU_EXTYPE_H */
