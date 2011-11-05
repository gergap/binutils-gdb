/* tc-rl78.h - header file for Renesas RL78
   Copyright 2011
   Free Software Foundation, Inc.

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to the Free
   Software Foundation, 51 Franklin Street - Fifth Floor, Boston, MA
   02110-1301, USA.  */

#define TC_RL78

extern int target_little_endian;

#define LISTING_HEADER "RL78 GAS LE"
#define LISTING_LHS_WIDTH 8
#define LISTING_WORD_SIZE 1

#define TARGET_ARCH bfd_arch_rl78

#define TARGET_BYTES_BIG_ENDIAN 0

#define TARGET_FORMAT "elf32-rl78"

/* We don't need to handle .word strangely.  */
#define WORKING_DOT_WORD

/* Permit temporary numeric labels.  */
#define LOCAL_LABELS_FB 1
/* But make sure that the binutils treat them as locals.  */
#define LOCAL_LABEL_PREFIX '.'

/* Allow classic-style constants.  */
#define NUMBERS_WITH_SUFFIX 1

/* .-foo gets turned into PC relative relocs.  */
#define DIFF_EXPR_OK

#define md_end rl78_md_end
extern void rl78_md_end (void);

#define TC_FRAG_TYPE struct rl78_bytesT *
#define TC_FRAG_INIT rl78_frag_init
extern void rl78_frag_init (fragS *);

/* Call md_pcrel_from_section(), not md_pcrel_from().  */
#define MD_PCREL_FROM_SECTION(FIXP, SEC) md_pcrel_from_section (FIXP, SEC)
extern long md_pcrel_from_section (struct fix *, segT);

/* RL78 doesn't have a 32 bit PCREL relocations.  */
#define TC_FORCE_RELOCATION_SUB_LOCAL(FIX, SEG) 1

#define TC_VALIDATE_FIX_SUB(FIX, SEG)		\
  rl78_validate_fix_sub (FIX)
extern int rl78_validate_fix_sub (struct fix *);

#define TC_CONS_FIX_NEW(FRAG, WHERE, NBYTES, EXP) \
  rl78_cons_fix_new (FRAG, WHERE, NBYTES, EXP)
extern void rl78_cons_fix_new (fragS *, int, int, expressionS *);

#define tc_fix_adjustable(x) 0

#define RELOC_EXPANSION_POSSIBLE 1
#define MAX_RELOC_EXPANSION      8