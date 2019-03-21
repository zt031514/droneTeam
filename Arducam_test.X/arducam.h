/* 
 * Search & Rescue Drone
 * Data Acquisition Unit Firmware
 *  
 * Members: Cole Hinton, Zachary Tumbleson, Richard McGinnis, & Jay Elliot
 * 
 * File: arducam.h
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

extern void arducam_initialize(void);
extern void arducam_capture(void);
extern void arducam_acquire(void);