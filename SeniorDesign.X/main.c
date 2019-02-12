/* 
 * Search & Rescue Drone
 * Data Acquisition Unit Firmware
 *  
 * Members: Cole Hinton, Zachary Tumbleson, Richard McGinnis, & Jay Elliot
 * 
 * File: main.c
 * 
 * Date Created: 1/14/19
 * 
 * Revision Date: 2/11/19
 * 
 * Revision: 0.03
 */

#include "config.h"
#include "serial_uart.h"
#include "serial_i2c.h"
#include "serial_spi.h"
#include "lepton.h"
#include "arducam.h"
#include "eth_wiz.h"
#include "gnss5.h"


//Mission Start Checks - CMH
uint8_t pulse_check = 0;
uint8_t mission_check = 1; //CHANGE TO 0 for final program - CMH

//Thermal Image Variables - CMH
uint8_t packet_length = 164;
uint8_t image_packet[164];
uint8_t num_of_packets = 60; //With Telemetry OFF
uint8_t packet_index = 60;
uint8_t discard = 0;

//Turn this variable below into a string array - CMH
uint16_t GPS_location = 0;
uint16_t GPS_location_current = 0;
uint16_t GPS_array_length = 0;


//Function Declarations - CMH
void lepton_capture_packet(void);
void lepton_send_packet(void);

// Main program
int main(int argc, char** argv) 
{  
    // Setup the oscillator
    OSCILLATOR_Initialize();
    
    // Initialize for camera settings - CMH
    PORT_PIN_Initialize1();
    
    // Initialize the I2C - CMH
    i2c_initialize();
    
    // Initialize the Flir Lepton - CMH
    lepton_initialize();
    
    // Initialize the Arducam - CMH
    arducam_initialize();
    
    // Initialize for data acquisition - CMH
    PORT_PIN_Initialize2();
    
    // Initialize the UART - CMH
    UART_Initialize();
    
    // Initialize the SPI for ETHWIZ and FLIR - CMH
    SPI_ETHWIZ_Initialize();
    SPI_FLIR_Initialize();
    
    IOCBFbits.IOCBF5 = 0;
    PIR3bits.RCIF = 0;
   
    //Set all SPI to active low state other - CMH
    PORTBbits.RB4 = 1;  //FLIR
    PORTDbits.RD1 = 1;  //ARDUCAM
    PORTDbits.RD3 = 1;  //ETH WIZ
    
    //Wait until the start signal is given - CMH
    while(mission_check == 0) continue;
    
    //Assert FLIR to start image collection - CMH
    PORTBbits.RB4 = 1;
    
    
    //Infinite while loop while waiting for interrupts - CMH
    while (mission_check == 1)
    {
        //While loop to get one whole image - CMH
        do{
            //Capture a packet - CMH
            lepton_capture_packet();
            
            //Send if not a discard packet
            if(discard != 1){
                lepton_send_packet();
            }
            
        }while(packet_index < 59);
        
        //Acquire GPS location - CMH
        gnss5_acquire();
        
    }
    
    
    //Switch SPI bus to work for the Arducam - CMH
    SPI_Arducam_Initialize();
    

    while (mission_check == 2){
        
        //Get the current GPS location here - CMH
        gnss5_acquire();
        
        //Check to see if any of the locations match - CMH
        for(int i = 0; i == GPS_array_length; i++){
            //will need to add some tolerances
            if (GPS_location_current == GPS_location){
                
            }
        }
        
    }
    
    while(1) continue;
}


//Capture a packet from the FLIR - CMH
void lepton_capture_packet(void){
    discard = 0;
    for(int i = 0; i < packet_length; i++){
        PORTBbits.RB4 = 0;
        image_packet[i] = SPI_Write(0x00);
        PORTBbits.RB4 = 1;
    }
    
    if((image_packet[0]&0x0F) == 0x0F){
        discard = 1;
        packet_index = -1;
    }
    else{
        packet_index++;
        if( packet_index > 59){
            packet_index = 0;
        }
    }
    
    
}


//Send a FLIR packet to ETH WIZ - CMH
void lepton_send_packet(void){
    //Assert Active Low - CMH
    PORTDbits.RD3 = 0;
    
    for(int i = 0; i < packet_length; i++){
        SPI_ETHWIZ_Write(image_packet[i]);
    }
    
    //De-assert - CMH
    PORTDbits.RD3 = 1;
}
