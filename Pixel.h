#include <stdint.h>

// Assumption: Top left of display is 0,0
/* Format of colour value: <BGND 1 bit><Red 5 bits><Green 5 bits><Blue 5 bits>
 * If the MSB is set (the BGND bit) then the pixel is transparent 
 * Other bits then define the colours
 */
// Define the primary colours
#define BGND 0x0000
#define RED (0x1f << 11)
#define GRN (0x1f << 5)
#define BLUE (0x1f)
#define WHI (0x7fff)
#define RGBToWord( R,  G,  B) (  ((R&0xf8) << (11-3)) | ((G&0xfc) << (5-2)) | ((B&0xf8)>>3) )

// Set the display up
void initDisplay();
// Simple Pixel drawing routines
void putPixel(uint16_t x, uint16_t y, uint16_t colour);
void putImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *Image);
void fillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t Colour);
void putText(const char *Text, uint16_t len, uint16_t x, uint16_t y, uint16_t ForeColour, uint16_t BackColour);
