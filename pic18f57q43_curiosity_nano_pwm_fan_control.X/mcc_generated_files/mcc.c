/**
  @Generated PIC10 / PIC12 / PIC16 / PIC18 MCUs Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    mcc.c

  @Summary:
    This is the mcc.c file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.76
        Device            :  PIC18F57Q43 (Curiosity Nano)
        Driver Version    :  2.00
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

#include "mcc.h"

void SYSTEM_Initialize(void)
{
    INTERRUPT_Initialize();
    PMD_Initialize();
    PIN_MANAGER_Initialize();
    OSCILLATOR_Initialize();
    CCP2_Initialize();
    CCP3_Initialize();
    ADCC_Initialize();
    CCP1_Initialize();
    TMR1_Initialize();
    UART1_Initialize();
    PWM1_Initialize();
    PWM2_Initialize();
}

void OSCILLATOR_Initialize(void)
{
     
    OSCCON1 = 0x00;                                                             // NOSC HFINTOSC @ 64 MHz; NDIV 1;  
    OSCCON3 = 0x00;                                                             // CSWHOLD may proceed; SOSCPWR Low power;
    OSCEN = 0x00;     
    OSCFRQ = 0x08;                                                              // HFFRQ 64_MHz; 
    OSCTUNE = 0x00;
}

void PMD_Initialize(void)
{    
    PMD0 = 0x00;
    PMD1 = 0x00;    
    PMD3 = 0x00;    
    PMD4 = 0x00; 
    PMD5 = 0x00;     
    PMD6 = 0x00;     
    PMD7 = 0x00; 
    PMD8 = 0x00;
}

/**
 End of File
*/
