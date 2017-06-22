//main functions
#include "myLib.h"
#include "font.h"

u16 *videoBuffer = (u16 *) 0x6000000;

//set specific pixel to u16 color
void setPixel(int row, int col, unsigned short color) {
	videoBuffer[OFFSET(row, col, 240)] = color;
}

//draw image function
void drawImage3(int r, int c, int width, int height, const u16* image) {
  for (u16 i = 0; i < height; i++)
  {
  		DMA[3].src = (volatile u32 *) (image + (width*i));
    	DMA[3].dst = (volatile u32 *) (videoBuffer + (240*(r + i) + c));
    	DMA[3].cnt = width | DMA_ON;
  }
}

//draw sprite within 13x8 sprite area (0,0) to (12,7), ignores  bgcolor
void drawSprite3(int x, int y, const u16* image) {
	for (u16 height = 0; height < 16; height++) {
		for (u16 width = 0; width < 16; width++) {
			//pulls color from image array
			u16 color = image[height*16+width];
			switch (color) {
				//ignore 0x7FFF (alpha default)
				case 0x7FFF:
					break;
				default:
					//draw if not transparent->0x7FFF pixel
					setPixel(16*(y+1)+height,width+(x+1)*16,image[width+height*16]);
					break;
			}
		}
	}
	
}

//draw tile within 15x10 tile area
void drawTile(int x, int y, const u16* tile) {
	drawImage3(16*(y-1), 16*(x-1), 16, 16, tile);
}

//draw sprite within 13x8 sprite area (0,0) to (12,7), does not ignore bg
void drawSprite(int x, int y, const u16* sprite) {
	drawImage3(16+y*16, 16+x*16, 16, 16, sprite);
}

//fills screen with a single color
void fillScreen(volatile u16 color)  {
	REG_DMA3SAD = (volatile u32)&color;
	REG_DMA3DAD = (volatile u32)videoBuffer;
	REG_DMA3CNT = (240*160) | DMA_ON | DMA_SOURCE_FIXED;
}

void waitForVBlank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}


//draw char
void drawChar(int row, int col, char ch, u16 color) {
	int r, c;
	for(r=0; r<8; r++)
	{
		for(c=0; c<6; c++)
		{
			if(fontdata_6x8[ch*48+OFFSET(r,c,6)])
			{
				setPixel(row+r, col+c, color);
			}
		}
	}
}

//draw complete string from buffer
void drawString(int row, int col, char *str, u16 color)
{
	while(*str)
	{
		drawChar(row, col, *str++, color);
		col += 6;
	}
}


//loops to delay computing time
void delay(int time) {
  volatile int x;
  for (int i = 0; i < time*8000; i++) {
  	x = x + 1;
  }
}