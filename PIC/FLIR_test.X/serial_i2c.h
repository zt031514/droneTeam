/* 
 * Search & Rescue Drone
 * Data Acquisition Unit Firmware
 *  
 * Members: Cole Hinton, Zachary Tumbleson, Richard McGinnis, & Jay Elliot
 * 
 * File: serial_i2c.h
 * 
 * Date: 1/14/19
 * 
 * Revision: 1.0
 */

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


// Initialization of the UART
extern void i2c_initialize(void);
extern void i2c_waitForIdle(void);
extern void i2c_start(void);
extern void i2c_repStart(void);
extern void i2c_stop(void);
extern uint8_t i2c_read( uint8_t ack );
extern uint8_t i2c_write( uint8_t i2cWriteData );