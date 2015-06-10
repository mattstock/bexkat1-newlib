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
	st.l	%0,     (%0)
	st.l	%1, 0x04(%0)
	st.l	%2, 0x08(%0)
	st.l	%3, 0x0c(%0)
	st.l	%4, 0x10(%0)
	st.l	%5, 0x14(%0)
	st.l	%6, 0x18(%0)
	st.l	%7, 0x1c(%0)
	st.l	%8, 0x20(%0)
	st.l	%9, 0x24(%0)
	st.l	%10, 0x28(%0)
	st.l	%11, 0x2c(%0)
	st.l	%12, 0x30(%0)
	st.l	%13, 0x34(%0)
	st.l	%14, 0x38(%0)
	st.l	%15, 0x3c(%0)
	st.l	%16, 0x40(%0)
	st.l	%17, 0x44(%0)
	st.l	%18, 0x48(%0)
	st.l	%19, 0x4c(%0)
	st.l	%20, 0x50(%0)
	st.l	%fp, 0x54(%0)
	st.l	%sp, 0x58(%0)
	ldiu	%0, 0
	rts
.Lend1:
	.size	setjmp,.Lend1-setjmp

	.global	longjmp
	.type	longjmp,@function
longjmp:
	ldiu	%2, 0x00
	cmp	%1, %2
	beq	.Lreturn1
	ld.l	%1, 0x04(%0)
	ld.l	%2, 0x08(%0)
	ld.l	%3, 0x0c(%0)
	ld.l	%4, 0x10(%0)
	ld.l	%5, 0x14(%0)
	ld.l	%6, 0x18(%0)
	ld.l	%7, 0x1c(%0)
	ld.l	%8, 0x20(%0)
	ld.l	%9, 0x24(%0)
	ld.l	%10, 0x28(%0)
	ld.l	%11, 0x2c(%0)
	ld.l	%12, 0x30(%0)
	ld.l	%13, 0x34(%0)
	ld.l	%sp, 0x38(%0)
	ld.l	%fp, 0x3c(%0)
	mov	%0, %1
	rts
.Lreturn1:
	ld.l	%1, 0x04(%0)
	ld.l	%2, 0x08(%0)
	ld.l	%3, 0x0c(%0)
	ld.l	%4, 0x10(%0)
	ld.l	%5, 0x14(%0)
	ld.l	%6, 0x18(%0)
	ld.l	%7, 0x1c(%0)
	ld.l	%8, 0x20(%0)
	ld.l	%9, 0x24(%0)
	ld.l	%10, 0x28(%0)
	ld.l	%11, 0x2c(%0)
	ld.l	%12, 0x30(%0)
	ld.l	%13, 0x34(%0)
	ld.l	%sp, 0x38(%0)
	ld.l	%fp, 0x3c(%0)
	ldiu	%0, 0x01
	rts
.Lend2:
	.size	longjmp,.Lend2-longjmp
