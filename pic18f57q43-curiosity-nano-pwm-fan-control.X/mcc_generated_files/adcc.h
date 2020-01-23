/**
  ADCC Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    adcc.h

  @Summary
    This is the generated header file for the ADCC driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs 

  @Description
    This header file provides APIs for driver for ADCC.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.76
        Device            :  PIC18F57Q43
        Driver Version    :  2.1.4
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

#ifndef ADCC_H
#define ADCC_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include "mcc.h"

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif


typedef uint16_t adc_result_t;
#ifndef uint24_t
typedef __uint24 uint24_t;
#endif

/** ADCC Channel Definition

 @Summary
   Defines the channels available for conversion.

 @Description
   This routine defines the channels that are available for the module to use.

 Remarks:
   None
 */

typedef enum
{
    Thermistor =  0x01,
    channel_VSS =  0x3B,
    channel_Temp =  0x3C,
    channel_DAC1 =  0x3D,
    channel_FVR_Buffer1 =  0x3E,
    channel_FVR_Buffer2 =  0x3F
} adcc_channel_t;

float temperature = 0.00000;
uint16_t adc_val = 0;                                                           // Actual ADC conversion value
uint16_t y_therm = 1000;                                                        // Give a starting value
uint8_t therm_short = 0;
uint8_t therm_open = 0;

/**
  Section: ADCC Module APIs
*/

/**
  @Summary
    Initializes the ADCC.

  @Description
    This routine initializes the ADCC and must be called before any other ADCC routine.
    This routine should only be called once during system initialization.

  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Comment
    

  @Example
    <code>
    adc_result_t convertedValue;    

    ADCC_Initialize();
    convertedValue = ADCC_GetSingleConversion(channel_ANA0);
    </code>
*/
void ADCC_Initialize(void);

/**
  @Summary
    Starts A/D conversion on selected analog channel.

  @Description
    This routine is used to trigger A/D conversion on selected analog channel.
    
  @Preconditions
    ADCC_Initialize() function should have been called before calling this function.

  @Returns
    None

  @Param
    channel: Analog channel number on which A/D conversion has to be applied.
             For available analog channels refer adcc_channel_t enum from adcc.h file

  @Example
    <code>
    adc_result_t convertedValue; 

    ADCC_Initialize();   
    ADCC_StartConversion(channel_ANA0);
    while(!ADCC_IsConversionDone());
    convertedValue = ADCC_GetConversionResult();
    </code>
*/
void ADCC_StartConversion(adcc_channel_t channel);

/**
  @Summary
    Returns result of latest A/D conversion.

  @Description
    This routine is used to retrieve the result of latest A/D conversion.
    This routine returns the conversion value only after the conversion is complete.
    

  @Preconditions
    ADCC_Initialize(), ADCC_StartConversion() functions should have been called
    before calling this function.
    Completion status should be checked using ADCC_IsConversionDone() routine.

  @Returns
    Returns the result of A/D conversion.

  @Param
    None

  @Example
    <code>
    adc_result_t convertedValue;

    ADCC_Initialize();    
    ADCC_StartConversion(channel_ANA0);
    while(!ADCC_IsConversionDone());
    convertedValue = ADCC_GetConversionResult();
    </code>
 */
adc_result_t ADCC_GetConversionResult(void);

/**
  @Summary
    Returns the result of A/D conversion for requested analog channel.

  @Description
    This routine is used to retrieve the result of A/D conversion for requested 
    analog channel.

  @Preconditions
    ADCC_Initialize() and ADCC_DisableContinuousConversion() functions should have 
    been called before calling this function.

  @Returns
    Returns the result of A/D conversion.

  @Param
    channel: Analog channel number for which A/D conversion has to be applied
             For available analog channels refer adcc_channel_t enum from adcc.h file

  @Example
    <code>
    adcc_channel_t convertedValue;

    ADCC_Initialize();
    ADCC_DisableContinuousConversion();
    
    convertedValue = ADCC_GetSingleConversion(channel_ANA0);
    </code>
*/
adc_result_t ADCC_GetSingleConversion(adcc_channel_t channel);

/**
  @Summary
   Returns the value of ADCC Filter register.

  @Description
    This routine is used to retrieve value of ADCC Filter register.

  @Preconditions
    ADCC_Initialize() should have been called before calling this function.

  @Returns
    16-bit value obtained from ADFLTRH and ADFLTRL registers.

  @Param
    None.

  @Example
    <code>
    uint16_t filterValue;
    ADCC_Initialize();
    filterValue = ADCC_GetFilterValue();
    </code>
*/
uint16_t ADCC_GetFilterValue(void);

/**
  @Summary
    Implements ISR

  @Description
    This routine is used to set the callback for the ADTI Interrupt.

  @Returns
    None

  @Param
    Callback Function to be called
*/
void ADCC_SetADTIInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Implements ISR

  @Description
    This routine is used to implement the ISR for the interrupt-driven
    implementations.

  @Returns
    None

  @Param
    None
*/
void ADCC_ThresholdISR(void);

/**
  @Summary
    Default ADCC Interrupt Handler

  @Description
    This is the default Interrupt Handler function

  @Preconditions
    Initialize  the ADCC module with interrupt before calling this isr.

  @Param
    None

  @Returns
    None
*/
void ADCC_DefaultInterruptHandler(void);
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif    //ADCC_H
/**
 End of File
*/

