/* 
 * Search & Rescue Drone
 * Data Acquisition Unit Firmware
 *  
 * Members: Cole Hinton, Zachary Tumbleson, Richard McGinnis, & Jay Elliot
 * 
 * File: config.c
 * 
 * Date: 1/14/19
 * 
 * Revision: 1.00
 */

// ---------------------------------------------------------------------------
//
//      Source file containing the configuration subroutines
//
// ---------------------------------------------------------------------------

#include "config.h"

// Setup oscillator to 32MHz
void OSCILLATOR_Initialize(void) {
    // NOSC HFINTOSC; NDIV 1; 
    OSCCON1 = 0x60;
    OSCFRQ = 0x06;
}

// For Camera Setup
void PORT_PIN_Initialize1(void) {
    
    //Set all to digital
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    ANSELD = 0x00;
    ANSELE = 0x00;
    
    //not done
    TRISA = 0x00;
    TRISB = 0b01100100;
    TRISC = 0b10011000;
    TRISD = 0x00;
    TRISE = 0x00;
    
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;
    
    //turn off interrupts for PPS
    PIE3bits.RCIE = 0;
    INTCONbits.PEIE = 0;
    INTCONbits.GIE = 0;
    PIE0bits.IOCIE = 0;
    
    //Unlock PPS
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0;
    
    //UART for GNSS 5
    RC6PPS = 0x10;
    RXPPS = 0x17; 
    
    //SPI on SSP1 for Cameras
    SSP1DATPPS = 0x11;  
    RB7PPS = 0x14;      
    RC1PPS = 0x15;      
        
    //I2C on SSP2 for Cameras
    SSP2DATPPS = 0x14;  
    RC3PPS = 0x16;      
    RC4PPS = 0x17;      
    
    //Lock PPS
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 1;
   
    //Re-enable interrupts
    PIE3bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    PIE0bits.IOCIE = 1;
    
    PIR3 = 0;               //Clear Flags
}

// For Data Acquisition
void PORT_PIN_Initialize2(void) {
    
    //turn off interrupts for PPS
    PIE3bits.RCIE = 0;
    INTCONbits.PEIE = 0;
    INTCONbits.GIE = 0;
    PIE0bits.IOCIE = 0;
    
    //Unlock PPS
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0;
    
    //SPI on SSP2 for ETH WIZ
    SSP2DATPPS = 0x0A;  
    RB1PPS = 0x16;      
    RB3PPS = 0x17;      
     
    //Lock PPS
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 1;
   
    //Re-enable interrupts
    PIE3bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    PIE0bits.IOCIE = 1;
    
    PIR3 = 0;               //Clear Flags
}
