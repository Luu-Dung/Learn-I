	.arch armv8-a
	.file	"dummy.c"
	.text
	.align	2
	.global	add32
	.type	add32, %function
add32:
	sub	sp, sp, #16
	str	w0, [sp, 12]
	str	w1, [sp, 8]
	ldr	w0, [sp, 8]
	str	w0, [sp, 12]
	mov	w0, 0
	add	sp, sp, 16
	ret
	.size	add32, .-add32
	.ident	"GCC: (Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
