#include "LMS_Impl.h"

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

// not matching
__attribute__((noinline))
void LMSi_MemCopy(const void* dest, const void* src, s32 size)
{
    char* destMem = (char*)dest;
    char* srcMem = (char*)src;

    if (size > 0) {

        for (int i = 0; size; i++) {

            destMem[i] = srcMem[i];
            size--;
        }
    }
}

/*
    char* srcMem = (char*)src;
    char* destMem = (char*)dest;
    
    if (size > 0) {

        u32 remainder = size & 3;

        if (size - 1 >= 3) {
            

            for (u32 memOffset = 0; size - remainder != memOffset;) {

                destMem[0] = srcMem[0];
                destMem[1] = srcMem[1];
                destMem[2] = srcMem[2];
                destMem[3] = srcMem[3];
                memOffset += 4;

                srcMem += 4;
                destMem += 4;
            }
        }
    }
*/

/*
void LMSi_MemCopy(const void* dest, const void* src, s32 size)
{
    if (!size) {

        return;
    }

    if (size - 1 < 3) {

        return;
    }

    s32 sizeRounded = size & 3;

    int IdIntoMem;
    for (IdIntoMem = 0; size - sizeRounded != IdIntoMem; IdIntoMem += 4) {

        char* curSrcPtr = ((char*)src) + IdIntoMem;
        char* curDestPtr = ((char*)dest) + IdIntoMem;

        curDestPtr[-1] = curSrcPtr[-1];
        curDestPtr[0] = curSrcPtr[0];
        curDestPtr[1] = curSrcPtr[1];
        curDestPtr[2] = curSrcPtr[2];
    }

    if (sizeRounded != 0) {

        char* curSrcPtr = ((char*)src) + IdIntoMem;
        char* curDestPtr = ((char*)dest) + IdIntoMem;

        for (int i = -sizeRounded; i != 0; i++) {

            curSrcPtr[0] = curDestPtr[0];

            curSrcPtr++;
            curDestPtr++;
        }
    }
}
*/