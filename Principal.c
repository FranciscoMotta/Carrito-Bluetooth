/*
 * File:   Principal.c
 * Author: HP
 *
 * Created on 13 de marzo de 2020, 07:42 PM
 */


// PIC18F4550 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator (HS))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = OFF     // CCP2 MUX bit (CCP2 input/output is multiplexed with RB3)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include <xc.h>

#define _XTAL_FREQ 20000000  //Definición del cristal

//ENTRADAS PARA EL MÓDULO DE PUENTE EN H

#define IN1 PORTBbits.RB0 
#define IN2 PORTBbits.RB1
#define IN3 PORTBbits.RB2
#define IN4 PORTBbits.RB3

#define PUERTOS_IN TRISB



void USART_init (void) // CONFIGURACIÓN DE LA COMUNICACIÓN USART
{
       //Configuración de los pines
    TRISCbits.RC6 = 0;      //RC6 = Tx -> Salida
    TRISCbits.RC7 = 1;      //RC7 = Rx -> Entrad
    
    TXSTAbits.BRGH = 0;
    BAUDCONbits.BRG16 = 0;
    TXSTAbits.SYNC = 0;
    RCSTAbits.SPEN = 1;
    
//Resgistros de configuración para envío de datos
//    TXSTAbits.TX9 = 0;
//    TXSTAbits.TXEN1 = 1;
    
        //Recepción
    RCSTAbits.RC9 = 0;      //8 bits
    RCSTAbits.CREN = 1;     //Activamos recepción
  
    SPBRG = 31;
}


char USART_Rx() //Función de recepción de datos 
{ 
    return RCREG; 
}

void main(void) //Principal
{
    USART_init(); //Se inicial el USART
    PUERTOS_IN = 0x00; //SE Configuran los pines como salida
    TRISDbits.RD0 = 0;
    
    char  dato_entrada = 0;
   
    
    while (1)
    {
        dato_entrada = USART_Rx();
        if (dato_entrada == '1')
        {
            IN1 = 1;
            IN2 = 0;
            IN3 = 0;
            IN4 = 0;
                    
            PORTDbits.RD0 = 1;
        } else 
        {
            if (dato_entrada == '2')
            {
            IN1 = 0;
            IN2 = 1;
            IN3 = 0;
            IN4 = 0;
                PORTDbits.RD0 = 0;
            } else {
                if (dato_entrada == '3')
                {
            IN1 = 0;
            IN2 = 0;
            IN3 = 1;
            IN4 = 0;
            PORTDbits.RD0 = 1;
                } else {
                    if (dato_entrada == '4')
                    {
            IN1 = 0;
            IN2 = 0;
            IN3 = 0;
            IN4 = 1;
            PORTDbits.RD0 = 0; 
                    }
                    else {
                    }
                }
            }
        }
    }
    return;
}
