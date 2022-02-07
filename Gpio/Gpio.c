#include "Gpio.h"

/* General function for setting a bitmask in a register, starting on bit position
*/
static void SetBit(uint32_t reg, uint32_t bits, uint32_t bitMask)
{
	// somehow need to check the range !!!
	REG32(reg) |= bitMask<<bits;
}

/* General function for resetting a bitmask in a register, starting on bit position
*/
static void ResetBit(uint32_t reg, uint32_t bits, uint32_t bitMask)
{
	// somehow need to check the range !!!
	REG32(reg) &= ~bitMask<<bits;
}


/* General function for setting a bitmask in a register, for all bit positions
*/
static void SetRegister(uint32_t reg, uint32_t bits, uint32_t bitMask, uint8_t bitsInBitfield)
{
	uint32_t pinNumber = 0;
	
	while(pinNumber < 16)
	{
		if( bits & (1 << pinNumber) )
		{
			SetBit(reg, bitsInBitfield*pinNumber, bitMask);
		}
		pinNumber++;
	}
}

/* General function for resetting bitmask in a register, for all bit positions
*/
static void ResetRegister(uint32_t reg, uint32_t bits, uint32_t bitMask, uint8_t bitsInBitfield)
{
	uint32_t pinNumber = 0;
	
	while(pinNumber < 16)
	{
		if( bits & (1 << pinNumber) )
		{
			ResetBit(reg, bitsInBitfield*pinNumber, bitMask);
		}
		pinNumber++;
	}
}

// Somehow make an array of pointers to functions??? (to avoid repeating)
/* Function for setting the mode of a GPIO: input, output, alternate function, analog
*/
static void SetGpioMode(uint32_t reg_gpio, uint32_t pin, uint32_t mode)
{
	SetRegister(reg_gpio + GPIO_MODER_OFFSET, pin, mode, TWO_BIT_FIELD);
}

/* Function for resetting the mode of a GPIO
*/
static void ResetGpioMode(uint32_t reg_gpio, uint32_t pin, uint32_t mode)
{
	ResetRegister(reg_gpio + GPIO_MODER_OFFSET, pin, mode, TWO_BIT_FIELD);
}

/* Function for setting the output type of a GPIO: push-pull, open drain
*/
static void SetGpioOType(uint32_t reg_gpio, uint32_t pin, uint32_t outputType)
{
	SetRegister(reg_gpio + GPIO_OTYPE_OFFSET, pin, outputType, ONE_BIT_FIELD);
}
/* Function for resetting the output type of a GPIO
*/
static void ResetGpioOType(uint32_t reg_gpio, uint32_t pin, uint32_t outputType)
{
	ResetRegister(reg_gpio + GPIO_OTYPE_OFFSET, pin, outputType, ONE_BIT_FIELD);
}

/* Function for setting speed of a GPIO: low, medium, high, very high
*/
static void SetGpioSpeed(uint32_t reg_gpio, uint32_t pin, uint32_t speed)
{
	SetRegister(reg_gpio + GPIO_OSPEED_OFFSET, pin, speed, ONE_BIT_FIELD);
}
/* Function for resetting speed of a GPIO
*/
static void ResetGpioSpeed(uint32_t reg_gpio, uint32_t pin, uint32_t speed)
{
	ResetRegister(reg_gpio + GPIO_OSPEED_OFFSET, pin, speed, ONE_BIT_FIELD);
}

/* Function for setting pull-up and pull-down of a GPIO
*/
static void SetGpioPuPd(uint32_t reg_gpio, uint32_t pin, uint32_t pUpPDown)
{
	SetRegister(reg_gpio + GPIO_PUPDR_OFFSET, pin, pUpPDown, ONE_BIT_FIELD);
}
/* Function for resetting pull-up and pull-down of a GPIO
*/
static void ResetGpioPuPd(uint32_t reg_gpio, uint32_t pin, uint32_t pUpPDown)
{
	ResetRegister(reg_gpio + GPIO_PUPDR_OFFSET, pin, pUpPDown, ONE_BIT_FIELD);
}

/* Function for locking a GPIO: no lock, lock
*/
static void SetGpioLock(uint32_t reg_gpio, uint32_t pin, uint32_t lock)
{
	SetRegister(reg_gpio + GPIO_LCKR_OFFSET, pin, lock, ONE_BIT_FIELD);
}
/* Function for reseting lock a GPIO
*/
static void ResetGpioLock(uint32_t reg_gpio, uint32_t pin, uint32_t lock)
{
	ResetRegister(reg_gpio + GPIO_LCKR_OFFSET, pin, lock, ONE_BIT_FIELD);
}

/* Function for setting alternate function of a GPIO: AF0, AF1, AF2, AF3
*/
static void SetGpioAF(uint32_t reg_gpio, uint32_t pin, uint32_t* altFunc)
{
	if(pin < PIN8)
	{
		SetRegister(reg_gpio + GPIO_AFLR_OFFSET, pin, altFunc[0], FOUR_BIT_FIELD);
	}
	else
	{
		pin %= PIN8;
		SetRegister(reg_gpio + GPIO_AFHR_OFFSET, pin, altFunc[1], FOUR_BIT_FIELD);
	}
}
/* Function for resetting alternate function of a GPIO
*/
static void ResetGpioAF(uint32_t reg_gpio, uint32_t pin, uint32_t* altFunc)
{
	if(pin < PIN8)
	{
		ResetRegister(reg_gpio + GPIO_AFLR_OFFSET, pin, altFunc[0], FOUR_BIT_FIELD);
	}
	else
	{
		pin %= PIN8;
		ResetRegister(reg_gpio + GPIO_AFHR_OFFSET, pin, altFunc[1], FOUR_BIT_FIELD);
	}
}

/* Used for getting the register in regards of the port (port is A..F)
*/
static void GetGpioReg(uint32_t* regValue, gpioPorts port)
{
	// see how to make a custom assertion
	switch(port)
	{
		case GPIOA:
			*regValue = REG_GPIOA;
			break;
		case GPIOB:
			*regValue = REG_GPIOB;
			break;
		case GPIOC:
			*regValue = REG_GPIOC;
			break;
		case GPIOD:
			*regValue = REG_GPIOD;
			break;
		case GPIOE:
			*regValue = REG_GPIOE;
			break;
		case GPIOF:
			*regValue = REG_GPIOF;
			break;
		//default:
			// TODO: Go fatal // maybe create custom ASSERT fucntion
		//	break;
	}
}

/* Function for resetting GPIO pin configuration
*/
static void ResetGPIO( GpioStructure* gpio, uint32_t reg_gpio, uint32_t pin)
{
	ResetGpioMode(reg_gpio, pin, gpio->mode);
	ResetGpioOType(reg_gpio, pin, gpio->outputType);
	ResetGpioSpeed(reg_gpio, pin, gpio->speed);
	ResetGpioPuPd(reg_gpio, pin, gpio->pullUpPullDown);
	ResetGpioLock(reg_gpio, pin, gpio->lock); // this was also not tested in any way, dunno how to test it
	ResetGpioAF(reg_gpio, pin, gpio->AF); // this was not tested, needs to be tested maybe with UART
}

/* Function for setting pin output to 1
*/
void SetPinOutput(gpioPorts port, pinNumber pin)
{
	uint32_t reg_gpio;
	GetGpioReg(&reg_gpio, port);
	
	REG32(reg_gpio + GPIO_BSRR_OFFSET) |= (1 << pin);
}

/* Function for resetting pin output to 0
*/
void ResetPinOutput(gpioPorts port, pinNumber pin)
{
	uint32_t reg_gpio;
	GetGpioReg(&reg_gpio, port);
	
	REG32(reg_gpio + GPIO_BSRR_OFFSET) |= (1 << (pin + 16)); // magic number, put define
}

/* Function for toggling the pin output between 0 and 1
*/
void TogglePinOutput(gpioPorts port, pinNumber pin)
{
	uint32_t reg_gpio;
	GetGpioReg(&reg_gpio, port);
	
	if(REG32(reg_gpio + GPIO_ODR_OFFSET) & (1 << pin))
	{
		pin += 16;
	}
	
	REG32(reg_gpio + GPIO_BSRR_OFFSET) ^= (1 << pin);
}

// somehow make it possible to use multiple pins and not to recreate the whole structure???
/* Function for setting up GPIO pin with given parameters and port
*/
void SetUpGPIO(GpioStructure* gpio, gpioPorts port)
{
	uint32_t pin = gpio->pin;
	
	uint32_t reg_gpio;
	
	GetGpioReg(&reg_gpio, port);
	
	// if the pin has some configuration, we want to make sure it gets reseted
	ResetGPIO(gpio, reg_gpio, port);
	
	// think of a way to set up multiple pins
	SetGpioMode(reg_gpio, pin, gpio->mode);
	SetGpioOType(reg_gpio, pin, gpio->outputType);
	SetGpioSpeed(reg_gpio, pin, gpio->speed);
	SetGpioPuPd(reg_gpio, pin, gpio->pullUpPullDown);
	SetGpioLock(reg_gpio, pin, gpio->lock); // this was also not tested in any way, dunno how to test it
	SetGpioAF(reg_gpio, pin, gpio->AF); // this was not tested, needs to be tested maybe with UART
}
