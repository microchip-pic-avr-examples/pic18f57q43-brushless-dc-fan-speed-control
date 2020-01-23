/**
  CCP2 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    ccp2.c

  @Summary
    This is the generated driver implementation file for the CCP2 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides implementations for driver APIs for CCP2.
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
#include "ccp2.h"

#define TIMER_CONST    3750000                                                  // 2 pulses per rev (Freq / TimerPrescaler) * (60 / pulses per rev) = (1 MHz / 8) * (60 / 2)

void CCP2_Initialize(void)
{	  
	CCP2CON = 0x85;                                                             // MODE every rising edge; 	 
	CCP2CAP = 0x00;                                                             // CCP2CTS CCP2 pin;  
	CCPR2H = 0x00;    
	CCPR2L = 0x00;    	
	CCPTMRS0bits.C2TSEL = 0x1;                                                  // Selecting Timer 1    
    PIR8bits.CCP2IF = 0;                                                        // Clear the CCP2 interrupt flag   
    PIE8bits.CCP2IE = 1;                                                        // Enable the CCP2 interrupt
}

void CCP2_CaptureISR(void)
{
    CCP2_PERIOD_REG_T module;
       
    module.ccpr2l = CCPR2L;                                                     // Copy captured value.
    module.ccpr2h = CCPR2H;
    
    prevCapt2 = currentCapt2;
    currentCapt2 = module.ccpr2_16Bit;
    
    if(currentCapt2 > prevCapt2)
    {
        fan2_tmr_val = currentCapt2 - prevCapt2;
    }
    else
    {
        fan2_tmr_val = (0xFFFF - prevCapt2) + currentCapt2;
    }
    
    fan2_spd = TIMER_CONST / fan2_tmr_val;                                      // Calculate actual fan speed
    
    if(fan2_spd < y_therm)                                                      // Fan 2 too slow?
    {
        PWM1S1P2 += 1;                                                          // Increase PWM duty cycle
        PWM1CONbits.LD = 1;                                                     // Latch new PWM value
    }
    if(fan2_spd > y_therm)                                                      // Fan 2 too fast?
    {
        PWM1S1P2 -= 1;                                                          // Decrease PWM duty cycle
        PWM1CONbits.LD = 1;                                                     // Latch new PWM value
    }   
    PIR8bits.CCP2IF = 0;
}

/**
 End of File
*/