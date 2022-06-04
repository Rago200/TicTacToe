/*
 * ps2.h
 *
 *  Created on: Oct 5, 2020
 *      Author: Ragotham Senthilkumar
 */

#ifndef PS2_H_
#define PS2_H_

extern volatile uint32_t PS2_X_DIR; //X_position
extern volatile uint32_t PS2_Y_DIR; //Y_Position

/**
 * Configure the IO pins for BOTH the X and Y directions of the analog
 * joystick.  The X direction should be configured to place the results in
 * MEM[0].  The Y direction should be configured to place the results in MEM[1].
 **/
void ece353_ADC14_Config();

/**
 * Interrupt handler used to store the X and Y position.
 */
void ADC14_IRQHandler();

#endif /* PS2_H_ */
