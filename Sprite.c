

#include "Sprite.h"
#include "Pixel.h"
void initSprite(Sprite * sp,const uint16_t * InitArray, uint8_t Width, uint8_t Height, uint16_t x, uint16_t y)
{

    // Construction based on a 1 D pixel array    
    sp->x = x;
    sp->y = y;
    sp->width = Width;
    sp->height = Height;
    uint16_t PixelIndex=0;
    sp->SpriteImage = InitArray;
    
}
// Show/hide the sprite        
void drawSprite(Sprite * sp)
{
    if (sp->visible)
    {    
        putImage(sp->x,sp->y,sp->width,sp->height,sp->SpriteImage);
    } 
    else
    {
        fillRectangle(sp->x,sp->y,sp->width,sp->height,0);
    }    
}
void showSprite(Sprite * sp)
{
    sp->visible = 1;
    drawSprite(sp);
}
void hideSprite(Sprite * sp)                
{
    sp->visible = 0;
    drawSprite(sp);
}
// Move to the specified position
void moveSprite(Sprite * sp,uint16_t x, uint16_t y)
{
    uint8_t WasVisible = sp->visible;
    if (WasVisible)
        hideSprite(sp);
    sp->x = x;
    sp->y = y;
    if (WasVisible)
        showSprite(sp);    
}
// Does the sprite contain the given point : sprite must be visible
uint8_t withinSprite(Sprite * sp,uint16_t x, uint16_t y)
{
    if (sp->visible == 0)
        return 0;
    if ( (x>=sp->x) && ( x < (sp->x+sp->width) ) )
    {
        if ( (y>=sp->y) && ( y < (sp->y+sp->height) ) )
            return 1;
        else
            return 0;
                  
    }
    else
        return 0;
}
