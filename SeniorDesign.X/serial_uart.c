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

// Initialization of the UART
void UART_Initialize(void) {
    
    //Initialize UART for Asynchronous 8-bit receive and transmission
    BAUD1CON = 0x08;
    TX1STA = 0x24;
    RC1STA = 0x90;
    
    //Set Baud rate
    SP1BRGL = 0x40;    
    SP1BRGH = 0x03;

}

// Read from the UART
uint8_t UART_Read(void) {
    
    //Put character
    uint8_t c;

    //Wait until RCIF is 1 
    while(!(PIR3bits.RCIF));
    
    if(RC1STAbits.OERR) // check for over run error 
    {
        RC1STAbits.CREN = 0;
        RC1STAbits.CREN = 1; //Reset CREN
    }
    //Clear interrupt flag
    IOCCFbits.IOCCF0 = 0;
    
    //Read CHAR from RC1REG and clear RCIF
    // flag when RCIF equals 1
    if(PIR3bits.RCIF == 1)
    {
       c = RC1REG;
       PIR3bits.RCIF = 0; 
    }
    
    //Return received character
    return c;
    
}

//Used by printf. Sends byte to TX1REG for transmission
void putch(unsigned char byte)  
{
    //Wait while TXIF and TRMT both equal zero
    while(!(PIR3bits.TXIF) && !(TX1STAbits.TRMT)); 
    
    //Set TX1REG equal to byte
    TX1REG = byte;
}
