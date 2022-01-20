/*
 * audio_buffers.h
 *
 *  Created on: Nov 10, 2021
 *      Author: fil
 */

#ifndef SRC_BB2XX_BSP_CORE_AUDIO_BUFFERS_H_
#define SRC_BB2XX_BSP_CORE_AUDIO_BUFFERS_H_


extern	uint16_t	intermediate_dac1[DAC_SIGNAL_LEN/2];
extern	uint16_t	intermediate_dac2[DAC_SIGNAL_LEN/2];
extern	uint16_t	signal_out_dac1[DAC_SIGNAL_LEN];
extern	uint16_t	signal_out_dac2[DAC_SIGNAL_LEN];

extern	int16_t		signal_out_i2s[I2S_SIGNAL_LEN];
extern	int16_t		i2s_buffer[I2S_SIGNAL_LEN];
extern	int16_t		signal_out_spdif[SPDIF_SIGNAL_LEN];
extern	int16_t		spdif_buffer[SPDIF_SIGNAL_LEN];

#endif /* SRC_BB2XX_BSP_CORE_AUDIO_BUFFERS_H_ */
