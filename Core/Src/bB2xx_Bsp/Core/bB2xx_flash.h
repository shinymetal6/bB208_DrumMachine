/*
 * bB2xx_flash.h
 *
 *  Created on: Oct 29, 2021
 *      Author: fil
 */

#ifndef SRC_BB2XX_BSP_CORE_BB2XX_FLASH_H_
#define SRC_BB2XX_BSP_CORE_BB2XX_FLASH_H_

#include "../Components/w25qxx.h"

#define		PARAM_NUMSECTORS	1
#define		PARAMS_PTR_F128		(F128_SIZE - SECTOR_SIZE)

extern	uint8_t bB2xx_flash_init(void);
extern	void bB2xx_flash_store_sysparams(void);
extern	void bB2xx_flash_get_sysparams(void);

#endif /* SRC_BB2XX_BSP_CORE_BB2XX_FLASH_H_ */
