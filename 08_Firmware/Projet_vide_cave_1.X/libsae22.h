
#ifndef LIB_SAE_22_H
#define	LIB_SAE_22_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

//Pragma to disable unused functions warning
#pragma warning disable 520
//Pragma to turn if(var = val) warning into errors
#pragma warning error 758

#define _XTAL_FREQ 16000000

/* General Init */
/* Public functions */
void SAE22_Initialize(void);
/* Internal functions */
void OSCILLATOR_Initialize(void);

/* EUSART */
/* Public functions */
uint8_t EUSART_Read(void);
void EUSART_Write(uint8_t txData);
void putch(char txData);
/* Internal functions */
void EUSART_Initialize(void);

/* PWM */
/* Public functions */
void PWM1_LoadDuty(uint8_t duty);
void PWM2_LoadDuty(uint8_t duty);
/* Internal functions */
void PSMC_Initialize(void);

/* Timer 1 & CCP3 */
/* Public functions */
bool periodMeasureAvailable(void);
uint32_t lastPeriodMeasure(void);
/* Internal functions */
void TMR1_Initialize(void);
void TMR1_ISR(void);
void CCP3_Initialize(void);
void CCP3_CaptureISR(void);

/* Main ISR */
/* Public functions */
void SAE22_ISR(void);

/* LCD */
/* Public functions */
void LCD_Initialize(void);
void LCD_WriteText(char *msg);
void LCD_WriteData(uint8_t data);

/* line : 1 or 2 - Column : 1 to 40*/
void LCD_GoTo(uint8_t line, uint8_t column);

void LCD_Clear(void);
void LCD_Home(void);

/* Internal functions */
void LCD_WriteCommand(uint8_t cmd);
uint8_t LCD_ReadCommand(void);
void LCD_SoftReset(void);
void LCD_WriteHighNibble(uint8_t data);

#endif

