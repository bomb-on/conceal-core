// ARM64 implementation for context switching functions
// These functions are used for user-level threading/coroutines
// Full implementation for ARM64/Apple Silicon

#ifdef __aarch64__

#include "Context.h"
#include <string.h>

// ARM64 implementation of getmcontext
int getmcontext(mctx* mc) {
    if (!mc) {
        return -1;
    }
    
    // Clear the context structure
    memset(mc, 0, sizeof(*mc));
    
    // Save the current context using inline assembly
    __asm__ volatile (
        // Save general purpose registers x0-x28
        "stp x0, x1, [%0, #0]\n\t"
        "stp x2, x3, [%0, #16]\n\t"
        "stp x4, x5, [%0, #32]\n\t"
        "stp x6, x7, [%0, #48]\n\t"
        "stp x8, x9, [%0, #64]\n\t"
        "stp x10, x11, [%0, #80]\n\t"
        "stp x12, x13, [%0, #96]\n\t"
        "stp x14, x15, [%0, #112]\n\t"
        "stp x16, x17, [%0, #128]\n\t"
        "stp x18, x19, [%0, #144]\n\t"
        "stp x20, x21, [%0, #160]\n\t"
        "stp x22, x23, [%0, #176]\n\t"
        "stp x24, x25, [%0, #192]\n\t"
        "stp x26, x27, [%0, #208]\n\t"
        "str x28, [%0, #224]\n\t"
        
        // Save frame pointer (x29) and link register (x30)
        "str x29, [%0, #232]\n\t"  // mc_fp
        "str x30, [%0, #240]\n\t"  // mc_lr
        
        // Save stack pointer
        "mov x1, sp\n\t"
        "str x1, [%0, #248]\n\t"   // mc_sp
        
        // Save program counter (return address)
        "adr x1, 1f\n\t"          // Get address of next instruction
        "str x1, [%0, #256]\n\t"   // mc_pc
        "1:\n\t"
        
        // We'll skip CPSR for now as it's complex to save/restore properly
        
        : // no outputs
        : "r" (mc->mc_x)          // input: pointer to register array
        : "x1", "memory"         // clobbered registers
    );
    
    return 0; // Success
}

// ARM64 implementation of setmcontext
void setmcontext(const mctx* mc) {
    if (!mc) {
        return;
    }
    
    // Restore context using inline assembly
    __asm__ volatile (
        // Restore general purpose registers x0-x28
        "ldp x0, x1, [%0, #0]\n\t"
        "ldp x2, x3, [%0, #16]\n\t"
        "ldp x4, x5, [%0, #32]\n\t"
        "ldp x6, x7, [%0, #48]\n\t"
        "ldp x8, x9, [%0, #64]\n\t"
        "ldp x10, x11, [%0, #80]\n\t"
        "ldp x12, x13, [%0, #96]\n\t"
        "ldp x14, x15, [%0, #112]\n\t"
        "ldp x16, x17, [%0, #128]\n\t"
        "ldp x18, x19, [%0, #144]\n\t"
        "ldp x20, x21, [%0, #160]\n\t"
        "ldp x22, x23, [%0, #176]\n\t"
        "ldp x24, x25, [%0, #192]\n\t"
        "ldp x26, x27, [%0, #208]\n\t"
        "ldr x28, [%0, #224]\n\t"
        
        // Restore frame pointer and link register
        "ldr x29, [%0, #232]\n\t"  // mc_fp
        "ldr x30, [%0, #240]\n\t"  // mc_lr
        
        // Restore stack pointer
        "ldr x1, [%0, #248]\n\t"   // mc_sp
        "mov sp, x1\n\t"
        
        // Jump to the saved program counter
        "ldr x1, [%0, #256]\n\t"   // mc_pc
        "br x1\n\t"               // Branch to saved PC
        
        : // no outputs
        : "r" (mc->mc_x)          // input: pointer to register array
        : "memory"               // memory is clobbered
    );
    
    // This point should never be reached
    __builtin_unreachable();
}

#endif // __aarch64__
