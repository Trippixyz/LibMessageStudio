#include <stdbool.h>
#include <cstddef>
#include <nn/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* (*LMS_AllocFuncPtr)(u64 size); // TODO: u64 is prolly right parameter
typedef void (*LMS_FreeFuncPtr)(void* ptr);

// memory
void* LMSi_Malloc(size_t size);
void LMSi_Free(void* ptr);
void LMS_SetMemFuncs(LMS_AllocFuncPtr alloc_ptr, LMS_FreeFuncPtr free_ptr);
s32 LMSi_MemCmp(const void* ptr1, const void* ptr2, s32 size);
void LMSi_MemCopy(const void* dest, const void* src, s32 size);

#ifdef __cplusplus
}
#endif