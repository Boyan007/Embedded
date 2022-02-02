#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "Register.h"

#define REG_GPIO_START				0x40020000  // GPIO A 0x4002 0000 - 0x4002 03FF, next offset 0x0400
#define GPIO_OFFSET						0x0400

#define REG_GPIOA							REG_GPIO_START
#define REG_GPIOB							(REG_GPIOA + GPIO_OFFSET)
#define REG_GPIOC							(REG_GPIOB + GPIO_OFFSET)
#define REG_GPIOD							(REG_GPIOC + GPIO_OFFSET)
#define REG_GPIOE							(REG_GPIOD + GPIO_OFFSET)
#define REG_GPIOF							(REG_GPIOE + GPIO_OFFSET)

#define GPIO_MODER_OFFSET			0x00000000	// Mode - the double bits say which pin number is it
																						//	00: Input (reset state)
																						//	01: General purpose output mode
																						//  10: Alternate function mode
																						//  11: Analog mode

#define GPIO_OTYPE_OFFSET			0x00000004  //  0: Output push-pull - the bits say which pin it is
																						// 0: Output push-pull (reset state)
																						// 1: Output open-drain

#define GPIO_OSPEED_OFFSET		0x00000008	// 00: Low speed - the bits say which pin it is
																						// 00: Low speed
																						// 01: Medium speed
																						// 10: High speed
																						// 11: Very high speed

#define GPIO_PUPDR_OFFSET			0x0000000C	// 10: Pull-down - the bits say which pin it is
																						// 00: No pull-up, pull-down
																						// 01: Pull-up
																						// 10: Pull-down
																						// 11: Reserved
																						
#define GPIO_IDR_OFFSET			  0x00000010	//  1: Input data - the bits say which pin it is
																						// Bits 15:0 IDRy: Port input data
																						
#define GPIO_ODR_OFFSET				0x00000014	// 	1: Output data - the bits say which pin it is
																						// Bits 15:0 ODRy: Port output data
																						
#define GPIO_BSRR_OFFSET			0x00000018	//  0-15 sets bit; 16-31 resets bit
																						// 0: No action on the corresponding ODRx bit
																						// 1: Sets the corresponding ODRx bit

#define GPIO_LCKR_OFFSET			0x0000001C	//  Lock register

#define GPIO_AFLR_OFFSET			0x00000020	//  Alternate function low register - pins 0-7
																						// 0000: AF0 - 1111: AF15
																						
#define GPIO_AFHR_OFFSET			0x00000024	//  Alternate function low register - pins 8-15
																						// 0000: AF0 - 1111: AF15
#define ONE_BIT_FIELD					0x01
#define TWO_BIT_FIELD					0x02
#define FOUR_BIT_FIELD				0x02

#define PIN_8									0x08

typedef uint32_t pinNumber;

typedef struct
{
	uint32_t mode;
	uint32_t outputType;
	uint32_t speed;
	uint32_t pullUpPullDown;
	uint32_t inputData;
	uint32_t outputData;
	uint32_t bitSetReset;
	uint32_t lock;
	uint32_t AF[2];//lower index, lower alternate function - (small endian???)
	uint32_t pin;
} GpioStructure;

enum moder
{
	INPUT 										= 0x00,
	GENERAL_PURPOSE_OUTPUT		= 0x01,
	ALTERNATE_FUNCTION 				= 0x02,
	ANALOG										= 0x03
};

enum outputType
{
	PUSH_PULL 								= 0x00,
	OPEN_DRAIN 								= 0x01
};

enum speed
{
	SLOW_SPEED								= 0x00, 
	MEDIUM_SPEED							= 0x01,
	HIGH_SPEED								= 0x02,
	VERY_HIGH_SPEED						= 0x03
};

enum pullUpPullDown
{
	NO_PULL										= 0x00,
	PULL_UP										= 0x01,
	PULL_DOWN									= 0x02
};

enum outputData
{
	OUTPUT										= 0x01
};

enum bitSetReset
{
	SET_BIT										= 0x01,
	RESET_BIT									= 0x10000
};

enum lockGpio
{
	NO_LOCK										= 0x00,
	LOCK											= 0x01
};

enum alternateFunction
{
	AF0												= 0x00,
	AF1												= 0x01,
	AF2												= 0x02,
	AF3												= 0x03,
};

typedef enum{
	GPIOA,
	GPIOB,
	GPIOC,
	GPIOD,
	GPIOE,
	GPIOF
} gpioPorts;

void SetUpGPIO(GpioStructure* gpio, gpioPorts port);

void SetPin(gpioPorts port, pinNumber pin);
void ResetPin(gpioPorts port, pinNumber pin);
void TogglePin(gpioPorts port, pinNumber pin);

#endif
