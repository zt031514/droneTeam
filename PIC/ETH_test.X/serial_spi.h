/* 
 * Search & Rescue Drone
 * Data Acquisition Unit Firmware
 *  
 * Members: Cole Hinton, Zachary Tumbleson, Richard McGinnis, & Jay Elliot
 * 
 * File: serial_spi.h
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

extern void SPI_ETHWIZ_Initialize(void);

extern uint8_t SPI_ETHWIZ_Write(uint8_t data);

extern void SPI_FLIR_Initialize(void);

extern void SPI_Arducam_Initialize(void);

extern uint8_t SPI_Write(uint8_t data);