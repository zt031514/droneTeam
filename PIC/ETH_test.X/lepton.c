/* 
 * Search & Rescue Drone
 * Data Acquisition Unit Firmware
 *  
 * Members: Cole Hinton, Zachary Tumbleson, Richard McGinnis, & Jay Elliot
 * 
 * File: lepton.c
 * 
 * Date Created: 1/14/19
 * 
 * Revision Date: 2/11/19
 * 
 * Revision: 1.00
 */

#include "lepton.h"
#include "serial_i2c.h"


// ************************************************************
//
//  This function initializes the necessary settings within
//  the FLIR Lepton Thermal Camera through the I2C bus. Due
//  to many of the settings needed being default within the
//  Lepton like Telemetry disabled, Radiometry disabled, and 
//  it being unnecessary to run Far Field Correction (FFC),
//  the only AGC feature needs to be enabled on the Lepton.
//
// ************************************************************
void lepton_initialize(){
    
    //Enable the AGC - CMH
    i2c_start();
    i2c_write(0x2A);    //Device Address - CMH
    i2c_write(0x01);    //AGC Address - CMH
    i2c_write(0x05);    //Write Command - CMH
    i2c_write(0x00);    //Enable AGC with 8-bit compression (1) - CMH
    i2c_write(0x01);    //Enable AGC with 8-bit compression (2) - CMH
    i2c_stop();
    
    //Telemetry is default disabled - CMH
    
    //Radiometry is default diabled - CMH
    
    //FFC does not need to run - CMH
}
