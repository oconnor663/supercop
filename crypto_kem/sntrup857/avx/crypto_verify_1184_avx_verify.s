	.file	"verify.c"
	.text
	.globl	crypto_kem_sntrup857_avx_crypto_verify_1184
	.type	crypto_kem_sntrup857_avx_crypto_verify_1184, @function
crypto_kem_sntrup857_avx_crypto_verify_1184:
.LFB5274:
	.cfi_startproc
	movl	$1152, %eax
	vpxor	%xmm0, %xmm0, %xmm0
.L2:
	vmovdqu	(%rdi), %ymm2
	vpxor	(%rsi), %ymm2, %ymm1
	addq	$32, %rdi
	addq	$32, %rsi
	vpor	%ymm1, %ymm0, %ymm0
	subl	$32, %eax
	jns	.L2
	cmpl	$-31, %eax
	jl	.L4
	movslq	%eax, %rdx
	addq	%rdx, %rdi
	addq	%rdx, %rsi
	jmp	.L2
.L4:
	vpsrlw	$8, %ymm0, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vpsrld	$16, %ymm0, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vpsrlq	$32, %ymm0, %ymm1
	vpor	%ymm1, %ymm0, %ymm1
	vextracti128	$0x1, %ymm1, %xmm0
	vpextrb	$0, %xmm1, %esi
	vpextrb	$8, %xmm1, %eax
	vpextrb	$0, %xmm0, %ecx
	orl	%esi, %eax
	vpextrb	$8, %xmm0, %edx
	orl	%ecx, %eax
	orl	%edx, %eax
	movzbl	%al, %eax
	decl	%eax
	shrl	$31, %eax
	decl	%eax
	ret
	.cfi_endproc
.LFE5274:
	.size	crypto_kem_sntrup857_avx_crypto_verify_1184, .-crypto_kem_sntrup857_avx_crypto_verify_1184
	.ident	"GCC: (GNU) 9.2.1 20190827 (Red Hat 9.2.1-1)"
	.section	.note.GNU-stack,"",@progbits
