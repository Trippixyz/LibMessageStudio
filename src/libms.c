#include "libms.h"

static LMS_AllocFuncPtr LMSi_sAllocFuncPtr;
static LMS_FreeFuncPtr LMSi_sFreeFuncPtr;

// matching
__attribute__((noinline))
void* LMSi_Malloc(size_t size)
{
    return LMSi_sAllocFuncPtr(size);
}

// matching
__attribute__((noinline))
void LMSi_Free(void* ptr)
{
    LMSi_sFreeFuncPtr(ptr);
}

// matching
__attribute__((noinline))
void LMS_SetMemFuncs(LMS_AllocFuncPtr alloc_ptr, LMS_FreeFuncPtr free_ptr)
{
    LMSi_sAllocFuncPtr = alloc_ptr;
    LMSi_sFreeFuncPtr = free_ptr;
}

// matching
__attribute__((noinline))
s32 LMSi_MemCmp(const void* ptr1, const void* ptr2, s32 size)
{
    if (size < 1) {
        return 1;
    }

    const char* ptr1Char = (const char*)ptr1;
    const char* ptr2Char = (const char*)ptr2;

    for (int idIntoMem = 0; idIntoMem < size; idIntoMem++) {

        if (ptr1Char[idIntoMem] != ptr2Char[idIntoMem]) {

            return 0;
        }
    }

    return 1;
}

// matching
__attribute__((noinline))
void LMSi_MemCopy(const void* dest, const void* src, s32 size)
{
    for (int i = 0; i < size; i++)
    {
        ((u8*)dest)[i] = ((u8*)src)[i];
    }
}