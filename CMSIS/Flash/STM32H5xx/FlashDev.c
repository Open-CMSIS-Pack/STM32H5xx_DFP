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
 * Project:      Flash Device Description for ST STM32H5xx Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.0.0
 *    Initial release
 */

#include "..\FlashOS.h"                /* FlashOS Structures */


#ifdef FLASH_MEM

#ifdef STM32H5xx_2048_0x08
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32H5xx 2M NSecure Flash",      /* Device Name */
    ONCHIP,                            /* Device Type */
    0x08000000,                        /* Device Start Address */
    0x00200000,                        /* Device Size in Bytes (2048 kB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size 8kB (256 sectors) */
    SECTOR_END
  };
#endif

#ifdef STM32H5xx_2048_0x0C
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32H5xx 2M Secure Flash",       /* Device Name */
    ONCHIP,                            /* Device Type */
    0x0C000000,                        /* Device Start Address */
    0x00200000,                        /* Device Size in Bytes (2048 kB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size 8kB (256 sectors) */
    SECTOR_END
  };
#endif

#ifdef STM32H503_128K_0x08
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32H503 128k NSecure Flash",    /* Device Name */
    ONCHIP,                            /* Device Type */
    0x08000000,                        /* Device Start Address */
    0x00020000,                        /* Device Size in Bytes ( 128 kB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size 8kB ( 16 sectors) */
    SECTOR_END
  };
#endif
#ifdef STM32H5xx_1024_0x08
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32H5xx 1M NSecure Flash",      /* Device Name */
    ONCHIP,                            /* Device Type */
    0x08000000,                        /* Device Start Address */
    0x00100000,                        /* Device Size in Bytes (2048 kB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size 8kB (256 sectors) */
    SECTOR_END
  };
#endif

#ifdef STM32H5xx_1024_0x0C
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32H5xx 1M Secure Flash",       /* Device Name */
    ONCHIP,                            /* Device Type */
    0x0C000000,                        /* Device Start Address */
    0x00100000,                        /* Device Size in Bytes (2048 kB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size 8kB (256 sectors) */
    SECTOR_END
  };
#endif
#ifdef STM32H5xx_512_0x08
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32H5xx 512K NSecure Flash",      /* Device Name */
    ONCHIP,                            /* Device Type */
    0x08000000,                        /* Device Start Address */
    0x00080000,                        /* Device Size in Bytes (2048 kB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size 8kB (256 sectors) */
    SECTOR_END
  };
#endif

#ifdef STM32H5xx_512_0x0C
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32H5xx 512K Secure Flash",       /* Device Name */
    ONCHIP,                            /* Device Type */
    0x0C000000,                        /* Device Start Address */
    0x00080000,                        /* Device Size in Bytes (2048 kB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size 8kB (256 sectors) */
    SECTOR_END
  };
#endif
#ifdef STM32H5xx_256_0x08
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32H5xx 256K NSecure Flash",      /* Device Name */
    ONCHIP,                            /* Device Type */
    0x08000000,                        /* Device Start Address */
    0x00040000,                        /* Device Size in Bytes (2048 kB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size 8kB (256 sectors) */
    SECTOR_END
  };
#endif

#ifdef STM32H5xx_256_0x0C
  struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                    /* Driver Version, do not modify! */
    "STM32H5xx 256K Secure Flash",       /* Device Name */
    ONCHIP,                            /* Device Type */
    0x0C000000,                        /* Device Start Address */
    0x00040000,                        /* Device Size in Bytes (2048 kB) */
    1024,                              /* Programming Page Size */
    0,                                 /* Reserved, must be 0 */
    0xFF,                              /* Initial Content of Erased Memory */
    400,                               /* Program Page Timeout 400 mSec */
    400,                               /* Erase Sector Timeout 400 mSec */
    /* Specify Size and Address of Sectors */
    0x2000, 0x000000,                  /* Sector Size 8kB (256 sectors) */
    SECTOR_END
  };
#endif
#endif /* FLASH_MEM */
