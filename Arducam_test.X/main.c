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



//Thermal Image Variables - CMH
uint8_t packet_length = 164;    //Length of single VoSPI packet from FLIR - CMH
uint8_t image_packet[164];      //Array for storage of VoSPI packet - CMH
uint8_t num_of_packets = 60;    //Number of packets when telemetry disabled - CMH
int8_t packet_index = 60;      //Current index of received packets - CMH
uint8_t discard = 0;            //Discard packet flag - CMH
uint8_t num_discard = 0;


//Function Declarations - CMH

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
    
    //Initialize the UART for use with the GNSS 5 - CMH
    UART_Initialize();

    //Initialize SPI for use with the FLIR Lepton - CMH
    SPI_FLIR_Initialize();
    
    //Initialize the Flir Lepton through the I2C - CMH
    lepton_initialize();
   
    //Pull RST pins high to de-assert - CMH
    PORTDbits.RD2 = 1; //ETH WIZ
    PORTDbits.RD0 = 1; //GNSS 5    
    
    //De-assert all Slave Select for SPI - CMH
    PORTCbits.RC0 = 1;  //FLIR
    PORTDbits.RD1 = 1;  //ARDUCAM
    PORTDbits.RD3 = 1;  //ETH WIZ
    
    __delay_ms(1000);
    
    printf("HELLO\r\n");
    PORTCbits.RC0 = 0;
    PORTCbits.RC0 = 1;
    __delay_ms(2000);
    
    
    uint8_t frames = 0;
    //The Do-While loop to get one whole image of 60 packets - CMH
    while(frames < 60){
        packet_index = -1;
        do{
            //Capture a packet from the FLIR - CMH
            lepton_capture_packet();
            
        }while(packet_index < 59);
        
        printf("%d ", frames);
        frames ++;
    }
    printf("\n\r");
        //Send GPS location over ETH WIZ - CMH
        //ADD CODE HERE
        
        //Look to see if the Visual mission start signal was received - CMH
        //ADD CODE HERE
        
    
    //Infinite loop to end the program - CMH
    while(1) continue;
}



