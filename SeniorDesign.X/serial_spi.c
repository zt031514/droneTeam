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

// ************************************************************
//
//  This function initializes the SPI on SSP2 for use between
//  the PIC and the ETH WIZ Click Module. It sets the PIC to
//  be the master, to send a clock of 32 MHz, and to run in
//  SPI Mode 0. It also clears the respective interrupt 
//  flag.
//
// ************************************************************
void SPI_ETHWIZ_Initialize(void) {
    
    SSP2STAT = 0x40;    //not done
    SSP2CON1 = 0x20;    //not done
    
    SSP2IF = 0;         //Clear interrupt flag - CMH
  
}

// ************************************************************
//
//  This function writes data to the ETH WIZ through the SPI bus
//  on SSP2. It takes in an unsigned 8bit integer and immediately
//  puts the value into the buffer to be sent. The program then
//  waits for the interrupt flag to be set, signaling the 
//  transmission is complete. The flag is then cleared. This
//  function also doubles as a SPI Read function because the
//  new value in the buffer that came from the ETH WIZ is returned
//  from the function as an unsigned 8bit integer.
//
// ************************************************************
uint8_t SPI_ETHWIZ_Write(uint8_t data) {
        
    SSP2BUF = data;             //Store value to be sent in buffer - CMH
    while(!SSP2IF) continue;    /*Wait for Interrupt Flag to signal 
                                  transmission complete - CMH*/
    SSP2IF = 0;                 //Clear interrupt flag - CMH
    return(SSP2BUF);            //Return received value - CMH
    
}

// ************************************************************
//
//  This function initializes the SPI bus on SSP1 to work with
//  the FLIR Lepton thermal camera. It sets the PIC to be the 
//  master, to send a clock of 8 MHz, and to run in SPI mode 3.
//  The respective interrupt flag is also cleared.
//
// ************************************************************
void SPI_FLIR_Initialize(void) {
    
    SSP1STAT = 0x00;        //Comment not done - CMH
    SSP1CON1 = 0x30;        //Comment not done - CMH
    
    SSP1IF = 0;             //Clear interrupt flag - CMH
  
}

// ************************************************************
//
//  This function initializes the SPI bus on SSP1 to work with
//  the Arducam visual camera. It sets the PIC to be the 
//  master, to send a clock of 8 MHz, and to run in SPI mode 0.
//  The respective interrupt flag is also cleared.
//
// ************************************************************
void SPI_Arducam_Initialize(void) {
    
    SSP1STAT = 0x40;        //Comment not done - CMH
    SSP1CON1 = 0x20;        //Comment not done - CMH
            
    SSP1IF = 0;             //Clear interrupt flag - CMH
  
}

// ************************************************************
//
//  This function is the SPI write function for the SPI bus on
//  SSP1. It takes in an unsigned 8bit integer and immediately
//  puts the value into the buffer to be sent. The program then
//  waits for the interrupt flag to be set, signaling the 
//  transmission is complete. The flag is then cleared. This
//  function also doubles as a SPI Read function because the
//  new value in the buffer that came from the slave is returned
//  from the function as an unsigned 8bit integer.
//
// ************************************************************
uint8_t SPI_Write(uint8_t data) {
    
    SSP1BUF = data;             //Store value to be sent in buffer - CMH
    while(!SSP1IF) continue;    /*Wait for Interrupt Flag to signal 
                                  transmission complete - CMH*/
    SSP1IF = 0;                 //Clear the interrupt flag - CMH
    return(SSP1BUF);            //Return received value - CMH
    
}


