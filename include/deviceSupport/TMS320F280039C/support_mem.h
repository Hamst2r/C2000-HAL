#ifndef SUPPORT_MEM_H_
#define SUPPORT_MEM_H_

// #include "mem.h"

#define MEM_CFG_BASE        (0x0005F400UL)



#define MEM_S_Type(type)        ((type) << 8)
#define MEM_GetType(sect)       ((sect) >> 8)
#define MEM_GetNum(sect)        ((sect) & 0xFFU)

enum MEM_Type
{
    MEM_Type_D   = 0U,
    MEM_Type_LS  = 1U,
    MEM_Type_GS  = 2U,
    MEM_Type_MSG = 3U,
    MEM_Type_ROM = 4U
};

enum MEM_Sect
{
    // DxRAM - Dedicated RAM
    MEM_Sect_M0             = MEM_S_Type(MEM_Type_D) | 0x00U, //!< M0 RAM
    MEM_Sect_M1             = MEM_S_Type(MEM_Type_D) | 0x01U, //!< M1 RAM
    MEM_Sect_D_All          = MEM_S_Type(MEM_Type_D) | 0xFFU, //!< All M and D RAM

    // LSxRAM - Local shared RAM
    MEM_Sect_LS0            = MEM_S_Type(MEM_Type_LS) | 0x00U, //!< LS0 RAM
    MEM_Sect_LS1            = MEM_S_Type(MEM_Type_LS) | 0x01U, //!< LS1 RAM
    MEM_Sect_LS2            = MEM_S_Type(MEM_Type_LS) | 0x02U, //!< LS2 RAM
    MEM_Sect_LS3            = MEM_S_Type(MEM_Type_LS) | 0x03U, //!< LS3 RAM
    MEM_Sect_LS4            = MEM_S_Type(MEM_Type_LS) | 0x04U, //!< LS4 RAM
    MEM_Sect_LS5            = MEM_S_Type(MEM_Type_LS) | 0x05U, //!< LS5 RAM
    MEM_Sect_LS6            = MEM_S_Type(MEM_Type_LS) | 0x06U, //!< LS6 RAM
    MEM_Sect_LS7            = MEM_S_Type(MEM_Type_LS) | 0x07U, //!< LS7 RAM
    MEM_Sect_LS_All         = MEM_S_Type(MEM_Type_LS) | 0xFFU, //!< All LS RAM

    // GSxRAM - Global shared RAM
    MEM_Sect_GS0            = MEM_S_Type(MEM_Type_GS) | 0x00U, //!< GS0 RAM
    MEM_Sect_GS1            = MEM_S_Type(MEM_Type_GS) | 0x01U, //!< GS1 RAM
    MEM_Sect_GS2            = MEM_S_Type(MEM_Type_GS) | 0x02U, //!< GS2 RAM
    MEM_Sect_GS3            = MEM_S_Type(MEM_Type_GS) | 0x03U, //!< GS3 RAM
    MEM_Sect_GS_All         = MEM_S_Type(MEM_Type_GS) | 0xFFU, //!< All GS RAM

    // MSGxRAM - Message RAM
    MEM_Sect_MSG_CPUTOCLA1  = MEM_S_Type(MEM_Type_MSG) | 0x01U, //!< CPU-to-CLA1 message RAM
    MEM_Sect_MSG_CLA1TOCPU  = MEM_S_Type(MEM_Type_MSG) | 0x02U, //!< CLA1-to-CPU message RAM
    MEM_Sect_MSG_CLA1TODMA  = MEM_S_Type(MEM_Type_MSG) | 0x05U, //!< CLA1-to-DMA message RAM
    MEM_Sect_MSG_DMATOCLA1  = MEM_S_Type(MEM_Type_MSG) | 0x06U, //!< DMA-to-CLA1 message RAM
    MEM_Sect_MSG_All        = MEM_S_Type(MEM_Type_MSG) | 0xFFU, //!< All message RAM

    // ROM memory sections
    MEM_Sect_ROM_Boot       = MEM_S_Type(MEM_Type_ROM) | 0x00U, //!< BOOT ROM
    MEM_Sect_ROM_Secure     = MEM_S_Type(MEM_Type_ROM) | 0x01U, //!< Secure ROM
    MEM_Sect_ROM_CLAData    = MEM_S_Type(MEM_Type_ROM) | 0x02U, //!< CLA Data ROM
    MEM_Sect_ROM_All        = MEM_S_Type(MEM_Type_ROM) | 0xFFU, //!< All ROMs

    // All sections
    MEM_Sect_All            = 0xFFFFU //!< All configurable RAM
};


enum MEM_LsRamMode
{
    MEM_LsRamMode_CPU_Only,     //!< CPU is the owner of the section
    MEM_LsRamMode_CPU_CLA_Data, //!< CPU and CLA1 share this section
    MEM_LsRamMode_CLA_Program   //!< Section is CLA program memory
};

#endif /* SUPPORT_MEM_H_ */
