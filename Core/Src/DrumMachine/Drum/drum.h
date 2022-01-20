/*
 * drum.h
 *
 *  Created on: Dec 16, 2021
 *      Author: fil
 */

#ifndef SRC_DRUMMACHINE_DRUM_DRUM_H_
#define SRC_DRUMMACHINE_DRUM_DRUM_H_

#define EnableDacTim(n)               (n.TIMx->CR1 |= TIM_CR1_CEN)
#define DisableDacTim(n)              (n.TIMx->CR1 &= (~((U16)TIM_CR1_CEN)))

#endif /* SRC_DRUMMACHINE_DRUM_DRUM_H_ */
