	.text
	.file	"encode.c"
	.globl	crypto_kem_sntrup653_avx_crypto_encode_int16 # -- Begin function crypto_kem_sntrup653_avx_crypto_encode_int16
	.type	crypto_kem_sntrup653_avx_crypto_encode_int16,@function
crypto_kem_sntrup653_avx_crypto_encode_int16: # @crypto_kem_sntrup653_avx_crypto_encode_int16
	.cfi_startproc
# %bb.0:
	movzwl	(%rsi), %eax
	movb	%al, (%rdi)
	movb	%ah, 1(%rdi)
	retq
.Lfunc_end0:
	.size	crypto_kem_sntrup653_avx_crypto_encode_int16, .Lfunc_end0-crypto_kem_sntrup653_avx_crypto_encode_int16
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 8.0.0 (Fedora 8.0.0-3.fc30)"
	.section	".note.GNU-stack","",@progbits
