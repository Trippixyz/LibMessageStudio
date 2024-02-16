#include "msgfile.h"

#include "libms.h"

// matching
LMS_MessageBinary* LMS_InitMessage(const void* data)
{
    LMS_MessageBinary* msgBinary = (LMS_MessageBinary*)LMSi_Malloc(sizeof(LMS_MessageBinary));

    msgBinary->common.data = (const char*)data;

    LMSi_AnalyzeMessageBinary(&msgBinary->common, "MsgStdBn", 3);

    msgBinary->lbl1Offset = LMSi_SearchBlockByName(&msgBinary->common, "LBL1");
    msgBinary->txt2Offset = LMSi_SearchBlockByName(&msgBinary->common, "TXT2");
    msgBinary->atr1Offset = LMSi_SearchBlockByName(&msgBinary->common, "ATR1");
    msgBinary->ato1Offset = LMSi_SearchBlockByName(&msgBinary->common, "ATO1");
    msgBinary->tsy1Offset = LMSi_SearchBlockByName(&msgBinary->common, "TSY1");

    return msgBinary;
}

// matching
void LMS_CloseMessage(LMS_MessageBinary* msgBinary)
{
    if (msgBinary->common.blocks) {

        LMSi_Free(msgBinary->common.blocks);
    }
    
    LMSi_Free(msgBinary);
}

// matching
s32 LMS_SearchMessageBlockByName(LMS_MessageBinary *msgBinary, const char* blockName)
{
    return LMSi_SearchBlockByName(&msgBinary->common, blockName);
}

#pragma region Messages

// matching
s32 LMS_GetTextNum(LMS_MessageBinary* msgBinary)
{
    if (msgBinary->txt2Offset != -1) {

        return *(s32*)msgBinary->common.blocks[msgBinary->txt2Offset].data;
    }

    return -1;
}

// not matching (functionally correct)
s32 LMS_GetTextSize(LMS_MessageBinary* msgBinary, s32 id)
{
    if (msgBinary->txt2Offset == -1) {

        return -1;
    }
    
    s32* txt2DataIterator = (s32*)msgBinary->common.blocks[msgBinary->txt2Offset].data;
        
    if (txt2DataIterator[0] <= id) {

        return -1;
    }

    txt2DataIterator += txt2DataIterator[id + 1] / 4;

    if (!*txt2DataIterator) {

        return -1;
    }

    u8* addrIntoFile = (u8*)txt2DataIterator;

    switch (msgBinary->common.encoding)
    {
        case LMS_MessageEncoding_UTF8:
        {
            u8 curCharacter;

            while (TRUE) {

                curCharacter = *(u8*)addrIntoFile;

                if (curCharacter == 0x0E) {

                    addrIntoFile += *(u8*)&addrIntoFile[5] + 7;

                    break;
                }
                else if (curCharacter == 0x0F) {

                    addrIntoFile += 3;
                }
                else if (!curCharacter) {

                    break;
                }
                    
                addrIntoFile++;
            }
            break;
        }
        case LMS_MessageEncoding_UTF16:
        {
            u16 curCharacter;

            while (TRUE) {

                curCharacter = *(u16*)addrIntoFile;

                if (curCharacter == 0x0E) {

                    addrIntoFile += *(u16*)&addrIntoFile[6] + 8;

                    continue;
                }
                else if (curCharacter == 0x0F) {

                    addrIntoFile += 6;
                }
                else if (!curCharacter) {

                    break;
                }
                    
                addrIntoFile += 2;
            }
            break;
        }
        case LMS_MessageEncoding_UTF32:
        {
            u32 curCharacter;

            while (TRUE) {

                curCharacter = *(u32*)addrIntoFile;

                if (curCharacter == 0x0E) {

                    addrIntoFile += *(u32*)&addrIntoFile[8] + 10;

                    break;
                }
                else if (curCharacter == 0x0F) {

                    addrIntoFile += 6;
                }
                else if (!curCharacter) {

                    break;
                }
                    
                addrIntoFile += 4;
            }
            break;
        }
    }

    return (char*)addrIntoFile - (char*)txt2DataIterator;
}

// matching
s32 LMS_GetTextStyle(LMS_MessageBinary* msgBinary, s32 id)
{
    if (msgBinary->tsy1Offset != -1) {

        return *(s32*)&msgBinary->common.blocks[msgBinary->tsy1Offset].data[id * 4];
    }

    return -3;
}

// matching
s32 LMS_GetTextStyleByLabel(LMS_MessageBinary* msgBinary, const char* label)
{
    s32 styleId = LMS_GetTextIndexByLabel(msgBinary, label);

    if (styleId > -1) {

        return LMS_GetTextStyle(msgBinary, styleId);
    }

    return styleId;
}

s32 LMS_GetTextIndexByLabel(LMS_MessageBinary* msgBinary, const char* label)
{
    if (msgBinary->lbl1Offset == -1) {
        
        return -2;
    }

    s32 nameLength = 0;
    while (label[nameLength++]);

    LMS_BinaryBlock* lbl1Block = &msgBinary->common.blocks[msgBinary->lbl1Offset];

    const char* lbl1Data = lbl1Block->data;
    u32* hashTableData = (u32*)lbl1Block->data;

    s32 hashTableID = LMSi_GetHashTableIndexFromLabel(label, *lbl1Data);

    u32 labelCount = hashTableData[hashTableID * 2 + 1];
    u32 labelOffset = hashTableData[hashTableID * 2 + 2];

    if (labelCount == 0) {

        return -1;
    }

    for (u32 i = 0; i <= labelCount; i++) {

        u8 size = *(u8*)&lbl1Data[labelOffset];

        if (size + 1 == nameLength) {

            if (LMSi_MemCmp(label, &lbl1Data[labelOffset + 1], size) != 0) {

                return *(s32*)&lbl1Data[labelOffset + size + 1];
            }
        }
        
        labelOffset += size + 5;
    }
    
    return -1;

    /*
    while (1) {

        if (labelCount == 0) {

            return -1;
        }

        size = (u32)*(u8*)&lbl1Data[labelOffset];

        if (size == nameLength && LMSi_MemCmp(label, &lbl1Data[labelOffset + 1], size) != 0) {

            return *(s32*)&lbl1Data[labelOffset + size + 1];
        }

        labelCount--;
        labelOffset += size + 5;
    }

    return -1;
    */


    /*
    if (msgBinary->lbl1Offset != -1) {

        s64 labelLength = 0;
        while (label[labelLength] != '\0') {

            labelLength++;
        }

        char* lbl1Data = (char*)msgBinary->common.blocks[msgBinary->lbl1Offset].data;

        s32 hashTableId = LMSi_GetHashTableIndexFromLabel(label, *(u32*)lbl1Data);

        u32 lol = hashTableId * 2 - 1;

        
    }
    return -2;
    */
}

// matching
const char16_t* LMS_GetText(LMS_MessageBinary* msgBinary, s32 id)
{
    if (msgBinary->txt2Offset == -1) {
        
        return (char16_t*)0;
    }

    char *txt2Data = (char*)msgBinary->common.blocks[msgBinary->txt2Offset].data;

    s32 textCount = *(s32*)txt2Data;
    
    if (id < textCount) {

        u32 textOffsetIntoTXT2 = ((u32*)txt2Data)[id + 1];

        return (char16_t *)&txt2Data[textOffsetIntoTXT2];
    }
    return (char16_t*)0;
}

#pragma endregion