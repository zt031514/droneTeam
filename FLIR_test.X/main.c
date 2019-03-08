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
void lepton_capture_packet(void);
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
    
    //These four lines below is the write/read sequencence for the VoSPI on
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
        num_discard++;
        packet_index = -1;  //set packet index to an impossible value - CMH
    }
    //If it is not a discard packet - CMH
    else{
        num_discard = 0;
        packet_index++;         //increment to the current index - CMH
        if( packet_index > 59){ //If the index is incremented past the
            packet_index = 0;   //final index, set back to 0 - CMH
        }
    }
    
    if(num_discard > 150){
        __delay_ms(2000);
        num_discard = 0;
    }
    
}

