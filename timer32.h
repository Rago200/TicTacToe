/*
 * timer32.h
 *
 *  Created on: Oct 5, 2020
 *      Author: YOUR NAME
 */

#ifndef TIMER32_H_
#define TIMER32_H_

#include "msp.h"
#include <stdbool.h>
#include <stdint.h>

extern volatile bool S2_PRESSED; //Checks if button is pressed
extern volatile bool LCD_UPDATE; //Checks if LCD need to UPdated

/*
 *Timer 32(1) Interupt is called every 10 ms
 */
void timer32_1_interupt();

/*
 * Timer 32(2) Interupt is called every 100 ms
 */
void timer32_2_interupt();

/**
 * Timer32(2) Interrupt Service Handler\
 * Updates LCD screen.
 */
void T32_INT2_IRQHandler();

/*
 * Checks is MKII S2 button is pressed.
 */
bool ece353_MKII_S2();

/**
 * Timer32(1) Interrupt Service Handler
 * Checks if the button has been pressed for 70ms and debounces and starts ADC Conversion
 */
void T32_INT1_IRQHandler();

#endif /* TIMER32_H_ */
