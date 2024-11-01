
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
 *
 *	File	:	MCP3008_AVR_ATmega328p.ino
 *	Release	:	v1.0
 *
 *	Created on	:	Thu 10 Oct 2024
 *		Author	:	hii-nice-2-meet-u
 *
 *		[ Contact Information ]
 *	G-mail		:	0x0.whitecat@gmail.com
 *	Discord		:	@hii_nice.2.meet.u
 *	Github		:	https://github.com/hii-nice-2-meet-u
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "hii.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//$ PIN Macros MCP3008
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//' --------------------------------
//> Define Pin
//' --------------------------------

#define MCP3008_CS		PINC0
#define MCP3008_MISO	PINC1
#define MCP3008_MOSI	PINC2
#define MCP3008_SCLK	PINC3

//' --------------------------------
//> Macro Bit
//' --------------------------------

#define bit_CS			(1 << MCP3008_CS)
#define bit_MISO		(1 << MCP3008_MISO)
#define bit_MOSI		(1 << MCP3008_MOSI)
#define bit_SCLK		(1 << MCP3008_SCLK)

#define bit_Start		(1 << 4)
#define bit_Config		(1 << 3)	//& Single_Ended
// #define bit_Config		(0)			//& Differential

//' --------------------------------
//> Macro Set PIN
//' --------------------------------

#define setH_CS			PORTC |= bit_CS;
#define setL_CS			PORTC &= ~bit_CS;
#define setH_MOSI		PORTC |= bit_MOSI;
#define setL_MOSI		PORTC &= ~bit_MOSI;
#define setH_SCLK		PORTC |= bit_SCLK;
#define setL_SCLK		PORTC &= ~bit_SCLK;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//` Function initialize MCP3008
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//$ Function initialize MCP3008
void _init_MCP3008(void)
{
	//- Set PIN Mode
	DDRC |= (bit_CS | bit_MOSI | bit_SCLK);
	DDRC &= ~bit_MISO;

	//>> Close Communication
	setH_CS;

	blink(25);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//` Function Read MCP3008
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//$ Send & Receive DATA MCP3008
unsigned short MCP3008_Read(unsigned char Ch)
{
	//>> Start Communication
	setL_CS;
	setL_SCLK;

	//' --------------------------------

	//- Temporary Variables of Transmit Byte
	unsigned char TEMP_8b = (Ch | bit_Start | bit_Config);

	//- Repeat For Sent bit
	for (unsigned char ii = 0; ii < 5; ii++)
	{
		//+ Transmit Bit
		if (TEMP_8b & 0b00010000)
		{
			setH_MOSI;
		}
		else
		{
			setL_MOSI;
		}

		TEMP_8b <<= 1;

		//+ Shift to next Communication Bit
		setH_SCLK;
		setL_SCLK;
	}

	//' --------------------------------

	//- Temporary Variables of Receive Byte
	unsigned short TEMP_16b = 0;

	//- Repeat For Receive bit
	for (unsigned char ii = 0; ii < 12; ii++)
	{
		//+ Shift to next Communication Bit
		setH_SCLK;
		setL_SCLK;

		//+ Get Receive Bit
		TEMP_16b <<= 1;
		if (PINC & bit_MISO)
		{
			TEMP_16b |= 1;
		}
	}

	//' --------------------------------

	//>> Close Communication
	setH_CS;

	//? RETURN Receive Value
	return TEMP_16b;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~