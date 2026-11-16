/**
 * Generated Pins header File
 * 
 * @file pins.h
 * 
 * @defgroup  pinsdriver Pins Driver
 * 
 * @brief This is generated driver header for pins. 
 *        This header file provides APIs for all pins selected in the GUI.
 *
 * @version Driver Version  3.0.0
*/

/*
© [2026] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef PINS_H
#define PINS_H

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set IO_RB0 aliases
#define LED1_TRIS                 TRISBbits.TRISB0
#define LED1_LAT                  LATBbits.LATB0
#define LED1_PORT                 PORTBbits.RB0
#define LED1_WPU                  WPUBbits.WPUB0
#define LED1_OD                   ODCONBbits.
#define LED1_ANS                  ANSELBbits.ANSB0
#define LED1_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define LED1_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define LED1_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define LED1_GetValue()           PORTBbits.RB0
#define LED1_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define LED1_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define LED1_SetPullup()          do { WPUBbits.WPUB0 = 1; } while(0)
#define LED1_ResetPullup()        do { WPUBbits.WPUB0 = 0; } while(0)
#define LED1_SetPushPull()        do { ODCONBbits. = 0; } while(0)
#define LED1_SetOpenDrain()       do { ODCONBbits. = 1; } while(0)
#define LED1_SetAnalogMode()      do { ANSELBbits.ANSB0 = 1; } while(0)
#define LED1_SetDigitalMode()     do { ANSELBbits.ANSB0 = 0; } while(0)
// get/set IO_RB1 aliases
#define LED2_TRIS                 TRISBbits.TRISB1
#define LED2_LAT                  LATBbits.LATB1
#define LED2_PORT                 PORTBbits.RB1
#define LED2_WPU                  WPUBbits.WPUB1
#define LED2_OD                   ODCONBbits.
#define LED2_ANS                  ANSELBbits.ANSB1
#define LED2_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define LED2_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define LED2_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define LED2_GetValue()           PORTBbits.RB1
#define LED2_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define LED2_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define LED2_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define LED2_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define LED2_SetPushPull()        do { ODCONBbits. = 0; } while(0)
#define LED2_SetOpenDrain()       do { ODCONBbits. = 1; } while(0)
#define LED2_SetAnalogMode()      do { ANSELBbits.ANSB1 = 1; } while(0)
#define LED2_SetDigitalMode()     do { ANSELBbits.ANSB1 = 0; } while(0)
// get/set IO_RB2 aliases
#define LED3_TRIS                 TRISBbits.TRISB2
#define LED3_LAT                  LATBbits.LATB2
#define LED3_PORT                 PORTBbits.RB2
#define LED3_WPU                  WPUBbits.WPUB2
#define LED3_OD                   ODCONBbits.
#define LED3_ANS                  ANSELBbits.ANSB2
#define LED3_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define LED3_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define LED3_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define LED3_GetValue()           PORTBbits.RB2
#define LED3_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define LED3_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define LED3_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define LED3_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define LED3_SetPushPull()        do { ODCONBbits. = 0; } while(0)
#define LED3_SetOpenDrain()       do { ODCONBbits. = 1; } while(0)
#define LED3_SetAnalogMode()      do { ANSELBbits.ANSB2 = 1; } while(0)
#define LED3_SetDigitalMode()     do { ANSELBbits.ANSB2 = 0; } while(0)
// get/set IO_RB3 aliases
#define LED4_TRIS                 TRISBbits.TRISB3
#define LED4_LAT                  LATBbits.LATB3
#define LED4_PORT                 PORTBbits.RB3
#define LED4_WPU                  WPUBbits.WPUB3
#define LED4_OD                   ODCONBbits.
#define LED4_ANS                  ANSELBbits.ANSB3
#define LED4_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define LED4_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define LED4_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define LED4_GetValue()           PORTBbits.RB3
#define LED4_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define LED4_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define LED4_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define LED4_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define LED4_SetPushPull()        do { ODCONBbits. = 0; } while(0)
#define LED4_SetOpenDrain()       do { ODCONBbits. = 1; } while(0)
#define LED4_SetAnalogMode()      do { ANSELBbits.ANSB3 = 1; } while(0)
#define LED4_SetDigitalMode()     do { ANSELBbits.ANSB3 = 0; } while(0)
/**
 * @ingroup  pinsdriver
 * @brief GPIO and peripheral I/O initialization
 * @param none
 * @return none
 */
void PIN_MANAGER_Initialize (void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt on Change Handling routine
 * @param none
 * @return none
 */
void PIN_MANAGER_IOC(void);


#endif // PINS_H
/**
 End of File
*/