#ifndef __LCD__
#define __LCD__

#include <stdint.h>
#include "stm32f10x.h"

#define LCD_DC			GPIO_Pin_1
#define LCD_CE			GPIO_Pin_2
#define LCD_RST			GPIO_Pin_3

void lcd_setup(void);

void lcd_clear(void);
void lcd_draw_bitmap(const uint8_t* data);
void lcd_draw_text(int row, int col, const char* text);

void lcd_copy(void);

#endif // __LCD__
