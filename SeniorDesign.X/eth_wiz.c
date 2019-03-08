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
 * Revision Date: 3/5/19
 * 
 * Revision: 0.04
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "config.h"
#include "eth_wiz.h"
#include "serial_spi.h"

//Control Phase value for block and R/W selection in ETH WIZ - CMH
uint8_t common_reg_write = 0b00000100;
uint8_t common_reg_read  = 0b00000000;
uint8_t s0_reg_write     = 0b00001100;
uint8_t s0_reg_read      = 0b00001000;
uint8_t s0_tx_buf_write  = 0b00010100;
uint8_t s0_tx_buf_read   = 0b00010000;
uint8_t s0_rx_buf_write  = 0b00011100;
uint8_t s0_rx_buf_read    = 0b00011000;

//Variables for pointer registers of TX and RX - CMH
uint8_t s0_tx_pointer_high = 0x00;
uint8_t s0_tx_pointer_low = 0x00;
uint8_t s0_rx_readpointer_high = 0x00;
uint8_t s0_rx_readpointer_low = 0x00;
uint8_t s0_rx_writepointer_high = 0x00;
uint8_t s0_rx_writepointer_low = 0x00;
uint8_t s0_rx_datalength_high = 0x00;
uint8_t s0_rx_datalength_low = 0x00;
uint8_t s0_SR_check = 0x00;
uint8_t s0_IR_check = 0x00;


// ************************************************************
//
//  This function configures the proper settings on the W5500
//  (the chipset on the ETH WIZ Click). See comments below to 
//  see the exact settings for each register. Overall setup is
//  for TCP through Socket 0.
//
// ************************************************************
void eth_wiz_configure(void){
    
    /*
     * Start of the Common Registers set up
     */
    
    //Mode Register - Wake on LAN disabled, Pinf Block Mode enabled,
    //PPPoE mode disabled, and Force ARP disabled - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x00); //Address = 0x0000
    SPI_ETHWIZ_Write(common_reg_write);
    SPI_ETHWIZ_Write(0b00010000);
    PORTDbits.RD3 = 1;
    __delay_ms(1);
    
    //Set the Gateway IP Address to 10.0.0.254 - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x01); //Start Address = 0x0001
    SPI_ETHWIZ_Write(common_reg_write);
    SPI_ETHWIZ_Write(0x0A); //10
    SPI_ETHWIZ_Write(0x00); //0
    SPI_ETHWIZ_Write(0x00); //0
    SPI_ETHWIZ_Write(0xFE); //254
    PORTDbits.RD3 = 1;
    __delay_ms(1);

    //Set the Subnet Mask address to 255.255.0.0 - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x05); //Start Address = 0x0005
    SPI_ETHWIZ_Write(common_reg_write);
    SPI_ETHWIZ_Write(0xFF); //255
    SPI_ETHWIZ_Write(0xFF); //255
    SPI_ETHWIZ_Write(0x00); //0
    SPI_ETHWIZ_Write(0x00); //0
    PORTDbits.RD3 = 1;
    __delay_ms(1);
    
    //Set the Source Hardware Address address to 00.21.74.08.36.85 - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x09); //Start Address = 0x0009
    SPI_ETHWIZ_Write(common_reg_write);
    SPI_ETHWIZ_Write(0x00); //00
    SPI_ETHWIZ_Write(0x21); //21
    SPI_ETHWIZ_Write(0x74); //74
    SPI_ETHWIZ_Write(0x08); //08
    SPI_ETHWIZ_Write(0x36); //36
    SPI_ETHWIZ_Write(0x85); //85
    PORTDbits.RD3 = 1;
    __delay_ms(1);
    
    //Set the Source IP address to 10.0.0.2 - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x0F); //Start Address = 0x000F
    SPI_ETHWIZ_Write(common_reg_write);
    SPI_ETHWIZ_Write(0x0A); //10
    SPI_ETHWIZ_Write(0x00); //0
    SPI_ETHWIZ_Write(0x00); //0
    SPI_ETHWIZ_Write(0x02); //2
    PORTDbits.RD3 = 1;
    __delay_ms(1);
    
    //Set the Destination unreachable interrupt mask to enabled - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x15); //Address = 0x0015
    SPI_ETHWIZ_Write(common_reg_write);
    SPI_ETHWIZ_Write(0b01000000);
    PORTDbits.RD3 = 1;
    __delay_ms(1);
    
    //Enable the Socket 0 and disable all other sockets - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x18); //Address = 0x0018
    SPI_ETHWIZ_Write(common_reg_write);
    SPI_ETHWIZ_Write(0b00000001);
    PORTDbits.RD3 = 1;
    __delay_ms(1);
    
    //Set Retry Count Register to 0 (=RCR+1) - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x1B); //Address = 0x001B
    SPI_ETHWIZ_Write(common_reg_write);
    SPI_ETHWIZ_Write(0b00000000);
    PORTDbits.RD3 = 1;
    __delay_ms(1);
    
    //Set the Physical Operation Mode to all capable - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x2E); //Address = 0x002E
    SPI_ETHWIZ_Write(common_reg_write);
    SPI_ETHWIZ_Write(0b10111000);
    PORTDbits.RD3 = 1;
    __delay_ms(1);
    
    
    /*
     * Start of the Socket 0 (S0) Registers set up
     */
    
    //Set S0 Mode Register to enable Multicasting and sets 
    //the protocol mode to TCP - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x00); //Address = 0x0000
    SPI_ETHWIZ_Write(s0_reg_write);
    SPI_ETHWIZ_Write(0b10000001);
    PORTDbits.RD3 = 1;
    __delay_ms(1);
        
    //Set the S0 Source Port Register to d'10,000' - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x04); //Address = 0x0004
    SPI_ETHWIZ_Write(s0_reg_write);
    SPI_ETHWIZ_Write(0x27);
    SPI_ETHWIZ_Write(0x10); //10,000 = 0x2710
    PORTDbits.RD3 = 1;
    __delay_ms(1);

    //Set the S0 Destination IP Address Register to 10.0.0.1 - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x0C); //Address = 0x000C
    SPI_ETHWIZ_Write(s0_reg_write);
    SPI_ETHWIZ_Write(0x0A); //10
    SPI_ETHWIZ_Write(0x00); //0
    SPI_ETHWIZ_Write(0x00); //0
    SPI_ETHWIZ_Write(0x01); //1
    PORTDbits.RD3 = 1;
    __delay_ms(1);
    
    //Set the S0 Destination Port Register to d'10,000' - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x10); //Address = 0x0010
    SPI_ETHWIZ_Write(s0_reg_write);
    SPI_ETHWIZ_Write(0x27);
    SPI_ETHWIZ_Write(0x10); //10,000 = 0x2710
    PORTDbits.RD3 = 1;
    __delay_ms(1);
       
    //Set the S0 Max Segment Size Register to d'1460' - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x12); //Address = 0x0012
    SPI_ETHWIZ_Write(s0_reg_write);
    SPI_ETHWIZ_Write(0x05);
    SPI_ETHWIZ_Write(0xB4); //1,460 = 0x05B4
    PORTDbits.RD3 = 1;
    __delay_ms(1);
    
    //Set the S0 Fragment Register to d'0' to not allow for IP Fragments - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x2D); //Address = 0x002D
    SPI_ETHWIZ_Write(s0_reg_write);
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x00); //0 = 0x0000
    PORTDbits.RD3 = 1;
    __delay_ms(1);
    
    //Set the S0 Keep Alive Time Register to d'10' - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x2F); //Address = 0x002F
    SPI_ETHWIZ_Write(s0_reg_write);
    SPI_ETHWIZ_Write(0x0A); //10 = 0x0A
    PORTDbits.RD3 = 1;
    __delay_ms(1);
}


// ************************************************************
//
//  This function goes through the sequence to create the 
//  Socket 0. Will open and then listen.
//
// ************************************************************
void eth_wiz_createSocket(void){
    uint8_t connected = 0;
    do{
        //Open the Socket 0 - CMH
        PORTDbits.RD3 = 0;
        SPI_ETHWIZ_Write(0x00);
        SPI_ETHWIZ_Write(0x01); //Address = 0x0001
        SPI_ETHWIZ_Write(s0_reg_write);
        SPI_ETHWIZ_Write(0x01); //OPEN = 0x01
        PORTDbits.RD3 = 1;
        __delay_ms(1);

        //check if the socket is open - CMH
        do{
            PORTDbits.RD3 = 0;
            SPI_ETHWIZ_Write(0x00);
            SPI_ETHWIZ_Write(0x03); //Address = 0x0001
            SPI_ETHWIZ_Write(s0_reg_read);
            s0_SR_check = SPI_ETHWIZ_Write(0x00); 
            PORTDbits.RD3 = 1;
        }while(s0_SR_check != 0x13); //SR needs to equal SOCK_INIT

        //Order the socket to listen for a connection request from
        //the "client" - CMH
        PORTDbits.RD3 = 0;
        SPI_ETHWIZ_Write(0x00);
        SPI_ETHWIZ_Write(0x01); //Address = 0x0001
        SPI_ETHWIZ_Write(s0_reg_write);
        SPI_ETHWIZ_Write(0x02); //LISTEN = 0x02
        PORTDbits.RD3 = 1;
        __delay_ms(1);

        //check if the socket is listening for a client - CMH
        do{
            PORTDbits.RD3 = 0;
            SPI_ETHWIZ_Write(0x00);
            SPI_ETHWIZ_Write(0x03); //Address = 0x0003
            SPI_ETHWIZ_Write(s0_reg_read);
            s0_SR_check = SPI_ETHWIZ_Write(0x00); 
            PORTDbits.RD3 = 1;
        }while(s0_SR_check != 0x13); //SR needs to equal SOCKET_LISTEN - CMH

        while(PORTBbits.RB0 != 0) continue; //until INTn is asserted - CMH

        //Read in the S0_IR and S0_SR register for interrupt checking - CMH
        PORTDbits.RD3 = 0;
        SPI_ETHWIZ_Write(0x00);
        SPI_ETHWIZ_Write(0x02); //Address = 0x0002
        SPI_ETHWIZ_Write(s0_reg_read);
        s0_IR_check = SPI_ETHWIZ_Write(0x00);   //0x0002
        s0_SR_check = SPI_ETHWIZ_Write(0x00);   //0x0003
        PORTDbits.RD3 = 1;
    
        //if Timed out and socket closed - CMH
        if((s0_IR_check & 0x08) == 0x08 && s0_SR_check == 0x00){
            //Clear (by writing a 1) the time out flag 
            //to de-assert the INTn Pin - CMH
            PORTDbits.RD3 = 0;
            SPI_ETHWIZ_Write(0x00);
            SPI_ETHWIZ_Write(0x02); //Address = 0x0002
            SPI_ETHWIZ_Write(s0_reg_write);
            SPI_ETHWIZ_Write(0x08);
            PORTDbits.RD3 = 1;
            
            connected = 0;
        }
        //if successfully connected and sock_established - CMH
        else if((s0_IR_check & 0x01) == 0x01 && s0_SR_check == 0x17){
            //Clear (by writing a 1) the tconnection flag 
            //to de-assert the INTn Pin - CMH
            PORTDbits.RD3 = 0;
            SPI_ETHWIZ_Write(0x00);
            SPI_ETHWIZ_Write(0x02); //Address = 0x0002
            SPI_ETHWIZ_Write(s0_reg_write);
            SPI_ETHWIZ_Write(0x01);
            PORTDbits.RD3 = 1;
            
            //Set the connected flag to exit loop and function - CMH
            connected = 1;
        }
        
        
    }while(connected == 0);
      
    
}


// ************************************************************
//
//  This function starts the transmission sequence for the 
//  ETH WIZ. It finds the current location of the TX buffer
//  pointer and then starts the write to buffer sequence at
//  the address of the pointer. Data can be written to the
//  buffer after calling this function.
//
// ************************************************************
void eth_wiz_transmit_start(void){
    //Read in the starting tx pointer address - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x24); //Address = 0x0024
    SPI_ETHWIZ_Write(s0_reg_read);
    s0_tx_pointer_high = SPI_ETHWIZ_Write(0x00);
    s0_tx_pointer_low  = SPI_ETHWIZ_Write(0x00);
    PORTDbits.RD3 = 1;
    PORTDbits.RD3 = 1;
    
    //Start the write to buffer sequence - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(s0_tx_pointer_high);           //Address high
    SPI_ETHWIZ_Write(s0_tx_pointer_low);            //Address low
    SPI_ETHWIZ_Write(s0_tx_buf_write);              //TX Write Command
    
    //data can now be written to TX buffer in main program - CMH
}


// ************************************************************
//
//  This function ends the transmission sequence into the TX
//  buffer and the initiates the SEND sequence by calling the
//  SEND command. After ending the TX write sequence, the TX
//  pointer is updated to its new value by adding the number
//  of written bytes to the previous value of the pointer.
//  After that, the SEND command is sent.
//
// ************************************************************
void eth_wiz_transmit_end(void){
    //De-assert RD3 to stop transmission into TX buffer - CMH
    PORTDbits.RD3 = 1;
    
    //combined high and low bytes of tx pointer and add
    //the number of bytes added to buffer - CMH
    uint16_t value = (s0_tx_pointer_high << 8) | s0_tx_pointer_low;
    value = (value + 164) & (0xFFFF);
    s0_tx_pointer_high = (value & 0xFF00) >> 8;
    s0_tx_pointer_low  = value & 0x00FF;
    
    //Update the pointer address for the amount of data added - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x24); //Address = 0x0024
    SPI_ETHWIZ_Write(s0_reg_write);
    SPI_ETHWIZ_Write(s0_tx_pointer_high);
    SPI_ETHWIZ_Write(s0_tx_pointer_low);
    PORTDbits.RD3 = 1;
    PORTDbits.RD3 = 1;
    
    //Initiate the send command in the S0_CR Register - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x01); //Address = 0x0024
    SPI_ETHWIZ_Write(s0_reg_write);
    SPI_ETHWIZ_Write(0x20);   //SEND = 0x20
    PORTDbits.RD3 = 1;
    
}


// ************************************************************
//
//  This function performs the receive data sequence. Will only
//  be called after a receive interrupt is sent from the ETH
//  WIZ.
//
// ************************************************************
void eth_wiz_receive(void){
    //Read in Received Data Size Register (0x0026 - 0x0027),
    //the RX Read Data Pointer Register (0x0028 - 0x0029), and
    //the RX Write pointer Register (0x002A - 0x002B) - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x26); //Starting Address = 0x0026
    SPI_ETHWIZ_Write(s0_reg_read);
    s0_rx_datalength_high = SPI_ETHWIZ_Write(0x00);         //0x0026
    s0_rx_datalength_low  = SPI_ETHWIZ_Write(0x00);         //0x0027
    s0_rx_readpointer_high = SPI_ETHWIZ_Write(0x00);        //0x0028
    s0_rx_readpointer_low  = SPI_ETHWIZ_Write(0x00);        //0x0029
    s0_rx_writepointer_high = SPI_ETHWIZ_Write(0x00);       //0x002A
    s0_rx_writepointer_low  = SPI_ETHWIZ_Write(0x00);       //0x002B
    PORTDbits.RD3 = 1;
    
    //determine the amount of bytes to read in from RX buffer - CMH
    uint16_t datalength = (s0_rx_datalength_high << 8) | s0_rx_datalength_low;
    
    //Read in data from the RX buffer - CMH
    PORTDbits.RD3 = 0;
     //Starting Address = RX Read Data Pointer
    SPI_ETHWIZ_Write(s0_rx_readpointer_high);
    SPI_ETHWIZ_Write(s0_rx_readpointer_low);
    SPI_ETHWIZ_Write(s0_rx_buf_read);
    for(int i = 0; i < datalength; i++){
        //Collect these values into an array - CMH
        SPI_ETHWIZ_Write(0x00);
    }
    PORTDbits.RD3 = 1;
    __delay_us(1);
    
    //Update the Read Pointer to the Write Pointer Address - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x28); //Starting Address = 0x0028
    SPI_ETHWIZ_Write(s0_reg_write);
    SPI_ETHWIZ_Write(s0_rx_writepointer_high);
    SPI_ETHWIZ_Write(s0_rx_writepointer_low);
    PORTDbits.RD3 = 1;
    
    //Order the RECV command to notify W5500 that the data was read in - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x01); //Address = 0x0024
    SPI_ETHWIZ_Write(s0_reg_write);
    SPI_ETHWIZ_Write(0x40);   //RECV = 0x40
    PORTDbits.RD3 = 1;
}

