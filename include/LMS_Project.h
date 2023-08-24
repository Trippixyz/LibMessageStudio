#pragma once

#include "LMS_Impl.h"

// PROJECT BINARY
typedef struct
{
    LMS_Binary common;
    s32 clb1Offset; // _20
    s32 clr1Offset; // _24
    s32 alb1Offset; // _28
    s32 ati2Offset; // _2C
    s32 ali2Offset; // _30
    s32 tgg2Offset; // _34
    s32 tag2Offset; // _38
    s32 tgp2Offset; // _3C
    s32 tgl2Offset; // _40
    s32 syl3Offset; // _44
    s32 slb1Offset; // _48
    s32 cti1Offset; // _4C
} LMS_ProjectBinary;

LMS_ProjectBinary* LMS_InitProject(const void* data);
void LMS_CloseProject(LMS_ProjectBinary* prjBinary);

s32 LMS_SearchProjectBlockByName(LMS_ProjectBinary *prjBinary, const char* blockName);


//////////////////////////////////////////////
//                  Colors                  //
//////////////////////////////////////////////

typedef struct
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} LMS_Color;

typedef enum
{
    LMS_ColorResult_NoColors = -5,
    LMS_ColorResult_ColorLabelNotFound = -2,
    LMS_ColorResult_IndexOutOfRange = -1,
    LMS_ColorResult_ColorFound
} LMS_ColorResult;

s32 LMS_GetColorNum(LMS_ProjectBinary* prjBinary);

s32 LMS_GetColorIndexByName(LMS_ProjectBinary* prjBinary, const char* name);

LMS_ColorResult LMS_GetColor(LMS_ProjectBinary* prjBinary, s32 id, LMS_Color* output);
LMS_ColorResult LMS_GetColorByName(LMS_ProjectBinary* prjBinary, const char* name, LMS_Color* output);


//////////////////////////////////////////////
//                Attributes                //
//////////////////////////////////////////////

typedef struct
{
    s8 type;
    u16 listId;
    s32 offset;
} LMS_AttrInfo;

// inlined
LMS_AttrInfo* LMS_GetAttrInfo(LMS_ProjectBinary* prjBinary, s32 id);
s32 LMS_GetAttrInfoIndexByName(LMS_ProjectBinary* prjBinary,char* name);

s8 LMS_GetAttrType(LMS_ProjectBinary* prjBinary, s32 id);

s32 LMS_GetAttrOffset(LMS_ProjectBinary* prjBinary, s32 id);

s32 LMS_GetAttrListItemNum(LMS_ProjectBinary* prjBinary, s32 id);

const char* LMS_GetAttrListItemName(LMS_ProjectBinary* prjBinary, s32 id, s32 itemId);
const char* LMS_GetAttrListItemNameByName(LMS_ProjectBinary* prjBinary, char* name, s32 itemId);


//////////////////////////////////////////////
//                   Tags                   //
//////////////////////////////////////////////

// TagGroup
u16 LMS_GetTagGroupNum(LMS_ProjectBinary* prjBinary);
const char* LMS_GetTagGroupName(LMS_ProjectBinary* prjBinary, u16 tagGroupId);

// Tag
u16 LMS_GetTagNum(LMS_ProjectBinary* prjBinary, u16 tagGroupId);
const char* LMS_GetTagName(LMS_ProjectBinary* prjBinary, u16 tagGroupId, u16 tagId);

// TagParam
typedef enum
{
    Default
} TagParamType;

u16 LMS_GetTagParamNum(LMS_ProjectBinary* prjBinary, u16 tagGroupId, u16 tagId);
const char* LMS_GetTagParamName(LMS_ProjectBinary* prjBinary, u16 tagGroupId, u16 tagId, u16 tagParamId);
TagParamType LMS_GetTagParamType(LMS_ProjectBinary* prjBinary, u16 tagGroupId, u16 tagId, u16 tagParamId);

// TagList
u16 LMS_GetTagListItemNum(LMS_ProjectBinary* prjBinary, u16 tagGroupId, u16 tagId, u16 tagParamId);
const char* LMS_GetTagListItemName(LMS_ProjectBinary* prjBinary, u16 tagGroupId, u16 tagId, u16 tagParamId, u16 tagListItemId);


//////////////////////////////////////////////
//                  Styles                  //
//////////////////////////////////////////////

typedef struct
{
    int regionWidth;
    int lineNum;
    int fontIndex;
    int baseColorIndex;
} LMS_Style;

s32 LMS_GetStyleNum(LMS_ProjectBinary* prjBinary);

LMS_Style* LMS_GetStyle(LMS_ProjectBinary* prjBinary, s32 id); // inlined

s32 LMS_GetRegionWidth(LMS_ProjectBinary* prjBinary, s32 id);
s32 LMS_GetLineNum(LMS_ProjectBinary* prjBinary, s32 id);
s32 LMS_GetFontIndex(LMS_ProjectBinary* prjBinary, s32 id);
s32 LMS_GetBaseColorIndex(LMS_ProjectBinary* prjBinary, s32 id);


//////////////////////////////////////////////
//                 Contents                 //
//////////////////////////////////////////////

s32 LMS_GetContentsNum(LMS_ProjectBinary* prjBinary);

const char* LMS_GetContentPath(LMS_ProjectBinary* prjBinary, s32 id);