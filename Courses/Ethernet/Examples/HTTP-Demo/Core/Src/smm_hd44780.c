#include "smm_hd44780.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"

//RS
#define RS_PIN   GPIO_PIN_11
#define RS_PORT  GPIOD
#define RS_RCC   __HAL_RCC_GPIOD_CLK_ENABLE

//Rw
#define RW_PIN   GPIO_PIN_10
#define RW_PORT  GPIOD
#define RW_RCC   __HAL_RCC_GPIOD_CLK_ENABLE

//E
#define E_PIN    GPIO_PIN_7
#define E_PORT   GPIOD
#define E_RCC    __HAL_RCC_GPIOD_CLK_ENABLE

//D4
#define D4_PIN   GPIO_PIN_15
#define D4_PORT  GPIOD
#define D4_RCC   __HAL_RCC_GPIOD_CLK_ENABLE

//D5
#define D5_PIN   GPIO_PIN_14
#define D5_PORT  GPIOD
#define D5_RCC   __HAL_RCC_GPIOD_CLK_ENABLE

//D6
#define D6_PIN   GPIO_PIN_13
#define D6_PORT  GPIOD
#define D6_RCC   __HAL_RCC_GPIOD_CLK_ENABLE

//D7
#define D7_PIN   GPIO_PIN_12
#define D7_PORT  GPIOD
#define D7_RCC   __HAL_RCC_GPIOD_CLK_ENABLE

#define RS_LOW   RS_PORT->ODR &=~RS_PIN
#define RS_HIGH  RS_PORT->ODR |= RS_PIN
#define E_LOW    E_PORT->ODR &=~E_PIN
#define E_HIGH   E_PORT->ODR |= E_PIN

#define HD44780_Delay(x) usDelay(x)
#define E_BLINK  E_HIGH; HD44780_Delay(20); E_LOW; HD44780_Delay(20)
void HD44780_InitPins(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	//__HAL_RCC_GPIOD_CLK_ENABLE();
	RS_RCC();
	RW_RCC();
	E_RCC();
	D4_RCC();
	D5_RCC();
	D6_RCC();
	D7_RCC();
	
	
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	
	GPIO_InitStructure.Pin = RS_PIN;
	HAL_GPIO_Init(RS_PORT, &GPIO_InitStructure);
	HAL_GPIO_WritePin(RS_PORT, RS_PIN, (GPIO_PinState) 0);
	
	GPIO_InitStructure.Pin = RW_PIN;
	HAL_GPIO_Init(RW_PORT, &GPIO_InitStructure);
	HAL_GPIO_WritePin(RW_PORT, RW_PIN, (GPIO_PinState) 0);
	
	GPIO_InitStructure.Pin = E_PIN;
	HAL_GPIO_Init(E_PORT, &GPIO_InitStructure);
	HAL_GPIO_WritePin(E_PORT, E_PIN, (GPIO_PinState) 0);
	
	GPIO_InitStructure.Pin = D4_PIN;
	HAL_GPIO_Init(D4_PORT, &GPIO_InitStructure);
	HAL_GPIO_WritePin(D4_PORT, D4_PIN, (GPIO_PinState) 0);
	
	GPIO_InitStructure.Pin = D5_PIN;
	HAL_GPIO_Init(D5_PORT, &GPIO_InitStructure);
	HAL_GPIO_WritePin(D5_PORT, D5_PIN, (GPIO_PinState) 0);
	
	GPIO_InitStructure.Pin = D6_PIN;
	HAL_GPIO_Init(D6_PORT, &GPIO_InitStructure);
	HAL_GPIO_WritePin(D6_PORT, D6_PIN, (GPIO_PinState) 0);
	
	GPIO_InitStructure.Pin = D7_PIN;
	HAL_GPIO_Init(D7_PORT, &GPIO_InitStructure);
	HAL_GPIO_WritePin(D7_PORT, D7_PIN, (GPIO_PinState) 0);
}
/****************************************/
/* Commands*/
#define HD44780_CLEARDISPLAY        0x01
#define HD44780_RETURNHOME          0x02
#define HD44780_ENTRYMODESET        0x04
#define HD44780_DISPLAYCONTROL      0x08
#define HD44780_CURSORSHIFT         0x10
#define HD44780_FUNCTIONSET         0x20
#define HD44780_SETCGRAMADDR        0x40
#define HD44780_SETDDRAMADDR        0x80

/* Flags for display entry mode */
#define HD44780_ENTRYRIGHT          0x00
#define HD44780_ENTRYLEFT           0x02
#define HD44780_ENTRYSHIFTINCREMENT 0x01
#define HD44780_ENTRYSHIFTDECREMENT 0x00

/* Flags for display on/off control */
#define HD44780_DISPLAYON           0x04
#define HD44780_CURSORON            0x02
#define HD44780_BLINKON             0x01

/* Flags for display/cursor shift */
#define HD44780_DISPLAYMOVE         0x08
#define HD44780_CURSORMOVE          0x00
#define HD44780_MOVERIGHT           0x04
#define HD44780_MOVELEFT            0x00

/* Flags for function set */
#define HD44780_8BITMODE            0x10
#define HD44780_4BITMODE            0x00
#define HD44780_2LINE               0x08
#define HD44780_1LINE               0x00
#define HD44780_5x10DOTS            0x04
#define HD44780_5x8DOTS             0x00
/****************************************/
typedef struct
{
	uint8_t DisplayControl;
	uint8_t DisplayFunction;
	uint8_t DisplayMode;
	uint8_t Rows;
	uint8_t Cols;
	uint8_t currentX;
	uint8_t currentY;
} HD44780_Options_t;
HD44780_Options_t HD44780_Opts;
/****************************************/
void HD44780_Cmd4bit(uint8_t cmd)
{
	/* Set output port */
	HAL_GPIO_WritePin(D7_PORT, D7_PIN, (GPIO_PinState)((cmd>>3) & 1) );
	HAL_GPIO_WritePin(D6_PORT, D6_PIN, (GPIO_PinState)((cmd>>2) & 1) );
	HAL_GPIO_WritePin(D5_PORT, D5_PIN, (GPIO_PinState)((cmd>>1) & 1) );
	HAL_GPIO_WritePin(D4_PORT, D4_PIN, (GPIO_PinState)(cmd & 1) );
	E_BLINK;
}
/****************************************/
void HD44780_Cmd(uint8_t cmd)
{
	/* Command mode */
	RS_LOW;
	
	/* High nibble */
	HD44780_Cmd4bit(cmd >> 4);
	/* Low nibble */
	HD44780_Cmd4bit(cmd & 0x0F);
}
/****************************************/
void HD44780_Data(uint8_t data)
{
	/* Data mode */
	RS_HIGH;
	
	/* High nibble */
	HD44780_Cmd4bit(data >> 4);
	/* Low nibble */
	HD44780_Cmd4bit(data & 0x0F);
}
/****************************************/
void HD44780_CursorSet(uint8_t col, uint8_t row)
{
	uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
	
	/* Go to beginning */
	if (row >= HD44780_Opts.Rows)
		row = 0;
	
	/* Set current column and row */
	HD44780_Opts.currentX = col;
	HD44780_Opts.currentY = row;
	
	/* Set location address */
	HD44780_Cmd(HD44780_SETDDRAMADDR | (col + row_offsets[row]));
}
/****************************************/
/****************************************/
void HD44780_Clear(void)
{
	HD44780_Cmd(HD44780_CLEARDISPLAY);
	HD44780_Delay(3000);
}
/****************************************/
void HD44780_Puts(uint8_t x, uint8_t y, char* str)
{
	HD44780_CursorSet(x, y);
	while (*str)
	{
		if (HD44780_Opts.currentX >= HD44780_Opts.Cols)
		{
			HD44780_Opts.currentX = 0;
			HD44780_Opts.currentY++;
			HD44780_CursorSet(HD44780_Opts.currentX, HD44780_Opts.currentY);
		}
		if (*str == '\n')
		{
			HD44780_Opts.currentY++;
			HD44780_CursorSet(HD44780_Opts.currentX, HD44780_Opts.currentY);
		}
		else if (*str == '\r')
		{
			HD44780_CursorSet(0, HD44780_Opts.currentY);
		} 
		else
		{
			HD44780_Data(*str);
			HD44780_Opts.currentX++;
		}
		str++;
	}
}
/****************************************/
void HD44780_DisplayOn(void)
{
	HD44780_Opts.DisplayControl |= HD44780_DISPLAYON;
	HD44780_Cmd(HD44780_DISPLAYCONTROL | HD44780_Opts.DisplayControl);
}
/****************************************/
void HD44780_DisplayOff(void)
{
	HD44780_Opts.DisplayControl &= ~HD44780_DISPLAYON;
	HD44780_Cmd(HD44780_DISPLAYCONTROL | HD44780_Opts.DisplayControl);
}
/****************************************/
void HD44780_BlinkOn(void)
{
	HD44780_Opts.DisplayControl |= HD44780_BLINKON;
	HD44780_Cmd(HD44780_DISPLAYCONTROL | HD44780_Opts.DisplayControl);
}
/****************************************/
void HD44780_BlinkOff(void)
{
	HD44780_Opts.DisplayControl &= ~HD44780_BLINKON;
	HD44780_Cmd(HD44780_DISPLAYCONTROL | HD44780_Opts.DisplayControl);
}
/****************************************/
void HD44780_CursorOn(void)
{
	HD44780_Opts.DisplayControl |= HD44780_CURSORON;
	HD44780_Cmd(HD44780_DISPLAYCONTROL | HD44780_Opts.DisplayControl);
}
/****************************************/
void HD44780_CursorOff(void)
{
	HD44780_Opts.DisplayControl &= ~HD44780_CURSORON;
	HD44780_Cmd(HD44780_DISPLAYCONTROL | HD44780_Opts.DisplayControl);
}
/****************************************/
void HD44780_ScrollLeft(void)
{
	HD44780_Cmd(HD44780_CURSORSHIFT | HD44780_DISPLAYMOVE | HD44780_MOVELEFT);
}
/****************************************/
void HD44780_ScrollRight(void)
{
	HD44780_Cmd(HD44780_CURSORSHIFT | HD44780_DISPLAYMOVE | HD44780_MOVERIGHT);
}
/****************************************/
void HD44780_CreateChar(uint8_t location, uint8_t *data)
{
	uint8_t i;
	/* We have 8 locations available for custom characters */
	location &= 0x07;
	HD44780_Cmd(HD44780_SETCGRAMADDR | (location << 3));
	
	for (i = 0; i < 8; i++)
		HD44780_Data(data[i]);
}
/****************************************/
void HD44780_PutCustom(uint8_t x, uint8_t y, uint8_t location)
{
	HD44780_CursorSet(x, y);
	HD44780_Data(location);
}
/****************************************/
void HD44780_Init(uint8_t cols, uint8_t rows)
{
	// Init pinout
	HD44780_InitPins();
	// At least 40ms
	HD44780_Delay(45000);
	// Set LCD width and height
	HD44780_Opts.Rows = rows;
	HD44780_Opts.Cols = cols;
	// Set cursor pointer to beginning for LCD
	HD44780_Opts.currentX = 0;
	HD44780_Opts.currentY = 0;
	
	HD44780_Opts.DisplayFunction = HD44780_4BITMODE | HD44780_5x8DOTS | HD44780_1LINE;
	if (rows > 1)
		HD44780_Opts.DisplayFunction |= HD44780_2LINE;
	
	// Try to set 4bit mode
	HD44780_Cmd4bit(0x03);
	HD44780_Delay(4500);
	
	// Second try
	HD44780_Cmd4bit(0x03);
	HD44780_Delay(4500);
	
	// Third goo!
	HD44780_Cmd4bit(0x03);
	HD44780_Delay(4500);	
	
	// Set 4-bit interface
	HD44780_Cmd4bit(0x02);
	HD44780_Delay(100);
	
	// Set # lines, font size, etc.
	HD44780_Cmd(HD44780_FUNCTIONSET | HD44780_Opts.DisplayFunction);

	// Turn the display on with no cursor or blinking default
	HD44780_Opts.DisplayControl = HD44780_DISPLAYON;
	HD44780_DisplayOn();

	// Clear lcd
	HD44780_Clear();

	// Default font directions
	HD44780_Opts.DisplayMode = HD44780_ENTRYLEFT | HD44780_ENTRYSHIFTDECREMENT;
	HD44780_Cmd(HD44780_ENTRYMODESET | HD44780_Opts.DisplayMode);

	// Delay
	HD44780_Delay(4500);
}
