/* 
 * Search & Rescue Drone
 * Data Acquisition Unit Firmware
 *  
 * Members: Cole Hinton, Zachary Tumbleson, Richard McGinnis, & Jay Elliot
 * 
 * File: eth_wiz.h
 * 
 * Date: 1/14/19
 * 
 * Revision: 0.01
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

extern void eth_wiz_configure(void);
extern void eth_wiz_createSocket(void);
extern void eth_wiz_transmit_start(void);
extern void eth_wiz_transmit_end(void);
extern void eth_wiz_receive(void);