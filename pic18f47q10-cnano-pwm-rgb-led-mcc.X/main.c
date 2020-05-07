/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.79.0
        Device            :  PIC18F47Q10
        Driver Version    :  2.00
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"

/*
                         Main application
 */

#define MIN_DCY                 0
#define MAX_DCY                 1023

/* this must be set to 0 for a common-kathode LED */
#define LED_COMMON_ANODE        1

#if LED_COMMON_ANODE
    #define LED_MIN_BRIGHT      (MAX_DCY)
    #define LED_MAX_BRIGHT      (MIN_DCY)
    #define LED_INC_BRIGHT(x)   (MAX_DCY - (x))
    #define LED_DEC_BRIGHT(x)   (MIN_DCY + (x))
#else /* LED_COMMON_ANODE */
    #define LED_MAX_BRIGHT      (MAX_DCY)
    #define LED_MIN_BRIGHT      (MIN_DCY)
    #define LED_DEC_BRIGHT(x)   (MAX_DCY - (x))
    #define LED_INC_BRIGHT(x)   (MIN_DCY + (x))
#endif /* LED_COMMON_ANODE */


void RGB_LED_Handler(void);

void main(void)
{
    /* Initialize the device */
    SYSTEM_Initialize();
    TMR2_SetInterruptHandler(RGB_LED_Handler);

    /* Enable the Global Interrupts */
    INTERRUPT_GlobalInterruptEnable();

    /* Enable the Peripheral Interrupts */
    INTERRUPT_PeripheralInterruptEnable();

    while (1)
    {
        ;
    }
}

void RGB_LED_Handler(void)
{
    /* this implements a cyclic color game on the RGB LED:
     * 
     * step 0: red   decreases from max down to 0, 
     *   while green increases from 0 upto max
     *   while blue  is turned off
     * 
     * step 1: red is turned off
     *   while green decreases from max down to 0
     *   while blue  increases from 0 upto max
     * 
     * step 2: red   increases from 0 upto max
     *   while green is turned off
     *   while blue  decreases down to 0
     * 
     * goto step 0
     */

    static uint16_t dcyR = LED_MIN_BRIGHT;
    static uint16_t dcyG = LED_MIN_BRIGHT;
    static uint16_t dcyB = LED_MIN_BRIGHT;
    static uint16_t counter = MIN_DCY;
    static uint8_t  sector = 0;
    
    PWM1_LoadDutyValue(dcyR);
    PWM2_LoadDutyValue(dcyG);
    PWM3_LoadDutyValue(dcyB);

    switch(sector)
    {
        case 0: dcyR = LED_DEC_BRIGHT(counter);
                dcyG = LED_INC_BRIGHT(counter);
                dcyB = LED_MIN_BRIGHT;
                break;
        case 1: dcyR = LED_MIN_BRIGHT;
                dcyG = LED_DEC_BRIGHT(counter);
                dcyB = LED_INC_BRIGHT(counter);
                break;
        case 2: dcyR = LED_INC_BRIGHT(counter);
                dcyG = LED_MIN_BRIGHT;
                dcyB = LED_DEC_BRIGHT(counter);
                break;
        default:dcyR = LED_MIN_BRIGHT;
                dcyG = LED_MIN_BRIGHT;
                dcyB = LED_MIN_BRIGHT;
                break;
    }
    
    counter++;
    if(counter > MAX_DCY)
    {
        counter = MIN_DCY;
        sector++;
        if(sector >= 3)
            sector = 0;
    }
}

