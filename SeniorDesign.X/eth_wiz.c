/* 
 * Search & Rescue Drone
 * Data Acquisition Unit Firmware
 *  
 * Members: Cole Hinton, Zachary Tumbleson, Richard McGinnis, & Jay Elliot
 * 
 * File: eth_wiz.c
 * 
 * Date Created: 1/14/19
 * 
 * Revision Date: 2/20/19
 * 
 * Revision: 0.02
 */


//Control Phase value for block and R/W selection in ETH WIZ - CMH
uint8_t common_reg_write = 0b00000100;
uint8_t common_reg_read  = 0b00000000;
uint8_t s0_reg_write     = 0b00001100;
uint8_t s0_reg_read      = 0b00001000;
uint8_t s0_tx_buf_write  = 0b00010100;
uint8_t s0_tx_buf_read   = 0b00010000;
uint8_t s0_rx_buf_write  = 0b00011100;
uint8_t s0_rx_bf_read    = 0b00011000;


#include "eth_wiz.h"
#include "serial_spi.h"

void eth_wiz_configure(void){
    
}


void eth_wiz_createSocket(void){
    
}


void eth_wiz_transmit(void){
    
}


void eth_wiz_receive(void){
    
}