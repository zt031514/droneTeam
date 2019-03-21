/* 
 * Search & Rescue Drone
 * Data Acquisition Unit Firmware
 *  
 * Members: Cole Hinton, Zachary Tumbleson, Richard McGinnis, & Jay Elliot
 * 
 * File: arducam.c
 * 
 * Date: 1/14/19
 * 
 * Revision: 0.01
 */

#include "arducam.h"
#include "serial_spi.h"

// ************************************************************
//
//  This function initializes the arducam to the desired
//  settings through the I2C bus.
//
// ************************************************************
void arducam_initialize(void){
    
}


// ************************************************************
//
//  This function gets the captures an image from the buffer
//  on the Arducam using the SPI bus.
//
// ************************************************************
void arducam_capture(void){
    
    uint8_t data = SPI_Write(0x00);
    
    
}


// ************************************************************
//
//  This function sends the received image from the Arducam 
//  over the SPI to the ETH WIZ to be sent to the ground 
//  station software.
//
// ************************************************************
void arducam_send(void){
    
}