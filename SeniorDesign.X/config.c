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


#include "config.h"

// ************************************************************
//
//  This function setups oscillator to function at 32MHz.
//
// ************************************************************
void OSCILLATOR_Initialize(void) {
    OSCCON1 = 0x60;     // NOSC HFINTOSC; NDIV 1; 
    OSCFRQ = 0x06;      // Fosc set to 32MHz
}


// ************************************************************
//
//  This function is the first Initialization function for 
//  the port pins. Within all pins are set to be digital, set
//  to be either an input or an output, and the various serial
//  protocols have pins mapped for use. The necessary interrupts
//  are enabled and their respective flags are cleared.
//
// ************************************************************
void PORT_PIN_Initialize1(void) {
    
    //Set all pins of the microcontroller to digital pins - CMH
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
    
    //Initially set all pins to 0 (low) state - CMH 
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    //Turn off the latch for all pins - CMH
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;
    
    //Turn off interrupts to allow for the unlocking sequence below - CMH
    PIE3bits.RCIE = 0;
    INTCONbits.PEIE = 0;
    INTCONbits.GIE = 0;
    PIE0bits.IOCIE = 0;
    
    //Unlock Peripheral Pin Select (PPS) using the unlock sequence - CMH
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0;
    
    //Sets up the pins for UART between PIC and GNSS 5 - CMH
    RC6PPS = 0x10;
    RXPPS = 0x17; 
    
    //Sets up the pins for SPI on SSP1 between PIC and Cameras - CMH 
    SSP1DATPPS = 0x11;  
    RB7PPS = 0x14;      
    RC1PPS = 0x15;      
        
    //Sets up the pins for I2C on SSP2 between PIC and Cameras - CMH
    SSP2DATPPS = 0x14;  
    RC3PPS = 0x16;      
    RC4PPS = 0x17;      
    
    //Lock PPS using the locking sequence - CMH
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 1;
   
    //Re-enable the necessary interrupts - CMH
    PIE3bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    PIE0bits.IOCIE = 1;
 
    //Clear all the interrupt flags - CMH
    PIR3 = 0;               
}

// ************************************************************
//
//  This function is the second Initialization function for 
//  the port pins. Within all pins are set to be digital, set
//  to be either an input or an output, and the various serial
//  protocols have pins mapped for use. The necessary interrupts
//  are re-enabled and their respective flags are cleared.
//
// ************************************************************
void PORT_PIN_Initialize2(void) {
    
    //Turn off the interrupts to allow for the PPS unlocking sequence - CMH
    PIE3bits.RCIE = 0;
    INTCONbits.PEIE = 0;
    INTCONbits.GIE = 0;
    PIE0bits.IOCIE = 0;
    
    //Unlock PPS using the unlocking sequence - CMH
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0;
    
    //Set up the pins for SPI on SSP2 between PIC and ETH WIZ - CMH
    //Replaces the I2C that was previously on SSP2 - CMH
    SSP2DATPPS = 0x0A;  
    RB1PPS = 0x16;      
    RB3PPS = 0x17;      
     
    //Lock the PPS using the locking sequence - CMH
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 1;
   
    //Re-enable the necessary interrupts - CMH
    PIE3bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    PIE0bits.IOCIE = 1;
    
    //Clear all the respective interrupt flags - CMH
    PIR3 = 0;              
}
