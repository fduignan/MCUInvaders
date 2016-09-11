// spi.h for LPC1114
// Author: Frank Duignan.  
// Updates posted on http://ioprog.com and http://eleceng.dit.ie/frank

#include "lpc111x.h"
#include <stdint.h>
#include "spi.h"
void initSPI(int bits, int mode)
{
	int drain_count,drain;
	// Pin assignment:
	// Pin 1 : MISO
	// Pin 2 : MOSI
	// Pin 3 : SCK	
	// Pin 4 : D/C input to ILI display
	// Pin 5 : Reset for ILI display
	// Pin 25 : SSEL        
	
// Interesting discovery: Using SSEL slows down the transfer of data to the display - a lot
// Using SSEL on each byte transferred (SPI hardware control of the SSEL pin) the max SPI clock speed was 1MHz approx
// Forcing SSEL low from boot and keeping it there allows clock speed of 24MHz. Yay! :)
    // will default to a speed of 1MHz
    SYSAHBCLKCTRL |= BIT6+BIT16; // turn on IOCON and GPIO0
    
    // Make Port 0 bit 5 behave as a generic output port (open drain)
    IOCON_PIO0_5 |= BIT8;
    // Make Port 0 bit 10 behave as a generic I/O port
    IOCON_SWCLK_PIO0_10  = 1; 
    IOCON_R_PIO0_11 = BIT7 + BIT0; // Make R_PIO0_11 behave as a standard output
    GPIO0DIR |= BIT11 + BIT5; // Make GPIO0_11 and GPIO0_5 outputs
    GPIO0DATA &= ~(BIT11+BIT5);                 // Make outputs 11,5 low initially  
    
    IOCON_PIO0_2 = 0;// 0x11;// Hardware control of SSEL0 with pullup
    GPIO0DIR |= BIT2; // Make GPIO0.2 an output
    GPIO0DATA &= ~BIT2; // Make GPIO0.2 high initially
    IOCON_SCK0_LOC = 0; // route SCK0 towards PIO0_10
    IOCON_SWCLK_PIO0_10 = 0x12;// select SCK0 function and enable pullup
    IOCON_PIO0_8 = 0x11; // select MISO0 function and enable pullup
    IOCON_PIO0_9 = 0x11; // select MOSI0 function and enable pullup
    SYSAHBCLKCTRL |= BIT11; // turn on SSP0 
    PRESETCTRL &= ~BIT0; // ensure SPI0 is in reset
    PRESETCTRL |= BIT0; // take SPI0 out of reset
    // Next need to set up the SPI frame format and speed
    // PCLK = 48MHz.  
    SSP0CLKDIV = 1; // divide down PCLK by 1 
    SSP0CPSR = 2; // Set CPS divisor to 1 to give clock rate of 24MHz (Outside spec of display but works)
    SSP0CR0 = (bits-1) + (mode << 6) ;
    SSP0CR1 =  BIT1; // select master mode and enable SPI  
    
    for (drain_count = 0; drain_count < 32; drain_count++)
		drain = transferSPI(0x00);
}

uint8_t transferSPI(uint8_t data)
{    
    SSP0DR = data;
    while (((SSP0SR & BIT4)!=0));
    return SSP0DR;
}
