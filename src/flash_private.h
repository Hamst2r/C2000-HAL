#ifndef FLASH_PRIVATE_H_
#define FLASH_PRIVATE_H_

#define FLASH_FRDCNTL       ( REG32( FLASH_BASE + 0x00UL  ) )   // Flash Read Control
#define FLASH_FBAC          ( REG32( FLASH_BASE + 0x1EUL  ) )   // Flash Bank Access Control
#define FLASH_FBFALLBACK    ( REG32( FLASH_BASE + 0x20UL  ) )   // Flash Bank Fallback Power
#define FLASH_FBPRDY        ( REG32( FLASH_BASE + 0x22UL  ) )   // Flash Bank Pump Ready
#define FLASH_FPAC1         ( REG32( FLASH_BASE + 0x24UL  ) )   // Flash Pump Access Control 1
#define FLASH_FPAC2         ( REG32( FLASH_BASE + 0x26UL  ) )   // Flash Pump Access Control 2
#define FLASH_FMSTAT        ( REG32( FLASH_BASE + 0x2AUL  ) )   // Flash Module Status
#define FLASH_FRD_INTF_CTRL ( REG32( FLASH_BASE + 0x180UL ) )   // Flash Read Interface Control

#define FLASH_ECC_ENABLE                ( REG32( FLASH_ECC_BASE + 0x00UL ) )    // ECC Enable
#define FLASH_ECC_SINGLE_ERR_ADDR_LOW   ( REG32( FLASH_ECC_BASE + 0x02UL ) )    // Single Error Address Low
#define FLASH_ECC_SINGLE_ERR_ADDR_HIGH  ( REG32( FLASH_ECC_BASE + 0x04UL ) )    // Single Error Address High
#define FLASH_ECC_UNC_ERR_ADDR_LOW      ( REG32( FLASH_ECC_BASE + 0x06UL ) )    // Uncorrectable Error Address Low
#define FLASH_ECC_UNC_ERR_ADDR_HIGH     ( REG32( FLASH_ECC_BASE + 0x08UL ) )    // Uncorrectable Error Address High
#define FLASH_ECC_ERR_STATUS            ( REG32( FLASH_ECC_BASE + 0x0AUL ) )    // Error Status
#define FLASH_ECC_ERR_POS               ( REG32( FLASH_ECC_BASE + 0x0CUL ) )    // Error Position
#define FLASH_ECC_ERR_STATUS_CLR        ( REG32( FLASH_ECC_BASE + 0x0EUL ) )    // Error Status Clear
#define FLASH_ECC_ERR_CNT               ( REG32( FLASH_ECC_BASE + 0x10UL ) )    // Error Control
#define FLASH_ECC_ERR_THRESHOLD         ( REG32( FLASH_ECC_BASE + 0x12UL ) )    // Error Threshold
#define FLASH_ECC_ERR_INTFLG            ( REG32( FLASH_ECC_BASE + 0x14UL ) )    // Error Interrupt Flag
#define FLASH_ECC_ERR_INTCLR            ( REG32( FLASH_ECC_BASE + 0x16UL ) )    // Error Interrupt Flag Clear
#define FLASH_ECC_FDATAH_TEST           ( REG32( FLASH_ECC_BASE + 0x18UL ) )    // Data High Test
#define FLASH_ECC_FDATAL_TEST           ( REG32( FLASH_ECC_BASE + 0x1AUL ) )    // Data Low Test
#define FLASH_ECC_FADDR_TEST            ( REG32( FLASH_ECC_BASE + 0x1CUL ) )    // ECC Test Address
#define FLASH_ECC_FECC_TEST             ( REG32( FLASH_ECC_BASE + 0x1EUL ) )    // ECC Test Address
#define FLASH_ECC_FECC_CTRL             ( REG32( FLASH_ECC_BASE + 0x20UL ) )    // ECC Control
#define FLASH_ECC_FOUTH_TEST            ( REG32( FLASH_ECC_BASE + 0x22UL ) )    // Test Data Out High
#define FLASH_ECC_FOUTL_TEST            ( REG32( FLASH_ECC_BASE + 0x24UL ) )    // Test Data Out Low
#define FLASH_ECC_STATUS                ( REG32( FLASH_ECC_BASE + 0x26UL ) )    // ECC Status

#define FLASH_M_FRDCNTL_RWAIT       (0x0FUL <<  8)
#define FLASH_M_FBPRDY_PUMPRDY      (0x01UL << 15)
#define FLASH_M_FBFALLBACK(bank)    (0x03UL << ((bank) * 2))
#define FLASH_M_FPAC1_PMPPWR        (0x01UL)
#define FLASH_M_FRD_INTF_CTRL_PF    (0x01UL)
#define FLASH_M_FRD_INTF_CTRL_CACHE (0x02UL)

#define FLASH_ECC_M_ENABLE          (0x0FUL)

#define FLASH_S_FRDCNTL_RWAIT(wait)      ((wait) << 8)
#define FLASH_S_FBFALLBACK(bank,mode)    ((mode) << ((bank) * 2))


#endif /* FLASH_PRIVATE_H_ */
