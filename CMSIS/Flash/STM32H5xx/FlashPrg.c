/* -----------------------------------------------------------------------------
 * Copyright (c) 2023 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software. Permission is granted to anyone to use this
 * software for any purpose, including commercial applications, and to alter
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *
 * $Date:        02. February 2023
 * $Revision:    V1.0.0
 *
 * Project:      Flash Programming Functions for ST STM32H5xx Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.0.0
 *    Initial release
 */

/* Note:
   Flash has 8K sector size.
   STM32H5xx devices have Dual Bank Flash configuration. */

#include "..\FlashOS.h"        /* FlashOS Structures */

typedef volatile unsigned long    vu32;
typedef          unsigned long     u32;

#define M32(adr) (*((vu32 *) (adr)))

// Peripheral Memory Map
#define FLASH_BASE       (0x40022000)
#define DBGMCU_BASE      (0xE0044000)
#define FLASHSIZE_BASE   (0x08FFF80C)

#define FLASH           ((FLASH_TypeDef  *) FLASH_BASE)
#define DBGMCU          ((DBGMCU_TypeDef *) DBGMCU_BASE)

// Debug MCU
typedef struct {
  vu32 IDCODE;
} DBGMCU_TypeDef;

// Flash Registers
typedef struct
{
  vu32 ACR;             /*!< FLASH access control register,                                     Address offset: 0x00 */
  vu32 NSKEYR;          /*!< FLASH non-secure key register,                                     Address offset: 0x04 */
  vu32 SECKEYR;         /*!< FLASH secure key register,                                         Address offset: 0x08 */
  vu32 OPTKEYR;         /*!< FLASH option key register,                                         Address offset: 0x0C */
  vu32 NSOBKKEYR;       /*!< FLASH non-secure option bytes keys key register,                   Address offset: 0x10 */
  vu32 SECOBKKEYR;      /*!< FLASH secure option bytes keys key register,                       Address offset: 0x14 */
  vu32 OPSR;            /*!< FLASH OPSR register,                                               Address offset: 0x18 */
  vu32 OPTCR;           /*!< Flash Option Control Register,                                     Address offset: 0x1C */
  vu32 NSSR;            /*!< FLASH non-secure status register,                                  Address offset: 0x20 */
  vu32 SECSR;           /*!< FLASH secure status register,                                      Address offset: 0x24 */
  vu32 NSCR;            /*!< FLASH non-secure control register,                                 Address offset: 0x28 */
  vu32 SECCR;           /*!< FLASH secure control register,                                     Address offset: 0x2C */
  vu32 NSCCR;           /*!< FLASH non-secure clear control register,                           Address offset: 0x30 */
  vu32 SECCCR;          /*!< FLASH secure clear control register,                               Address offset: 0x34 */
  vu32 RESERVED1;       /*!< Reserved1,                                                         Address offset: 0x38 */
  vu32 PRIVCFGR;        /*!< FLASH privilege configuration register,                            Address offset: 0x3C */
  vu32 NSOBKCFGR;       /*!< FLASH non-secure option byte key configuration register,           Address offset: 0x40 */
  vu32 SECOBKCFGR;      /*!< FLASH secure option byte key configuration register,               Address offset: 0x44 */
  vu32 HDPEXTR;         /*!< FLASH HDP extension register,                                      Address offset: 0x48 */
  vu32 RESERVED2;       /*!< Reserved2,                                                         Address offset: 0x4C */
  vu32 OPTSR_CUR;       /*!< FLASH option status current register,                              Address offset: 0x50 */
  vu32 OPTSR_PRG;       /*!< FLASH option status to program register,                           Address offset: 0x54 */
  vu32 RESERVED3[2];    /*!< Reserved3,                                                         Address offset: 0x58-0x5C */
  vu32 NSEPOCHR_CUR;    /*!< FLASH non-secure epoch current register,                           Address offset: 0x60 */
  vu32 NSEPOCHR_PRG;    /*!< FLASH non-secure epoch to program register,                        Address offset: 0x64 */
  vu32 SECEPOCHR_CUR;   /*!< FLASH secure epoch current register,                               Address offset: 0x68 */
  vu32 SECEPOCHR_PRG;   /*!< FLASH secure epoch to program register,                            Address offset: 0x6C */
  vu32 OPTSR2_CUR;      /*!< FLASH option status current register 2,                            Address offset: 0x70 */
  vu32 OPTSR2_PRG;      /*!< FLASH option status to program register 2,                         Address offset: 0x74 */
  vu32 RESERVED4[2];    /*!< Reserved4,                                                         Address offset: 0x78-0x7C */
  vu32 NSBOOTR_CUR;     /*!< FLASH non-secure unique boot entry current register,               Address offset: 0x80 */
  vu32 NSBOOTR_PRG;     /*!< FLASH non-secure unique boot entry to program register,            Address offset: 0x84 */
  vu32 SECBOOTR_CUR;    /*!< FLASH secure unique boot entry current register,                   Address offset: 0x88 */
  vu32 SECBOOTR_PRG;    /*!< FLASH secure unique boot entry to program register,                Address offset: 0x8C */
  vu32 OTPBLR_CUR;      /*!< FLASH OTP block lock current register,                             Address offset: 0x90 */
  vu32 OTPBLR_PRG;      /*!< FLASH OTP block Lock to program register,                          Address offset: 0x94 */
  vu32 WRP12R_CUR;      /*!< FLASH write sector group 1 protection for Bank2 current register    Address offset: 0x98 */
  vu32 WRP12R_PRG;      /*!< FLASH write sector group 1 protection for Bank2 to program register Address offset: 0x9C */
  vu32 SECBB1R1;        /*!< FLASH secure block-based bank 1 register 1,                        Address offset: 0xA0 */
  vu32 SECBB1R2;        /*!< FLASH secure block-based bank 1 register 2,                        Address offset: 0xA4 */
  vu32 SECBB1R3;        /*!< FLASH secure block-based bank 1 register 3,                        Address offset: 0xA8 */
  vu32 SECBB1R4;        /*!< FLASH secure block-based bank 1 register 4,                        Address offset: 0xAC */
  vu32 SECBB1R5;        /*!< FLASH secure block-based bank 1 register 5,                        Address offset: 0xB0 */
  vu32 SECBB1R6;        /*!< FLASH secure block-based bank 1 register 6,                        Address offset: 0xB4 */
  vu32 SECBB1R7;        /*!< FLASH secure block-based bank 1 register 7,                        Address offset: 0xB8 */
  vu32 SECBB1R8;        /*!< FLASH secure block-based bank 1 register 8,                        Address offset: 0xBC */
  vu32 PRIVBB1R1;       /*!< FLASH privilege block-based bank 1 register 1,                     Address offset: 0xC0 */
  vu32 PRIVBB1R2;       /*!< FLASH privilege block-based bank 1 register 2,                     Address offset: 0xC4 */
  vu32 PRIVBB1R3;       /*!< FLASH privilege block-based bank 1 register 3,                     Address offset: 0xC8 */
  vu32 PRIVBB1R4;       /*!< FLASH privilege block-based bank 1 register 4,                     Address offset: 0xCC */
  vu32 PRIVBB1R5;       /*!< FLASH privilege block-based bank 1 register 5,                     Address offset: 0xD0 */
  vu32 PRIVBB1R6;       /*!< FLASH privilege block-based bank 1 register 6,                     Address offset: 0xD4 */
  vu32 PRIVBB1R7;       /*!< FLASH privilege block-based bank 1 register 7,                     Address offset: 0xD8 */
  vu32 PRIVBB1R8;       /*!< FLASH privilege block-based bank 1 register 8,                     Address offset: 0xDC */
  vu32 SECWM1R_CUR;     /*!< FLASH secure watermark 1 current register,                         Address offset: 0xE0 */
  vu32 SECWM1R_PRG;     /*!< FLASH secure watermark 1 to program register,                      Address offset: 0xE4 */
  vu32 WRP11R_CUR;      /*!< FLASH write sector group protection current register for bank1,    Address offset: 0xE8 */
  vu32 WRP11R_PRG;      /*!< FLASH write sector group protection to program register for bank1, Address offset: 0xEC */
  vu32 EDATA1R_CUR;     /*!< FLASH data sectors configuration current register for bank1,       Address offset: 0xF0 */
  vu32 EDATA1R_PRG;     /*!< FLASH data sectors configuration to program register for bank1,    Address offset: 0xF4 */
  vu32 HDP1R_CUR;       /*!< FLASH HDP configuration current register for bank1,                Address offset: 0xF8 */
  vu32 HDP1R_PRG;       /*!< FLASH HDP configuration to program register for bank1,             Address offset: 0xFC */
  vu32 ECCCORR;         /*!< FLASH ECC correction register,                                     Address offset: 0x100 */
  vu32 ECCDETR;         /*!< FLASH ECC detection register,                                      Address offset: 0x104 */
  vu32 ECCDR;           /*!< FLASH ECC data register,                                           Address offset: 0x108 */
  vu32 RESERVED8[35];   /*!< Reserved8,                                                         Address offset: 0x10C-0x194 */
  vu32 WRP22R_CUR;      /*!< FLASH write sector group 2 protection for Bank2 current register    Address offset: 0x198 */
  vu32 WRP22R_PRG;      /*!< FLASH write sector group 2 protection for Bank2 to program register Address offset: 0x19C */
  vu32 SECBB2R1;        /*!< FLASH secure block-based bank 2 register 1,                        Address offset: 0x1A0 */
  vu32 SECBB2R2;        /*!< FLASH secure block-based bank 2 register 2,                        Address offset: 0x1A4 */
  vu32 SECBB2R3;        /*!< FLASH secure block-based bank 2 register 3,                        Address offset: 0x1A8 */
  vu32 SECBB2R4;        /*!< FLASH secure block-based bank 2 register 4,                        Address offset: 0x1AC */
  vu32 SECBB2R5;        /*!< FLASH secure block-based bank 2 register 5,                        Address offset: 0x1B0 */
  vu32 SECBB2R6;        /*!< FLASH secure block-based bank 2 register 6,                        Address offset: 0x1B4 */
  vu32 SECBB2R7;        /*!< FLASH secure block-based bank 2 register 7,                        Address offset: 0x1B8 */
  vu32 SECBB2R8;        /*!< FLASH secure block-based bank 2 register 8,                        Address offset: 0x1BC */
  vu32 PRIVBB2R1;       /*!< FLASH privilege block-based bank 2 register 1,                     Address offset: 0x1C0 */
  vu32 PRIVBB2R2;       /*!< FLASH privilege block-based bank 2 register 2,                     Address offset: 0x1C4 */
  vu32 PRIVBB2R3;       /*!< FLASH privilege block-based bank 2 register 3,                     Address offset: 0x1C8 */
  vu32 PRIVBB2R4;       /*!< FLASH privilege block-based bank 2 register 4,                     Address offset: 0x1CC */
  vu32 PRIVBB2R5;       /*!< FLASH privilege block-based bank 2 register 5,                     Address offset: 0x1D0 */
  vu32 PRIVBB2R6;       /*!< FLASH privilege block-based bank 2 register 6,                     Address offset: 0x1D4 */
  vu32 PRIVBB2R7;       /*!< FLASH privilege block-based bank 2 register 7,                     Address offset: 0x1D8 */
  vu32 PRIVBB2R8;       /*!< FLASH privilege block-based bank 2 register 8,                     Address offset: 0x1DC */
  vu32 SECWM2R_CUR;     /*!< FLASH secure watermark 2 current register,                         Address offset: 0x1E0 */
  vu32 SECWM2R_PRG;     /*!< FLASH secure watermark 2 to program register,                      Address offset: 0x1E4 */
  vu32 WRP21R_CUR;       /*!< FLASH write sector group protection current register for bank2,    Address offset: 0x1E8 */
  vu32 WRP21R_PRG;       /*!< FLASH write sector group protection to program register for bank2, Address offset: 0x1EC */
  vu32 EDATA2R_CUR;     /*!< FLASH data sectors configuration current register for bank2,       Address offset: 0x1F0 */
  vu32 EDATA2R_PRG;     /*!< FLASH data sectors configuration to program register for bank2,    Address offset: 0x1F4 */
  vu32 HDP2R_CUR;       /*!< FLASH HDP configuration current register for bank2,                Address offset: 0x1F8 */
  vu32 HDP2R_PRG;       /*!< FLASH HDP configuration to program register for bank2,             Address offset: 0x1FC */
} FLASH_TypeDef;
// Flash Keys
#define FLASH_KEY1               0x45670123
#define FLASH_KEY2               0xCDEF89AB
#define FLASH_OPTKEY1            0x08192A3B
#define FLASH_OPTKEY2            0x4C5D6E7F

// Flash Control Register definitions
#define FLASH_CR_LOCK           ((u32)(  1U      ))
#define FLASH_CR_PG             ((u32)(  1U <<  1))
#define FLASH_CR_SER            ((u32)(  1U <<  2))
#define FLASH_CR_BER            ((u32)(  1U <<  3))
#define FLASH_CR_FW             ((u32)(  1U <<  4))
#define FLASH_CR_STRT           ((u32)(  1U <<  5))
#define FLASH_CR_PNB_MSK        ((u32)(0x7F <<  6))
#define FLASH_CR_MER            ((u32)(  1U << 15))
#define FLASH_CR_BKSEL          ((u32)(  1U << 31))


// Flash Status Register definitions
#define FLASH_SR_BSY            ((u32)(  1U      ))
#define FLASH_SR_WBNE           ((u32)(  1U <<  1))
#define FLASH_SR_DBNE           ((u32)(  1U <<  3))
#define FLASH_SR_EOP            ((u32)(  1U << 16))
#define FLASH_SR_WRPERR         ((u32)(  1U << 17))
#define FLASH_SR_PGSERR         ((u32)(  1U << 18))
#define FLASH_SR_STRBERR        ((u32)(  1U << 19))
#define FLASH_SR_INCERR         ((u32)(  1U << 20))
#define FLASH_SR_OBKERR         ((u32)(  1U << 21))
#define FLASH_SR_OBKWERR        ((u32)(  1U << 22))
#define FLASH_SR_OPTCHANGEERR   ((u32)(  1U << 23))

// Flash option register definitions
#define FLASH_OPTR_RDP          ((u32)(0xFF      ))
#define FLASH_OPTR_RDP_NO       ((u32)(0xAA      ))
#define FLASH_OPTR_TZEN         ((u32)(0xFF000000))


#define FLASH_PGERR             (FLASH_SR_WRPERR | FLASH_SR_STRBERR | FLASH_SR_PGSERR | \
                                 FLASH_SR_INCERR | FLASH_SR_OBKERR  | FLASH_SR_OBKWERR)

#if defined FLASH_MEM
static u32 gFlashBase;                  /* Flash base address */
static u32 gFlashSize;                  /* Flash size in bytes */

static vu32 *pFlashCR;                  /* Pointer to Flash Control register */
static vu32 *pFlashSR;                  /* Pointer to Flash Status register */
static vu32 *pFlashCCR;                 /* Pointer to Flash Clear Control register */
static vu32 *pFlashkey;
#endif /* FLASH_MEM */

static void DSB(void)
{
  __asm ("dsb");
}

static void NOP(void)
{
  __asm ("nop");
}

// Helper macro: set N block-based secure registers of a bank to value
#define SET_FLASH_SECBB(bank, count, value)     \
    do {                                        \
        if ((count) >= 1U) FLASH->SECBB##bank##R1 = (value); \
        if ((count) >= 2U) FLASH->SECBB##bank##R2 = (value); \
        if ((count) >= 3U) FLASH->SECBB##bank##R3 = (value); \
        if ((count) >= 4U) FLASH->SECBB##bank##R4 = (value); \
        if ((count) >= 5U) FLASH->SECBB##bank##R5 = (value); \
        if ((count) >= 6U) FLASH->SECBB##bank##R6 = (value); \
        if ((count) >= 7U) FLASH->SECBB##bank##R7 = (value); \
        if ((count) >= 8U) FLASH->SECBB##bank##R8 = (value); \
    } while (0)
		
#if    defined(STM32H5xx_512_0x08)  || defined(STM32H5xx_512_0x0C)  || \
       defined(STM32H5xx_256_0x08)  || defined(STM32H5xx_256_0x0C)

  #define SECBB_REG_COUNT   1U    /* 256K / 512K */

#elif  defined(STM32H5xx_1024_0x08) || defined(STM32H5xx_1024_0x0C)

  #define SECBB_REG_COUNT   2U    /* 1M */

#elif  defined(STM32H5xx_2048_0x08) || defined(STM32H5xx_2048_0x0C)

  #define SECBB_REG_COUNT   4U    /* 2M */

#elif  defined(STM32H5xx_3072_0x08) || defined(STM32H5xx_3072_0x0C)

  #define SECBB_REG_COUNT   6U    /* 3M */

#elif  defined(STM32H5xx_4096_0x08) || defined(STM32H5xx_4096_0x0C)

  #define SECBB_REG_COUNT   8U    /* 4M */

#else
  #error "Unsupported STM32H5xx flash size define"
#endif

#define sec_value 0xFFFFFFFFU


static void __disable_irq(void)
{
  __asm volatile ("cpsid i" : : : "memory");
}


/*
 * Get Flash security Mode
 *    Return Value:   0 = non-secure Flash
 *                    1 = secure Flash
 */

static u32 GetFlashSecureMode (void) {
  u32 flashSecureMode;

  flashSecureMode = ((FLASH->OPTSR2_PRG & FLASH_OPTR_TZEN)==0xB4000000) ? 1U : 0U;

  return (flashSecureMode);
}


/*
 * Get Flash Type
 *    Return Value:   0 = Single-Bank flash
 *                    1 = Dual-Bank Flash (configurable)
 */

#if defined FLASH_MEM
static u32 GetFlashType (void) {
//  u32 flashType = 0U;
//
//  switch ((DBGMCU->IDCODE & 0xFFFU))
//  {
//    case 0x474:             /* Flash Category ? devices, 8k sectors (STM32H503) */
//    case 0x484:             /* Flash Category ? devices, 8k sectors (STM32H563/H573 and STM32H562) */
//    default:                /* devices have a dual bank flash */
//      flashType = 1U;       /* Dual-Bank Flash type */
//    break;
//  }
//
//  return (flashType);
  return (1U);              /* always Dual-Bank Flash */
}
#endif /* FLASH_MEM */


/*
 * Get Flash Bank Mode
 *    Return Value:   0 = Single-Bank mode
 *                    1 = Dual-Bank mode
 */

#if defined FLASH_MEM
static u32 GetFlashBankMode (void) {
//  u32 flashBankMode;
//
//  flashBankMode = (xxxx) ? 1U : 0U;
//
//  return (flashBankMode);
  return (1U);              /* always Dual-Bank mode */
}
#endif /* FLASH_MEM */


/*
 * Get Flash Bank Number
 *    Parameter:      adr:  Sector Address
 *    Return Value:   Bank Number (0..1)
 *                    Flash bank size is always the half of the Flash size
 */

#if defined FLASH_MEM
static u32 GetFlashBankNum(u32 adr) {
  u32 flashBankNum;

  if (GetFlashType() == 1U)
  {
    /* Dual-Bank Flash */
    if (GetFlashBankMode() == 1U)
    {
      /* Dual-Bank Flash configured as Dual-Bank */
      if (adr >= (gFlashBase + (gFlashSize >> 1)))
      {
        flashBankNum = 1U;
      }
      else
      {
        flashBankNum = 0U;
      }
    }
    else
    {
      /* Dual-Bank Flash configured as Single-Bank */
      flashBankNum = 0U;
    }
  }
  else
  {
    /* Single-Bank Flash */
    flashBankNum = 0U;
  }

  return (flashBankNum);
}
#endif /* FLASH_MEM */


/*
 * Get Flash Page Number
 *    Parameter:      adr:  Page Address
 *    Return Value:   Page Number (0..127)
 */

#if defined FLASH_MEM
static u32 GetFlashPageNum (unsigned long adr) {
  u32 flashPageNum;

  if (GetFlashType() == 1U)
  {
    /* Dual-Bank Flash */
    if (GetFlashBankMode() == 1U)
    {
      /* Dual-Bank Flash configured as Dual-Bank */
      flashPageNum = (((adr & ((gFlashSize >> 1) - 1U)) ) >> 13); /* 8K sector size */
    }
    else
    {
      /* Dual-Bank Flash configured as Single-Bank */
      flashPageNum = (((adr & (gFlashSize        - 1U)) ) >> 13); /* 8K sector size */
    }
  }
  else
  {
      /* Single-Bank Flash */
      flashPageNum = (((adr & (gFlashSize        - 1U)) ) >> 13); /* 8K sector size */
  }

  return (flashPageNum);
}
#endif /* FLASH_MEM */


/*
 * Get Flash Page Size
 *    Return Value:   flash page size (in Bytes)
 */

#if 0 //defined FLASH_MEM
static u32 GetFlashPageSize (void)
{
  u32 flashPageSize;

  if (GetFlashType() == 1U)
  {
    /* Dual-Bank Flash */
    if (GetFlashBankMode() == 1U)
    {
      /* Dual-Bank Flash configured as Dual-Bank */
      flashPageSize = 0x2000;                            /* 8K sector size */
    }
    else
    {
      /* Dual-Bank Flash configured as Single-Bank */
      flashPageSize = 0x2000;                            /* 8K sector size */
    }
  }
  else
  {
    /* Single-Bank Flash */
      flashPageSize = 0x2000;                            /* 8K sector size */
  }

  return (flashPageSize);
}
#endif /* FLASH_MEM */


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc)
{
  (void)clk;
  (void)fnc;

  __disable_irq();

#if defined FLASH_MEM
  if (GetFlashSecureMode() == 0U)
  {                                                      /* Flash non-secure */
    /* set used Control, Status register */
    pFlashCR  = &FLASH->NSCR;
    pFlashSR  = &FLASH->NSSR;
    pFlashCCR = &FLASH->NSCCR;
		pFlashkey = &FLASH->NSKEYR;
		
		if(((*pFlashCR) & FLASH_CR_LOCK) !=0){
    /* unlock FLASH_NSCR */
    *pFlashkey = FLASH_KEY1;
    *pFlashkey = FLASH_KEY2;
		}
    DSB();
    while (FLASH->NSSR & FLASH_SR_BSY) NOP();            /* Wait until operation is finished */
  }
  else
  {    
			
		/* Flash secure */
    /* set used Control, Status register */
    pFlashCR  = &FLASH->SECCR;
    pFlashSR  = &FLASH->SECSR;
    pFlashCCR = &FLASH->SECCCR;
		pFlashkey = &FLASH->SECKEYR;
		
    /* unlock FLASH_SECCR */
		if(((*pFlashCR) & FLASH_CR_LOCK) !=0){
    /* unlock FLASH_NSCR */
    *pFlashkey = FLASH_KEY1;
    *pFlashkey = FLASH_KEY2;
		}
    DSB();
    while (FLASH->SECSR & FLASH_SR_BSY) NOP();           /* Wait until operation is finished */

   // Bank1
    SET_FLASH_SECBB(1, SECBB_REG_COUNT, sec_value);

    // Bank2
    SET_FLASH_SECBB(2, SECBB_REG_COUNT, sec_value);
		
			/*set SAU for device info region*/
	  	M32(0xE000EDD8)=0x0;
			M32(0xE000EDDC)=0x08FFF800;
			M32(0xE000EDE0)=0x08FFF8E1;
			M32(0xE000EDD0)=0x1;
  }

  while (*pFlashSR & FLASH_SR_BSY) NOP();                /* Wait until operation is finished */

	#if defined(STM32H5xx_4096_0x08) || defined(STM32H5xx_4096_0x0C) 
		gFlashSize=0x400000;
		gFlashBase = adr;
	#else
  gFlashBase = adr;
  gFlashSize = (M32(FLASHSIZE_BASE) & 0x0000FFFF) << 10;
#endif
#endif /* FLASH_MEM */

#if defined FLASH_OPT
  (void)adr;

  /* not yet coded */
#endif /* FLASH_OPT */

  return (0);
}


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc)
{
  (void)fnc;

#if defined FLASH_MEM
  /* Lock Flash operation */
  *pFlashCR = FLASH_CR_LOCK;
  DSB();
  while (*pFlashSR & FLASH_SR_BSY) NOP();                /* Wait until operation is finished */
#endif /* FLASH_MEM */

#if defined FLASH_OPT
  /* not yet coded */
#endif /* FLASH_OPT */

  return (0);
}


/*
 *  Blank Check Checks if Memory is Blank
 *    Parameter:      adr:  Block Start Address
 *                    sz:   Block Size (in bytes)
 *                    pat:  Block Pattern
 *    Return Value:   0 - OK,  1 - Failed
 */

int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat)
{
  (void)adr;
  (void)sz;
  (void)pat;

#if defined FLASH_MEM
  /* force erase even if the content is 'Initial Content of Erased Memory'.
     Only a erased sector can be programmed. I think this is because of ECC */
  return (1);
#endif /* FLASH_MEM */

#if defined FLASH_OPT
  /* For OPT algorithm Flash is always erased */
  return (0);
#endif /* FLASH_OPT */
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM
int EraseChip (void)
{
  *pFlashCCR = FLASH_PGERR;                              /* Reset Error Flags */

  *pFlashCR  = FLASH_CR_MER;                             /* Bank A/B mass erase enabled */
  *pFlashCR |= FLASH_CR_STRT;                            /* Start erase */
  DSB();

  while (*pFlashSR & FLASH_SR_BSY) NOP();                /* Wait until operation is finished */

  return (0);                                            /* Done */
}
#endif /* FLASH_MEM */

#ifdef FLASH_OPT
int EraseChip (void) {

  /* not yet coded */
  return (0);                                            /* Done */
}
#endif /* FLASH_OPT */


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM
int EraseSector (unsigned long adr)
{
  u32 b, p;

  b = GetFlashBankNum(adr);                              /* Get Bank Number 0..1  */
  p = GetFlashPageNum(adr);                              /* Get Page Number 0..127 */

  while (*pFlashSR & FLASH_SR_BSY) NOP();                /* Wait until operation is finished */

  *pFlashCCR  = FLASH_PGERR;                             /* Reset Error Flags */

  *pFlashCR  = (FLASH_CR_SER |                           /* Page Erase Enabled */
                 (p <<  6)   |                           /* page Number. 0 to 127 for each bank */
                 (b << 31)    );                         /* bank Number. 0 to 1 */
  *pFlashCR |=  FLASH_CR_STRT;                           /* Start Erase */
  DSB();

  while (*pFlashSR & FLASH_SR_BSY) NOP();                /* Wait until operation is finished */

  if (*pFlashSR & FLASH_PGERR) {                         /* Check for Error */
    *pFlashCCR  = FLASH_PGERR;                           /* Reset Error Flags */
    return (1);                                          /* Failed */
  }

  return (0);                                            /* Done */
}
#endif /* FLASH_MEM */

#if defined FLASH_OPT
int EraseSector (unsigned long adr) {
  /* erase sector is not needed for Flash Option bytes */

  (void)adr;

  return (0);                                              /* Done */
}
#endif /* FLASH_OPT */


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf)
{

  sz = (sz + 15) & ~15U;                                 /* Adjust size for four words */

  while (*pFlashSR & FLASH_SR_BSY) NOP();                /* Wait until operation is finished */

  *pFlashCCR = FLASH_PGERR;                              /* Reset Error Flags */

  *pFlashCR = FLASH_CR_PG ;	                             /* Programming Enabled */

  while (sz)
  {
//    M32(adr    ) = *((u32 *)(buf + 0));                  /* Program the 1st word of the quad-word */
//    M32(adr + 4) = *((u32 *)(buf + 4));                  /* Program the 2nd word of the quad-word */
//    M32(adr + 8) = *((u32 *)(buf + 8));                  /* Program the 3rd word of the quad-word */
//    M32(adr +12) = *((u32 *)(buf +12));                  /* Program the 4th word of the quad-word */

    M32(adr    ) = (u32)((*(buf+ 0)      ) |
                         (*(buf+ 1) <<  8) |
                         (*(buf+ 2) << 16) |
                         (*(buf+ 3) << 24) );            /* Program the 1st word of the quad-word */
    M32(adr + 4) = (u32)((*(buf+ 4)      ) |
                         (*(buf+ 5) <<  8) |
                         (*(buf+ 6) << 16) |
                         (*(buf+ 7) << 24) );            /* Program the 2nd word of the quad-word */
    M32(adr + 8) = (u32)((*(buf+ 8)      ) |
                         (*(buf+ 9) <<  8) |
                         (*(buf+10) << 16) |
                         (*(buf+11) << 24) );            /* Program the 3rd word of the quad-word */
    M32(adr +12) = (u32)((*(buf+12)      ) |
                         (*(buf+13) <<  8) |
                         (*(buf+14) << 16) |
                         (*(buf+15) << 24) );            /* Program the 4th word of the quad-word */
    DSB();

    while (*pFlashSR & FLASH_SR_BSY) NOP();              /* Wait until operation is finished */

    if (*pFlashSR & FLASH_PGERR) {                       /* Check for Error */
      *pFlashCCR  = FLASH_PGERR;                         /* Reset Error Flags */
      return (1);                                        /* Failed */
    }

    adr += 16;                                           /* Next quad-word */
    buf += 16;
    sz  -= 16;
  }

  *pFlashCR = 0U;                                       /* Reset CR */

  return (0);
}
#endif /* FLASH_MEM */


#ifdef FLASH_OPT
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf)
{
  (void)adr;
  (void)sz;

  /* not yet coded */
  return (0);                                            /* Done */
}
#endif /* FLASH_OPT */


/*
 *  Verify Flash Contents
 *    Parameter:      adr:  Start Address
 *                    sz:   Size (in bytes)
 *                    buf:  Data
 *    Return Value:   (adr+sz) - OK, Failed Address
 */

#ifdef FLASH_OPT
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf)
{
  (void)adr;
  (void)sz;

  /* not yet coded */
  return (adr + sz);
}
#endif /* FLASH_OPT */
