/* crt0.S -- startup file for bexkat1
 * 
 * Copyright (c) 2008, 2009, 2014  Anthony Green
 * Copyright (c) 2015  Matt Stock
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions. No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 */

	.globl	__start
	.weak	_start
	.text
	.type	__start,@function
__start:
_start:
	ldi	%sp, _stack	/* set the top of stack */
	ldiu	%fp, 0	/* zero fp to allow unwinders to stop */

	/* zero the bss area */
	ldi	%0, __bss_start
	ldiu	%1, 0
	ldi	%2, _end
	sub	%2, %2, %0
        push    %2
        push    %1
        push    %0
	jsrd	memset

	/* Call _init to invoke static constructors, etc.  */
	jsrd	_init
	
	/* Call _fini at exit time for static destructors.  */
	ldi	%0, _fini
	jsrd	atexit
	
	/* Set argc and argv.  These are populated by the simulator.  */
	ldiu	%0, 0
	push	%0
	push	%0
	
	jsrd	main
	jsrd	exit

.Lend:
	.size	__start,(.Lend-__start)
