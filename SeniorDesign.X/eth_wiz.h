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

extern void eth_wiz_configure();
extern void eth_wiz_createSocket();
extern void eth_wiz_transmit();
extern void eth_wiz_receive();