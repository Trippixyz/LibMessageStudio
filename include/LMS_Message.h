#pragma once

#include "LibMessageStudio/LibMessageStudio.h"
#include <nn/types.h>

#ifdef __cplusplus
extern "C" {
#endif

// STANDARD BINARY
struct LMS_MessageBinary
{
    LMS_Binary common;
    s32 lbl1Offset; // _20
    s32 txt2Offset; // _24
    s32 atr1Offset; // _28
    s32 ato1Offset; // _2C
    s32 tsy1Offset; // _30
    // u32 _34;
};

LMS_MessageBinary* LMS_InitMessage(const void* data);
void LMS_CloseMessage(LMS_MessageBinary* msgBinary);

s32 LMS_SearchMessageBlockByName(LMS_MessageBinary *msgBinary, const char* blockName);

#pragma region Messages

s32 LMS_GetTextNum(LMS_MessageBinary* msgBinary);

s32 LMS_GetTextSize(LMS_MessageBinary* msgBinary, s32 id);

s32 LMS_GetLabelByTextIndex(LMS_MessageBinary* msgBinary, s32 id, const char* output);

s32 LMS_GetTextStyle(LMS_MessageBinary* msgBinary, s32 id);
s32 LMS_GetTextStyleByLabel(LMS_MessageBinary* msgBinary, const char* label);

s32 LMS_GetTextIndexByLabel(LMS_MessageBinary* msgBinary, const char* label);

const wchar_t* LMS_GetText(LMS_MessageBinary* msgBinary, s32 id);
const wchar_t* LMS_GetTextByLabel(LMS_MessageBinary* msgBinary, const char* label);

#pragma endregion

#ifdef __cplusplus
}
#endif