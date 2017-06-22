//myLib.h
//Name: Quang Vo

//data typedef
#ifndef myLib_h
#define myLib_h
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;


#define NUMSPRITES 2;

#define REG_DISPCNT *(u16*) 0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)

#define SCANLINECOUNTER *(volatile u16 *)0x4000006
#define OFFSET(row, col, rowlen) ((row)*(rowlen)+(col))
extern unsigned short *videoBuffer;

//colors
#define COLOR(R,G,B) (((R) & 0x1F) | (((G) & 0x1F) << 5) | (((B) & 0x1F) << 10))

#define BLACK   COLOR(0 , 0 , 0 )
#define WHITE   COLOR(31, 31, 31)
#define RED     COLOR(31, 0 , 0 )
#define GREEN   COLOR(0 , 31, 0 )
#define BLUE    COLOR(0 , 0 , 31)
#define YELLOW  COLOR(31, 31, 0 )
#define MAGENTA COLOR(31, 0 , 31)
#define CYAN    COLOR(0 , 31, 31)
#define ORANGE  COLOR(31, 15, 0 )
#define BROWN   COLOR(18, 9 , 0 )
#define PURPLE  COLOR(15, 0 , 15)
#define TEAL    COLOR(0 , 15, 15)
#define MAROON  COLOR(15, 0 , 0 )
#define GREY    COLOR(15, 15, 15)
#define PINK    COLOR(31, 18, 19)

//buttons
#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

#define BUTTONS *(volatile unsigned int *)0x4000130

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

//DMA
#define REG_BG0CNT *(volatile u16*) 0x4000008;

#define REG_DMA0SAD         *(volatile u32*)0x40000B0 		// source address
#define REG_DMA0DAD         *(volatile u32*)0x40000B4       // destination address
#define REG_DMA0CNT         *(volatile u32*)0x40000B8       // control register

// DMA channel 1 register definitions
#define REG_DMA1SAD         *(volatile u32*)0x40000BC 		// source address
#define REG_DMA1DAD         *(volatile u32*)0x40000C0       // destination address
#define REG_DMA1CNT         *(volatile u32*)0x40000C4       // control register

// DMA channel 2 register definitions
#define REG_DMA2SAD         *(volatile u32*)0x40000C8 		// source address
#define REG_DMA2DAD         *(volatile u32*)0x40000CC       // destination address
#define REG_DMA2CNT         *(volatile u32*)0x40000D0       // control register

// DMA channel 3 register definitions
#define REG_DMA3SAD         *(volatile u32*)0x40000D4 		// source address
#define REG_DMA3DAD         *(volatile u32*)0x40000D8       // destination address
#define REG_DMA3CNT         *(volatile u32*)0x40000DC       // control register

//initial sprite definitions
#define OBJ_ENABLE 0x1000
#define MODE1D (1<6)

typedef struct {
	u16 attr0;
	u16 attr1;
	u16 attr2;
	u16 fill;
} OamEntry;

#define OAMEM ((OamEntry*)0x7000000)

#define ATTR0_REG (0<<8) //default all shown
#define ATTR0_HIDE (2<<8) //hidden sprites



typedef struct
{
	const volatile void *src;
	volatile void *dst;
	unsigned int cnt;
} DMA_CONTROLLER;


#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

enum GameSTATE {
	TITLE,
};

//prototypes
void waitForVBlank();
void drawString(int row, int col, char *str, u16 color);
extern const unsigned char fontdata_6x8[12288];
void drawChar(int row, int col, char ch, u16 color);
void setPixel(int row, int col, u16 color);
void drawImage3(int r, int c, int width, int height, const u16* image);
void drawSprite(int x, int y, const u16* sprite);
void drawSprite3(int x, int y, const u16* sprite);
void drawTile(int x, int y, const u16* tile);
void fillScreen(volatile u16 color);
void fillWalls(volatile u16 image);
void delay(int time);
void blankscreen();

#endif