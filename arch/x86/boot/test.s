	.file	"test.c"
	.section	.text.unlikely,"x"
.LCOLDB0:
	.text
.LHOTB0:
	.p2align 4,,15
	.globl	getChar
	.def	getChar;	.scl	2;	.type	32;	.endef
	.seh_proc	getChar
getChar:
	.seh_endprologue
	ret
	.seh_endproc
	.section	.text.unlikely,"x"
.LCOLDE0:
	.text
.LHOTE0:
	.section	.text.unlikely,"x"
.LCOLDB1:
	.text
.LHOTB1:
	.p2align 4,,15
	.globl	putChar
	.def	putChar;	.scl	2;	.type	32;	.endef
	.seh_proc	putChar
putChar:
	.seh_endprologue
	ret
	.seh_endproc
	.section	.text.unlikely,"x"
.LCOLDE1:
	.text
.LHOTE1:
	.ident	"GCC: (GNU) 5.4.0"
