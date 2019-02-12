/* 
 * Search & Rescue Drone
 * Data Acquisition Unit Firmware
 *  
 * Members: Cole Hinton, Zachary Tumbleson, Richard McGinnis, & Jay Elliot
 * 
 * File: serial_spi.c
 * 
 * Date: 1/14/19
 * 
 * Revision: 1.0
 */

#include "serial_spi.h"

//Done - CMH
void SPI_ETHWIZ_Initialize(void) {
    
    SSP2STAT = 0x40;
    SSP2CON1 = 0x20;
    
    SSP2IF = 0;
  
}

//Done - CMH
uint8_t SPI_ETHWIZ_Write(uint8_t data) {
    
    SSP2BUF = data;
    while(!SSP2IF) continue;
    SSP2IF = 0;
    return(SSP2BUF);
    
}

//Done - CMH
void SPI_FLIR_Initialize(void) {
    
    SSP1STAT = 0x00;
    SSP1CON1 = 0x30;
    
    SSP1IF = 0;
  
}

//Done - CMH
void SPI_Arducam_Initialize(void) {
    
    SSP1STAT = 0x40;
    SSP1CON1 = 0x20;
    
    SSP1IF = 0;
  
}

//Done - CMH
uint8_t SPI_Write(uint8_t data) {
    
    SSP1BUF = data;
    while(!SSP1IF) continue;
    SSP1IF = 0;
    return(SSP1BUF);
    
}


