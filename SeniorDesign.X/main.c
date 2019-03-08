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


//Function Declarations - CMH
void lepton_capture_packet(void);
void lepton_send_packet(void);
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
                    
                }while((IR_check & 0x10) != 0x10);
                
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
            
            //Send the packet from FLIR - CMH
            lepton_send_packet();
            
        }while(packet_index < 59);
        
        //Acquire current GPS location - CMH
        //gnss5_acquire();
        
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
//  This function sends a packet of thermal data through
//  the ETH WIZ using the SPI Write function for the ETH WIZ.
//
// ************************************************************
void lepton_send_packet(void){
    uint8_t junk = 0;
    //start transmission
    eth_wiz_transmit_start();
    
    //These four lines below are the write/read sequencence for the SPI on
    //the ETH WIZ. It had to be done with these all in a row because the
    //XC8 compiler for the PIC does not generate optimized code so
    //the program wouldn't run fast enough to get the full image until
    //I wrote in this way - CMH
    /*
    SSP2BUF = image_packet[0];             
    delay();
    image_packet[0] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[1];             
    delay();
    image_packet[1] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[2];             
    delay();
    image_packet[2] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[3];             
    delay();
    image_packet[3] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[4];             
    delay();
    image_packet[4] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[5];             
    delay();
    image_packet[5] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[6];             
    delay();
    image_packet[6] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[7];             
    delay();
    image_packet[7] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[8];             
    delay();
    image_packet[8] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[9];             
    delay();
    image_packet[9] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[10];             
    delay();
    image_packet[10] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[11];             
    delay();
    image_packet[11] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[12];             
    delay();
    image_packet[12] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[13];             
    delay();
    image_packet[13] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[14];             
    delay();
    image_packet[14] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[15];             
    delay();
    image_packet[15] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[16];             
    delay();
    image_packet[16] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[17];             
    delay();
    image_packet[17] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[18];             
    delay();
    image_packet[18] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[19];             
    delay();
    image_packet[19] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[20];             
    delay();
    image_packet[20] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[21];             
    delay();
    image_packet[21] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[22];             
    delay();
    image_packet[22] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[23];             
    delay();
    image_packet[23] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[24];             
    delay();
    image_packet[24] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[25];             
    delay();
    image_packet[25] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[26];             
    delay();
    image_packet[26] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[27];             
    delay();
    image_packet[27] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[28];             
    delay();
    image_packet[28] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[29];             
    delay();
    image_packet[29] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[30];             
    delay();
    image_packet[30] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[31];             
    delay();
    image_packet[31] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[32];             
    delay();
    image_packet[32] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[33];             
    delay();
    image_packet[33] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[34];             
    delay();
    image_packet[34] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[35];             
    delay();
    image_packet[35] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[36];             
    delay();
    image_packet[36] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[37];             
    delay();
    image_packet[37] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[38];             
    delay();
    image_packet[38] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[39];             
    delay();
    image_packet[39] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[40];             
    delay();
    image_packet[40] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[41];             
    delay();
    image_packet[41] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[42];             
    delay();
    image_packet[42] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[43];             
    delay();
    image_packet[43] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[44];             
    delay();
    image_packet[44] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[45];             
    delay();
    image_packet[45] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[46];             
    delay();
    image_packet[46] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[47];             
    delay();
    image_packet[47] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[48];             
    delay();
    image_packet[48] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[49];             
    delay();
    image_packet[49] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[50];             
    delay();
    image_packet[50] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[51];             
    delay();
    image_packet[51] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[52];             
    delay();
    image_packet[52] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[53];             
    delay();
    image_packet[53] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[54];             
    delay();
    image_packet[54] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[55];             
    delay();
    image_packet[55] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[56];             
    delay();
    image_packet[56] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[57];             
    delay();
    image_packet[57] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[58];             
    delay();
    image_packet[58] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[59];             
    delay();
    image_packet[59] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[60];             
    delay();
    image_packet[60] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[61];             
    delay();
    image_packet[61] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[62];             
    delay();
    image_packet[62] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[63];             
    delay();
    image_packet[63] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[64];             
    delay();
    image_packet[64] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[65];             
    delay();
    image_packet[65] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[66];             
    delay();
    image_packet[66] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[67];             
    delay();
    image_packet[67] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[68];             
    delay();
    image_packet[68] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[69];             
    delay();
    image_packet[69] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[70];             
    delay();
    image_packet[70] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[71];             
    delay();
    image_packet[71] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[72];             
    delay();
    image_packet[72] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[73];             
    delay();
    image_packet[73] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[74];             
    delay();
    image_packet[74] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[75];             
    delay();
    image_packet[75] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[76];             
    delay();
    image_packet[76] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[77];             
    delay();
    image_packet[77] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[78];             
    delay();
    image_packet[78] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[79];             
    delay();
    image_packet[79] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[80];             
    delay();
    image_packet[80] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[81];             
    delay();
    image_packet[81] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[82];             
    delay();
    image_packet[82] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[83];             
    delay();
    image_packet[83] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[84];             
    delay();
    image_packet[84] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[85];             
    delay();
    image_packet[85] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[86];             
    delay();
    image_packet[86] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[87];             
    delay();
    image_packet[87] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[88];             
    delay();
    image_packet[88] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[89];             
    delay();
    image_packet[89] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[90];             
    delay();
    image_packet[90] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[91];             
    delay();
    image_packet[91] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[92];             
    delay();
    image_packet[92] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[93];             
    delay();
    image_packet[93] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[94];             
    delay();
    image_packet[94] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[95];             
    delay();
    image_packet[95] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[96];             
    delay();
    image_packet[96] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[97];             
    delay();
    image_packet[97] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[98];             
    delay();
    image_packet[98] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[99];             
    delay();
    image_packet[99] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[100];             
    delay();
    image_packet[100] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[101];             
    delay();
    image_packet[101] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[102];             
    delay();
    image_packet[102] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[103];             
    delay();
    image_packet[103] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[104];             
    delay();
    image_packet[104] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[105];             
    delay();
    image_packet[105] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[106];             
    delay();
    image_packet[106] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[107];             
    delay();
    image_packet[107] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[108];             
    delay();
    image_packet[108] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[109];             
    delay();
    image_packet[109] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[110];             
    delay();
    image_packet[110] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[111];             
    delay();
    image_packet[111] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[112];             
    delay();
    image_packet[112] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[113];             
    delay();
    image_packet[113] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[114];             
    delay();
    image_packet[114] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[115];             
    delay();
    image_packet[115] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[116];             
    delay();
    image_packet[116] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[117];             
    delay();
    image_packet[117] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[118];             
    delay();
    image_packet[118] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[119];             
    delay();
    image_packet[119] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[120];             
    delay();
    image_packet[120] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[121];             
    delay();
    image_packet[121] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[122];             
    delay();
    image_packet[122] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[123];             
    delay();
    image_packet[123] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[124];             
    delay();
    image_packet[124] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[125];             
    delay();
    image_packet[125] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[126];             
    delay();
    image_packet[126] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[127];             
    delay();
    image_packet[127] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[128];             
    delay();
    image_packet[128] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[129];             
    delay();
    image_packet[129] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[130];             
    delay();
    image_packet[130] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[131];             
    delay();
    image_packet[131] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[132];             
    delay();
    image_packet[132] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[133];             
    delay();
    image_packet[133] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[134];             
    delay();
    image_packet[134] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[135];             
    delay();
    image_packet[135] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[136];             
    delay();
    image_packet[136] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[137];             
    delay();
    image_packet[137] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[138];             
    delay();
    image_packet[138] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[139];             
    delay();
    image_packet[139] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[140];             
    delay();
    image_packet[140] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[141];             
    delay();
    image_packet[141] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[142];             
    delay();
    image_packet[142] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[143];             
    delay();
    image_packet[143] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[144];             
    delay();
    image_packet[144] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[145];             
    delay();
    image_packet[145] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[146];             
    delay();
    image_packet[146] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[147];             
    delay();
    image_packet[147] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[148];             
    delay();
    image_packet[148] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[149];             
    delay();
    image_packet[149] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[150];             
    delay();
    image_packet[150] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[151];             
    delay();
    image_packet[151] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[152];             
    delay();
    image_packet[152] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[153];             
    delay();
    image_packet[153] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[154];             
    delay();
    image_packet[154] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[155];             
    delay();
    image_packet[155] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[156];             
    delay();
    image_packet[156] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[157];             
    delay();
    image_packet[157] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[158];             
    delay();
    image_packet[158] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[159];             
    delay();
    image_packet[159] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[160];             
    delay();
    image_packet[160] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[161];             
    delay();
    image_packet[161] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[162];             
    delay();
    image_packet[162] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    SSP2BUF = image_packet[163];             
    delay();
    image_packet[163] = 0;
    SSP2IF = 0;
    junk = SSP2BUF;
    */
    
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
    
    //Assert the Slave Select (CS) pin for the FLIR - CMH
    PORTCbits.RC0 = 0;
    
    //These four lines below are the write/read sequencence for the VoSPI on
    //the FLIR. It had to be done with these all in a row because the
    //XC8 compiler for the PIC does not generate optimized code so
    //the program wouldn't run fast enough to get the full image until
    //I wrote in this way - CMH
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[0] = SSP1BUF;
    
    SSP1BUF = 0x00;             
    delay();  
    SSP1IF = 0;                 
    image_packet[1] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();   
    SSP1IF = 0;                 
    image_packet[2] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();   
    SSP1IF = 0;                 
    image_packet[3] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();  
    SSP1IF = 0;                 
    image_packet[4] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();     
    SSP1IF = 0;                 
    image_packet[5] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();      
    SSP1IF = 0;                 
    image_packet[6] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();       
    SSP1IF = 0;                 
    image_packet[7] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();       
    SSP1IF = 0;                 
    image_packet[8] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();       
    SSP1IF = 0;                 
    image_packet[9] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();      
    SSP1IF = 0;                 
    image_packet[10] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();   
    SSP1IF = 0;                 
    image_packet[11] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();     
    SSP1IF = 0;                 
    image_packet[12] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();     
    SSP1IF = 0;                 
    image_packet[13] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();       
    SSP1IF = 0;                 
    image_packet[14] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();       
    SSP1IF = 0;                 
    image_packet[15] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();       
    SSP1IF = 0;                 
    image_packet[16] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[17] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();       
    SSP1IF = 0;                 
    image_packet[18] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();      
    SSP1IF = 0;                 
    image_packet[19] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();       
    SSP1IF = 0;                 
    image_packet[20] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();       
    SSP1IF = 0;                 
    image_packet[21] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();       
    SSP1IF = 0;                 
    image_packet[22] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();       
    SSP1IF = 0;                 
    image_packet[23] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();     
    SSP1IF = 0;                 
    image_packet[24] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();       
    SSP1IF = 0;                 
    image_packet[25] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[26] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[27] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[28] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[29] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[30] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[31] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[32] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[33] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[34] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[35] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[36] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[37] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[38] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[39] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[40] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[41] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[42] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[43] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[44] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[45] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[46] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[47] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[48] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[49] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[50] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[51] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[52] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[53] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[54] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[55] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[56] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[57] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[58] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[59] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();    
    SSP1IF = 0;                 
    image_packet[60] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[61] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[62] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[63] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[64] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[65] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();
    SSP1IF = 0;                 
    image_packet[66] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[67] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[68] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[69] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[70] = SSP1BUF;
    SSP1BUF = 0x00;            
    delay();        
    SSP1IF = 0;                 
    image_packet[71] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[72] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[73] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[74] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[75] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[76] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[77] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[78] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[79] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[80] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[81] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[82] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[83] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[84] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[85] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[86] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[87] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[88] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[89] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[90] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[91] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[92] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[93] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[94] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[95] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[96] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[97] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[98] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[99] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[100] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[101] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[102] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[103] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[104] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[105] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[106] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[107] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[108] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[109] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[110] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[111] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[112] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[113] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[114] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[115] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[116] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[117] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[118] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();       
    SSP1IF = 0;                 
    image_packet[119] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[120] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[121] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[122] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[123] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[124] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[125] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[126] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[127] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[128] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[129] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[130] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[131] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[132] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[133] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
   SSP1IF = 0;                 
    image_packet[134] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[135] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[136] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[137] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[138] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[139] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[140] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[141] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
   SSP1IF = 0;                 
    image_packet[142] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();       
    SSP1IF = 0;                 
    image_packet[143] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[144] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[145] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[146] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[147] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[148] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();       
    SSP1IF = 0;                 
    image_packet[149] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[150] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[151] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[152] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
   SSP1IF = 0;                 
    image_packet[153] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[154] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[155] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
   SSP1IF = 0;                 
    image_packet[156] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
   SSP1IF = 0;                 
    image_packet[157] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();       
   SSP1IF = 0;                 
    image_packet[158] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[159] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[160] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
   SSP1IF = 0;                 
    image_packet[161] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[162] = SSP1BUF;
    SSP1BUF = 0x00;             
    delay();        
    SSP1IF = 0;                 
    image_packet[163] = SSP1BUF;
    
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




