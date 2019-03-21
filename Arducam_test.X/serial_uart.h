/* 
 * Search & Rescue Drone
 * Data Acquisition Unit Firmware
 *  
 * Members: Cole Hinton, Zachary Tumbleson, Richard McGinnis, & Jay Elliot
 * 
 * File: serial_uart.h
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
extern void UART_Initialize(void);

// Read from the UART
extern uint8_t UART_Read(void);

extern void putch(unsigned char byte);

