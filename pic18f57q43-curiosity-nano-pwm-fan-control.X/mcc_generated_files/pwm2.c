/**
  PWM2 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    pwm2.c

  @Summary
    This is the generated driver implementation file for the PWM2 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides implementations for driver APIs for PWM2.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.76
        Device            :  PIC18F57Q43 (Curiosity Nano)
        Driver Version    :  1.0.0
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.00 and above or later
        MPLAB             :  MPLAB X 5.10
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

#include <xc.h>
#include "pwm2.h"

void PWM2_Initialize(void)
{
    
    PWM2ERS = 0x00;                                                             // PWMERS External Reset Disabled;      
    PWM2CLK = 0x02;                                                             // PWMCLK FOSC;    
    PWM2LDS = 0x00;                                                             // PWMLDS Autoload disabled;
    PWM2PRL = 0x00;                                                             // 0x0A00 = 25 kHz PWM frequency @ 64 MHz FOSC 
    PWM2PRH = 0x0A;     
    PWM2CPRE = 0x00;                                                            // PWMCPRE No prescale;     
    PWM2PIPOS = 0x00;                                                           // PWMPIPOS No postscale;     
    PWM2GIR = 0x00;                                                             // PWMS1P2IF PWM2 output match did not occur; PWMS1P1IF PWM1 output match did not occur;    
    PWM2GIE = 0x00;                                                             // PWMS1P2IE disabled; PWMS1P1IE disabled;     
    PWM2S1CFG = 0x00;                                                           // PWMPOL2 disabled; PWMPOL1 disabled; PWMPPEN disabled; PWMMODE PWMOUT1 and PWMOUT2 in left aligned mode;      
    PWM2S1P1L = 0x00;                                                           // PWMS1P1L 0;     
    PWM2S1P1H = 0x05;                                                           // PWMS1P1H 05;     
    PWM2S1P2L = 0x00;                                                           // PWMS1P2L 0;    
    PWM2S1P2H = 0x05;                                                           // PWMS1P2H 05;      
    PWM2CON = 0x80;                                                             // PWMEN enabled; PWMLD disabled; PWMERSPOL disabled; PWMERSNOW disabled;
}

/**
 End of File
*/