/* A setjmp.c for Bexkat1
   Copyright (C) 2009  Anthony Green
   Copyright (C) 2015  Matt Stock

   The authors hereby grant permission to use, copy, modify, distribute,
   and license this software and its documentation for any purpose, provided
   that existing copyright notices are retained in all copies and that this
   notice is included verbatim in any distributions. No written agreement,
   license, or royalty fee is required for any of the authorized uses.
   Modifications to this software may be copyrighted by their authors
   and need not follow the licensing terms described here, provided that
   the new terms are clearly indicated on the first page of each file where
   they apply.  */

	.text
	.global	setjmp
	.type	setjmp,@function
setjmp:
	ld.l    %12, 8(%sp)
	st.l	%0, (%12)
	st.l	%1, 0x04(%12)
	st.l	%2, 0x08(%12)
	st.l	%3, 0x0c(%12)
	st.l	%4, 0x10(%12)
	st.l	%5, 0x14(%12)
	st.l	%6, 0x18(%12)
	st.l	%7, 0x1c(%12)
	st.l	%8, 0x20(%12)
	st.l	%9, 0x24(%12)
	st.l	%10, 0x28(%12)
	st.l	%11, 0x2c(%12)
	st.l	%13, 0x30(%12)
	st.l	%fp, 0x34(%12)
	st.l	%sp, 0x38(%12)
	ldiu	%12, 0
	rts
.Lend1:
	.size	setjmp,.Lend1-setjmp

	.global	longjmp
	.type	longjmp,@function
longjmp:
	ld.l    %12, 12(%sp)
	ldiu	%1, 0x00
	cmp	%1, %12
	beq	.Lreturn1
	ld.l    %12, 8(%sp)
	ld.l	%0, (%12)
	ld.l	%1, 0x04(%12)
	ld.l	%2, 0x08(%12)
	ld.l	%3, 0x0c(%12)
	ld.l	%4, 0x10(%12)
	ld.l	%5, 0x14(%12)
	ld.l	%6, 0x18(%12)
	ld.l	%7, 0x1c(%12)
	ld.l	%8, 0x20(%12)
	ld.l	%9, 0x24(%12)
	ld.l	%10, 0x28(%12)
	ld.l	%11, 0x2c(%12)
	ld.l	%13, 0x30(%12)
	ld.l	%fp, 0x34(%12)
	ld.l	%sp, 0x38(%12)
	rts
.Lreturn1:
	ld.l    %12, 8(%sp)
	ld.l	%0, (%12)
	ld.l	%1, 0x04(%12)
	ld.l	%2, 0x08(%12)
	ld.l	%3, 0x0c(%12)
	ld.l	%4, 0x10(%12)
	ld.l	%5, 0x14(%12)
	ld.l	%6, 0x18(%12)
	ld.l	%7, 0x1c(%12)
	ld.l	%8, 0x20(%12)
	ld.l	%9, 0x24(%12)
	ld.l	%10, 0x28(%12)
	ld.l	%11, 0x2c(%12)
	ldiu	%12, 0
	ld.l	%13, 0x30(%12)
	ld.l	%fp, 0x34(%12)
	ld.l	%sp, 0x38(%0)
	rts
.Lend2:
	.size	longjmp,.Lend2-longjmp
