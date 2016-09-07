#include <stdint.h>
#include "lpc111x.h"
#include "GameController.h"
// The arcade version of the invaders game I remember had a joystick which simply contained two buttons: left and right
// It also had a fire button.  Will stick with this three button interface.
// Will use GPIO1
// BIT3 : LEFT
// BIT4 : FIRE
// BIT5 : RIGHT
// Button inputs are pulled high when not pressed
void initGameController()
{
    // Initialize the game controller hardware (ADC, IO ports etc)
    // Turn on GPIO1 (and pull-ups)
    SYSAHBCLKCTRL |= BIT6; // ensure clock is on for GPIO
    IOCON_SWDIO_PIO1_3 = BIT0 + BIT4 + BIT7;
    IOCON_PIO1_4 = BIT4 + BIT5 + BIT7; 
    IOCON_PIO1_5 = BIT4;
}
int16_t readGameController()
{
    static int16_t PreviousFireButtonState = 0;
    int16_t ReturnValue = 0;
    if ((GPIO1DATA & BIT3) == 0)
        ReturnValue |= BIT0; // Left
    
    if ((GPIO1DATA & BIT5) == 0)
        ReturnValue |= BIT1; // Right
    
    if ((GPIO1DATA & BIT4) == 0)
    {
        if (!PreviousFireButtonState)
        {   // return fire if fire had not been previously pressed
            ReturnValue |= BIT2;                         
            //delay(200000);    
        }        
        PreviousFireButtonState = 1;        
    }    
    else
    {
        PreviousFireButtonState = 0;
    }    
    return ReturnValue;
}
    
