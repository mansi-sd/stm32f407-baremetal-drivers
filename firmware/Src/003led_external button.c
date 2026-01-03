/*
 * 003led_external button.c
 *
 *  Created on: Dec 23, 2025
 *      Author: mansisd
 */


/*
 * 002led_button.c
 *
 *  Created on: Dec 23, 2025
 *      Author: mansisd
 */
#include "stm32f407xx_gpio_driver.h"


#include "stm32f407xx.h"
#define HIGH ENABLE
#define LOW DISABLE
#define BTN_Pressed LOW
void delay()
{
 for(uint32_t i = 0; i<500000/2 ; i++);
}
int main(void)
{
	GPIO_Handle_t GpioLed, GpioBtn;

	GpioLed.pGPIOx = GPIOA;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;		//if you use it in the pull up mode led glows with a very less intensity and that is un-noticiable
	// Optional (but usually required if your driver expects it)
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OP_HS;
	GPIO_PClkCtrl(GPIOA, ENABLE);
	GPIO_Init(&GpioLed);


	GpioBtn.pGPIOx = GPIOB;
	GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;		//it is of no use here to activate internal pull resistor since , if u check the schematic of button u see there that there is already a external pull resistor given by the micro-controller
	// Optional (but usually required if your driver expects it)
	GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OP_HS;
	GPIO_PClkCtrl(GPIOB, ENABLE);
	GPIO_Init(&GpioBtn);
	while(1){
	if(GPIO_ReadFromInputPin(GPIOB,GPIO_PIN_NO_12)== BTN_Pressed){
		delay();
		GPIO_ToggleOutputPin(GPIOA,GPIO_PIN_NO_8);
	}
	}

	return 0;



}
