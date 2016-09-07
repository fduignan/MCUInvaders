#include <stdint.h>
#include "Pixel.h"
#include "lpc111x.h"
#include "spi.h"
#include "font5x7.h"

void delay_ms(uint32_t dly)
{ // Approx dly ms delay_ms @ 48MHz
        dly=dly*4300;
        while(dly--);
}

void resetDisplay()
{
    GPIO0DATA &=~BIT5;
    delay_ms(10);
    GPIO0DATA |=BIT5;
    delay_ms(120);
}
void CommandMode()
{
    GPIO0DATA &= ~BIT11;
}
void DataMode()
{
    GPIO0DATA |= BIT11;
}
void LCD_Write_Cmd(uint8_t cmd)
{
    CommandMode();
    transferRawSPI(cmd);
}
void LCD_Write_Data(uint8_t data)
{
    DataMode();
    transferRawSPI(data);
}
void gotoXY(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{   // open up an area for drawing on the display
    LCD_Write_Cmd(0x2a);        // Write start, end column addresses
    LCD_Write_Data(x1>>8);      // start column high byte
    LCD_Write_Data(x1);         // start column low byte
    LCD_Write_Data(x2>>8);      // end column high byte
    LCD_Write_Data(x2);         // end column low byte
    LCD_Write_Cmd(0x2b);        // Write start, end page (row) addresses
    LCD_Write_Data(y1>>8);      // start row high byte
    LCD_Write_Data(y1);         // start row low byte
    LCD_Write_Data(y2>>8);      // end row high byte
    LCD_Write_Data(y2);         // end row low byte
    LCD_Write_Cmd(0x2c);        // Send command to indicate image/colour data will follow (ram write command)
}
void initDisplay()
{
            
    // Pin assignment:
    // Pin 1 : MISO
    // Pin 2 : MOSI
    // Pin 3 : SCK	
    // Pin 4 : D/C input to ILI display
    // Pin 5 : Reset for ILI display
    // Pin 25 : SSEL
    
    initSPI(8,0);
   
    // Display initialization based on UTFT Library by Henning Karlsen and ILI9341 Application note
    // This routine has been trimmed back as much as I could get away with
    resetDisplay();

    LCD_Write_Cmd(0xC0);    // Power control 
    LCD_Write_Data(0x21);   // Set GVDD = 4.5V  (varies contrast)

    LCD_Write_Cmd(0xC1);    // Power control 
    LCD_Write_Data(0x10);   // default value

    LCD_Write_Cmd(0xC5);    //VCM control 
    LCD_Write_Data(0x31);   // default values
    LCD_Write_Data(0x3c);   // 

    LCD_Write_Cmd(0xC7);    //VCM control2 
    LCD_Write_Data(0xc0);   // default value

    LCD_Write_Cmd(0x36);    // Memory Access Control 
    LCD_Write_Data(0x48);   // Set display orientation and RGB colour order

    LCD_Write_Cmd(0x3A);    // Set Pixel format
    LCD_Write_Data(0x55);   // To 16 bits per pixel

    LCD_Write_Cmd(0xB1);    // Frame rate control
    LCD_Write_Data(0x00);   // Use Fosc without divisor
    LCD_Write_Data(0x1B);   // set 70Hz refresh rate

    LCD_Write_Cmd(0xB6);    // Display Function Control 
    LCD_Write_Data(0x00);   // Use default values
    LCD_Write_Data(0x82);
    LCD_Write_Data(0x27);  
    
    LCD_Write_Cmd(0x11);    //Exit Sleep 
    delay_ms(120); 
                                
    LCD_Write_Cmd(0x29);    //Display on 
    LCD_Write_Cmd(0x2c); 
    delay_ms(5);
    
    fillRectangle(0,0,240,320,0);
    
    
}
// Simple Pixel drawing routines
void putPixel(uint16_t x, uint16_t y, uint16_t Colour)
{
    gotoXY(x,y,x+1,y+1);
    LCD_Write_Data(Colour >> 8);
    LCD_Write_Data(Colour & 0xff);
}
void putImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *Image)
{
    uint16_t Colour;
    gotoXY(x,y,x+width-1,y+height-1);
    for (y = 0; y < height; y++)
    {
        for (x=0; x < width; x++)
        {
            Colour = *(Image++);                        
            LCD_Write_Data(Colour >> 8);
            LCD_Write_Data(Colour & 0xff);                
        }
    }
    /*Colour = *(Image++);                        
            LCD_Write_Data(Colour >> 8);
            LCD_Write_Data(Colour & 0xff);
    Colour = *(Image++);                        
            LCD_Write_Data(Colour >> 8);
            LCD_Write_Data(Colour & 0xff);
            */
}
void fillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t Colour)
{
    gotoXY(x,y,x+width-1,y+height-1);
    for (y = 0; y < height; y++)
        for (x=0; x < width; x++)
        {
            LCD_Write_Data(Colour >> 8);
            LCD_Write_Data(Colour & 0xff);
        }
}
#define FONT_WIDTH 5
#define FONT_HEIGHT 7
static uint16_t TextBox[FONT_WIDTH*FONT_HEIGHT];
// Assuming font starts at ASCII character 32 (a space) and ends at 127
void putText(const char *Text, uint16_t len, uint16_t x, uint16_t y, uint16_t ForeColour, uint16_t BackColour)
{
    int Index=0;
    int Row,Col;
    const uint8_t *CharacterCode = 0;
    
    for (Index = 0; Index < len; Index++)
    {
        
        CharacterCode = &Font5x7[FONT_WIDTH*(Text[Index]-32)]; 
        Col=0;
        while (Col < FONT_WIDTH)
        {
            Row = 0;
            while (Row < FONT_HEIGHT)
            {
                if (CharacterCode[Col] & (1 << Row))
                {
                    TextBox[(Row*FONT_WIDTH)+Col]=ForeColour;                    
                }
                else
                {
                    TextBox[(Row*FONT_WIDTH)+Col]=BackColour;                    
                }                                
                Row++;                
            }            
            Col++;
        }
        putImage(x,y,FONT_WIDTH,FONT_HEIGHT,(const uint16_t *)TextBox);
        x = x + FONT_HEIGHT + 2;
        
        
    }
}
