#include "stdint.h"
#include "system_stm32f4xx.h"
#include "Gpio.h"

#define RCC 									0x40023800 //0x40023800 - 0x400238FF
#define RCC_AHB1ENR_OFFSET		0x00000030

#define RCC_AHB1ENR_A					0x00000001
#define RCC_AHB1ENR_B					0x00000002
#define RCC_AHB1ENR_C					0x00000004
#define RCC_AHB1ENR_D					0x00000008
#define RCC_AHB1ENR_E					0x00000010
#define RCC_AHB1ENR_F					0x00000020

static void init(void)
{
	//Enable RCC FOR D line
	REG32(RCC + RCC_AHB1ENR_OFFSET) |= RCC_AHB1ENR_D;
	
	// maybe to make functions for enabling of all ports
}

int main()
{
	init();
	
	volatile int i=0;
	
	GpioStructure gpioStructure;
	gpioStructure.mode = GENERAL_PURPOSE_OUTPUT;
	gpioStructure.outputType = PUSH_PULL;
	gpioStructure.speed = SLOW_SPEED;
	gpioStructure.pullUpPullDown = PULL_DOWN;
	gpioStructure.outputData = OUTPUT;
	gpioStructure.AF[0] = AF0;
	gpioStructure.AF[1] = AF0;
	gpioStructure.pin = 12;
	
	SetUpGPIO(&gpioStructure, GPIOD);
	
	while(1)
	{
		
		while(i<1000000) 
		{
			i++;
		}
		TogglePinOutput(GPIOD, 12);
		while(i>0)
		{
			i--;
		}
		TogglePinOutput(GPIOD, 12);
	}
}
