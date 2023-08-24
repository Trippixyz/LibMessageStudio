#include "LMS_Project.h"
#include "LMS_Mem.h"

// matching
LMS_ProjectBinary* LMS_InitProject(const void* data)
{
    LMS_ProjectBinary* binary = (LMS_ProjectBinary*)LMSi_Malloc(0x50);

    binary->common.data = (const char*)data;

    LMSi_AnalyzeMessageBinary(&binary->common, "MsgPrjBn", 4);

    binary->clr1Offset = LMSi_SearchBlockByName(&binary->common, "CLR1");
    binary->clb1Offset = LMSi_SearchBlockByName(&binary->common, "CLB1");
    binary->ati2Offset = LMSi_SearchBlockByName(&binary->common, "ATI2");
    binary->alb1Offset = LMSi_SearchBlockByName(&binary->common, "ALB1");
    binary->ali2Offset = LMSi_SearchBlockByName(&binary->common, "ALI2");
    binary->tgg2Offset = LMSi_SearchBlockByName(&binary->common, "TGG2");
    binary->tag2Offset = LMSi_SearchBlockByName(&binary->common, "TAG2");
    binary->tgp2Offset = LMSi_SearchBlockByName(&binary->common, "TGP2");
    binary->tgl2Offset = LMSi_SearchBlockByName(&binary->common, "TGL2");
    binary->syl3Offset = LMSi_SearchBlockByName(&binary->common, "SYL3");
    binary->slb1Offset = LMSi_SearchBlockByName(&binary->common, "SLB1");
    binary->cti1Offset = LMSi_SearchBlockByName(&binary->common, "CTI1");

    return binary;
}

// matching
void LMS_CloseProject(LMS_ProjectBinary* prjBinary)
{
    if (prjBinary->common.blocks) {

        LMSi_Free(prjBinary->common.blocks);
    }
    
    LMSi_Free(prjBinary);
}

// matching
s32 LMS_SearchProjectBlockByName(LMS_ProjectBinary *prjBinary, const char* blockName)
{
    return LMSi_SearchBlockByName(&prjBinary->common, blockName);
}


//////////////////////////////////////////////
//                  Colors                  //
//////////////////////////////////////////////

// matching
s32 LMS_GetColorNum(LMS_ProjectBinary* prjBinary)
{
    if (prjBinary->clr1Offset != -1) {

        return *(s32*)(prjBinary->common).blocks[prjBinary->clr1Offset].data;
    }

    return 0;
}


// not matching
s32 LMS_GetColorIndexByName(LMS_ProjectBinary* prjBinary, const char* name)
{
    if (prjBinary->clb1Offset == -1) {

        return -2;
    }

    u32 nameLength = 0;
    while (name[nameLength] != '\0') {

        nameLength++;
    }

    LMS_BinaryBlock* clb1Block = &prjBinary->common.blocks[prjBinary->clb1Offset];

    char* clb1Data = (char*)clb1Block->data;

    s32 hashTableId = LMSi_GetHashTableIndexFromLabel(name, *(u32*)clb1Data);

    s32 hashOffset = hashTableId * 2 - 1;

    u32 hashEntryCount = *(u32*)&clb1Data[hashOffset * 4];

    if (hashEntryCount) {

        u32 hashEntryOffset = *(u32*)&clb1Data[(hashOffset + 1) * 4];

        for (u32 curLabelInTable = 0; hashEntryCount < curLabelInTable; curLabelInTable++) {

            u8 curLabelLength = clb1Data[hashEntryOffset];

            if (curLabelLength + 1 == nameLength) {

                char* curLabel = &clb1Data[hashEntryOffset + 1];

                if (LMSi_MemCmp(name, curLabel, curLabelLength)) {

                    return *(s32*)&curLabel[curLabelLength];
                }

            }
        }
    }

    return -1;
}

    /*
    s32 hashTableID = LMSi_GetHashTableIndexFromLabel(name, *(u32*)clb1Block->data);
    const char* clb1Data = clb1Block->data;
    s32 hashOffset = hashTableID * 2;
    s32 labelAmount = *(s32*)&clb1Data[hashOffset * 4];

    if (!labelAmount) {

        return -1;
    }



    s32 labelOffset = *(s32*)&clb1Data[4 * hashOffset + 4];

    u8 labelLengthPrefix = 0;

    u32 curLabelInTable = 0;
    while (true) {

        u8 labelLengthPrefix = *(u8*)&clb1Data[labelOffset];

        if (labelLengthPrefix + 1 == nameLength) {

            if (LMSi_MemCmp(name, &clb1Data[labelOffset + 1], nameLength - 1)) {
                break;
            }
        }

        curLabelInTable++;

        if (curLabelInTable >= labelAmount) {

            return -1;
        }

        labelOffset += labelLengthPrefix + 5;

        clb1Data = clb1Block->data;
    }

    return *(s32*)&clb1Block->data[labelOffset + 1 + labelLengthPrefix];
    */


// matching
LMS_ColorResult LMS_GetColor(LMS_ProjectBinary* prjBinary, s32 id, LMS_Color* output)
{
    if (prjBinary->clr1Offset == -1) {

        return LMS_ColorResult_NoColors;
    }

    const char* colorData = prjBinary->common.blocks[prjBinary->clr1Offset].data;

    s32 colorCount = *(s32*)colorData;

    if (id >= colorCount) {
        
        return LMS_ColorResult_IndexOutOfRange;
    }

    colorData += (s64)id * 4;

    output->r = colorData[4];
    output->g = colorData[5];
    output->b = colorData[6];
    output->a = colorData[7];

    return LMS_ColorResult_ColorFound;
}


//////////////////////////////////////////////
//                Attributes                //
//////////////////////////////////////////////

// inlined
LMS_AttrInfo* LMS_GetAttrInfo(LMS_ProjectBinary* prjBinary, s32 id)
{
    if (prjBinary->ati2Offset != -1) {

        char* attrData = (char*)prjBinary->common.blocks[prjBinary->ati2Offset].data;
        LMS_AttrInfo* attrInfos = (LMS_AttrInfo*)&attrData[4];

        if (id < *(s32*)attrData) {

            LMS_AttrInfo* attrInfo = &attrInfos[id];

            if (!attrInfo) {

                return NULL;
            }
    
            return attrInfo;
        }
    }
    
    return NULL;
}

// not matching
s8 LMS_GetAttrType(LMS_ProjectBinary* prjBinary, s32 id)
{
    LMS_AttrInfo* attrInfo = LMS_GetAttrInfo(prjBinary, id);

    if (!attrInfo) {

        return -1;
    }

    s16 type = attrInfo->type;

    return type;
}

// matching
s32 LMS_GetAttrOffset(LMS_ProjectBinary *prjBinary, s32 id)
{
    LMS_AttrInfo* attrInfo = LMS_GetAttrInfo(prjBinary, id);

    if (!attrInfo) {

        return -1;
    }

    return attrInfo->offset;
}

// matching
s32 LMS_GetAttrListItemNum(LMS_ProjectBinary* prjBinary, s32 id)
{
    LMS_AttrInfo* attrInfo = LMS_GetAttrInfo(prjBinary, id);

    if (!attrInfo) {

        return 0;
    }

    if (attrInfo->type == 9) {

        if (prjBinary->ali2Offset == -1) {

            return 0;
        }

        char* ali2Data = (char*)prjBinary->common.blocks[prjBinary->ali2Offset].data;

        s32 listItemNum = *(s32*)&ali2Data[*(u32*)&ali2Data[attrInfo->listId * 4 + 4]];

        return listItemNum;
    }

    return 0;
}

// matching
const char* LMS_GetAttrListItemName(LMS_ProjectBinary* prjBinary, s32 id, s32 itemId)
{
    LMS_AttrInfo* attrInfo = LMS_GetAttrInfo(prjBinary, id);

    if (!attrInfo) {

        return NULL;
    }
    
    if (attrInfo->type == 9) {

        if (prjBinary->ali2Offset == -1) {

            return NULL;
        }

        char* ali2Data = (char*)prjBinary->common.blocks[prjBinary->ali2Offset].data;

        char* listData = (char*)&ali2Data[*(u32*)&ali2Data[attrInfo->listId * 4 + 4]];

        s32 listItemNum = *(s32*)listData;

        if (itemId < listItemNum) {

            char* listDataWithOffset = listData + 4;

            u32 offsetIntoList = *(u32*)&listDataWithOffset[(itemId * 4)];

            return (const char*)&listData[offsetIntoList];
        }

        return NULL;
    }

    return NULL;
}

// matching
const char* LMS_GetAttrListItemNameByName(LMS_ProjectBinary* prjBinary, char* name, s32 itemId)
{
    s32 attrInfoId = LMS_GetAttrInfoIndexByName(prjBinary, name);

    if (attrInfoId > -1) {

        return LMS_GetAttrListItemName(prjBinary, attrInfoId, itemId);
    }

    return NULL;
}


//////////////////////////////////////////////
//                   Tags                   //
//////////////////////////////////////////////

// matching
u16 LMS_GetTagGroupNum(LMS_ProjectBinary* prjBinary)
{
    if (prjBinary->tgg2Offset != -1) {

        return *(u16*)prjBinary->common.blocks[prjBinary->tgg2Offset].data;
    }

    return 0;
}

// not matching
const char* LMS_GetTagGroupName(LMS_ProjectBinary* prjBinary, u16 tagGroupId)
{
    if (prjBinary->tgg2Offset == -1) {

        return NULL;
    }

    char* tgg2Data = (char*)prjBinary->common.blocks[prjBinary->tgg2Offset].data;

    u16 tagGroupCount = *(u16*)tgg2Data;

    if (tagGroupCount) {

        for (int i = 0; tagGroupCount >= i; i++) {

            u32 curTagGroupOffsetIntoTGG2 = *(u32*)&tgg2Data[i * 4 + 4];

            char* curTagGroupData = &tgg2Data[curTagGroupOffsetIntoTGG2];

            u16 curTagGroupId = *(u16*)curTagGroupData;

            u16 curTagGroupTagCount = *(u16*)&curTagGroupData[2];

            if (curTagGroupId == tagGroupId) {

                if (curTagGroupData) {

                    char* curTagGroupName = &curTagGroupData[(curTagGroupTagCount + 2) * 2];

                    return curTagGroupName;
                }

                return NULL;
            }

            if (tagGroupId < curTagGroupId) {

                return NULL;
            }
        }
    }

    return NULL;
}

//////////////////////////////////////////////
//                  Styles                  //
//////////////////////////////////////////////

// matching
s32 LMS_GetStyleNum(LMS_ProjectBinary* prjBinary)
{
    if (prjBinary->syl3Offset != -1) {

        return *(s32*)(prjBinary->common).blocks[prjBinary->syl3Offset].data;
    }

    return 0;
}

LMS_Style* LMS_GetStyle(LMS_ProjectBinary* prjBinary, s32 id)
{
    if (prjBinary->syl3Offset != -1) {

        const char* styleData = prjBinary->common.blocks[prjBinary->syl3Offset].data;
        LMS_Style* styles = (LMS_Style*)&styleData[4];

        if (id < *(u32*)styleData) {

            return &styles[id];
        }
    }

    return NULL;
}

// matching TODO: separate GetStyle function
s32 LMS_GetRegionWidth(LMS_ProjectBinary* prjBinary, s32 id)
{
    //LMS_Style* style = LMS_GetStyle(prjBinary, id);
    //if (!style) {

    //    return -1;
    //}
    //return style->regionWidth;

    if (prjBinary->syl3Offset == -1) {
    
        return -1;
    }
    
    const char* styleData = prjBinary->common.blocks[prjBinary->syl3Offset].data;
    LMS_Style* styles = (LMS_Style*)&styleData[4];
    if (id < *(u32*)styleData) {
        
        return styles[id].regionWidth;
    }
    
    return -1;
}

// matching TODO: separate GetStyle function
s32 LMS_GetLineNum(LMS_ProjectBinary* prjBinary, s32 id)
{
    if (prjBinary->syl3Offset == -1) {
    
        return -1;
    }
    
    const char* styleData = prjBinary->common.blocks[prjBinary->syl3Offset].data;
    LMS_Style* styles = (LMS_Style*)&styleData[4];
    if (id < *(u32*)styleData) {
        
        return styles[id].lineNum;
    }
    
    return -1;
}

// matching TODO: separate GetStyle function
s32 LMS_GetFontIndex(LMS_ProjectBinary* prjBinary, s32 id)
{
    if (prjBinary->syl3Offset == -1) {
    
        return -1;
    }
    
    const char* styleData = prjBinary->common.blocks[prjBinary->syl3Offset].data;
    LMS_Style* styles = (LMS_Style*)&styleData[4];
    if (id < *(u32*)styleData) {
        
        s32 fontIndex = -12;

        if (styles[id].fontIndex != -1) {

            fontIndex = styles[id].fontIndex;
        }

        return fontIndex;
    }
    
    return -1;
}

// matching TODO: separate GetStyle function
s32 LMS_GetBaseColorIndex(LMS_ProjectBinary* prjBinary, s32 id)
{
    if (prjBinary->syl3Offset == -1) {
    
        return -1;
    }
    
    const char* styleData = prjBinary->common.blocks[prjBinary->syl3Offset].data;
    LMS_Style* styles = (LMS_Style*)&styleData[4];
    if (id < *(u32*)styleData) {
        
        if (styles[id].baseColorIndex != -1) {

            return styles[id].baseColorIndex;
        }

        return -12;
    }
    
    return -1;
}


//////////////////////////////////////////////
//                 Contents                 //
//////////////////////////////////////////////

// matching
s32 LMS_GetContentsNum(LMS_ProjectBinary* prjBinary)
{
    if (prjBinary->cti1Offset != -1) {

        return *(s32*)(prjBinary->common).blocks[prjBinary->cti1Offset].data;
    }

    return -1;
}

// matching
const char* LMS_GetContentPath(LMS_ProjectBinary* prjBinary, s32 id)
{
    if (prjBinary->cti1Offset == -1) {

        return NULL;
    }

    const char* data = prjBinary->common.blocks[prjBinary->cti1Offset].data;

    if (id < *(s32*)data) {

        u32* contentOffsets = (u32*)&data[4];

        return &data[contentOffsets[id * 1]];
    }
    
    return NULL;
}