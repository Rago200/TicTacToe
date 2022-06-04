/*
 * timer32.c
 *
 *  Created on: Oct 5, 2020
 *      Author: Ragotham Senthilkumar
 */

#include "timer32.h"

volatile bool S2_PRESSED = false; //Checks if button is pressed
volatile bool LCD_UPDATE = false; //Checsk if LCD need to UPdated

/*
 *Timer 32(1) Interupt is called every 10 ms
 */
void timer32_1_interupt(void){

    //converts 10ms into ticks
    uint32_t ticks = ((SystemCoreClock * 10) / 1000) - 1;

    //Stops the timer
    TIMER32_1->CONTROL = 0;

    //Loading the value of ticks
    TIMER32_1->LOAD = ticks;

    //Enables interupt
    __enable_irq();
    NVIC_EnableIRQ(T32_INT1_IRQn);

    //Enable timer, timer interrupt and 32 bit size and periodic mode.
    TIMER32_1->CONTROL = TIMER32_CONTROL_ENABLE |
                         TIMER32_CONTROL_MODE |
                         TIMER32_CONTROL_SIZE |
                         TIMER32_CONTROL_IE;
}

/*
 * Timer 32(2) Interupt is called every 100 ms
 */
void timer32_2_interupt(void){

    //converts 100ms into ticks
    uint32_t ticks = ((SystemCoreClock * 100) / 1000) - 1;

    //Stops the timer
    TIMER32_2->CONTROL = 0;

    //Loading the value of ticks
    TIMER32_2->LOAD = ticks;

    //Enables interupt
    __enable_irq();
    NVIC_EnableIRQ(T32_INT2_IRQn);

    //Enable timer, timer interrupt and 32 bit size and periodic mode.
    TIMER32_2->CONTROL = TIMER32_CONTROL_ENABLE |
                         TIMER32_CONTROL_MODE |
                         TIMER32_CONTROL_SIZE |
                         TIMER32_CONTROL_IE;
}

/**
 * Timer32(2) Interrupt Service Handler
 * Updates LCD Screen
 */
void T32_INT2_IRQHandler(void)
{
    //Setting the LCD SCreen global variable to ture so it updates
    LCD_UPDATE = true;

    //clears interrupt
    TIMER32_2->INTCLR = 0;
}

/*
 * Checks is MKII S2 button is pressed.
 */
bool ece353_MKII_S2(void){
    //returns true if button is pressed
    if((P3->IN & BIT5) == 0){
        return true;
    }
    //false otherwise
    return false;
}

/**
 * Timer32(1) Interrupt Service Handler
 * Checks if the button has been pressed for 70ms and debounces and starts ADC Conversion.
 */
void T32_INT1_IRQHandler(void)
{
    //Variable to keep track of debouncing
    static uint8_t button_state = 0x00;
    bool released = true;

    if(!released){
        if(!ece353_MKII_S2()){
            released = true;
        }
    }

    if(released){
        button_state = button_state << 1; //shifts the LSB to the left

        if(ece353_MKII_S2()){ //if pressed it sets LSB to 1
            button_state |= 0x01;
        }

        if(button_state == 0x7F){ //Checks if button has been pressed for 70 milli seconds
            S2_PRESSED = true;
            released = false;
        }
    }

    // Start the ADC conversion
    ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;

    // Clear the timer interrupt
    TIMER32_1->INTCLR = 0;
}
