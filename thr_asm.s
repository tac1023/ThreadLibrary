/*
 * thr_asm.a - Linux Intel 64 assembler portion of the thread library
 *             implementation.
 *
 *             This just provides a stub for the asm_yield function!
 */

	.text                    # Instruction Section
	.align	8                # Align
	.globl	asm_yield        # Set asm_yield visible
asm_yield:
  pushq    %rbp            # Save old frame pointer
  movq     %rsp,%rbp       # Establish new frame pointer
  cmpq     $0,%rdi         # Compare value in rdi against 0
  je       restore         # Jump to Restore if 0
  
save:
  movq     %rsp,0(%rdi)    # Save rsp
  movq     %rbx,8(%rdi)    # Save rbx
  movq     %r12,32(%rdi)   # Save r12
  movq     %r13,40(%rdi)   # Save r13
  movq     %r14,48(%rdi)   # Save r14
  movq     %r15,56(%rdi)   # Save r15

restore:  
  movq     8(%rsi),%rbx    # Restore rbx
  movq     32(%rsi),%r12   # Restore r12
  movq     40(%rsi),%r13   # Restore r13
  movq     48(%rsi),%r14   # Restore r14
  movq     56(%rsi),%r15   # Restore r15
  movq     16(%rsi),%rdi   # Restore rdi
  movq     0(%rsi),%rsp    # Restore rsp
  movq     24(%rsi),%rsi   # Restore rsi
  
  popq     %rbp            # Restore frame pointer
  ret                      # Return to caller
  