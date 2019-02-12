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

void arducam_initialize(void){
    
}


void arducam_capture(void){
    
    uint8_t data = SPI_Write(0x00);
    
    
}

void arducam_send(void){
    
}