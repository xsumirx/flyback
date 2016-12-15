/**
  COG1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    cog1.c

  @Summary
    This is the generated driver implementation file for the COG1 driver using MPLAB(c) Code Configurator

  @Description
    This header file provides implementations for driver APIs for COG1.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 3.16
        Device            :  PIC16F1709
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.20
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "cog1.h"

/**
  Section: COG1 APIs
*/

void COG1_Initialize(void)
{
    // Reset double buffered register COG1CON0
    COG1CON0 = 0x00;

    // Set the COG to the options selected in MPLAB(c) Code Configurator

    // G1POLD Active high; G1POLB Active high; G1POLC Active high; G1RDBS COGx_clock and COGxDBR; G1POLA Active high; G1FDBS COGx_clock and COGxDBF; 
    COG1CON1 = 0x00;

    // G1RIS3 disabled; G1RIS4 enabled; G1RIS1 disabled; G1RIS2 disabled; G1RIS0 disabled; G1RIS5 disabled; G1RIS6 disabled; 
    COG1RIS = 0x10;

    // G1RSIM2 immediate; G1RSIM3 immediate; G1RSIM0 immediate; G1RSIM1 immediate; G1RSIM6 immediate; G1RSIM4 immediate; G1RSIM5 immediate; 
    COG1RSIM = 0x00;

    // G1FIS2 disabled; G1FIS1 enabled; G1FIS4 disabled; G1FIS3 disabled; G1FIS6 disabled; G1FIS5 enabled; G1FIS0 disabled; 
    COG1FIS = 0x22;

    // G1FSIM1 after falling event phase delay; G1FSIM0 immediate; G1FSIM3 immediate; G1FSIM2 immediate; G1FSIM5 after falling event phase delay; G1FSIM4 immediate; G1FSIM6 immediate; 
    COG1FSIM = 0x22;

    // G1ARSEN disabled; G1ASE not shutdown; G1ASDAC inactive state; G1ASDBD inactive state; 
    COG1ASD0 = 0x00;

    // G1AS0E disabled; G1AS1E disabled; G1AS2E disabled; G1AS3E disabled; 
    COG1ASD1 = 0x00;

    // G1SDATD static data low; G1SDATC static data low; G1STRD static level; G1STRC static level; G1STRB static level; G1STRA waveform; G1SDATB static data low; G1SDATA static data low; 
    COG1STR = 0x01;

    // G1DBR 0; 
    COG1DBR = 0x00;

    // G1DBF 0; 
    COG1DBF = 0x00;

    // G1BLKR 10; 
    COG1BLKR = 0x0A;

    // G1BLKF 0; 
    COG1BLKF = 0x00;

    // G1PHR 0; 
    COG1PHR = 0x00;

    // G1PHF 0; 
    COG1PHF = 0x00;

    // G1EN enabled; G1CS FOSC/4; G1LD transfer complete; G1MD steered PWM mode; 
    COG1CON0 = 0x80;

}

void COG1_AutoShutdownEventSet()
{
    // Setting the GxASE bit of COGxASD0 register
    COG1ASD0bits.G1ASE = 1;
}

void COG1_AutoShutdownEventClear()
{
    // Clearing the GxASE bit of COGxASD0 register
    COG1ASD0bits.G1ASE = 0;
}
/**
 End of File
*/

