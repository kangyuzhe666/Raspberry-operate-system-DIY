/*

*	文件名：GPIO.c
*    GPIO函数
**/


#include "gpio.h"


/*这里声明私有函数 共有函数在头文件中声明 */
unsigned long GET_GPFSEL_ADDR(int pin);
unsigned long GET_GPSET_ADDR(int pin);
unsigned long GET_GPCLR_ADDR(int pin);

void bcm2835_peri_set_bits(volatile u32 * paddr, u32  value, u32  mask);
void bcm2835_peri_write(volatile u32 * paddr, u32  value);
u32  bcm2835_peri_read(volatile u32 * paddr);

//初始化GPIO
void gpio_init(void)
{
		GPIO_SET_GPFSEL(16, 1);
		GPIO_SET_GPFSEL(19, 1);
}



void GPIO_SET_GPFSEL(u8 pin, u8 mode)
{

    // Function selects are 10 pins per 32 bit word, 3 bits per pin
    volatile u32 * paddr = ( volatile u32 * ) GPFSEL0  + (pin/10);
    u8   shift = (pin % 10) * 3;
    u32  mask = 0b111  << shift; // BCM2835_GPIO_FSEL_MASK = 0b111
    u32  value = mode << shift;
    bcm2835_peri_set_bits(paddr, value, mask);
} 


// Set/clear only the bits in value covered by the mask
void bcm2835_peri_set_bits(volatile u32 * paddr, u32  value, u32  mask)
{
    u32  v = bcm2835_peri_read(paddr);
    v = (v & ~mask) | (value & mask);
    bcm2835_peri_write(paddr, v);
}

void bcm2835_peri_write(volatile u32 * paddr, u32  value)
{

	// Make sure we don't rely on the first write, which may get
	// lost if the previous access was to a different peripheral.
	*paddr = value;
	*paddr = value;
}

// safe read from peripheral
u32  bcm2835_peri_read(volatile u32 * paddr)
{

	// Make sure we dont return the _last_ read which might get lost
	// if subsequent code changes to a different peripheral
	u32 ret = *paddr;
	int a = *paddr; // Read without assigneing to an unused variable
	return ret;
}



int GPIO_SET_GPSET(int pin)
{
	volatile unsigned long *point = 0 ;
	int bit_num = 0;
	
	//检查
	
	if(pin > 53 || pin < 0)
	{
		return -1;
	}
	
	bit_num = pin % 32;
	point = (unsigned long*)GET_GPSET_ADDR(pin);
	
	if(point == 0)
	{
		return -1;
	}
	
	*point |= (1 << bit_num);
}





int GPIO_SET_GPCLR(int pin)
{
	volatile unsigned long *point = 0 ;
	int bit_num = 0;
	
	//检查
	
	if(pin > 53 || pin < 0)
	{
		return -1;
	}
	
	bit_num = pin % 32;
	point = (unsigned long*)GET_GPCLR_ADDR(pin);
	
	if(point == 0)
	{
		return -1;
	}
	
	*point |= (1 << bit_num);
}




unsigned long GET_GPFSEL_ADDR(int pin)
{
	int reg_num = 0;
	reg_num = pin/10;
	
	switch (reg_num)
	{
		case 0:
			return GPFSEL0;
			break;
		case 1:
			return GPFSEL1;
			break;
		case 2:
			return GPFSEL2;
			break;
		case 3:
			return GPFSEL3;
			break;
		case 4:
			return GPFSEL4;
			break;
		case 5:
			return GPFSEL5;
			break;
		default:
			return 0;
			break;
	}
}



unsigned long GET_GPSET_ADDR(int pin)
{
	if(pin < 29)
	{
		return GPSET0;
	}
	else 
	{
		return GPSET1;
	}
	return 0;
}


unsigned long GET_GPCLR_ADDR(int pin)
{
	if(pin < 29)
	{
		return GPCLR0;
	}
	else 
	{
		return GPCLR1;
	}
	return 0;
}

void blink_GPIO16(void)
{
	static int lit16 = 0;
	
	if( lit16 )
	{
		GPIO_SET_GPSET(16);
		lit16 = 0;
	}
	else
	{
		GPIO_SET_GPCLR(16);
		lit16 = 1;
	}
}

void blink_GPIO19(void)
{
	static int lit16 = 0;

	if( lit16 )
	{
		GPIO_SET_GPSET(19);
		lit16 = 0;
	}
	else
	{
		GPIO_SET_GPCLR(19);
		lit16 = 1;
	}
}
