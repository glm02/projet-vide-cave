/*
 * lcd.c - Pilote LCD (HD44780, mode 4 bits) extrait de libsae22.c (bibliotheque fournie en SAE22)
 * Brochage : RS=RA1, RW=RA2, E=RA3, D4=RA4, D5=RA5, D7=RA6, D6=RA7
 * Seule la partie LCD est reprise, pour pouvoir l'utiliser avec un projet MCC.
 */
#include <xc.h>
#include <stdint.h>
#include "mcc_generated_files/system/clock.h"   /* _XTAL_FREQ pour __delay_ms / __delay_us */
#include "lcd.h"

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
