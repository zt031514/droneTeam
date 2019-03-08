/* 
 * Search & Rescue Drone
 * Data Acquisition Unit Firmware
 *  
 * Members: Cole Hinton, Zachary Tumbleson, Richard McGinnis, & Jay Elliot
 * 
 * File: serial_uart.c
 * 
 * Date: 1/14/19
 * 
 * Revision: 0.01
 */

#include "serial_uart.h"

// ************************************************************
//
//  This function initializes the UART that is used to 
//  communicate between the GNSS 5 Click module and the PIC.
//  The UART bus is set up to
//
//      COMMENT NOT COMPLETE
//
// ************************************************************
void UART_Initialize(void) {
    
    //Initialize UART for Asynchronous 8-bit receive and transmission
    BAUD1CON = 0x08;
    TX1STA = 0x24;
    RC1STA = 0x90;
    
    //Set Baud rate - 115.2k
    SP1BRGL = 0x44;//0x44;    
    SP1BRGH = 0x00;

    //Clear Interrupt flags - CMH
    PIR3bits.RCIF = 0;

}

// ************************************************************
//
//  This function reads in data through UART. It first checks
//  to make sure all the various flag are set and looks for an
//  over run error. It then receives the byte of data and clears
//  the necessary flags. The received data is then returned from
//  the function as an unsigned 8bit integer.
//
// ************************************************************
uint8_t UART_Read(void) {
    
    //Variable for received byte - CMH
    uint8_t c;

    //Wait until the receive interrupt flag is set - CMH
    while(!(PIR3bits.RCIF));
    
    if(RC1STAbits.OERR) // check for over run error  - CMH
    {
        RC1STAbits.CREN = 0;
        RC1STAbits.CREN = 1; //Reset CREN - CMH
    }
    //Clear interrupt on change flag - CMH
    IOCCFbits.IOCCF0 = 0;
    
    //Read byte from RC1REG and clear receive interrupt
    // flag if receive interrupt flag is set - CMH
    if(PIR3bits.RCIF == 1)
    {
       c = RC1REG;          //Save received byte in variable - CMH
       PIR3bits.RCIF = 0;   //Clear receive interrupt flag - CMH
    }
    
    //Return received character - CMH
    return c;
    
}

// ************************************************************
//
//  This function acts as the write command to be used with
//  printf. It takes in a unsigned 8bit integer as the value
//  to be sent. It then waits until both the transmission 
//  interrupt flag and TRMT are set. The byte is stored in
//  the Transmission register to be sent.
//
// ************************************************************
void putch(unsigned char byte)  
{
    //Wait while TXIF and TRMT both are not set - CMH
    while(!(PIR3bits.TXIF) && !(TX1STAbits.TRMT)); 
    
    //Set TX1REG equal to byte - CMH
    TX1REG = byte;
}
