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

// Initialization of the I2C
void i2c_initialize(void) {
    
    SSP2CON1 = 0x28;        //ON with Master Mode
    SSP2CON2 = 0x00;        //Clear Flags
    SSP2ADD = 0x09;         //100 kHz with 4MHz clock
    SSP2STATbits.CKE = 0;   //Use I2C signal levels
    SSP2STATbits.SMP = 1;   //Standard Speed Mode
}

void i2c_waitForIdle()
{
    while((SSP2CON2 & 0x1F)| SSP2STATbits.R_nW){};
}

void i2c_start()
{
    //Wait for Idle
    i2c_waitForIdle();
    
    //Send Start Condition
    SSP2CON2bits.SEN = 1;
}

void i2c_repStart()
{
    //wait for Idle
    i2c_waitForIdle();
    
    //Send Restart Condition
    SSP2CON2bits.RSEN = 1;
}

void i2c_stop()
{
    //wait for Idle
    i2c_waitForIdle();
    
    //Send Stop condition
    SSP2CON2bits.PEN = 1;
}

uint8_t i2c_read( uint8_t ack )
{
    
    uint8_t readData;
    
    i2c_waitForIdle();      //Wait for Idle
    
    SSP2CON2bits.RCEN = 1;  //Starts receiver
    
    i2c_waitForIdle();      //Wait for Idle
    
    readData = SSP1BUF;     //Get the data
    
    i2c_waitForIdle();      //Wait for Idle
    
    if (ack){
        SSP2CON2bits.ACKDT = 0; //Acknowledge low for more data
    }
    else{
        SSP2CON2bits.ACKDT = 1; //Acknowledge high after final byte
    }
    
    SSP2CON2bits.ACKEN = 1; //Send Acknowledge sequence
    
    return(readData);
    
}

uint8_t i2c_write( uint8_t i2cWriteData )
{
    //Wait for Idle
    i2c_waitForIdle();
    
    //Write to Buffer to start transmission
    SSP2BUF = i2cWriteData;
    
    //Returns 1 if transmission acknowledge
    return(!SSP2CON2bits.ACKSTAT);
    
}