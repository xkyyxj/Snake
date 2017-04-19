.globl detect_memory
.type detect_memory, @function
detect_memory:
    pushal;

    movw $0x3000,%ax;
    movw %ax,%es;
    xor %edi,%edi;

    movl $0xe820,%eax;
    xorl %ebx,%ebx;
    movl $20,%ecx;
    movl %0x534d4150,%edx;

1:    
    int $0x15;
    jb 2f;
    cmp %ebx,0;
    jz 2f;

    addl $20,%edi;
    jmp 1b;
    
2:
    retl;