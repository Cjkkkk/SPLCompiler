	.file	"test.c"
	.def	__main;	.scl	2;	.type	32;	.endef
	.text
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$64, %rsp
	.seh_stackalloc	64
	.seh_endprologue
	call	__main
	movl	$1, -4(%rbp)
	movl	$2, -8(%rbp)
	movl	$3, -12(%rbp)
	movl	-12(%rbp), %eax
	movl	%eax, -16(%rbp)
	movl	$5, -20(%rbp)
	movl	-4(%rbp), %eax
	imull	-8(%rbp), %eax
	movl	%eax, %edx
	movl	-12(%rbp), %eax
	imull	-16(%rbp), %eax
	addl	%eax, %edx
	movl	-20(%rbp), %eax
	imull	-12(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, -24(%rbp)
	movl	$10, -28(%rbp)
	movl	$0, %eax
	addq	$64, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (Rev1, Built by MSYS2 project) 7.2.0"
