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
 * Revision Date: 3/7/19
 * 
 * Revision: 0.45
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
uint8_t num_discard = 0;

//GPS location variables. Need to turn location variables into strings - CMH
uint16_t GPS_location = 0;          //location of hotspots. To be an array - CMH
uint16_t GPS_location_current = 0;  //Current location of drone - CMH
uint16_t GPS_array_length = 0;      //Length of hotspot location array - CMH

//Variables for pointer registers and commands of RX - CMH
uint8_t s0_rx_readpointer_high  = 0x00;
uint8_t s0_rx_readpointer_low   = 0x00;
uint8_t s0_rx_writepointer_high = 0x00;
uint8_t s0_rx_writepointer_low  = 0x00;
uint8_t s0_rx_datalength_high   = 0x00;
uint8_t s0_rx_datalength_low    = 0x00;
uint8_t s0_reg_write     = 0b00001100;
uint8_t s0_reg_read      = 0b00001000;
uint8_t s0_rx_buf_read   = 0b00011000;
uint8_t ethwiz_rx_data[164];


//Function Declarations - CMH
void lepton_capture_packet(void);
void lepton_send_packet(void);
void eth_wiz_receive(void);
extern void delay(void);


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
   
    //Pull RST pins high to de-assert - CMH
    PORTDbits.RD2 = 1; //ETH WIZ
    PORTDbits.RD0 = 1; //GNSS 5    
    
    //De-assert all Slave Select(SS) for SPI - CMH
    PORTCbits.RC0 = 1;  //FLIR
    PORTDbits.RD1 = 1;  //ARDUCAM
    PORTDbits.RD3 = 1;  //ETH WIZ
    
    //Toggle SS for Flir to reset - CMH
    __delay_ms(1000);
    PORTCbits.RC0 = 0;
    PORTCbits.RC0 = 1;
    __delay_ms(500);
    
    //Initialize the settings on the ETH WIZ - CMH
    eth_wiz_configure();
    
    //Create the socket for the ETH WIZ - CMH
    eth_wiz_createSocket();
    
    //Wait until the start signal is given - CMH
    while(mission_stage_check == 0) {
        //This will contain code that is constantly polling the
        //ETH WIZ to see if the start signal has been received. - CMH
    }
    
    
    uint8_t first_send = 0;
    uint8_t IR_check = 0x00;
    //While still the thermal image stage - CMH
    while (mission_stage_check == 1)
    {
        packet_index = -1;
        do{
            //Capture a packet from the FLIR - CMH
            lepton_capture_packet();
            
            //if not the first time through
            if(first_send == 1){
                while(PORTBbits.RB0 == 1) continue;
                
                //Wait for SEND_OK (complete) from previous send - CMH
                do{
                    PORTDbits.RD3 = 0;
                    SPI_ETHWIZ_Write(0x00);
                    SPI_ETHWIZ_Write(0x02); //Address = 0x0002
                    SPI_ETHWIZ_Write(0b00001000); // S0_reg_read
                    IR_check = SPI_ETHWIZ_Write(0x00);
                    PORTDbits.RD3 = 1;
                    
                }while((IR_check & 0x10) != 0x10 || (IR_check & 0x04) != 0x04);
                
                //Set to 1 to clear flag - CMH
                PORTDbits.RD3 = 0;
                SPI_ETHWIZ_Write(0x00);
                SPI_ETHWIZ_Write(0x02); //Address = 0x0002
                SPI_ETHWIZ_Write(0b00001100); // S0_reg_write
                SPI_ETHWIZ_Write(0x10);
                PORTDbits.RD3 = 1;
                
                first_send = 1;
            }
            else { //if first time through
                first_send = 1;
            }
            
            //If data received, stop thermal collection - CMH
            if((IR_check & 0x04) != 0x04){
                packet_index = 60;
            }
            //If nothing received, send the packet from FLIR - CMH
            else{
                lepton_send_packet();
            }
                
            
        }while(packet_index < 59);
        
        
        
        //Acquire current GPS location - CMH
        //gnss5_acquire();
        
        //Send GPS location over ETH WIZ - CMH
        //ADD CODE HERE
        
        
    }
    __delay_ms(500);
    eth_wiz_receive();
    mission_stage_check = 2;
    
    //Set up SPI bus to work with the Arducam - CMH
    SPI_Arducam_Initialize();
    
    
    //Runs the visual image collection stage - CMH
    while (mission_stage_check == 2){
        
        //Get the current GPS location - CMH
        //gnss5_acquire();
        
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
//  This function sends a packet of thermal data through
//  the ETH WIZ using the SPI Write function for the ETH WIZ.
//
// ************************************************************
void lepton_send_packet(void){
    
    //start transmission
    eth_wiz_transmit_start();
    
    for(int j = 0; j <164; j++){
        SPI_ETHWIZ_Write(image_packet[j]);
        image_packet[j] = 0;
    }
    
    eth_wiz_transmit_end();
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
    
    int i = 0;
    
    //Assert the Slave Select (CS) pin for the FLIR - CMH
    PORTCbits.RC0 = 0;
    
    //These four lines below are the write/read sequencence for the VoSPI on
    //the FLIR. It had to be done with these all in a row because the
    //XC8 compiler for the PIC does not generate optimized code so
    //the program wouldn't run fast enough to get the full image until
    //I wrote in this way - CMH
    while(i < 164){
        SSP1BUF = 0x00;             
        delay();
        SSP1IF = 0;                 
        image_packet[0] = SSP1BUF;
        i++;
    }
    
    //De-assert the CS pin so next byte is not received until
    //pin is asserted again - CMH
    PORTCbits.RC0 = 1;
    
    //If the first byte of data says the entire packet is a discard packet - CMH
    if((image_packet[0]&0x0F) == 0x0F){
        discard = 1;        //Set the discard flag - CMH
        num_discard++;      //Increment the number of discard packets - CMH
        packet_index = -1;  //set packet index to -1 so next real packet can
                            //have the index of 0 when incremented below - CMH
    }
    //If it is not a discard packet - CMH
    else{
        num_discard = 0;        //Reset the number of discarded packets - CMH
        packet_index++;         //increment to the current index - CMH
        if( packet_index > 59){ //If the index is incremented past the
            packet_index = 0;   //final index, set back to 0 - CMH
        }
    }
    
    //If there has been more than 150 discarded packets in a row,
    //wait for two seconds so the FLIR VoSPI can reset itself.
    //Also reset the number of discarded packets to 0 - CMH
    if(num_discard > 150){
        __delay_ms(2000);
        num_discard = 0;
    }
    
}


// ************************************************************
//
//  This function performs the receive data sequence. Will only
//  be called after a receive interrupt is sent from the ETH
//  WIZ.
//
// ************************************************************
void eth_wiz_receive(void){
    //Read in Received Data Size Register (0x0026 - 0x0027),
    //the RX Read Data Pointer Register (0x0028 - 0x0029), and
    //the RX Write pointer Register (0x002A - 0x002B) - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x26); //Starting Address = 0x0026
    SPI_ETHWIZ_Write(s0_reg_read);
    s0_rx_datalength_high = SPI_ETHWIZ_Write(0x00);         //0x0026
    s0_rx_datalength_low  = SPI_ETHWIZ_Write(0x00);         //0x0027
    s0_rx_readpointer_high = SPI_ETHWIZ_Write(0x00);        //0x0028
    s0_rx_readpointer_low  = SPI_ETHWIZ_Write(0x00);        //0x0029
    s0_rx_writepointer_high = SPI_ETHWIZ_Write(0x00);       //0x002A
    s0_rx_writepointer_low  = SPI_ETHWIZ_Write(0x00);       //0x002B
    PORTDbits.RD3 = 1;
    
    //determine the amount of bytes to read in from RX buffer - CMH
    uint16_t datalength = (s0_rx_datalength_high << 8) | s0_rx_datalength_low;
    
    //Read in data from the RX buffer - CMH
    PORTDbits.RD3 = 0;
     //Starting Address = RX Read Data Pointer
    SPI_ETHWIZ_Write(s0_rx_readpointer_high);
    SPI_ETHWIZ_Write(s0_rx_readpointer_low);
    SPI_ETHWIZ_Write(s0_rx_buf_read);
    for(int i = 0; i < datalength; i++){
        //Collect these values into an array - CMH
        ethwiz_rx_data[i] = SPI_ETHWIZ_Write(0x00);
    }
    PORTDbits.RD3 = 1;
    
    
    //Update the Read Pointer to the Write Pointer Address - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x28); //Starting Address = 0x0028
    SPI_ETHWIZ_Write(s0_reg_write);
    SPI_ETHWIZ_Write(s0_rx_writepointer_high);
    SPI_ETHWIZ_Write(s0_rx_writepointer_low);
    PORTDbits.RD3 = 1;
    
    //Order the RECV command to notify W5500 that the data was read in - CMH
    PORTDbits.RD3 = 0;
    SPI_ETHWIZ_Write(0x00);
    SPI_ETHWIZ_Write(0x01); //Address = 0x0024
    SPI_ETHWIZ_Write(s0_reg_write);
    SPI_ETHWIZ_Write(0x40);   //RECV = 0x40
    SPI_ETHWIZ_Write(0b00000100);
    PORTDbits.RD3 = 1;
    
    
}

