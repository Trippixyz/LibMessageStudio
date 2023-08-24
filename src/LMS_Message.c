#include "LMS_Message.h"
#include "LMS_Mem.h"

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

// not matching
s32 LMS_GetTextSize(LMS_MessageBinary* msgBinary, s32 id)
{
    if (msgBinary->txt2Offset != -1) {
        
        if (*(s32*)msgBinary->common.blocks[msgBinary->txt2Offset].data > id) {

            if (*(s32*)(msgBinary->common.blocks[msgBinary->txt2Offset].data + id * 0x4)) {

                switch (msgBinary->common.encoding)
                {
                    case LMS_MessageEncoding_UTF8:
                        
                        break;
                    case LMS_MessageEncoding_UTF16:
                        
                        break;
                    case LMS_MessageEncoding_UTF32:
                        
                        break;
                }
            }
        }
    }

    return -1;
}

s32 LMS_GetTextIndexByLabel(LMS_MessageBinary* msgBinary, const char* label)
{
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
}

// matching
const wchar_t* LMS_GetText(LMS_MessageBinary* msgBinary, s32 id)
{
    if (msgBinary->txt2Offset == -1) {
        
        return NULL;
    }

    char *txt2Data = (char*)msgBinary->common.blocks[msgBinary->txt2Offset].data;

    s32 textCount = *(s32*)txt2Data;
    
    if (id < textCount) {

        u32 textOffsetIntoTXT2 = ((u32*)txt2Data)[id + 1];

        return (wchar_t *)&txt2Data[textOffsetIntoTXT2];
    }
    return NULL;
}

#pragma endregion