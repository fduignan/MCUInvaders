// spi.h for LPC1114
#include <stdint.h>
void initSPI(int bits, int mode);
uint8_t transferSPI(uint8_t data);
