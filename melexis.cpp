/*
	MELEXIS.cpp - MELEXIS library

	For interfacing with the MLX90363 triaxis hall effect sensor
	
	by Travis Howse <tjhowse@gmail.com>
	2012.   License, GPL v2 or later

*/

/******************************************************************************
 * Includes
 ******************************************************************************/

#include "MELEXIS.h"
#import <Arduino.h> 
#include <SPI.h>
/******************************************************************************
 * Definitions
 ******************************************************************************/
 

#define MELEXIS_GET1 0x13
#define MELEXIS_GET2 0x14
#define MELEXIS_GET3 0x15
#define MELEXIS_MemoryRead 0x01
#define MELEXIS_EEPROMWrite 0x03
#define MELEXIS_EEChallengeAns 0x05
#define MELEXIS_EEReadChallenge 0x0F
#define MELEXIS_NOP 0x10
#define MELEXIS_DiagnosticDetails 0x16
#define MELEXIS_OscCounterStart 0x18
#define MELEXIS_OscCounterStop 0x1A
#define MELEXIS_Reboot 0x2F
#define MELEXIS_Standby 0x31

#define MELEXIS_Get3Ready 0x2D
#define MELEXIS_MemoryReadAnswer 0x02
#define MELEXIS_EEPROMWriteChallenge 0x04
#define MELEXIS_EEReadAnswer 0x28
#define MELEXIS_EEPROMWriteStatus 0x0E
#define MELEXIS_Challenge 0x11
#define MELEXIS_DiagnosticsAnswer 0x17
#define MELEXIS_OscCounterStart 0x09
#define MELEXIS_OscCounterStopAck 0x1B
#define MELEXIS_StandbyAck 0x32
#define MELEXIS_Errorframe 0x3D
#define MELEXIS_NTT 0x3E
#define MELEXIS_ReadyMessage 0x2C

// Each EEPROM setting has an address, a bit offset and a length
#define MELEXIS_EE_MAPXYZ_A 0x102A
#define MELEXIS_EE_MAPXYZ_O 0
#define MELEXIS_EE_MAPXYZ_L 3
#define MELEXIS_EE_MAPXYZ MELEXIS_EE_MAPXYZ_A,MELEXIS_EE_MAPXYZ_O,MELEXIS_EE_MAPXYZ_L
#define MELEXIS_EE_3D_A 0x102A
#define MELEXIS_EE_3D_O 3
#define MELEXIS_EE_3D_L 1
#define MELEXIS_EE_3D MELEXIS_EE_3D_A,MELEXIS_EE_3D_O,MELEXIS_EE_3D_L
#define MELEXIS_EE_FILTER_A 0x102A
#define MELEXIS_EE_FILTER_O 4
#define MELEXIS_EE_FILTER_L 2
#define MELEXIS_EE_FILTER MELEXIS_EE_FILTER_A,MELEXIS_EE_FILTER_O,MELEXIS_EE_FILTER_L
#define MELEXIS_EE_VIRTUALGAINMAX_A 0x102E
#define MELEXIS_EE_VIRTUALGAINMAX_O 8
#define MELEXIS_EE_VIRTUALGAINMAX_L 8
#define MELEXIS_EE_VIRTUALGAINMAX MELEXIS_EE_VIRTUALGAINMAX_A,MELEXIS_EE_VIRTUALGAINMAX_O,MELEXIS_EE_VIRTUALGAINMAX_L
#define MELEXIS_EE_VIRTUALGAINMIN_A 0x102E
#define MELEXIS_EE_VIRTUALGAINMIN_O 0
#define MELEXIS_EE_VIRTUALGAINMIN_L 8
#define MELEXIS_EE_VIRTUALGAINMIN MELEXIS_EE_VIRTUALGAINMIN_A,MELEXIS_EE_VIRTUALGAINMIN_O,MELEXIS_EE_VIRTUALGAINMIN_L
#define MELEXIS_EE_KALPHA_A 0x1022
#define MELEXIS_EE_KALPHA_O 0
#define MELEXIS_EE_KALPHA_L 16
#define MELEXIS_EE_KALPHA MELEXIS_EE_KALPHA_A,MELEXIS_EE_KALPHA_O,MELEXIS_EE_KALPHA_L
#define MELEXIS_EE_KBETA_A 0x1024
#define MELEXIS_EE_KBETA_O 0
#define MELEXIS_EE_KBETA_L 16
#define MELEXIS_EE_KBETA MELEXIS_EE_KBETA_A,MELEXIS_EE_KBETA_O,MELEXIS_EE_KBETA_L
#define MELEXIS_EE_SMISM_A 0x1032
#define MELEXIS_EE_SMISM_O 0
#define MELEXIS_EE_SMISM_L 16
#define MELEXIS_EE_SMISM MELEXIS_EE_SMISM_A,MELEXIS_EE_SMISM_O,MELEXIS_EE_SMISM_L
#define MELEXIS_EE_ORTH_B1B2_A 0x1026
#define MELEXIS_EE_ORTH_B1B2_O 0
#define MELEXIS_EE_ORTH_B1B2_L 8
#define MELEXIS_EE_ORTH_B1B2 MELEXIS_EE_ORTH_B1B2_A,MELEXIS_EE_ORTH_B1B2_O,MELEXIS_EE_ORTH_B1B2_L
#define MELEXIS_EE_KT_A 0x1030
#define MELEXIS_EE_KT_O 0
#define MELEXIS_EE_KT_L 16
#define MELEXIS_EE_KT MELEXIS_EE_KT_A,MELEXIS_EE_KT_O,MELEXIS_EE_KT_L
#define MELEXIS_EE_PHYST_A 0x1028
#define MELEXIS_EE_PHYST_O 8
#define MELEXIS_EE_PHYST_L 8
#define MELEXIS_EE_PHYST MELEXIS_EE_PHYST_A,MELEXIS_EE_PHYST_O,MELEXIS_EE_PHYST_L
#define MELEXIS_EE_PINFILTER_A 0x1001
#define MELEXIS_EE_PINFILTER_O 0
#define MELEXIS_EE_PINFILTER_L 2
#define MELEXIS_EE_PINFILTER MELEXIS_EE_PINFILTER_A,MELEXIS_EE_PINFILTER_O,MELEXIS_EE_PINFILTER_L
#define MELEXIS_EE_USERID_H_A 0x103A
#define MELEXIS_EE_USERID_H_O 0
#define MELEXIS_EE_USERID_H_L 16
#define MELEXIS_EE_USERID_H MELEXIS_EE_USERID_H_A,MELEXIS_EE_USERID_H_O,MELEXIS_EE_USERID_H_L
#define MELEXIS_EE_USERID_L_A 0x103C
#define MELEXIS_EE_USERID_L_O 0
#define MELEXIS_EE_USERID_L_L 16
#define MELEXIS_EE_USERID_L MELEXIS_EE_USERID_L_A,MELEXIS_EE_USERID_L_O,MELEXIS_EE_USERID_L_L


 
const char cba_256_TAB [] = {0x00, 0x2f, 0x5e, 0x71, 0xbc, 0x93, 0xe2, 0xcd,
							0x57, 0x78, 0x09, 0x26, 0xeb, 0xc4, 0xb5, 0x9a,
							0xae, 0x81, 0xf0, 0xdf, 0x12, 0x3d, 0x4c, 0x63,
							0xf9, 0xd6, 0xa7, 0x88, 0x45, 0x6a, 0x1b, 0x34,
							0x73, 0x5c, 0x2d, 0x02, 0xcf, 0xe0, 0x91, 0xbe,
							0x24, 0x0b, 0x7a, 0x55, 0x98, 0xb7, 0xc6, 0xe9,
							0xdd, 0xf2, 0x83, 0xac, 0x61, 0x4e, 0x3f, 0x10,
							0x8a, 0xa5, 0xd4, 0xfb, 0x36, 0x19, 0x68, 0x47,
							0xe6, 0xc9, 0xb8, 0x97, 0x5a, 0x75, 0x04, 0x2b,
							0xb1, 0x9e, 0xef, 0xc0, 0x0d, 0x22, 0x53, 0x7c,
							0x48, 0x67, 0x16, 0x39, 0xf4, 0xdb, 0xaa, 0x85,
							0x1f, 0x30, 0x41, 0x6e, 0xa3, 0x8c, 0xfd, 0xd2,
							0x95, 0xba, 0xcb, 0xe4, 0x29, 0x06, 0x77, 0x58,
							0xc2, 0xed, 0x9c, 0xb3, 0x7e, 0x51, 0x20, 0x0f,
							0x3b, 0x14, 0x65, 0x4a, 0x87, 0xa8, 0xd9, 0xf6,
							0x6c, 0x43, 0x32, 0x1d, 0xd0, 0xff, 0x8e, 0xa1,
							0xe3, 0xcc, 0xbd, 0x92, 0x5f, 0x70, 0x01, 0x2e,
							0xb4, 0x9b, 0xea, 0xc5, 0x08, 0x27, 0x56, 0x79,
							0x4d, 0x62, 0x13, 0x3c, 0xf1, 0xde, 0xaf, 0x80,
							0x1a, 0x35, 0x44, 0x6b, 0xa6, 0x89, 0xf8, 0xd7,
							0x90, 0xbf, 0xce, 0xe1, 0x2c, 0x03, 0x72, 0x5d,
							0xc7, 0xe8, 0x99, 0xb6, 0x7b, 0x54, 0x25, 0x0a,
							0x3e, 0x11, 0x60, 0x4f, 0x82, 0xad, 0xdc, 0xf3,
							0x69, 0x46, 0x37, 0x18, 0xd5, 0xfa, 0x8b, 0xa4,
							0x05, 0x2a, 0x5b, 0x74, 0xb9, 0x96, 0xe7, 0xc8,
							0x52, 0x7d, 0x0c, 0x23, 0xee, 0xc1, 0xb0, 0x9f,
							0xab, 0x84, 0xf5, 0xda, 0x17, 0x38, 0x49, 0x66,
							0xfc, 0xd3, 0xa2, 0x8d, 0x40, 0x6f, 0x1e, 0x31,
							0x76, 0x59, 0x28, 0x07, 0xca, 0xe5, 0x94, 0xbb,
							0x21, 0x0e, 0x7f, 0x50, 0x9d, 0xb2, 0xc3, 0xec,
							0xd8, 0xf7, 0x86, 0xa9, 0x64, 0x4b, 0x3a, 0x15,
							0x8f, 0xa0, 0xd1, 0xfe, 0x33, 0x1c, 0x6d, 0x42};
					
const uint16_t EE_Keys [][8] =	{{17485, 31053, 57190, 57724, 7899, 53543, 26763, 12528},
								{38105, 51302, 16209, 24847, 13134, 52339, 14530, 18350},
								{55636, 64477, 40905, 45498, 24411, 36677, 4213, 48843},
								{6368, 5907, 31384, 63325, 3562, 19816, 6995, 3147}};

const uint8_t slaveSelectPin = 20;
uint8_t i,j,k;
uint8_t crc;

uint8_t outbuffer[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t inbuffer[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

uint16_t EE_Value;

/******************************************************************************
 * Constructors
 ******************************************************************************/
MELEXIS::MELEXIS()
{	
	pinMode (slaveSelectPin, OUTPUT);
	digitalWrite(slaveSelectPin,HIGH); 
	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	SPI.setClockDivider(SPI_CLOCK_DIV4);
	SPI.setDataMode(SPI_MODE1);
}
/******************************************************************************
 * User API
 ******************************************************************************/

uint16_t MELEXIS::get_x()
{
	return inbuffer[0] | ((inbuffer[1]&0x3F)<<8);
}
uint16_t MELEXIS::get_y()
{
	return inbuffer[2] | (inbuffer[3]&0x3F)<<8;
}

uint16_t MELEXIS::get_z()
{
	return inbuffer[4] | (inbuffer[5]&0x3F)<<8;
}

uint8_t MELEXIS::get_diag()
{
	return (inbuffer[1]&0xC0)>>6;
}

uint8_t MELEXIS::get_roll()
{
	return (inbuffer[6] & 0x3F);
}

uint16_t MELEXIS::get_diag_0()
{
	return inbuffer[0] | inbuffer[1]<<8;
}

uint16_t MELEXIS::get_diag_1()
{
	return inbuffer[2] | inbuffer[3]<<8;
}

uint8_t MELEXIS::poll()
{
	memset(&outbuffer,0,sizeof(uint8_t)*8);

	outbuffer[2] = 0xFF;
	outbuffer[3] = 0xFF;
	outbuffer[6] = 0x80 | MELEXIS_GET3;
	
	return do_SPI();
}

uint8_t MELEXIS::diag_poll()
{
	memset(&outbuffer,0,sizeof(uint8_t)*8);

	outbuffer[2] = 0xFF;
	outbuffer[3] = 0xFF;
	outbuffer[6] = 0x80 | MELEXIS_DiagnosticDetails;
	
	return do_SPI();
}

uint8_t MELEXIS::reboot()
{
	memset(&outbuffer,0,sizeof(uint8_t)*8);
	outbuffer[6] = 0xC0 | MELEXIS_Reboot;
	
	return do_SPI();
}


uint8_t MELEXIS::do_SPI()
{
	// This function appends the checksum, clocks out the outbuffer and clocks in the inbuffer
	do_checksum(outbuffer);
	digitalWrite(slaveSelectPin,LOW);
	delayMicroseconds(1);
	for (i=0; i<8; i++)
		inbuffer[i] = SPI.transfer(outbuffer[i]);
	delayMicroseconds(1);
	digitalWrite(slaveSelectPin,HIGH); 
	return do_checksum(inbuffer);
}

uint16_t MELEXIS::set_eeprom(uint16_t addr, uint8_t offset, uint8_t length, uint16_t data)
{
	// Call this function using the defines above. E.G. set_eeprom(MELEXIS_EE_3D,1)

	//-1: Read word to be written into outbuffer
	EE_Value = get_eeprom_word(addr, offset, length); // Get the current value of that 16-bit word.
	// 0: Modify relevant bit/s.
	// Check that data can fit into length bits.
	if (data&(0xFFFF<<length))
		return 9; // Error codes 1-8 are defined in the datasheet.
	EE_Value &= ((0xFFFF<<(offset+length)|(0xFFFF>>(16-offset))));
	EE_Value |= (data<<offset);
	
	// 1: EEWrite(Addr, Key), ignore response
	memset(&outbuffer,0,sizeof(uint8_t)*8);
	outbuffer[1] = 0x003F&addr; // Low six bits of the address to be read
	outbuffer[2] = (get_EE_Key(addr)&0x00FF); // Write in the key appropriate to that EEPROM address.
	outbuffer[3] = (get_EE_Key(addr)&0xFF00)>>8;
	outbuffer[4] = (EE_Value&0x00FF);
	outbuffer[5] = (EE_Value&0xFF00)>>8;
	outbuffer[6] = 0xC0 | MELEXIS_EEPROMWrite;
	do_SPI(); // Transmit the message, ignore the response
	delayMicroseconds(2500);
	
	// 2: EEReadChallenge, read EEChallenge response
	memset(&outbuffer,0,sizeof(uint8_t)*8);
	outbuffer[6] = 0xC0 | MELEXIS_EEReadChallenge;
	do_SPI(); // Transmit the message
	delayMicroseconds(2500);
	
	// 3,4: XOR response with 0x1234,  Respond to challenge with EEChallengeAns, read EEReadAnswer response
	if ((inbuffer[6]&0x3F) != MELEXIS_EEPROMWriteChallenge)
		return 10; // For some reason the MLX didn't respond properly to our read request.
	EE_Value = (inbuffer[2]|(inbuffer[3]<<8));
	
	memset(&outbuffer,0,sizeof(uint8_t)*8);
	EE_Value ^= 0x1234;
	outbuffer[2] = (0x00FF&EE_Value);
	outbuffer[3] = (0xFF00&EE_Value)>>8;
	outbuffer[4] = (0x00FF&(~EE_Value));
	outbuffer[5] = (0xFF00&(~EE_Value))>>8;
	outbuffer[6] = 0xC0 | MELEXIS_EEChallengeAns;
	do_SPI(); // Transmit the message
	delay(40); // Wait tEEWrite
	
	if ((inbuffer[6]&0x3F) != MELEXIS_EEReadAnswer)
		return 11; // For some reason the MLX didn't respond properly
	// 5: Send NOP, receive EEWriteStatus	
	memset(&outbuffer,0,sizeof(uint8_t)*8);
	outbuffer[6] = 0xC0 | MELEXIS_NOP;
	do_SPI();
	if ((inbuffer[6]&0x3F) != MELEXIS_EEPROMWriteStatus)
		return 12; // For some reason the MLX didn't respond properly
	delayMicroseconds(2500);
	EE_Value = inbuffer[0]&0x0F;
	reboot();
	delayMicroseconds(2500);
		
	return EE_Value;
}

uint16_t MELEXIS::get_eeprom(uint16_t addr, uint8_t offset, uint8_t length)
{
	EE_Value = get_eeprom_word(addr, offset, length);
	return (EE_Value<<(16-length-offset))>>(16-length); // Spit out the read memory.
}

uint16_t MELEXIS::get_eeprom_word(uint16_t addr, uint8_t offset, uint8_t length)
{
	memset(&outbuffer,0,sizeof(uint8_t)*8);
	outbuffer[0] = 0x00FF&addr; // Low byte of address to be read
	outbuffer[1] = (0xFF00&addr)>>8; // High byte
	outbuffer[6] = 0xC0 | MELEXIS_MemoryRead; // Memory read opcode
	do_SPI(); // Transmit the message, ignore the response
	delayMicroseconds(2500);
	memset(&outbuffer,0,sizeof(uint8_t)*8);
	outbuffer[6] = 0xC0 | MELEXIS_NOP; // Send a NOP
	do_SPI(); // Receive response from the memory read
	delayMicroseconds(2500);
	return (inbuffer[0]|(inbuffer[1]<<8)); // Spit out the read memory.
}

uint16_t MELEXIS::get_EE_Key(uint16_t addr)
{
	// This does a lookup in the EE_Keys table and grabs the write key for a given EEPROM memory address.
	return EE_Keys[(addr&0x30)>>4][(addr&0x0E)>>1];
}

bool MELEXIS::do_checksum(uint8_t* message)
{
	// Sets the last byte of the message to the CRC-8 of the first seven bytes.
	// Also checks existing checksum, returns 0 if OK, 1 if fail.
	crc = message[7];
	message[7] = 0xFF; 
	for (j=0; j<7; j++)
		message[7] = cba_256_TAB[ message[j] ^ message[7] ];
	message[7] = ~message[7]; 
	
	return !(message[7]==crc);
	
}

