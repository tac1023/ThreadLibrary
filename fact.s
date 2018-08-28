#
# x86-64 (Linux) assembler source for computing the factorial function.
#
# The code is based on MIPS code from pages A-26 and A-27 of Patterson &
# Hennessy.
#
# It computes fact(10).
#
    .text
    .align    8
    .globl   main
main:
    pushq    %rbp            # Save old frame pointer
    movq     %rsp,%rbp       # Establish new frame pointer
#
    movq     $10,%rdi        # pass 10 as an argument
    call     fact            # Call factorial function
#
    movq     $.LC0,%rdi      # Pass format string as arg 1
    movq     %rax,%rsi       # Pass return value from fact as arg 2
    call     printf          # Call the printf function
#
    popq     %rbp            # Restore frame pointer
    ret                      # Return to caller

#
    .data
.LC0:
    .string    "The factorial of 10 is %ld\n"
#
#
# The factorial function itself
#
# ie fact(n)
#
    .text
    .align   8
    .globl   fact            # .globl also allows gdb to see label
fact:
    pushq    %rbp            # Save old frame pointer
    movq     %rsp,%rbp       # Establish new frame pointer
    subq     $8,%rsp         # Allocate one local
#
    cmpq     $0,%rdi         # Test n against 0
    jg       .L2             # Branch if n > 0
    movq     $1,%rax         # Return 1
    jmp      .L1             # Jump to code to return
#
.L2:
    movq     %rdi,-8(%rbp)   # Save n into the local
    subq     $1,%rdi         # Compute (n - 1)
    call     fact            # Recursive call
#
    imulq    -8(%rbp),%rax   # Compute n * fact(n - 1)
#
.L1:                         # Result is in %rax
    addq     $8,%rsp         # Deallocate the local
    popq     %rbp            # Restore frame pointer
    ret                      # Return to caller

