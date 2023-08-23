#pragma once

#include <cstddef>
#include <nn/types.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct LMS_BinaryBlock {
    const char* data;
    char type[4];
    u32 size;
    u16 unk;
} LMS_BinaryBlock;

typedef enum LMS_MessageEncoding : u8 {
    LMS_MessageEncoding_UTF8,
    LMS_MessageEncoding_UTF16,
    LMS_MessageEncoding_UTF32
} LMS_MessageEncoding;

typedef struct LMS_Binary {
    const char* data;
    u64 fileSize;
    LMS_MessageEncoding encoding;
    char version;
    u16 numBlocks;
    LMS_BinaryBlock* blocks;
} LMS_Binary;


// hash table
s32 LMSi_GetHashTableIndexFromLabel(const char* label, u32 numSlots);

// block
s32 LMSi_SearchBlockByName(LMS_Binary* binary, const char* blockName);
LMS_BinaryBlock* LMSi_GetBlockInfoByName(LMS_Binary* binary, const char* name);

// label
inline s32 LMSi_GetLabelIndexByName(LMS_BinaryBlock* binaryBlock, const char* name); // TODO: make a proper inlined function

// analyze
void LMSi_AnalyzeMessageBinary(LMS_Binary* binary, const char* magic, u8 version);

void LMSi_AnalyzeMessageHeader(LMS_Binary* binary);
void LMSi_AnalyzeMessageBlocks(LMS_Binary* binary);


#ifdef __cplusplus
}
#endif