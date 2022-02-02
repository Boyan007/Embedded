#include "Gpio.h"

/* General function for setting a bit(bitmask) in a register
*/
static void setRegister(uint32_t reg, uint32_t bit, uint32_t bitMask)
{
	// somehow need to check the range !!!
	REG32(reg) |= bitMask<<bit;
}

// Somehow make an array of pointers to functions??? (to avoid repeating)
/* Function for setting the mode of a GPIO: input, output, alternate function, analog
*/
static void setGpioMode(uint32_t reg_gpio, uint32_t pin, uint32_t mode)
{
	setRegister(reg_gpio + GPIO_MODER_OFFSET, TWO_BIT_FIELD * pin, mode);
}

/* Function for setting the output type of a GPIO: push-pull, open drain
*/
static void setGpioOType(uint32_t reg_gpio, uint32_t pin, uint32_t outputType)
{
	setRegister(reg_gpio + GPIO_OTYPE_OFFSET, ONE_BIT_FIELD * pin, outputType);
}

/* Function for setting speed of a GPIO: low, medium, high, very high
*/
static void setGpioSpeed(uint32_t reg_gpio, uint32_t pin, uint32_t speed)
{
	setRegister(reg_gpio + GPIO_OSPEED_OFFSET, ONE_BIT_FIELD * pin, speed);
}

/* Function for setting pull-up and pull-down of a GPIO
*/
static void setGpioPuPd(uint32_t reg_gpio, uint32_t pin, uint32_t pUpPDown)
{
	setRegister(reg_gpio + GPIO_PUPDR_OFFSET, ONE_BIT_FIELD * pin, pUpPDown);
}

/* Function for locking a GPIO: no lock, lock
*/
static void setGpioLock(uint32_t reg_gpio, uint32_t pin, uint32_t lock)
{
	setRegister(reg_gpio + GPIO_LCKR_OFFSET, ONE_BIT_FIELD * pin, lock);
}

/* Function for setting alternate function of a GPIO: AF0, AF1, AF2, AF3
*/
static void setGpioAF(uint32_t reg_gpio, uint32_t pin, uint32_t* altFunc)
{
	if(pin < PIN_8)
	{
		setRegister(reg_gpio + GPIO_AFLR_OFFSET, FOUR_BIT_FIELD * pin, altFunc[0]);
	}
	else
	{
		pin %= PIN_8;
		setRegister(reg_gpio + GPIO_AFHR_OFFSET, FOUR_BIT_FIELD * pin, altFunc[1]);
	}
}

/* Used for getting the register in regards of the port (port is A..F)
*/
static void getGpioReg(uint32_t* regValue, gpioPorts port)
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

/* Function for setting pin output to 1
*/
void SetPin(gpioPorts port, pinNumber pin)
{
	uint32_t reg_gpio;
	getGpioReg(&reg_gpio, port);
	
	REG32(reg_gpio + GPIO_BSRR_OFFSET) |= (1 << pin);
}

/* Function for resetting pin output to 0
*/
void ResetPin(gpioPorts port, pinNumber pin)
{
	uint32_t reg_gpio;
	getGpioReg(&reg_gpio, port);
	
	REG32(reg_gpio + GPIO_BSRR_OFFSET) |= (1 << (pin + 16)); // magic number, put define
}

/* Function for toggling the pin output between 0 and 1
*/
void TogglePin(gpioPorts port, pinNumber pin)
{
	uint32_t reg_gpio;
	getGpioReg(&reg_gpio, port);
	
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
	
	getGpioReg(&reg_gpio, port);
	
	// think of a way to set up multiple pins
	setGpioMode(reg_gpio, pin, gpio->mode);
	setGpioOType(reg_gpio, pin, gpio->outputType);
	setGpioSpeed(reg_gpio, pin, gpio->speed);
	setGpioPuPd(reg_gpio, pin, gpio->pullUpPullDown);
	setGpioLock(reg_gpio, pin, gpio->lock); // this was also not tested in any way, dunno how to test it
	setGpioAF(reg_gpio, pin, gpio->AF); // this was not tested, needs to be tested maybe with UART

}
