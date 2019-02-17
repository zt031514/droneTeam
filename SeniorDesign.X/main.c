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


//Mission Stage Check variable. Will increment through different 
//stages when signal received from ground station - CMH
uint8_t mission_stage_check = 1; //CHANGE TO 0 for final program - CMH

//Thermal Image Variables - CMH
uint8_t packet_length = 164;    //Length of single VoSPI packet from FLIR - CMH
uint8_t image_packet[164];      //Array for storage of VoSPI packet - CMH
uint8_t num_of_packets = 60;    //Number of packets when telemetry disabled - CMH
uint8_t packet_index = 60;      //Current index of received packets - CMH
uint8_t discard = 0;            //Discard packet flag - CMH

//GPS location variables. Need to turn location variables into strings - CMH
uint16_t GPS_location = 0;          //location of hotspots. To be an array - CMH
uint16_t GPS_location_current = 0;  //Current location of drone - CMH
uint16_t GPS_array_length = 0;      //Length of hotspot location array - CMH


//Function Declarations - CMH
void lepton_capture_packet(void);
void lepton_send_packet(void);


// ************************************************************
//
//  This function acts as the main function of the program.
//  The consists of three main stages. The first stage is for
//  initializing everything like the serial buses, the cameras,
//  etc. The second stage is for thermal image collection from
//  from the FLIR Lepton and GPS locations from the GNSS 5.
//  The third and final stage is for getting hotspot locations
//  from the ground station, comparing the current GPS location
//  to the list of hotspot locations, and taking a visual image 
//  with the Arducam if at one of the hotspot locations.
//
// ************************************************************
int main(int argc, char** argv) 
{  
    //Setup the oscillator - CMH
    OSCILLATOR_Initialize();
    
    //Run the first port pin initialization - CMH
    PORT_PIN_Initialize1();
    
    //Initialize the I2C for camera setup - CMH
    i2c_initialize();
    
    //Initialize the Flir Lepton through the I2C - CMH
    lepton_initialize();
    
    //Initialize the Arducam through the I2C - CMH
    arducam_initialize();
    
    //Run the second Port Pin Initialization - CMH
    PORT_PIN_Initialize2();
    
    //Initialize the UART for use with the GNSS 5 - CMH
    UART_Initialize();
    
    //Initialize SPI for use with the ETH WIZ Click - CMH
    SPI_ETHWIZ_Initialize();
    
    //Initialize SPI for use with the FLIR Lepton - CMH
    SPI_FLIR_Initialize();
    
    //Clear Interrupt flags - CMH
    IOCBFbits.IOCBF5 = 0;
    PIR3bits.RCIF = 0;
   
    //Set all Slave Select for SPI to active low state - CMH
    PORTBbits.RB4 = 1;  //FLIR
    PORTDbits.RD1 = 1;  //ARDUCAM
    PORTDbits.RD3 = 1;  //ETH WIZ
    
    //Wait until the start signal is given - CMH
    while(mission_stage_check == 0) {
        //This will contain code that is constantly polling the
        //ETH WIZ to see if the start signal has been received. - CMH
    }
    
       
    //While still the thermal image stage - CMH
    while (mission_stage_check == 1)
    {
        packet_index = -1;
        //The Do-While loop to get one whole image of 60 packets - CMH
        do{
            //Capture a packet from the FLIR - CMH
            lepton_capture_packet();
            
            //Send over ETH WIZ if not a discard packet - CMH
            if(discard != 1){
                lepton_send_packet();
            }
            
        }while(packet_index < 59);
        
        //Acquire current GPS location - CMH
        gnss5_acquire();
        
        //Send GPS location over ETH WIZ - CMH
        //ADD CODE HERE
        
        //Look to see if the Visual mission start signal was received - CMH
        //ADD CODE HERE
        
    }
    
    //Set up SPI bus to work with the Arducam - CMH
    SPI_Arducam_Initialize();
    
    
    //Runs the visual image collection stage - CMH
    while (mission_stage_check == 2){
        
        //Get the current GPS location - CMH
        gnss5_acquire();
        
        //Check to see if any of the hotspot locations match the current - CMH
        for(int i = 0; i == GPS_array_length; i++){
            //will need to add some tolerances
            if (GPS_location_current == GPS_location){
                
                //Take visual image with Arducam and send over ETH WIZ - CMH
                //ADD CODE HERE
                
            }
        }
        
    }
    
    //Infinite loop to end the program - CMH
    while(1) continue;
}


// ************************************************************
//
//  This function captures a packet of thermal image data
//  from the Flir Lepton over the SPI bus. The FLIR does not
//  use traditional SPI and instead uses a proprietary SPI
//  based protocol called VoSPI (Video over SPI). It doesn't
//  send one byte at a time and instead sends over a whole 
//  packet of data (164 bytes) continuously. This function
//  is set up to handle that constant stream of data.
//
// ************************************************************
void lepton_capture_packet(void){
    //Initially set the discard flag to 0 - CMH
    discard = 0;
    
    //For the length of an entire packet - CMH
    for(int i = 0; i < packet_length; i++){
        //Assert the Slave Select (CS) pin for the FLIR - CMH
        PORTBbits.RB4 = 0;
        //Get the i byte of data - CMH
        image_packet[i] = SPI_Write(0x00);
        //De-assert the CS pin so next byte is not received until
        //pin is asserted again - CMH
        PORTBbits.RB4 = 1;
    }
    //If the first byte of data says the entire packet is a discard packet - CMH
    if((image_packet[0]&0x0F) == 0x0F){
        discard = 1;        //Set the discard flag - CMH
        packet_index = -1;  //set packet index to an impossible value - CMH
    }
    //If it is not a discard packet - CMH
    else{
        packet_index++;         //increment to the current index - CMH
        if( packet_index > 59){ //If the index is incremented past the
            packet_index = 0;   //final index, set back to 0 - CMH
        }
    }
    
    
}


// ************************************************************
//
//  This function sends a packet of thermal data through
//  the ETH WIZ using the SPI Write function for the ETH WIZ.
//
// ************************************************************
void lepton_send_packet(void){
    //Assert the CS pin of the ETH WIZ - CMH
    PORTDbits.RD3 = 0;
    
    //For a whole packet of thermal image data
    for(int i = 0; i < packet_length; i++){
        //Send byte i of thermal data through the ETH WIZ - CMH
        SPI_ETHWIZ_Write(image_packet[i]);
        
        //Clears the data from the index in the array - CMH
        image_packet[i] = 0;
    }
    
    //De-assert the CS pin of the ETH WIZ- CMH
    PORTDbits.RD3 = 1;
}
