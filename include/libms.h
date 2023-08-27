#include "types.h"

typedef void* (*LMS_AllocFuncPtr)(u64 size); // TODO: u64 is prolly right parameter
typedef void (*LMS_FreeFuncPtr)(void* ptr);

// Description: This function allocates memory for libms.
// Parameters:
// - size: The size of the memory to be allocated.
// Returns: The pointer to the allocated memory.
void* LMSi_Malloc(size_t size);

// Description: This function frees memory for libms.
// Parameters:
// - ptr: The pointer to the allocated memory.
void LMSi_Free(void* ptr);

// Description: This function sets functions for allocating and freeing memory for libms.
// Parameters:
// - alloc_ptr: The allocating function.
// - free_ptr: The freeing function.
void LMS_SetMemFuncs(LMS_AllocFuncPtr alloc_ptr, LMS_FreeFuncPtr free_ptr);

// Description: This function compares memory for libms.
// Parameters:
// - ptr1: The first pointer.
// - ptr1: The second pointer.
// - size: The size of the memory to be compared.
// Returns: The Result of the comparing [1=Matching, 0=Not matching]
s32 LMSi_MemCmp(const void* ptr1, const void* ptr2, s32 size);

// Description: This function copies memory for libms.
// Parameters:
// - dest: The destination pointer.
// - src: The source pointer.
// - size: The size of the memory to be copied.
void LMSi_MemCopy(const void* dest, const void* src, s32 size);
