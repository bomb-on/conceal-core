// ARM64 stub implementation for context switching functions
// These functions are used for user-level threading/coroutines
// This is a minimal stub implementation for ARM64/Apple Silicon compatibility

#ifdef __aarch64__

#include "Context.h"

// Stub implementation that returns error
// In a full implementation, these would need to be rewritten for ARM64 architecture
int getmcontext(mctx* mc) {
    // Return error to indicate context switching is not supported
    return -1;
}

void setmcontext(const mctx* mc) {
    // No-op stub - in practice this might need to call abort() or similar
    // to indicate this functionality is not available
}

#endif // __aarch64__