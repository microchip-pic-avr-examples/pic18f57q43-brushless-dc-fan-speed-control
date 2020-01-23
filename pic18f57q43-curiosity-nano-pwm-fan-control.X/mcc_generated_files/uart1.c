/**
  UART1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    uart1.c

  @Summary
    This is the generated driver implementation file for the UART1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for UART1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.76
        Device            :  PIC18F57Q43 (Curiosity Nano)
        Driver Version    :  2.4.0
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.00 and above
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

/**
  Section: Included Files
*/
#include <xc.h>
#include "uart1.h"

static volatile uart1_status_t uart1RxLastError;

void (*UART1_FramingErrorHandler)(void);
void (*UART1_OverrunErrorHandler)(void);
void (*UART1_ErrorHandler)(void);

void UART1_DefaultFramingErrorHandler(void);
void UART1_DefaultOverrunErrorHandler(void);
void UART1_DefaultErrorHandler(void);

void UART1_Initialize(void)
{     
    U1CON0 = 0xA0;                                                              // BRGS high speed; MODE Asynchronous 8-bit mode; RXEN disabled; TXEN enabled; ABDEN disabled;     
    U1CON1 = 0x80;                                                              // RXBIMD Set RXBKIF on rising RX input; BRKOVR disabled; WUE disabled; SENDB disabled; ON enabled;    
    U1CON2 = 0x00;                                                              // TXPOL not inverted; FLO off; C0EN Checksum Mode 0; RXPOL not inverted; RUNOVF RX input shifter stops all activity; STP Transmit 1Stop bit, receiver verifies first Stop bit; 
    U1BRGL = 0xC7;                                                              // 9600 BR @ 64 MHz FOSC 
    U1BRGH = 0x06;    
    U1FIFO = 0x00;                                                              // STPMD in middle of first Stop bit; TXWRE No error;      
    U1UIR = 0x00;                                                               // Auto-baud not enabled; WUE not enabled by software; ABDIE disabled;
    U1ERRIR = 0x00; 
    U1ERRIE = 0x00;

    UART1_SetFramingErrorHandler(UART1_DefaultFramingErrorHandler);
    UART1_SetOverrunErrorHandler(UART1_DefaultOverrunErrorHandler);
    UART1_SetErrorHandler(UART1_DefaultErrorHandler);

    uart1RxLastError.status = 0;
}

uint8_t UART1_Read(void)
{
    while(!PIR4bits.U1RXIF)
    {
    }
    uart1RxLastError.status = 0;

    if(U1ERRIRbits.FERIF){
        uart1RxLastError.ferr = 1;
        UART1_FramingErrorHandler();
    }

    if(U1ERRIRbits.RXFOIF){
        uart1RxLastError.oerr = 1;
        UART1_OverrunErrorHandler();
    }

    if(uart1RxLastError.status){
        UART1_ErrorHandler();
    }

    return U1RXB;
}

void UART1_Write(uint8_t txData)
{
    while(0 == PIR4bits.U1TXIF)
    {
    }

    U1TXB = txData;                                                             // Write the data byte to the USART.
}

char getch(void)                                                                // Used by 'printf' library
{
    return UART1_Read();
}

void putch(char txData)
{
    UART1_Write(txData);
}

void UART1_DefaultFramingErrorHandler(void){}

void UART1_DefaultOverrunErrorHandler(void){}

void UART1_DefaultErrorHandler(void){
}

void UART1_SetFramingErrorHandler(void (* interruptHandler)(void)){
    UART1_FramingErrorHandler = interruptHandler;
}

void UART1_SetOverrunErrorHandler(void (* interruptHandler)(void)){
    UART1_OverrunErrorHandler = interruptHandler;
}

void UART1_SetErrorHandler(void (* interruptHandler)(void)){
    UART1_ErrorHandler = interruptHandler;
}






/**
  End of File
*/
