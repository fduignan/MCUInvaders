#include "Pixel.h"
#include "Sprite.h"
#include "GameController.h"
#include "serial.h"
#include "lpc111x.h"

// Screen is 240 wide and 320 high
// 
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

//  An invader is shown below
/*     
    ..........
    ....##....
    ...####...
    .##X##X##.
    .##X##X##.
    .########.
    .########.
    ..##..##..
    ..##..##..
    .##....##.
    ###....###
    ..........
*/
// Numeric declaration of this invader

#define IVDR RGBToWord(0xff,0xff,0x00)
const uint16_t InvaderImage[] = { \
    BGND,BGND,BGND,BGND,BGND,BGND,BGND,BGND,BGND,BGND,\
    BGND,BGND,BGND,BGND,IVDR,IVDR,BGND,BGND,BGND,BGND,\
    BGND,BGND,BGND,IVDR,IVDR,IVDR,IVDR,BGND,BGND,BGND,\
    BGND,IVDR,IVDR,BGND,IVDR,IVDR,BGND,IVDR,IVDR,BGND,\
    BGND,IVDR,IVDR,BGND,IVDR,IVDR,BGND,IVDR,IVDR,BGND,\
    BGND,IVDR,IVDR,IVDR,IVDR,IVDR,IVDR,IVDR,IVDR,BGND,\
    BGND,IVDR,IVDR,IVDR,IVDR,IVDR,IVDR,IVDR,IVDR,BGND,\
    BGND,BGND,IVDR,IVDR,BGND,BGND,IVDR,IVDR,BGND,BGND,\
    BGND,BGND,IVDR,IVDR,BGND,BGND,IVDR,IVDR,BGND,BGND,\
    BGND,IVDR,IVDR,BGND,BGND,BGND,BGND,IVDR,IVDR,BGND,\
    IVDR,IVDR,IVDR,BGND,BGND,BGND,BGND,IVDR,IVDR,IVDR,\
    BGND,BGND,BGND,BGND,BGND,BGND,BGND,BGND,BGND,BGND \
    };   

/* 
 The player Defender Ship is as follows:
 ..........
 ....##....
 ....##....
 .########.
 ##########
 ##########
 */
// Numeric declaration of Defender
const uint16_t DefenderImage[] = { \
    BGND,BGND,BGND,BGND,BGND,BGND,BGND,BGND,BGND,BGND,\
    BGND,BGND,BGND,BGND,WHI ,WHI ,BGND,BGND,BGND,BGND,\
    BGND,BGND,BGND,BGND,WHI ,WHI ,BGND,BGND,BGND,BGND,\
    BGND,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,BGND,\
    WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,\
    WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,WHI ,\
};



/* The missile image is as follows 
    ..#..
    .###.
    .###.
    .###.
    .###.
    #####
    #####
    ..#..
*/

const uint16_t MissileImage[]= { \
    BGND,BGND,GRN ,BGND,BGND, \
    BGND,GRN ,GRN ,GRN ,BGND, \
    BGND,GRN ,GRN ,GRN ,BGND, \
    BGND,GRN ,GRN ,GRN ,BGND, \
    BGND,GRN ,GRN ,GRN ,BGND, \
    GRN ,GRN ,GRN ,GRN ,GRN , \
    GRN ,GRN ,GRN ,GRN ,GRN , \
    BGND,BGND,GRN ,BGND,BGND  \
};


#define MAX_INVADERS 10
Sprite  Invaders[MAX_INVADERS]= {\
                    {3,3,10,12,InvaderImage,1} ,\
                    {23,3,10,12,InvaderImage,1} ,\
                    {43,3,10,12,InvaderImage,1} ,\
                    {63,3,10,12,InvaderImage,1} ,\
                    {83,3,10,12,InvaderImage,1}, \
                    {83,3,10,12,InvaderImage,1}, \
                    {103,3,10,12,InvaderImage,1}, \
                    {123,3,10,12,InvaderImage,1}, \
                    {143,3,10,12,InvaderImage,1}, \
                    {163,3,10,12,InvaderImage,1} \
                    };
                    
                    
Sprite  Defender={195,280,10,6,DefenderImage,0};
#define MAX_MISSILES 5
Sprite Missiles[MAX_MISSILES]={{0,0,5,8,MissileImage,0} ,\
                    {0,0,5,8,MissileImage,0} ,\
                    {0,0,5,8,MissileImage,0} ,\
                    {0,0,5,8,MissileImage,0} ,\
                    {0,0,5,8,MissileImage,0}  };
// The explosion image is as follows
/*     
    ....##....
    #...##...#
    .#..##..#.
    ..#.##.#..
    ...####...
    ##########
    ##########
    ...####...
    ..#.##.#..
    .#..##..#.
    #...##...#
    ....##....
*/                    
// Numeric declaration of Explosion
#define EXPL RGBToWord(0xff,0x00,0x00)
      const uint16_t ExplosionImage[] = { \
    BGND,BGND,BGND,BGND,EXPL,EXPL,BGND,BGND,BGND,BGND,\
    EXPL,BGND,BGND,BGND,EXPL,EXPL,BGND,BGND,BGND,EXPL,\
    BGND,EXPL,BGND,BGND,EXPL,EXPL,BGND,BGND,EXPL,BGND,\
    BGND,BGND,EXPL,BGND,EXPL,EXPL,BGND,EXPL,BGND,BGND,\
    BGND,BGND,BGND,EXPL,EXPL,EXPL,EXPL,BGND,BGND,BGND,\
    EXPL,EXPL,EXPL,EXPL,EXPL,EXPL,EXPL,EXPL,EXPL,EXPL,\
    EXPL,EXPL,EXPL,EXPL,EXPL,EXPL,EXPL,EXPL,EXPL,EXPL,\
    BGND,BGND,BGND,EXPL,EXPL,EXPL,EXPL,BGND,BGND,BGND,\
    BGND,BGND,EXPL,BGND,EXPL,EXPL,BGND,EXPL,BGND,BGND,\
    BGND,EXPL,BGND,BGND,EXPL,EXPL,BGND,BGND,EXPL,BGND,\
    EXPL,BGND,BGND,BGND,EXPL,EXPL,BGND,BGND,BGND,EXPL,\
    BGND,BGND,BGND,BGND,EXPL,EXPL,BGND,BGND,BGND,BGND,\
    };                

void delay(uint32_t dly)
{
    while(dly--);
}
    
void Explode(uint16_t x, uint16_t y)
{
    Sprite Bang={x-5,y-6,10,12,ExplosionImage,1};
    showSprite(&Bang);
    delay(100000);
    hideSprite(&Bang);
}

void blinky()
{
    SYSAHBCLKCTRL |= BIT6 + BIT16; // Turn on clock for GPIO and IOCON 
    IOCON_PIO0_3 &= ~(BIT2+BIT1+BIT0);  // ensure Pin 26 behaves as GPIO
    GPIO0DIR |= BIT3;       // Make Pin 26 an output        
    while(1) 
    {
      
        GPIO0DATA ^= BIT3;
        delay(1000000);        
    
    } 
}


int main()
{
    uint8_t GameOver = 0;
    uint8_t MissileCounter = 0;
    uint8_t InvaderCounter = 0;
    int DirectionChange=0;
    int Victory = 0;
    uint16_t x_step = 1;
    uint16_t ControllerState;
    initUART();    
    printString("MCU Invaders!");
    initDisplay();    
    
    
    initGameController();
    putText("MCU INVADERS", 12, 10, 40, RGBToWord(0xff,0xff,0xff), RGBToWord(0,0,0));
    delay(10000000);
    fillRectangle(0,0,240,320,0);
    // Show the defender
    showSprite(&Defender);
    while (!GameOver)    
    {
       
        ControllerState = readGameController();
        if (ControllerState & BIT0)
        {
            // Move right
            if (Defender.x < (SCREEN_WIDTH - 10))
            {
                moveSprite(&Defender,Defender.x+1,Defender.y);
                
            }
        }
        if (ControllerState & BIT1)
        {
            // Move left            
            if (Defender.x > 0)
            {
                moveSprite(&Defender,Defender.x-1,Defender.y);
                
            }
        }
        if (ControllerState & BIT2 )
        {
            // User pressed Fire!
            // See if there is an empty missile-in-flight slot
            for (MissileCounter = 0; MissileCounter < MAX_MISSILES; MissileCounter++)
            {
                if (Missiles[MissileCounter].visible==0)
                {
                    // Found an empty slot, place a missile just above the centre of the defender
                    showSprite(&Missiles[MissileCounter]);
                    moveSprite(&Missiles[MissileCounter],Defender.x+(Defender.width/2)-(Missiles[MissileCounter].width/2),Defender.y-Missiles[MissileCounter].height);                    
                    break; // exit remaining for loop cycles
                }
            }
        }
        // Update missiles
        for (MissileCounter = 0; MissileCounter < MAX_MISSILES; MissileCounter++)
        {
            if ((Missiles[MissileCounter].y > 0) && (Missiles[MissileCounter].visible))
            {
                moveSprite(&Missiles[MissileCounter],Missiles[MissileCounter].x,Missiles[MissileCounter].y-1);
            }
            else
            {
                if (Missiles[MissileCounter].visible)
                    Missiles[MissileCounter].visible = 0;
            }
            // Did any missile hit an invader?
            for (InvaderCounter = 0; InvaderCounter < MAX_INVADERS; InvaderCounter++)
            {
                if (withinSprite(&Invaders[InvaderCounter],Missiles[MissileCounter].x,Missiles[MissileCounter].y))
                {
                    // Missile meets invader!
                    hideSprite(&Invaders[InvaderCounter]);
                    hideSprite(&Missiles[MissileCounter]);                    
                    // Do an explosion at that location
                    Explode(Missiles[MissileCounter].x,Missiles[MissileCounter].y);
                }
            }        
        }
        // Update invaders
        DirectionChange = 0;
        Victory = 1;
        for (InvaderCounter = 0; InvaderCounter < MAX_INVADERS; InvaderCounter++)
        {
            if (Invaders[InvaderCounter].visible) 
            {
                Victory = 0; // didn't win yet, there are still invaders
                moveSprite(&Invaders[InvaderCounter],Invaders[InvaderCounter].x+x_step,Invaders[InvaderCounter].y+1);
                if (Invaders[InvaderCounter].x >= (SCREEN_WIDTH-Invaders[InvaderCounter].width))
                    DirectionChange = 1;
                if (Invaders[InvaderCounter].x == 0)
                    DirectionChange = 1;
                if (Invaders[InvaderCounter].y > SCREEN_HEIGHT)
                    Invaders[InvaderCounter].y = 0;
                if (withinSprite(&Defender,Invaders[InvaderCounter].x+Invaders[InvaderCounter].width/2,Invaders[InvaderCounter].y+Invaders[InvaderCounter].height/2))
                {
                    Explode(Defender.x,Defender.y);
                    printString("Game Over");
                    fillRectangle(0,0,240,320,0);
                    putText("GAME OVER", 9, 40, 100, RGBToWord(0xff,0xff,0xff), 0);
                    while(1);
                }
            }
            
        }
        if (Victory)
        {
            fillRectangle(0,0,240,320,RGBToWord(0,0,0xff));
            putText("VICTORY!", 8, 40, 100, RGBToWord(0xff,0xff,0), RGBToWord(0,0,0xff));
            while(1);
        }
        if (DirectionChange) // Did an invader hit either edge?
            x_step = -x_step; // if so, then reverse direction
                        
    }
    blinky();
    
}


