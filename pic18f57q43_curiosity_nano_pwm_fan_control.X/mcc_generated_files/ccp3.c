/**
  CCP3 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    ccp3.c

  @Summary
    This is the generated driver implementation file for the CCP3 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides implementations for driver APIs for CCP3.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.76
        Device            :  PIC18F57Q43 (Curiosity Nano)
        Driver Version    :  2.1.3
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.00 and above
         MPLAB 	          :  MPLAB X 5.10
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
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

/**
  Section: Included Files
*/

#include <xc.h>
#include "ccp3.h"

#define TIMER_CONST    3750000                                                  // 2 pulses per rev (Freq / TimerPrescaler) * (60 / pulses per rev) = (1 MHz / 8) * (60 / 2)

void CCP3_Initialize(void)
{	 
	CCP3CON = 0x85;                                                             // MODE every rising edge  	 
	CCP3CAP = 0x00;                                                             // CCP3CTS CCP3 pin;   
	CCPR3H = 0x00;    
	CCPR3L = 0x00;    	
	CCPTMRS0bits.C3TSEL = 0x1;                                                  // Selecting Timer 1    
    PIR11bits.CCP3IF = 0;                                                       // Clear the CCP3 interrupt flag    
    PIE11bits.CCP3IE = 1;                                                       // Enable the CCP3 interrupt
}

void CCP3_CaptureISR(void)
{
    CCP3_PERIOD_REG_T module;
       
    module.ccpr3l = CCPR3L;                                                     // Copy captured value.
    module.ccpr3h = CCPR3H;
    
    prevCapt3 = currentCapt3;
    currentCapt3 = module.ccpr3_16Bit;
    
    if(currentCapt3 > prevCapt3)
    {
        fan3_tmr_val = currentCapt3 - prevCapt3;
    }
    else
    {
        fan3_tmr_val = (0xFFFF - prevCapt3) + currentCapt3;
    }
    
    fan3_spd = TIMER_CONST / fan3_tmr_val;                                      // Calculate actual fan speed
    
    if(fan3_spd < y_therm)                                                      // Fan 3 too slow?
    {
        PWM2S1P1 += 1;                                                          // Increase PWM duty cycle
        PWM2CONbits.LD = 1;                                                     // Latch new PWM value
    }
    if(fan3_spd > y_therm)                                                      // Fan 3 too fast?
    {
        PWM2S1P1 -= 1;                                                          // Decrease PWM duty cycle
        PWM2CONbits.LD = 1;                                                     // Latch new PWM value
    }    
    PIR11bits.CCP3IF = 0;
}

/**
 End of File
*/