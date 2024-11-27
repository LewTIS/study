	.file	"c_07.c"
	.text
	.globl	get_max
	.type	get_max, @function
get_max:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	%edx, -32(%rbp)
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movl	(%rax), %eax
	movl	%eax, -12(%rbp)
	movl	$0, -8(%rbp)
	jmp	.L2
.L6:
	movl	$0, -4(%rbp)
	jmp	.L3
.L5:
	movl	-8(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rdx
	movl	-4(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	cmpl	%eax, -12(%rbp)
	jge	.L4
	movl	-8(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rdx
	movl	-4(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	movl	%eax, -12(%rbp)
.L4:
	addl	$1, -4(%rbp)
.L3:
	movl	-4(%rbp), %eax
	cmpl	-32(%rbp), %eax
	jl	.L5
	addl	$1, -8(%rbp)
.L2:
	movl	-8(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L6
	movl	-12(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	get_max, .-get_max
	.section	.rodata
.LC1:
	.string	"%d\n"
.LC2:
	.string	"%ld\n"
	.align 32
.LC0:
	.long	20
	.string	"lihua"
	.zero	250
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$392, %rsp
	.cfi_offset 3, -24
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movl	$1, -320(%rbp)
	movl	$2, -316(%rbp)
	movl	$3, -312(%rbp)
	movl	$4, -308(%rbp)
	movl	$16, %edi
	call	malloc@PLT
	movq	%rax, -360(%rbp)
	movl	$0, -396(%rbp)
	jmp	.L9
.L10:
	movl	-396(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-360(%rbp), %rax
	leaq	(%rdx,%rax), %rbx
	movl	$16, %edi
	call	malloc@PLT
	movq	%rax, (%rbx)
	addl	$1, -396(%rbp)
.L9:
	cmpl	$1, -396(%rbp)
	jle	.L10
	movl	$2, -392(%rbp)
	movl	$0, -388(%rbp)
	jmp	.L11
.L14:
	movl	$0, -384(%rbp)
	jmp	.L12
.L13:
	movl	-392(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -392(%rbp)
	movl	-388(%rbp), %edx
	movslq	%edx, %rdx
	leaq	0(,%rdx,8), %rcx
	movq	-360(%rbp), %rdx
	addq	%rcx, %rdx
	movq	(%rdx), %rcx
	movl	-384(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rcx, %rdx
	movl	%eax, (%rdx)
	addl	$1, -384(%rbp)
.L12:
	cmpl	$1, -384(%rbp)
	jle	.L13
	addl	$1, -388(%rbp)
.L11:
	cmpl	$1, -388(%rbp)
	jle	.L14
	movq	-360(%rbp), %rax
	movl	$2, %edx
	movl	$2, %esi
	movq	%rax, %rdi
	call	get_max
	movl	%eax, -380(%rbp)
	movl	-380(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$141, -376(%rbp)
	movl	$153, -372(%rbp)
	movl	-372(%rbp), %eax
	orl	%eax, -376(%rbp)
	movl	-376(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$15, -368(%rbp)
	movl	-368(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$16, %esi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$32, %esi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$1, -364(%rbp)
	movl	-364(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$8, %esi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$16, %esi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-304(%rbp), %rax
	leaq	.LC0(%rip), %rdx
	movl	$32, %ecx
	movq	%rax, %rdi
	movq	%rdx, %rsi
	rep movsq
	movq	%rsi, %rdx
	movq	%rdi, %rax
	movl	(%rdx), %ecx
	movl	%ecx, (%rax)
	movl	$10, -304(%rbp)
	movl	-304(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-304(%rbp), %rax
	addq	$4, %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$1684234849, -32(%rbp)
	movl	$6513249, -28(%rbp)
	leaq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	strlen@PLT
	movq	%rax, %rsi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-28(%rbp), %rax
	movq	%rax, %rdi
	call	strlen@PLT
	movq	%rax, %rsi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	movq	-24(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L16
	call	__stack_chk_fail@PLT
.L16:
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
