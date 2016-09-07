// Definitions for a simple sprite class.  
// The objective is to minimize the number of methods and data involved



#define MAX_HEIGHT 16
#define MAX_WIDTH 8
#define BACKGROUND_COLOUR 0
#include <stdint.h>
typedef struct  {
// Location of top left corner. 
    uint16_t x;
    uint16_t y;
// Dimensions of sprite
    uint16_t width;
    uint16_t height;
// The pixels for the sprite are stored at this location
    const uint16_t *SpriteImage;
    uint8_t visible;
} Sprite;
// Construction based on 2 D pixel array    
void initSprite(Sprite * sp,const uint16_t * InitArray, uint8_t Width, uint8_t Height, uint16_t x, uint16_t y);
// Show/hide the sprite        
void showSprite(Sprite * sp);
void hideSprite(Sprite * sp);       
void drawSprite(Sprite * sp);
// Move to the specified position
void moveSprite(Sprite * sp,uint16_t x, uint16_t y);
// Does the sprite contain the given point
uint8_t withinSprite(Sprite * sp,uint16_t x, uint16_t y);
            
