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

// get/set IO_RA0 aliases
#define Capteur_COURANT_TRIS                 TRISAbits.TRISA0
#define Capteur_COURANT_LAT                  LATAbits.LATA0
#define Capteur_COURANT_PORT                 PORTAbits.RA0
#define Capteur_COURANT_WPU                  WPUAbits.WPUA0
#define Capteur_COURANT_OD                   ODCONAbits.
#define Capteur_COURANT_ANS                  ANSELAbits.ANSA0
#define Capteur_COURANT_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define Capteur_COURANT_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define Capteur_COURANT_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define Capteur_COURANT_GetValue()           PORTAbits.RA0
#define Capteur_COURANT_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define Capteur_COURANT_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define Capteur_COURANT_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define Capteur_COURANT_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define Capteur_COURANT_SetPushPull()        do { ODCONAbits. = 0; } while(0)
#define Capteur_COURANT_SetOpenDrain()       do { ODCONAbits. = 1; } while(0)
#define Capteur_COURANT_SetAnalogMode()      do { ANSELAbits.ANSA0 = 1; } while(0)
#define Capteur_COURANT_SetDigitalMode()     do { ANSELAbits.ANSA0 = 0; } while(0)
// get/set IO_RB0 aliases
#define LED_TRIS                 TRISBbits.TRISB0
#define LED_LAT                  LATBbits.LATB0
#define LED_PORT                 PORTBbits.RB0
#define LED_WPU                  WPUBbits.WPUB0
#define LED_OD                   ODCONBbits.
#define LED_ANS                  ANSELBbits.ANSB0
#define LED_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define LED_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define LED_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define LED_GetValue()           PORTBbits.RB0
#define LED_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define LED_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define LED_SetPullup()          do { WPUBbits.WPUB0 = 1; } while(0)
#define LED_ResetPullup()        do { WPUBbits.WPUB0 = 0; } while(0)
#define LED_SetPushPull()        do { ODCONBbits. = 0; } while(0)
#define LED_SetOpenDrain()       do { ODCONBbits. = 1; } while(0)
#define LED_SetAnalogMode()      do { ANSELBbits.ANSB0 = 1; } while(0)
#define LED_SetDigitalMode()     do { ANSELBbits.ANSB0 = 0; } while(0)
// get/set IO_RB1 aliases
#define BUZZER_TRIS                 TRISBbits.TRISB1
#define BUZZER_LAT                  LATBbits.LATB1
#define BUZZER_PORT                 PORTBbits.RB1
#define BUZZER_WPU                  WPUBbits.WPUB1
#define BUZZER_OD                   ODCONBbits.
#define BUZZER_ANS                  ANSELBbits.ANSB1
#define BUZZER_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define BUZZER_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define BUZZER_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define BUZZER_GetValue()           PORTBbits.RB1
#define BUZZER_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define BUZZER_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define BUZZER_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define BUZZER_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define BUZZER_SetPushPull()        do { ODCONBbits. = 0; } while(0)
#define BUZZER_SetOpenDrain()       do { ODCONBbits. = 1; } while(0)
#define BUZZER_SetAnalogMode()      do { ANSELBbits.ANSB1 = 1; } while(0)
#define BUZZER_SetDigitalMode()     do { ANSELBbits.ANSB1 = 0; } while(0)
// get/set IO_RB2 aliases
#define OPTO_TRIAC_TRIS                 TRISBbits.TRISB2
#define OPTO_TRIAC_LAT                  LATBbits.LATB2
#define OPTO_TRIAC_PORT                 PORTBbits.RB2
#define OPTO_TRIAC_WPU                  WPUBbits.WPUB2
#define OPTO_TRIAC_OD                   ODCONBbits.
#define OPTO_TRIAC_ANS                  ANSELBbits.ANSB2
#define OPTO_TRIAC_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define OPTO_TRIAC_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define OPTO_TRIAC_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define OPTO_TRIAC_GetValue()           PORTBbits.RB2
#define OPTO_TRIAC_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define OPTO_TRIAC_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define OPTO_TRIAC_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define OPTO_TRIAC_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define OPTO_TRIAC_SetPushPull()        do { ODCONBbits. = 0; } while(0)
#define OPTO_TRIAC_SetOpenDrain()       do { ODCONBbits. = 1; } while(0)
#define OPTO_TRIAC_SetAnalogMode()      do { ANSELBbits.ANSB2 = 1; } while(0)
#define OPTO_TRIAC_SetDigitalMode()     do { ANSELBbits.ANSB2 = 0; } while(0)
// get/set IO_RB3 aliases
#define LED_D1_TRIS                 TRISBbits.TRISB3
#define LED_D1_LAT                  LATBbits.LATB3
#define LED_D1_PORT                 PORTBbits.RB3
#define LED_D1_WPU                  WPUBbits.WPUB3
#define LED_D1_OD                   ODCONBbits.
#define LED_D1_ANS                  ANSELBbits.ANSB3
#define LED_D1_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define LED_D1_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define LED_D1_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define LED_D1_GetValue()           PORTBbits.RB3
#define LED_D1_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define LED_D1_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define LED_D1_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define LED_D1_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define LED_D1_SetPushPull()        do { ODCONBbits. = 0; } while(0)
#define LED_D1_SetOpenDrain()       do { ODCONBbits. = 1; } while(0)
#define LED_D1_SetAnalogMode()      do { ANSELBbits.ANSB3 = 1; } while(0)
#define LED_D1_SetDigitalMode()     do { ANSELBbits.ANSB3 = 0; } while(0)
// get/set IO_RB4 aliases
#define BP_2_TRIS                 TRISBbits.TRISB4
#define BP_2_LAT                  LATBbits.LATB4
#define BP_2_PORT                 PORTBbits.RB4
#define BP_2_WPU                  WPUBbits.WPUB4
#define BP_2_OD                   ODCONBbits.
#define BP_2_ANS                  ANSELBbits.ANSB4
#define BP_2_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define BP_2_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define BP_2_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define BP_2_GetValue()           PORTBbits.RB4
#define BP_2_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define BP_2_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define BP_2_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define BP_2_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define BP_2_SetPushPull()        do { ODCONBbits. = 0; } while(0)
#define BP_2_SetOpenDrain()       do { ODCONBbits. = 1; } while(0)
#define BP_2_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define BP_2_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)
// get/set IO_RC6 aliases
#define IO_RC6_TRIS                 TRISCbits.TRISC6
#define IO_RC6_LAT                  LATCbits.LATC6
#define IO_RC6_PORT                 PORTCbits.RC6
#define IO_RC6_WPU                  WPUCbits.WPUC6
#define IO_RC6_OD                   ODCONCbits.
#define IO_RC6_ANS                  ANSELCbits.ANSC6
#define IO_RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define IO_RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define IO_RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define IO_RC6_GetValue()           PORTCbits.RC6
#define IO_RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define IO_RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define IO_RC6_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define IO_RC6_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define IO_RC6_SetPushPull()        do { ODCONCbits. = 0; } while(0)
#define IO_RC6_SetOpenDrain()       do { ODCONCbits. = 1; } while(0)
#define IO_RC6_SetAnalogMode()      do { ANSELCbits.ANSC6 = 1; } while(0)
#define IO_RC6_SetDigitalMode()     do { ANSELCbits.ANSC6 = 0; } while(0)
// get/set IO_RC7 aliases
#define IO_RC7_TRIS                 TRISCbits.TRISC7
#define IO_RC7_LAT                  LATCbits.LATC7
#define IO_RC7_PORT                 PORTCbits.RC7
#define IO_RC7_WPU                  WPUCbits.WPUC7
#define IO_RC7_OD                   ODCONCbits.
#define IO_RC7_ANS                  ANSELCbits.ANSC7
#define IO_RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define IO_RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define IO_RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define IO_RC7_GetValue()           PORTCbits.RC7
#define IO_RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define IO_RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define IO_RC7_SetPullup()          do { WPUCbits.WPUC7 = 1; } while(0)
#define IO_RC7_ResetPullup()        do { WPUCbits.WPUC7 = 0; } while(0)
#define IO_RC7_SetPushPull()        do { ODCONCbits. = 0; } while(0)
#define IO_RC7_SetOpenDrain()       do { ODCONCbits. = 1; } while(0)
#define IO_RC7_SetAnalogMode()      do { ANSELCbits.ANSC7 = 1; } while(0)
#define IO_RC7_SetDigitalMode()     do { ANSELCbits.ANSC7 = 0; } while(0)
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