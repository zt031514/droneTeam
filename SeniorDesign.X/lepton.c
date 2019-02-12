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


//Done - CMH
void lepton_initialize(){
    //Enable the AGC - CMH
    i2c_start();
    i2c_write(0x2A); //Device Address
    i2c_write(0x01); //AGC Address
    i2c_write(0x05);
    i2c_write(0x00);
    i2c_write(0x01);
    i2c_stop();
    
    //Telemetry is default disabled - CMH
    
    //Radiometry is default diabled - CMH
    
    //FFC does not need to run - CMH
}
