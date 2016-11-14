
// Configuration bits: selected in the GUI

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits->INTOSC oscillator: I/O function on CLKIN pin
#pragma config WDTE = OFF    // Watchdog Timer Enable->WDT disabled
#pragma config PWRTE = OFF    // Power-up Timer Enable->PWRT disabled
#pragma config MCLRE = OFF    // MCLR Pin Function Select->MCLR/VPP pin function is MCLR
#pragma config CP = OFF    // Flash Program Memory Code Protection->Program memory code protection is disabled
#pragma config BOREN = ON    // Brown-out Reset Enable->Brown-out Reset enabled
#pragma config CLKOUTEN = OFF    // Clock Out Enable->CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin
#pragma config IESO = ON    // Internal/External Switchover Mode->Internal/External Switchover Mode is enabled
#pragma config FCMEN = ON    // Fail-Safe Clock Monitor Enable->Fail-Safe Clock Monitor is enabled

// CONFIG2
#pragma config WRT = OFF    // Flash Memory Self-Write Protection->Write protection off
#pragma config PPS1WAY = ON    // Peripheral Pin Select one-way control->The PPSLOCK bit cannot be cleared once it is set by software
#pragma config ZCDDIS = ON    // Zero-cross detect disable->Zero-cross detect circuit is disabled at POR
#pragma config PLLEN = 0FF    // Phase Lock Loop enable->4x PLL is always enabled
#pragma config STVREN = ON    // Stack Overflow/Underflow Reset Enable->Stack Overflow or Underflow will cause a Reset
#pragma config BORV = LO    // Brown-out Reset Voltage Selection->Brown-out Reset Voltage (Vbor), low trip point selected.
#pragma config LPBOR = OFF    // Low-Power Brown Out Reset->Low-Power BOR is disabled
#pragma config LVP = ON    // Low-Voltage Programming Enable->Low-voltage programming enabled

#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "mcc_generated_files/mcc.h"

//#define _XTAL_FREQ 32000000

float DA_LSB,AD_LSB,a0,a1,a2, b1,b2,M1,M2,rk,rk_1,rk_2,Ver,Vref,Vc,Vout,DAC_DISCRETE, ADC_SCALE;


typedef union _wordContainer
{
    uint16_t word;
    uint8_t bytes[2];
}wordContainer;

volatile wordContainer timerValue;

void PWMInit()
{
    //40Khz
    //0.38
    TRISA4 = 0;
    RA2PPS = 0x0E;
    
    CCP1M0 = 0;
    CCP1M1 = 0;
    CCP1M2 = 1;
    CCP1M3 = 1;
    
    
    
    PR2 = 25;
    TMR2IF = 0;
    TMR2IE = 0;
    TMR2ON = 1;
    
    //PWM
    CCP1CON |= 0x30;
    CCPR1L = 0x8F;
    
}

void OSC_Init()
{
    //32 mHZ
    OSCCON = 0xf0;
    OSCSTAT = 0x00;
    OSCTUNE = 0x00;
}

void TMR1_Init()
{
    
    timerValue.word = 65410;
    
    TMR1H = timerValue.bytes[1];
    TMR1L = timerValue.bytes[0];
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 1;
    T1CON = 0x01;
   
}

void CCP1_Init()
{

//    CCPR1H = timerValue.bytes[1];
//    CCPR1L = timerValue.bytes[0];
//    
//    CCP1IF = 0;
//    CCP1IE = 0;
//    
//    CCP1CON = 0x08;
    
    TRISA4 = 0;
    ANSELA &= ~0x10;
    RA4PPS = 0x0c;

}

void CCP2_Init()
{
   
//    wordContainer value = timerValue;
//    value.word += 30;
//    
//    CCPR2H = value.bytes[1];
//    CCPR2L = value.bytes[0];
//
//    CCP2IF = 0;
//    CCP2IE = 0;
//    
//    CCP2CON = 0x09;
    
    TRISC1 = 0;
    ANSELC &= ~0x02;
    RC1PPS = 0x0D;
    
}

void ReadADCInput()
{
    
}

uint16_t adcVal = 0;

typedef union _floatToUN8
{
    float x;
    uint8_t un[3];
}floatToUN8;



void USART_Initialize(void)
{
     // Set the EUSART module to the options selected in the user interface.

    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD1CON = 0x08;

    // SPEN enabled; RX9 8-bit; CREN disabled; ADDEN disabled; SREN disabled; 
    RC1STA = 0x80;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TX1STA = 0x24;

    // Baud Rate = 9600; SP1BRGL 64; 
    SP1BRGL = 0x40;

    // Baud Rate = 9600; SP1BRGH 3; 
    SP1BRGH = 0x03;

}

void USART_Write(uint8_t txData)
{
    while(!TRMT);
    TX1REG = txData;    // Write the data byte to the USART.
}

void USART_WriteString(uint8_t *_data)
{
    int limiter = 0;
    while(*(_data + limiter) != 0 && limiter < 100)
    {
        USART_Write(*(_data + limiter));
        limiter++;
    }
}

void USART_WriteValue(float _data)
{
    uint8_t buf[15] = {0};    // declare array
    sprintf (buf, "%f", _data);  // here is sprintf prototype function usage
    USART_WriteString(buf);
}

void main(void) {
    
    OSC_Init();
    
    
    //------------------------------------Compensator Init ----------------------------//
    
    
    a0 = 0.0649 ; a1 = 0.00765; a2 = -0.0571;
    b1 = 0.77; b2 = 0.22;
    
    M1 = 0; M2 = 0; rk = 0; rk_1 = 0; rk_2 = 0;
    Vref = 2.5;
    
    DA_LSB = 5000.0/256.0;
    ADC_SCALE = 9.23;
    
//    float Vdesired = 25;       //Volts
//    float TimeSoftStart = 10; //mS
//
//    float softStartStep = Vdesired / (ADC_SCALE * TimeSoftStart);
//    
//    int delay = 0;
    
    
    //--------------------------------------------------------------------------------//
    
    TRISB6 = 0;
    LATB6 = 0;
    ANSELB &= ~0x40;
    
    TRISA4 = 0;
    ANSELA &= ~0x10;
    RA4PPS = 0x08;   //RA4->COG1:COG1A;
    
    //RC3 as CMP1-
    LATC3 = 0;
    ANSELC |= 0x80;
    TRISC |= 0x80;
    
    
    //ADC RC0
    ANSELC |= 0x01;
    TRISC |= 0x01;
    
    //TX RC2
    LATC2 = 0;
    WPUC2 = 1;
    ANSELC |= 0x04;
    TRISC &= ~0x04;
    RC2PPS = 0x14;   //RC2->EUSART:TX;
    
    //RC7 as CMP Out
    //LATC7 = 0;
    //ANSELC |= 0x80;
    //TRISC7 = 0;
    //RC7PPS = 0x16;   //RC7->CMP1:C1OUT;
    
    
    //TMR1_Init();
    //CCP1_Init();
    //CCP2_Init();
    
    PWM1_Initialize();
    PWM2_Initialize();
    TMR2_Initialize();
    COG1_Initialize();
    
    DAC_Initialize();
    CMP1_Initialize();
    
    ADC_Initialize();
    ADC_GetConversion(channel_AN4);
    
    USART_Initialize();
    __delay_ms(100);
    USART_WriteString("Boost Initializing.....");
    //SART_Write('H');
    USART_Write('\n');
    
    GIE = 1;
    PEIE = 1;
    
    floatToUN8 tempVal;
    
    while(1)
    {
        
//        if(delay < 10)
//        {
//            delay ++;
//            Vref += softStartStep;
//        }
        
        adcVal = ADC_GetConversion(channel_AN4) + ADC_GetConversion(channel_AN4) + ADC_GetConversion(channel_AN4) + ADC_GetConversion(channel_AN4);

        Vout = ((float)adcVal * 4.88)/4000; 
        
        Ver = Vref - Vout; /* Calculate error term */
        
        
        //Ver += 0.2;
        
        rk = Ver + (b1 * rk_1) + (b2 * rk_2);
        Vc = (a0*rk) + (rk_1 * a1) + (rk_2 * a2); /* Calculate output */
        
        DAC_DISCRETE = Vc*DA_LSB;
 
        //if(DAC_DISCRETE < 0)
            //DAC_DISCRETE = 0;
        
        DAC_SetOutput((unsigned int)fabs(DAC_DISCRETE));

        rk_2 = rk_1; /* Update variables */
        rk_1 = rk;

        USART_WriteString("Ver : ");
        USART_WriteValue(Ver);
        
        USART_WriteString("      ");
                
        USART_WriteString("Vc : ");
        USART_WriteValue(Vc);
        
        USART_Write('\n');
        
        __delay_ms(10);
        
    }
    return;
}


void interrupt isr()
{
    if(TMR1IE & TMR1IF)
    {
        TMR1H = timerValue.bytes[1];
        TMR1L = timerValue.bytes[0];
        TMR1IF = 0;
        LATB6 ^= 1;
    }
   
}



