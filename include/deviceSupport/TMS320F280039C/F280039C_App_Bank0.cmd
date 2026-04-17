/*=============================================================
 *  F280039C - Application image linked for BANK0
 *
 *  Layout:
 *    BANK0  -> application code
 *    BANK1  -> other application slot (reserved, not initialized)
 *    BANK2  -> BootLoader, persistent storage (reserved, not initialized)
 *============================================================*/
 

MEMORY
{
	BEGIN 		: origin = 0x00080000, length = 0x00000002
	BOOT_RSVD	: origin = 0x00000002, length = 0x00000126

	RAM_M0S		: origin = 0x00000128, length = 0x000002D8	/* stack */
	RAM_M1D		: origin = 0x00000400, length = 0x000003F8	/* on-chip RAM block M1 */
	// RAMM1_RSVD	: origin = 0x000007F8, length = 0x00000008 /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

	RAM_LS0_1	: origin = 0x00008000, length = 0x00001000	/* .bss */
	RAM_LS2_3	: origin = 0x00009000, length = 0x00001000	/* CPU/CLA shared data */
	RAM_LS4_7	: origin = 0x0000A000, length = 0x00002000	/* CLA program code and constant data */

	RAM_GS0_1	: origin = 0x0000C000, length = 0x00002000	/* CPU RAM functions */
	RAM_GS2_3	: origin = 0x0000E000, length = 0x00001FF8	/* CPU data */
	// RAMGS3_RSVD	: origin = 0x0000FFF8, length = 0x00000008 /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */


	/* Flash sectors */
	/* BANK 0 */
	FLASH_APP_HEAD	: origin = 0x080002, length = 0x000FFE	/* Image Header : Sector 0    */
	FLASH_APP_FAST	: origin = 0x081000, length = 0x004000	/* Ramfunc Code : Sector 1-4 */
	FLASH_APP_CODE	: origin = 0x085000, length = 0x00A000	/* App Code     : Sector 5-14 */
	FLASH_APP_DATA	: origin = 0x08F000, length = 0x001000	/* Constant data: Sector 15   */

	/* BANK 1 */
	FLASH_ALT_APP	: origin = 0x090000, length = 0x010000	/* Bank 1: Alt FW, 16 sectors */

	/* BANK 2 */
	// FLASH_BANK2_ALL		: origin = 0x0A0000, length = 0x00FFF0	/* Bank 2: BootLoader and Persistent storage, 16 sectors */
	FLASH_BL		: origin = 0x0A0000, length = 0x006000	/* BootLoader		: Sector 0-5 */
	FLASH_CAL		: origin = 0x0A6000, length = 0x001000	/* Calibration		: Sector 6 */
	FLASH_CFG		: origin = 0x0A7000, length = 0x002000	/* Configuration	: Sector 7-8 */
	FLASH_PROF		: origin = 0x0A9000, length = 0x004000	/* Profiles			: Sector 9-12 */

	BOOTROM			: origin = 0x003F8000, length = 0x00007FC0
	SECURE_ROM		: origin = 0x003F2000, length = 0x00006000

	RESET				: origin = 0x003FFFC0, length = 0x00000002

	CLA1_MSGRAMLOW		: origin = 0x001480, length = 0x000080
	CLA1_MSGRAMHIGH		: origin = 0x001500, length = 0x000080
}

SECTIONS
{
	codestart		: > BEGIN,				ALIGN(4)
	.reset			: > RESET,				TYPE = DSECT
	.text			: > FLASH_APP_CODE,		ALIGN(8)
	.cinit			: > FLASH_APP_CODE,		ALIGN(4)
	.switch			: > FLASH_APP_CODE,		ALIGN(4)
	.cio			: > FLASH_APP_CODE
	.pinit			: > FLASH_APP_CODE,		ALIGN(4)
	.const			: > FLASH_APP_CODE,		ALIGN(4)
	.init_array		: > FLASH_APP_CODE,		ALIGN(4)

	.stack			: > RAM_M0S
	.bss			: > RAM_LS0_1
	.bss:output		: > RAM_LS0_1
	.bss:cio		: > RAM_LS0_1
	.data			: > RAM_M1D
	.sysmem			: > RAM_GS2_3

	CPUdata			: > RAM_GS2_3
	CANdata			: > RAM_GS2_3

	/*  Allocate IQ math areas: */
	// IQmath         : > FLASH_APP_CODE,	ALIGN(8)
	// IQmathTables   : > FLASH_APP_CODE,	ALIGN(8)

	GROUP
	{
		.TI.ramfunc
		ramfuncs
		dclfuncs
	}		LOAD = FLASH_APP_FAST
			RUN = RAM_GS0_1,
			LOAD_START(RamfuncsLoadStart),
			LOAD_SIZE(RamfuncsLoadSize),
			LOAD_END(RamfuncsLoadEnd),
			RUN_START(RamfuncsRunStart),
			RUN_SIZE(RamfuncsRunSize),
			RUN_END(RamfuncsRunEnd),
			ALIGN(8)

	/* CLA specific sections */
	.scratchpad		: > RAM_LS0_1
	.bss_cla		: > RAM_LS0_1
	sharedVariables	: > RAM_LS0_1


	.const_cla		: LOAD = FLASH_APP_FAST,
					  RUN = RAM_LS4_7,
					  RUN_START(Cla1ConstRunStart),
					  LOAD_START(Cla1ConstLoadStart),
					  LOAD_SIZE(Cla1ConstLoadSize),
					  ALIGN(4)

	Cla1Prog		: LOAD = FLASH_APP_FAST,
					  RUN = RAM_LS4_7,
					  LOAD_START(Cla1ProgLoadStart),
					  RUN_START(Cla1ProgRunStart),
					  LOAD_SIZE(Cla1ProgLoadSize),
					  ALIGN(4)

   Cla1ToCpuMsgRAM  : > CLA1_MSGRAMLOW
   CpuToCla1MsgRAM  : > CLA1_MSGRAMHIGH

}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
