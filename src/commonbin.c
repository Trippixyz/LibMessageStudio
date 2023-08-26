#include "commonbin.h"

#include "libms.h"

// matching
s32 LMSi_GetHashTableIndexFromLabel(const char* label, u32 numSlots)
{
    char* curCharInLabel = (char*)label;
    u32 hash = 0;

    while (*curCharInLabel != 0) {

        hash = hash * 0x492 + *curCharInLabel;

        curCharInLabel++;
    }

    return (hash & 0xFFFFFFFF) % numSlots;
}

// matching
s32 LMSi_SearchBlockByName(LMS_Binary* binary, const char* blockName)
{
    u16 blocks = binary->numBlocks;

    for (u16 id = 0; id < blocks; id++) {

        if (LMSi_MemCmp(binary->blocks[id].type, blockName, sizeof(binary->blocks[id].type)))
            return id;
        
        blocks = binary->numBlocks;
    }

    return -1;
}

// matching
LMS_BinaryBlock* LMSi_GetBlockInfoByName(LMS_Binary* binary, const char* name)
{
    if (binary->numBlocks) {

        for (s32 i = 0; i < binary->numBlocks; i++) {

            if (LMSi_MemCmp(binary->blocks[i].type, name, sizeof(binary->blocks[i].type))) {

                return &binary->blocks[i];
            }
        }
    }

    return NULL;
}

// wip
inline s32 LMSi_GetLabelIndexByName(LMS_BinaryBlock* binaryBlock, const char* name)
{
    return 0;
}

// matching
void LMSi_AnalyzeMessageBinary(LMS_Binary* binary, const char* magic, u8 version)
{
    LMSi_AnalyzeMessageHeader(binary);
    LMSi_AnalyzeMessageBlocks(binary);
}

// matching
void LMSi_AnalyzeMessageHeader(LMS_Binary* binary)
{
    const char* data = binary->data;

    binary->encoding = *(u8*)&data[0x0C];
    binary->numBlocks = *(u16*)&data[0x0E];

    if (!binary->numBlocks) {

        binary->blocks = NULL;
    }
    else {

        binary->blocks = (LMS_BinaryBlock*)LMSi_Malloc(binary->numBlocks * sizeof(LMS_BinaryBlock));
    }

    binary->fileSize = *(u32*)&binary->data[0x12];
}

// matching
void LMSi_AnalyzeMessageBlocks(LMS_Binary* binary) {

    u64 curBlockDataOffset = 0x20;

    for (s64 curBlockId = 0; curBlockId < binary->numBlocks; curBlockId++) {

        LMS_BinaryBlock* curBlock = &binary->blocks[curBlockId];

        const char* blockData = &binary->data[curBlockDataOffset + 0x10];

        curBlock->data = blockData;

        // Read block type.
        curBlock->type[0] = binary->data[curBlockDataOffset + 0];
        curBlock->type[1] = binary->data[curBlockDataOffset | 1];
        curBlock->type[2] = binary->data[(curBlockDataOffset | 1)+1];
        curBlock->type[3] = binary->data[curBlockDataOffset | 3];

        u32 curBlockSize = *(u32*)&binary->data[(curBlockDataOffset | 3)+1];
        curBlock->size = curBlockSize;
        curBlock->unk = *(u16*)&binary->data[(curBlockDataOffset | 3)+5];
        

        curBlockDataOffset = (curBlockDataOffset + curBlockSize + 0x1f) & ~0xf;
    }
}