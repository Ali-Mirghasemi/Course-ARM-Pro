#include "stm32f4xx.h"
#include "smm_delay.h"

void HD44780_Clear(void);
void HD44780_Puts(uint8_t x, uint8_t y, char* str);
void HD44780_DisplayOn(void);
void HD44780_DisplayOff(void);
void HD44780_BlinkOn(void);
void HD44780_BlinkOff(void);
void HD44780_CursorOn(void);
void HD44780_CursorOff(void);
void HD44780_ScrollLeft(void);
void HD44780_ScrollRight(void);
void HD44780_CreateChar(uint8_t location, uint8_t *data);
void HD44780_PutCustom(uint8_t x, uint8_t y, uint8_t location);
void HD44780_Init(uint8_t cols, uint8_t rows);
