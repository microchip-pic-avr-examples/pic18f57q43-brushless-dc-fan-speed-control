/**
  ADCC Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    adcc.c

  @Summary
    This is the generated driver implementation file for the ADCC driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides implementations for driver APIs for ADCC.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.76
        Device            :  PIC18F57Q43 (Curiosity Nano)
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


#include <xc.h>
#include "adcc.h"

#define MAXADC          4095.000                                                // 12-Bit ADCC
#define K               273.1500                                                // Kelvin constant
#define LN              0.4343                                                  // Log(e)

#define MIN_THERM_ADC   1800                                                    // ADFLTR value at 20C
#define MAX_THERM_ADC   3488                                                    // ADFLTR value at 70C
#define MIN_SPD         1000
#define MAX_SPD         8000
#define SLOPE_M         4.15                                                    //((MAX_SPD - MIN_SPD) / (MAX_THERM_ADC - MIN_THERM_ADC))
#define B_OFFSET        -6475                                                   //(MAX_SPD - (SLOPE_M * MAX_THERM_ADC))



void (*ADCC_ADTI_InterruptHandler)(void);

void ADCC_Initialize(void)
{
    ADLTHL = 0x32;                                                              // Lower threshold set at 50 (0x0032)
    ADLTHH = 0x00; 
    ADUTHL = 0xA0;                                                              // Upper threshold set at 4000 (0x0FA0)
    ADUTHH = 0x0F;
    ADSTPTL = 0x00;                                                             // Setpoint set to 0
    ADSTPTH = 0x00;
    ADACCU = 0x00;   
    ADRPT = 0x20;                                                               // Set for 32 samples
    ADPCH = 0x00;    
    ADCON1 = 0x00;                                                              // DSEN disabled;     
    ADCON2 = 0x53;                                                              // CRS 5; Burst_average_mode     
    ADCON3 = 0x57;                                                              // CALC Filtered value vs setpoint
    ADSTAT = 0x00;   
    ADREF = 0x00;                                                               // NREF VSS; PREF VDD;     
    ADACT = 0x05;                                                                    
    ADCLK = 0x3F;                                                               // CS FOSC/128;   
    ADCON0 = 0x84;                                                              // FM right; ON enabled; CS FOSC/ADCLK 
       
    PIR2bits.ADTIF = 0;                                                         // Clear the ADC Threshold interrupt flag    
    PIE2bits.ADTIE = 1;                                                         // Enabling ADCC threshold interrupt.

    ADCC_SetADTIInterruptHandler(ADCC_DefaultInterruptHandler);
}

void ADCC_StartConversion(adcc_channel_t channel)
{    
    ADPCH = channel;                                                            // Select the A/D channel         
    ADCON0bits.ADON = 1;                                                        // Turn on the ADC module   
    ADCON0bits.ADGO = 1;                                                        // Start the conversion
}

adc_result_t ADCC_GetConversionResult(void)
{    
    return ((adc_result_t)((ADRESH << 8) + ADRESL));                            // Return the result
}

adc_result_t ADCC_GetSingleConversion(adcc_channel_t channel)
{    
    ADPCH = channel;                                                            // Select the A/D channel    
    ADCON0bits.ADON = 1;                                                        // Turn on the ADC module  
    ADCON0bits.ADCONT = 0;                                                      // Disable the continuous mode.    
    ADCON0bits.ADGO = 1;                                                        // Start the conversion
    while (ADCON0bits.ADGO)                                                     // Wait for the conversion to finish
    {
    }   
    return ((adc_result_t)((ADRESH << 8) + ADRESL));                            // Conversion finished, return the result
}

uint16_t ADCC_GetFilterValue(void)
{    
    return ((uint16_t)((ADFLTRH << 8) + ADFLTRL));                              // Return the contents of ADFLTRH and ADFLTRL registers
}

void ADCC_ThresholdISR(void)
{
    if (ADCC_ADTI_InterruptHandler)
        ADCC_ADTI_InterruptHandler();
}

void ADCC_SetADTIInterruptHandler(void (* InterruptHandler)(void))
{
    ADCC_ADTI_InterruptHandler = InterruptHandler;
}

void ADCC_DefaultInterruptHandler(void)
{   
    float T0 = 0.00335;                                                         // 1 / 298.15
    float B = 0.00025;                                                          // 1 / 3950 (beta value)
        
    adc_val = ADCC_GetFilterValue();                                            // Get thermistor value
    if(adc_val <= MIN_THERM_ADC)                                                // Is the temp at or below 20C?
    {
        if(ADSTATbits.LTHR == 0)                                                // Lower threshold is not violated
        {
            y_therm = MIN_SPD;                                                  // Set fan to min speed
            therm_short = 0;
            therm_open = 0;
        }
        else                                                                    // Lower thresh violation
        {
            y_therm = MAX_SPD;                                                  // Set fan to max speed
            therm_open = 1;                                                     // Therm failed open
        }
    }
    else if(adc_val >= MAX_THERM_ADC)                                           // Is the temp at or above 70C?
    {
        y_therm = MAX_SPD;                                                      // Set fan to max speed
        if(ADSTATbits.UTHR == 1)                                                // Check for therm short failure
        {            
            therm_short = 1;                                                    // Therm failed short
            therm_open = 0;
        }
    }
    else                                                                        // 20C < Temp < 70C
    {
        y_therm = SLOPE_M * adc_val + B_OFFSET;                                 // Speed based on y = mx + b line equation
        therm_short = 0;
        therm_open = 0;
    }

    temperature = (MAXADC / (float)adc_val) - 1.00000;                          // Find Resistance equivalent of input voltage
    temperature = log10f(temperature);                                          // Take the Log of the temperature
    temperature = temperature / LN;                                             // Divide the log(temp) value by the Log(e) value: converts log(x) into ln(x)
    temperature = B * temperature;                                              // Multiply by B constant
    temperature += T0;                                                          // Add T0 constant
    temperature = 1 / temperature;                                              // Invert to get degrees in Kelvin (K)
    temperature -= K;                                                           // Subtract the K constant to get actual temp in degrees C  
    
    PIR2bits.ADTIF = 0;
}

/**
 End of File
*/
