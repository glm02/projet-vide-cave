/*
 * File:   libsae22.c
 * Author: gaelrobin
 *
 * Created on April 4, 2022, 8:46 PM
 */


#include <xc.h>
#include "libsae22.h"

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection->INTOSC oscillator: I/O function on CLKIN pin
#pragma config WDTE = OFF    // Watchdog Timer Enable->WDT disabled
#pragma config PWRTE = OFF    // Power-up Timer Enable->PWRT disabled
#pragma config MCLRE = ON    // MCLR Pin Function Select->MCLR/VPP pin function is MCLR
#pragma config CP = OFF    // Flash Program Memory Code Protection->Program memory code protection is disabled
#pragma config CPD = OFF    // Data Memory Code Protection->Data memory code protection is disabled
#pragma config BOREN = ON    // Brown-out Reset Enable->Brown-out Reset enabled
#pragma config CLKOUTEN = OFF    // Clock Out Enable->CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin
#pragma config IESO = ON    // Internal/External Switchover->Internal/External Switchover mode is enabled
#pragma config FCMEN = ON    // Fail-Safe Clock Monitor Enable->Fail-Safe Clock Monitor is enabled

// CONFIG2
#pragma config WRT = OFF    // Flash Memory Self-Write Protection->Write protection off
#pragma config VCAPEN = OFF    // Voltage Regulator Capacitor Enable bit->Vcap functionality is disabled on RA6.
#pragma config PLLEN = OFF    // PLL Enable->4x PLL disabled
#pragma config STVREN = ON    // Stack Overflow/Underflow Reset Enable->Stack Overflow or Underflow will cause a Reset
#pragma config BORV = LO    // Brown-out Reset Voltage Selection->Brown-out Reset Voltage (Vbor), low trip point selected.
#pragma config LPBOR = OFF    // Low Power Brown-Out Reset Enable Bit->Low power brown-out is disabled
#pragma config LVP = ON    // Low-Voltage Programming Enable->Low-voltage programming enabled

//Global variables
volatile bool captureFlag = false;
volatile uint32_t periodCount = 0;
volatile uint32_t timer1Overflows = 0;


/*
 * Initialization functions
 * 
 */
void SAE22_Initialize(void)
{
    OSCILLATOR_Initialize();
    EUSART_Initialize();
    PSMC_Initialize();
    CCP3_Initialize();
    TMR1_Initialize();
    
    INLVLB4 = 1;    //BP_2 sur RB4 : entree en Schmitt Trigger. Valeur par défaut : 0
    INLVLC4 = 1;    //BP_1 sur RC4 : entree en Schmitt Trigger. Valeur par défaut : 1
    
}

void OSCILLATOR_Initialize(void) {
    // SCS FOSC; SPLLEN disabled; IRCF 16MHz_HF; 
    OSCCON = 0x78;
    // LFIOFR disabled; HFIOFL not stable; OSTS intosc; PLLR disabled; HFIOFS not stable; HFIOFR disabled; MFIOFR disabled; T1OSCR disabled; 
    OSCSTAT = 0x00;
    // TUN 0; 
    OSCTUNE = 0x00;
    // SBOREN disabled; BORFS disabled; 
    BORCON = 0x00;
}


/*
 * PWM (PSMC)
 * 
 */

void PSMC_Initialize(void) {
    
    //Synchronise PSMC1 to PSMC3
    PSMC1SYNCbits.P1SYNC0 = 1;
    PSMC1SYNCbits.P1SYNC1 = 1;
    PSMC1SYNCbits.P1SYNC2 = 0;

    //PWM_1 sur RC2 (PSMC3B)
    //Init PWM period
    PSMC3PRH = 0x03;
    PSMC3PRL = 0x1F;
    //Init duty cycle
    PSMC3DCH = 0;
    PSMC3DCL = 0;
    //Init Phase offset
    PSMC3PHH = 0;
    PSMC3PHL = 0;
    //Clock : Prescaler : 1, Fosc
    PSMC3CLK = 0x00;
    //output on B, normal polarity (active high)
    PSMC3STR0bits.P3STRB = 1;
    PSMC3POLbits.P3POLB = 0;
    PSMC3OENbits.P3OEB = 1;
    //Time base as source
    PSMC3PRS = 0x01;
    PSMC3PHS = 0x01;
    PSMC3DCS = 0x01;
    //enable PSMC in Single-Phase Mode this also loads steering and time buffers
    //PSMCxEN : 1 PSMCxLD : 1 PxDBFE : 0 PxDBRE: 0 PxMODE : 0000
    PSMC3CON = 0xC0;
    TRISCbits.TRISC2 = 0;

    //PWM_2 sur RC1 (PSMC1B)
    //Init PWM period
    PSMC1PRH = 0x03;
    PSMC1PRL = 0x1F;
    //Init duty cycle
    PSMC1DCH = 0;
    PSMC1DCL = 0;
    //Init Phase offset
    PSMC1PHH = 0;
    PSMC1PHL = 0;
    //Clock : Prescaler : 1, Fosc
    PSMC1CLK = 0x00;
    //output on B, normal polarity (active high)
    PSMC1STR0bits.P1STRB = 1;
    PSMC1POLbits.P1POLB = 0;
    PSMC1OENbits.P1OEB = 1;
    //Time base as source
    PSMC1PRS = 0x01;
    PSMC1PHS = 0x01;
    PSMC1DCS = 0x01;
    //enable PSMC in Single-Phase Mode this also loads steering and time buffers
    //PSMCxEN : 1 PSMCxLD : 1 PxDBFE : 0 PxDBRE: 0 PxMODE : 0000
    PSMC1CON = 0xC0;
    TRISCbits.TRISC1 = 0;
}

void PWM1_LoadDuty(uint8_t duty) {
    
    uint16_t psmc_dc;

    psmc_dc = (uint16_t) duty << 3; //Multiplication par 8 car registre de periode reglé à 800
    PSMC3DCH = (uint8_t) (psmc_dc >> 8);
    PSMC3DCL = (uint8_t) (psmc_dc & 0x00FF);
    //Activer le rechargement des registres
    PSMC3CONbits.PSMC3LD = 1;
}

void PWM2_LoadDuty(uint8_t duty) {
    
    uint16_t psmc_dc;

    psmc_dc = (uint16_t) duty << 3; //Multiplication par 8 car registre de periode reglé à 800
    PSMC1DCH = (uint8_t) (psmc_dc >> 8);
    PSMC1DCL = (uint8_t) (psmc_dc & 0x00FF);
    //Activer le rechargement des registres
    PSMC1CONbits.PSMC1LD = 1;
}
/*
 * EUSART
 * 
 */
void EUSART_Initialize(void) {
    
    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD1CON = 0x08;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RC1STA = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TX1STA = 0x24;

    // SP1BRGL 160; 
    SP1BRGL = 0xA0;

    // SP1BRGH 1; 
    SP1BRGH = 0x01;

}

uint8_t EUSART_Read(void) {
    
    while (!PIR1bits.RCIF) {
    }

    if (1 == RC1STAbits.OERR) {
        // EUSART error - restart

        RC1STAbits.CREN = 0;
        RC1STAbits.CREN = 1;
    }

    return RC1REG;
}

void EUSART_Write(uint8_t txData) {
    
    while (0 == PIR1bits.TXIF) {
    }

    TX1REG = txData; // Write the data byte to the USART.
}


void putch(char txData) {
    
    EUSART_Write(txData);
}
/*
 * CAPTURE & TIMER 1
 * 
 */
void TMR1_Initialize(void) {
    
    //T1GSS T1G_pin; TMR1GE disabled; T1GTM disabled; T1GPOL low; T1GGO done; T1GSPM disabled; 
    T1GCON = 0x00;

    //TMR1H 0; 
    TMR1H = 0x00;

    //TMR1L 0; 
    TMR1L = 0x00;

    // Clearing IF flag before enabling the interrupt.
    PIR1bits.TMR1IF = 0;

    // Enabling TMR1 interrupt.
    PIE1bits.TMR1IE = 1;

    // T1CKPS 1:1; T1OSCEN disabled; nT1SYNC synchronize; TMR1CS FOSC/4; TMR1ON enabled; 
    T1CON = 0x01;
}

inline void TMR1_ISR(void) {

    // Clear the TMR1 interrupt flag
    PIR1bits.TMR1IF = 0;
    timer1Overflows++;
}

void CCP3_Initialize(void) {

    // CCP3M Rising edge; DC3B 0; 
    CCP3CON = 0x05;

    // CCPR3L 0; 
    CCPR3L = 0x00;

    // CCPR3H 0; 
    CCPR3H = 0x00;

    // Clear the CCP3 interrupt flag
    PIR3bits.CCP3IF = 0;

    // Enable the CCP3 interrupt
    PIE3bits.CCP3IE = 1;
    
    /**
    APFCON2 register to select CCP3 Input on RB5
     */
    APFCON2 = 0x01;
    TRISB5 = 1; 
}

inline void CCP3_CaptureISR(void) {
    static uint16_t previousCapture = 0;
    static uint16_t capturedValue;

    // Clear the CCP3 interrupt flag
    PIR3bits.CCP3IF = 0;

    // Copy captured value.
    capturedValue = (CCPR3H<<8) | CCPR3L;

    
    if (timer1Overflows >= 1) {
        periodCount = (65536 - previousCapture) + ((timer1Overflows - 1) << 16) + capturedValue;
    } else {
        periodCount = (capturedValue - previousCapture);
    }
    
    periodCount = periodCount >> 2; //Divide by 4 to have a period in microseconds

    timer1Overflows = 0;
    captureFlag = 1;
    previousCapture = capturedValue;
}

bool periodMeasureAvailable(void) {
    
    return captureFlag;
}

uint32_t lastPeriodMeasure(void) {
    uint32_t res;
    //Mark the period as read by resetting the capture flag
    captureFlag = false;
    //Copy last period count while capture interrupt is disabled to
    PIE3bits.CCP3IE = 0;
    res = periodCount;
    PIE3bits.CCP3IE = 1;
    
    return res;
}

/*
 * ISR
 */
void SAE22_ISR(void) {
    // interrupt handler
    if (INTCONbits.PEIE == 1) {
        if (PIE1bits.TMR1IE == 1 && PIR1bits.TMR1IF == 1) {
            TMR1_ISR();
        }

        if (PIE3bits.CCP3IE == 1 && PIR3bits.CCP3IF == 1) {
            CCP3_CaptureISR();
        }
    }
}

/*
 * LCD
 */
/*
 * Reference datasheets
 * https://www.crystalfontz.com/controllers/Samsung/KS0070B/
 * https://cdn.displaytech-us.com/sites/default/files/display-data-sheet/162B%20series-v21.pdf
 * https://docs-emea.rs-online.com/webdocs/06dd/0900766b806dda15.pdf
 */

#define LCD_LAT                   LATA

/* LCD commands*/
#define LCD_CLEAR                 0x01
#define LCD_HOME                  0x02
#define LCD_ENTRY_MODE            0x04
#define LCD_DISPLAY_ONOFF         0x08
#define LCD_DISPLAY_CURSOR_SHIFT  0x10
#define LCD_FUNCTION_SET          0x20
#define LCD_CGRAM_SET             0x40
#define LCD_DDRAM_SET             0x80

/* Entry mode params */
#define LCD_ENTRY_SHIFT_CURSOR    0
#define LCD_ENTRY_SHIFT_DISPLAY   1
#define LCD_ENTRY_MOVE_LEFT       0
#define LCD_ENTRY_MOVE_RIGHT      2

/* Display ON/OFF params */
#define LCD_DISP_DISPLAY_OFF      0
#define LCD_DISP_DISPLAY_ON       4
#define LCD_DISP_CURSOR_OFF       0
#define LCD_DISP_CURSOR_ON        2
#define LCD_DISP_CURSOR_NOBLINK   0
#define LCD_DISP_CURSOR_BLINK     1

/* Cursor display shift parameters */
#define LCD_SHIFT_CURSOR          0
#define LCD_SHIFT_DISPLAY         8
#define LCD_SHIFT_LEFT            0
#define LCD_SHIFT_RIGHT           4

/* Function set parameters */
#define LCD_FUNCTION_FONT5x7      0
#define LCD_FUNCTION_FONT5x10     4
#define LCD_FUNCTION_ONE_LINE     0
#define LCD_FUNCTION_TWO_LINE     8
#define LCD_FUNCTION_4_BIT        0
#define LCD_FUNCTION_8_BIT        16

// get/set LCD_RS aliases
#define LCD_RS_TRIS                 TRISAbits.TRISA1
#define LCD_RS_LAT                  LATAbits.LATA1
#define LCD_RS_PORT                 PORTAbits.RA1
#define LCD_RS_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define LCD_RS_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define LCD_RS_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define LCD_RS_GetValue()           PORTAbits.RA1

// get/set LCD_RW aliases
#define LCD_RW_TRIS                 TRISAbits.TRISA2
#define LCD_RW_LAT                  LATAbits.LATA2
#define LCD_RW_PORT                 PORTAbits.RA2
#define LCD_RW_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define LCD_RW_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define LCD_RW_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define LCD_RW_GetValue()           PORTAbits.RA2

// get/set LCD_E aliases
#define LCD_E_TRIS                 TRISAbits.TRISA3
#define LCD_E_LAT                  LATAbits.LATA3
#define LCD_E_PORT                 PORTAbits.RA3
#define LCD_E_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define LCD_E_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define LCD_E_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define LCD_E_GetValue()           PORTAbits.RA3

// get/set LCD_D4 aliases
#define LCD_D4_TRIS                 TRISAbits.TRISA4
#define LCD_D4_LAT                  LATAbits.LATA4
#define LCD_D4_PORT                 PORTAbits.RA4
#define LCD_D4_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define LCD_D4_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define LCD_D4_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define LCD_D4_GetValue()           PORTAbits.RA4

// get/set LCD_D5 aliases
#define LCD_D5_TRIS                 TRISAbits.TRISA5
#define LCD_D5_LAT                  LATAbits.LATA5
#define LCD_D5_PORT                 PORTAbits.RA5
#define LCD_D5_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define LCD_D5_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define LCD_D5_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define LCD_D5_GetValue()           PORTAbits.RA5

// get/set LCD_D7 aliases
#define LCD_D7_TRIS                 TRISAbits.TRISA6
#define LCD_D7_LAT                  LATAbits.LATA6
#define LCD_D7_PORT                 PORTAbits.RA6
#define LCD_D7_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define LCD_D7_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define LCD_D7_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define LCD_D7_GetValue()           PORTAbits.RA6

// get/set LCD_D6 aliases
#define LCD_D6_TRIS                 TRISAbits.TRISA7
#define LCD_D6_LAT                  LATAbits.LATA7
#define LCD_D6_PORT                 PORTAbits.RA7
#define LCD_D6_SetHigh()            do { LATAbits.LATA7 = 1; } while(0)
#define LCD_D6_SetLow()             do { LATAbits.LATA7 = 0; } while(0)
#define LCD_D6_Toggle()             do { LATAbits.LATA7 = ~LATAbits.LATA7; } while(0)
#define LCD_D6_GetValue()           PORTAbits.RA7

/**
 * 
 * Writes 4 most significant bits of data D7 D6 D5 D4 to LCD_LAT
 * Bit 6 and 7 must be swapped : https://www.geeksforgeeks.org/swap-bits-in-a-given-number/
 */
void LCD_WriteHighNibble(uint8_t data) {
    uint8_t bit6, bit7, x;

    LCD_E_SetHigh();

    /* Move 6'th to rightmost side */
    bit6 = (data >> 6) & 1;
    /* Move 7'th to rightmost side */
    bit7 = (data >> 7) & 1;
    /* XOR the two bits */
    x = (bit6 ^ bit7);
    /* Put the xor bit back to their original positions */
    x = (x << 6) | (x << 7);
    /* XOR 'x' with the original number so that the
       two sets are swapped */
    data = data ^ x;

    LCD_LAT = (LCD_LAT & 0x0F) | (data & 0xF0); //Update LATCH with high nibble of data
    __delay_us(1); //(E pulse width  > 220 ns) and (tcycle > 500 ns)
    LCD_E_SetLow();
}

void LCD_SoftReset(void) {
    LCD_WriteHighNibble(LCD_FUNCTION_SET | LCD_FUNCTION_8_BIT);
    __delay_ms(5); // 4.1 ms minimum delay
    LCD_WriteHighNibble(LCD_FUNCTION_SET | LCD_FUNCTION_8_BIT);
    __delay_us(100);
    LCD_WriteHighNibble(LCD_FUNCTION_SET | LCD_FUNCTION_8_BIT);
    __delay_us(100);
    LCD_WriteHighNibble(LCD_FUNCTION_SET | LCD_FUNCTION_4_BIT);
    __delay_us(100);

}

void LCD_WriteCommand(uint8_t cmd) {
    __delay_ms(5); //Max instruction time 1.53 ms

    LCD_RS_SetLow(); //Command mode
    LCD_RW_SetLow(); //Write mode
    LCD_WriteHighNibble(cmd);
    LCD_WriteHighNibble(cmd << 4);
}

void LCD_WriteData(uint8_t data) {
    __delay_ms(2); //Max instruction time 1.53 ms

    LCD_RS_SetHigh(); //Data mode
    LCD_RW_SetLow(); //Write mode
    LCD_WriteHighNibble(data);
    LCD_WriteHighNibble(data << 4);
}

void LCD_WriteText(char *msg) {
    while (*msg != 0) {
        LCD_WriteData(*msg);
        msg++;
    }
}

/* line : 1 or 2 - Column : 1 to 40*/
void LCD_GoTo(uint8_t line, uint8_t column) {
    uint8_t addr = (line == 1) ? 0x00 : 0x40;
    addr += (column - 1);

    addr &= 0x7F;

    LCD_WriteCommand(LCD_DDRAM_SET | addr);

}

void LCD_Clear(void) {
    LCD_WriteCommand(LCD_CLEAR);
}

void LCD_Home(void) {
    LCD_WriteCommand(LCD_HOME);
}

void LCD_Initialize(void) {
    //GPIO Init - RA7...RA1 digital outputs
    ANSELA &= 0x01;
    TRISA &= 0x01;
    
    // Power up delay
    __delay_ms(100);

    LCD_SoftReset();
    LCD_WriteCommand(LCD_FUNCTION_SET | LCD_FUNCTION_4_BIT | LCD_FUNCTION_TWO_LINE | LCD_FUNCTION_FONT5x7);
    LCD_WriteCommand(LCD_DISPLAY_ONOFF | LCD_DISP_DISPLAY_ON | LCD_DISP_CURSOR_OFF | LCD_DISP_CURSOR_NOBLINK);
    LCD_Clear();
    LCD_WriteCommand(LCD_ENTRY_MODE | LCD_ENTRY_MOVE_RIGHT | LCD_ENTRY_SHIFT_CURSOR);
}