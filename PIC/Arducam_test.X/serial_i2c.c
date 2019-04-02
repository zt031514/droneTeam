/* 
 * Search & Rescue Drone
 * Data Acquisition Unit Firmware
 *  
 * Members: Cole Hinton, Zachary Tumbleson, Richard McGinnis, & Jay Elliot
 * 
 * File: serial_i2c.c
 * 
 * Date: 1/14/19
 * 
 * Revision: 1.00
 */

#include "serial_i2c.h"


// ************************************************************
//
//  This function initializes the I2C bus on the PIC. I2C 
//  will be used only in the first portion of the program
//  to set the required settings on both the FLIR Lepton
//  thermal camera and the Arducam 2MP visual camera.
//
// ************************************************************
void i2c_initialize(void) {
    
    SSP2CON1 = 0x28;        //ON with Master Mode - CMH
    SSP2CON2 = 0x00;        //Clear Flags - CMH
    SSP2ADD = 0x4F;         //100 kHz with 32MHz clock - CMH
    SSP2STATbits.CKE = 0;   //Use I2C signal levels - CMH
    SSP2STATbits.SMP = 1;   //Standard Speed Mode - CMH
}

// ************************************************************
//
//  This function is responsible for waiting for the I2C data
//  line to idle. It is used in many of the I2C functions 
//  written below.
//
// ************************************************************
void i2c_waitForIdle()
{
    //Waits for the I2C data line to idle - CMH
    while((SSP2CON2 & 0x1F)| SSP2STATbits.R_nW){};
}

// ************************************************************
//
//  This function starts the I2C communication. It waits for
//  an idle data line and then sends the start condition.
//
// ************************************************************
void i2c_start()
{
    //Wait for Idle - CMH
    i2c_waitForIdle();
    
    //Send Start Condition - CMH
    SSP2CON2bits.SEN = 1;
}

// ************************************************************
//
//  This function sends a repeat start signal for the I2C bus.
//  It functions as a simultaneous stop then start signal. It
//  waits for I2C to idle then send the repeat start condition.
//
// ************************************************************
void i2c_repStart()
{
    //Wait for Idle - CMH
    i2c_waitForIdle();
    
    //Send Restart Condition - CMH
    SSP2CON2bits.RSEN = 1;
}

// ************************************************************
//
//  This function stops transmission on the I2C bus. It waits
//  for idle then sends the stop condition.
//
// ************************************************************
void i2c_stop()
{
    //Wait for Idle - CMH
    i2c_waitForIdle();
    
    //Send Stop condition - CMH
    SSP2CON2bits.PEN = 1;
}

// ************************************************************
//
//  This function reads in data being sent from a slave over
//  the I2C data line. Takes in an unsigned 8bit integer variable 
//  that that should be either a 0 when it is the last byte
//  to be received or a 1 when there are still more bytes to
//  be received. The received data is returned from the function
//  as an unsigned 8bit integer.
//
// ************************************************************
uint8_t i2c_read( uint8_t ack )
{
    
    uint8_t readData;           //Variable for received data - CMH
    
    i2c_waitForIdle();          //Wait for Idle - CMH
    
    SSP2CON2bits.RCEN = 1;      //Enables data reception for a byte - CMH
    
    i2c_waitForIdle();          //Wait for Idle - CMH
    
    readData = SSP1BUF;         //Read in the data from the buffer - CMH
    
    i2c_waitForIdle();          //Wait for Idle - CMH
    
    //If the not last byte of data, then... else... - CMH
    if (ack){
        SSP2CON2bits.ACKDT = 0; //Acknowledge low for more data - CMH
    }
    else{
        SSP2CON2bits.ACKDT = 1; //Acknowledge high after final byte - CMH
    }
    
    SSP2CON2bits.ACKEN = 1;     //Send Acknowledge sequence - CMH
    
    return(readData);           //Return the received data - CMH
    
}


// ************************************************************
//
//  This function writes data to a slave device through the I2C
//  data line. It takes in an unsigned 8bit integer value, waits
//  for idle, then writes the value to the buffer. The function
//  returns the current state of the acknowledge bit.
//
// ************************************************************
uint8_t i2c_write( uint8_t i2cWriteData )
{
    //Wait for Idle - CMH
    i2c_waitForIdle();
    
    //Write to Buffer to start transmission - CMH
    SSP2BUF = i2cWriteData;
    
    //Returns 1 if transmission is acknowledged - CMH
    return(!SSP2CON2bits.ACKSTAT);
    
}