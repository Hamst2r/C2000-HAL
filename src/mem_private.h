#ifndef MEM_PRIVATE_H_
#define MEM_PRIVATE_H_

#define MEM_DxLOCK              ( REG32( MEM_CFG_BASE + 0x00UL ) )  // Dedicated RAM Config Lock
#define MEM_DxCOMMIT            ( REG32( MEM_CFG_BASE + 0x02UL ) )  // Dedicated RAM Config Lock Commit
#define MEM_DxACCPROT0          ( REG32( MEM_CFG_BASE + 0x08UL ) )  // Dedicated RAM Config
#define MEM_DxTEST              ( REG32( MEM_CFG_BASE + 0x10UL ) )  // Dedicated RAM TEST
#define MEM_DxINIT              ( REG32( MEM_CFG_BASE + 0x12UL ) )  // Dedicated RAM Init
#define MEM_DxINITDONE          ( REG32( MEM_CFG_BASE + 0x14UL ) )  // Dedicated RAM InitDone Status
#define MEM_DxRAMTEST_LOCK      ( REG32( MEM_CFG_BASE + 0x16UL ) )  // Lock register to Dx RAM TEST
#define MEM_LSxLOCK             ( REG32( MEM_CFG_BASE + 0x20UL ) )  // Local Shared RAM Config Lock
#define MEM_LSxCOMMIT           ( REG32( MEM_CFG_BASE + 0x22UL ) )  // Local Shared RAM Config Lock Commit
#define MEM_LSxMSEL             ( REG32( MEM_CFG_BASE + 0x24UL ) )  // Local Shared RAM Master Sel
#define MEM_LSxCLAPGM           ( REG32( MEM_CFG_BASE + 0x26UL ) )  // Local Shared RAM Prog/Exe control
#define MEM_LSxACCPROT0         ( REG32( MEM_CFG_BASE + 0x28UL ) )  // Local Shared RAM Config 0
#define MEM_LSxACCPROT1         ( REG32( MEM_CFG_BASE + 0x2AUL ) )  // Local Shared RAM Config 1
#define MEM_LSxTEST             ( REG32( MEM_CFG_BASE + 0x30UL ) )  // Local Shared RAM TEST
#define MEM_LSxINIT             ( REG32( MEM_CFG_BASE + 0x32UL ) )  // Local Shared RAM Init
#define MEM_LSxINITDONE         ( REG32( MEM_CFG_BASE + 0x34UL ) )  // Local Shared RAM InitDone Status
#define MEM_LSxRAMTEST_LOCK     ( REG32( MEM_CFG_BASE + 0x36UL ) )  // Lock register to LSx RAM TEST
#define MEM_GSxLOCK             ( REG32( MEM_CFG_BASE + 0x40UL ) )  // Global Shared RAM Config Lock
#define MEM_GSxCOMMIT           ( REG32( MEM_CFG_BASE + 0x42UL ) )  // Global Shared RAM Config Lock Commit
#define MEM_GSxACCPROT0         ( REG32( MEM_CFG_BASE + 0x48UL ) )  // Global Shared RAM Config 0
#define MEM_GSxTEST             ( REG32( MEM_CFG_BASE + 0x50UL ) )  // Global Shared RAM TEST
#define MEM_GSxINIT             ( REG32( MEM_CFG_BASE + 0x52UL ) )  // Global Shared RAM Init
#define MEM_GSxINITDONE         ( REG32( MEM_CFG_BASE + 0x54UL ) )  // Global Shared RAM InitDone Status
#define MEM_GSxRAMTEST_LOCK     ( REG32( MEM_CFG_BASE + 0x56UL ) )  // Lock register to GSx RAM TEST
#define MEM_MSGxLOCK            ( REG32( MEM_CFG_BASE + 0x60UL ) )  // Message RAM Config Lock
#define MEM_MSGxCOMMIT          ( REG32( MEM_CFG_BASE + 0x62UL ) )  // Message RAM Config Lock Commit
#define MEM_MSGxTEST            ( REG32( MEM_CFG_BASE + 0x70UL ) )  // Message RAM TEST
#define MEM_MSGxINIT            ( REG32( MEM_CFG_BASE + 0x72UL ) )  // Message RAM Init
#define MEM_MSGxINITDONE        ( REG32( MEM_CFG_BASE + 0x74UL ) )  // Message RAM InitDone Status
#define MEM_MSGxRAMTEST_LOCK    ( REG32( MEM_CFG_BASE + 0x76UL ) )  // Lock register for MSGx RAM TEST
#define MEM_ROM_LOCK            ( REG32( MEM_CFG_BASE + 0xA0UL ) )  // ROM Config Lock
#define MEM_ROM_TEST            ( REG32( MEM_CFG_BASE + 0xA2UL ) )  // ROM TEST
#define MEM_ROM_FORCE_ERROR     ( REG32( MEM_CFG_BASE + 0xA4UL ) )  // ROM Force Error

#define MEM_SECT_NUM_ALL    (0xFFU)


#endif /* MEM_PRIVATE_H_ */
