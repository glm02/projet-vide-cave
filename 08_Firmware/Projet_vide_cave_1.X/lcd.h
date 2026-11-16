#ifndef LCD_H
#define LCD_H

#include <stdint.h>

/* Fonctions publiques (identiques a libsae22) */
void LCD_Initialize(void);          /* a appeler APRES SYSTEM_Initialize() */
void LCD_Clear(void);
void LCD_Home(void);
void LCD_WriteText(char *msg);
void LCD_WriteData(uint8_t data);
/* line : 1 ou 2 - column : 1 a 40 */
void LCD_GoTo(uint8_t line, uint8_t column);

/* Fonctions internes */
void LCD_WriteCommand(uint8_t cmd);
void LCD_SoftReset(void);
void LCD_WriteHighNibble(uint8_t data);

#endif
