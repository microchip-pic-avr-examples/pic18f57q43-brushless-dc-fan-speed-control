/**
  CCP1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    ccp1.c

  @Summary
    This is the generated driver implementation file for the CCP1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides implementations for driver APIs for CCP1.
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
#include "ccp1.h"

#define TIMER_CONST    3750000                                                  // 2 pulses per rev (TMR1 Clock / TimerPrescaler) * (60 / pole pairs) = (1 MHz / 8) * (60 / 2)

void CCP1_Initialize(void)
{	 
	CCP1CON = 0x85;                                                             // MODE every rising edge	 
	CCP1CAP = 0x00;                                                             // CCP1CTS CCP1 pin;   
	CCPR1H = 0x00;    
	CCPR1L = 0x00;    	
	CCPTMRS0bits.C1TSEL = 0x1;                                                  // Selecting Timer 1    
    PIR3bits.CCP1IF = 0;                                                        // Clear the CCP1 interrupt flag    
    PIE3bits.CCP1IE = 1;                                                        // Enable the CCP1 interrupt
}

void CCP1_CaptureISR(void)
{
    CCP1_PERIOD_REG_T module;
        
    module.ccpr1l = CCPR1L;                                                     // Copy captured value.
    module.ccpr1h = CCPR1H;
    
    prevCapt1 = currentCapt1;                                                   // Copy last capture value
    currentCapt1 = module.ccpr1_16Bit;                                          // Get new capture value
    
    if(currentCapt1 > prevCapt1)
    {
        fan1_tmr_val = currentCapt1 - prevCapt1;                                // Find tach period
    }
    else                                                                        // Timer1 rolled over
    {
        fan1_tmr_val = (0xFFFF - prevCapt1) + currentCapt1;
    }
    
    fan1_spd = TIMER_CONST / fan1_tmr_val;                                      // Calculate actual fan speed
    
    if(fan1_spd < y_therm)                                                      // Fan 1 too slow?
    {
        PWM1S1P1 += 1;                                                          // Increase PWM duty cycle
        PWM1CONbits.LD = 1;                                                     // Latch new PWM value
    }
    if(fan1_spd > y_therm)                                                      // Fan 1 too fast?
    {
        PWM1S1P1 -= 1;                                                          // Decrease PWM duty cycle
        PWM1CONbits.LD = 1;                                                     // Latch new PWM value
    }    
    PIR3bits.CCP1IF = 0;
}

/**
 End of File
*/