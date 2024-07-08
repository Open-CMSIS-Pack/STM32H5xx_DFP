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
  vu32 ACR;              /* offset: 0x000 FLASH access control register */
  vu32 NSKEYR;           /* offset: 0x004 FLASH non-secure key register */
  vu32 SECKEYR;          /* offset: 0x008 FLASH secure key register */
  vu32 OPTKEYR;          /* offset: 0x00C FLASH option key register */
  vu32 NSOBKKEYR;        /* offset: 0x010 FLASH non-secure option bytes keys key register */
  vu32 SECOBKKEYR;       /* offset: 0x014 FLASH secure option bytes keys key register */
  vu32 OPSR;             /* offset: 0x018 FLASH OPSR register */
  vu32 OPTCR;            /* offset: 0x01C Flash Option Control Register */
  vu32 NSSR;             /* offset: 0x020 FLASH non-secure status register */
  vu32 SECSR;            /* offset: 0x024 FLASH secure status register */
  vu32 NSCR;             /* offset: 0x028 FLASH non-secure control register */
  vu32 SECCR;            /* offset: 0x02C FLASH secure control register */
  vu32 NSCCR;            /* offset: 0x030 FLASH non-secure clear control register */
  vu32 SECCCR;           /* offset: 0x034 FLASH secure clear control register */
  vu32 RESERVED1;        /* offset: 0x038 */
  vu32 PRIVCFGR;         /* offset: 0x03C FLASH privilege configuration register */
  vu32 NSOBKCFGR;        /* offset: 0x040 FLASH non-secure option byte key configuration register */
  vu32 SECOBKCFGR;       /* offset: 0x044 FLASH secure option byte key configuration register */
  vu32 HDPEXTR;          /* offset: 0x048 FLASH HDP extension register */
  vu32 RESERVED2;        /* offset: 0x04C*/
  vu32 OPTSR_CUR;        /* offset: 0x050 FLASH option status current register,                          */
  vu32 OPTSR_PRG;        /* offset: 0x054 FLASH option status to program register */
  vu32 RESERVED3[2];     /* offset: 0x058-0x05C */
  vu32 NSEPOCHR_CUR;     /* offset: 0x060 FLASH non-secure epoch current register */
  vu32 NSEPOCHR_PRG;     /* offset: 0x064 FLASH non-secure epoch to program register */
  vu32 SECEPOCHR_CUR;    /* offset: 0x068 FLASH secure epoch current register */
  vu32 SECEPOCHR_PRG;    /* offset: 0x06C FLASH secure epoch to program register */
  vu32 OPTSR2_CUR;       /* offset: 0x070 FLASH option status current register 2 */
  vu32 OPTSR2_PRG;       /* offset: 0x074 FLASH option status to program register 2 */
  vu32 RESERVED4[2];     /* offset: 0x078-0x07C */
  vu32 NSBOOTR_CUR;      /* offset: 0x080 FLASH non-secure unique boot entry current register */
  vu32 NSBOOTR_PRG;      /* offset: 0x084 FLASH non-secure unique boot entry to program register */
  vu32 SECBOOTR_CUR;     /* offset: 0x088 FLASH secure unique boot entry current register */
  vu32 SECBOOTR_PRG;     /* offset: 0x08C FLASH secure unique boot entry to program register */
  vu32 OTPBLR_CUR;       /* offset: 0x090 FLASH OTP block lock current register */
  vu32 OTPBLR_PRG;       /* offset: 0x094 FLASH OTP block Lock to program register */
  vu32 RESERVED5[2];     /* offset: 0x098-0x09C */
  vu32 SECBB1R1;         /* offset: 0x0A0 FLASH secure block-based bank 1 register 1 */
  vu32 SECBB1R2;         /* offset: 0x0A4 FLASH secure block-based bank 1 register 2 */
  vu32 SECBB1R3;         /* offset: 0x0A8 FLASH secure block-based bank 1 register 3 */
  vu32 SECBB1R4;         /* offset: 0x0AC FLASH secure block-based bank 1 register 4 */
  vu32 RESERVED6[4];     /* offset: 0x0B0-0x0BC */
  vu32 PRIVBB1R1;        /* offset: 0x0C0 FLASH privilege block-based bank 1 register 1 */
  vu32 PRIVBB1R2;        /* offset: 0x0C4 FLASH privilege block-based bank 1 register 2 */
  vu32 PRIVBB1R3;        /* offset: 0x0C8 FLASH privilege block-based bank 1 register 3 */
  vu32 PRIVBB1R4;        /* offset: 0x0CC FLASH privilege block-based bank 1 register 4 */
  vu32 RESERVED7[4];     /* offset: 0x0D0-0x0DC */
  vu32 SECWM1R_CUR;      /* offset: 0x0E0 FLASH secure watermark 1 current register */
  vu32 SECWM1R_PRG;      /* offset: 0x0E4 FLASH secure watermark 1 to program register */
  vu32 WRP1R_CUR;        /* offset: 0x0E8 FLASH write sector group protection current register for bank1 */
  vu32 WRP1R_PRG;        /* offset: 0x0EC FLASH write sector group protection to program register for bank1 */
  vu32 EDATA1R_CUR;      /* offset: 0x0F0 FLASH data sectors configuration current register for bank1 */
  vu32 EDATA1R_PRG;      /* offset: 0x0F4 FLASH data sectors configuration to program register for bank1 */
  vu32 HDP1R_CUR;        /* offset: 0x0F8 FLASH HDP configuration current register for bank1 */
  vu32 HDP1R_PRG;        /* offset: 0x0FC FLASH HDP configuration to program register for bank1 */
  vu32 ECCCORR;          /* offset: 0x100 FLASH ECC correction register */
  vu32 ECCDETR;          /* offset: 0x104 FLASH ECC detection register */
  vu32 ECCDR;            /* offset: 0x108 FLASH ECC data register */
  vu32 RESERVED8[37];    /* offset: 0x10C-0x19C */
  vu32 SECBB2R1;         /* offset: 0x1A0 FLASH secure block-based bank 2 register 1 */
  vu32 SECBB2R2;         /* offset: 0x1A4 FLASH secure block-based bank 2 register 2 */
  vu32 SECBB2R3;         /* offset: 0x1A8 FLASH secure block-based bank 2 register 3 */
  vu32 SECBB2R4;         /* offset: 0x1AC FLASH secure block-based bank 2 register 4 */
  vu32 RESERVED9[4];     /* offset: 0x1B0-0x1BC */
  vu32 PRIVBB2R1;        /* offset: 0x1C0 FLASH privilege block-based bank 2 register 1 */
  vu32 PRIVBB2R2;        /* offset: 0x1C4 FLASH privilege block-based bank 2 register 2 */
  vu32 PRIVBB2R3;        /* offset: 0x1C8 FLASH privilege block-based bank 2 register 3 */
  vu32 PRIVBB2R4;        /* offset: 0x1CC FLASH privilege block-based bank 2 register 4 */
  vu32 RESERVED10[4];    /* offset: 0x1D0-0x1DC */
  vu32 SECWM2R_CUR;      /* offset: 0x1E0 FLASH secure watermark 2 current register */
  vu32 SECWM2R_PRG;      /* offset: 0x1E4 FLASH secure watermark 2 to program register */
  vu32 WRP2R_CUR;        /* offset: 0x1E8 FLASH write sector group protection current register for bank2 */
  vu32 WRP2R_PRG;        /* offset: 0x1EC FLASH write sector group protection to program register for bank2 */
  vu32 EDATA2R_CUR;      /* offset: 0x1F0 FLASH data sectors configuration current register for bank2 */
  vu32 EDATA2R_PRG;      /* offset: 0x1F4 FLASH data sectors configuration to program register for bank2 */
  vu32 HDP2R_CUR;        /* offset: 0x1F8 FLASH HDP configuration current register for bank2 */
  vu32 HDP2R_PRG;        /* offset: 0x1FC FLASH HDP configuration to program register for bank2 */
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
#endif /* FLASH_MEM */

static void DSB(void)
{
  __asm ("dsb");
}

static void NOP(void)
{
  __asm ("nop");
}

//static void __enable_irq(void)
//{
//  __asm volatile ("cpsie i" : : : "memory");
//}

static void __disable_irq(void)
{
//  __asm volatile ("cpsid i" : : : "memory");
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

    /* unlock FLASH_NSCR */
    FLASH->NSKEYR = FLASH_KEY1;
    FLASH->NSKEYR = FLASH_KEY2;
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

    /* unlock FLASH_SECCR */
    FLASH->SECKEYR = FLASH_KEY1;
    FLASH->SECKEYR = FLASH_KEY2;
    DSB();
    while (FLASH->SECSR & FLASH_SR_BSY) NOP();           /* Wait until operation is finished */

    /* Flash block-based secure bank1 */
    FLASH->SECBB1R1 = 0xFFFFFFFF;
    FLASH->SECBB1R2 = 0xFFFFFFFF;
    FLASH->SECBB1R3 = 0xFFFFFFFF;
    FLASH->SECBB1R4 = 0xFFFFFFFF;

    if (GetFlashBankMode() == 1U)                        /* Flash secure DUAL BANK */
    {
      /* Flash block-based secure bank2 */
      FLASH->SECBB2R1 = 0xFFFFFFFF;
      FLASH->SECBB2R2 = 0xFFFFFFFF;
      FLASH->SECBB2R3 = 0xFFFFFFFF;
      FLASH->SECBB2R4 = 0xFFFFFFFF;
    }
			/*set SAU for device info region*/
	  	M32(0xE000EDD8)=0x0;
			M32(0xE000EDDC)=0x08FFF800;
			M32(0xE000EDE0)=0x08FFF8E1;
			M32(0xE000EDD0)=0x1;
  }

  while (*pFlashSR & FLASH_SR_BSY) NOP();                /* Wait until operation is finished */

  gFlashBase = adr;
  gFlashSize = (M32(FLASHSIZE_BASE) & 0x0000FFFF) << 10;
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
