#ifndef GBA_H
#define GBA_H


typedef unsigned int u32;

typedef unsigned short u16;

typedef unsigned char u8;


#define OFFSET(row, col, width) ((col)+(width)*(row))

#define REG_DISPCNT (*(volatile unsigned short *) 0x4000000)
#define MODE3 3
#define BG2_ENABLE (1<<10)

#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define WHITE COLOR(31, 31, 31)
#define RED COLOR(31, 0, 0)
#define GREEN COLOR(0, 31, 0)
#define BLUE COLOR(0, 0, 31)
#define MAGENTA COLOR(31, 0, 31)
#define CYAN COLOR(0, 31, 31)
#define YELLOW COLOR(31, 31, 0)
#define BLACK COLOR(0, 0, 0)
#define GRAY COLOR(5, 5, 5)

#define WIDTH 240
#define HEIGHT 160

extern volatile unsigned short *videoBuffer;

#define BUTTON_A        (1<<0)
#define BUTTON_B        (1<<1)
#define BUTTON_SELECT   (1<<2)
#define BUTTON_START    (1<<3)
#define BUTTON_RIGHT    (1<<4)
#define BUTTON_LEFT     (1<<5)
#define BUTTON_UP       (1<<6)
#define BUTTON_DOWN     (1<<7)
#define BUTTON_R        (1<<8)
#define BUTTON_L        (1<<9)

#define BUTTONS (*(volatile u32 *) 0x4000130)
#define KEY_DOWN(key, buttons) (~(buttons) & (key))

#define KEY_JUST_PRESSED(key, buttons, oldbuttons) (~(KEY_DOWN(key, oldbuttons)) & KEY_DOWN(key, buttons))

typedef struct
{
  const volatile void *src;
  const volatile void *dst;
  u32                  cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

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

#define SCANLINECOUNTER (*(volatile unsigned short *) 0x4000006)

extern u32 vBlankCounter;

void waitForVBlank(void);

#define UNUSED(param) ((void)((param)))

int randint(int min, int max);

void setPixel(int row, int col, u16 color);
void drawRectDMA(int row, int col, int width, int height, volatile u16 color);
void drawFullScreenImageDMA(const u16 *image);
void drawImageDMA(int row, int col, int width, int height, const u16 *image);
void undrawImageDMA(int row, int col, int width, int height, const u16 *image);
void fillScreenDMA(volatile u16 color);
void drawChar(int row, int col, char ch, u16 color);
void drawString(int row, int col, char *str, u16 color);
void drawCenteredString(int row, int col, int width, int height, char *str, u16 color);

extern const unsigned char fontdata_6x8[12288];

#endif
