/*
 * 002led_button.c
 *
 *  Created on: Dec 23, 2025
 *      Author: mansisd
 */


#include "stm32f407xx.h"
#define HIGH ENABLE
#define BTN_Pressed HIGH
void delay()
{
 for(uint32_t i = 0; i<500000/2 ; i++);
}
int main(void)
{
	GPIO_Handle_t GpioLed, GpioBtn;

	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;		//if you use it in the pull up mode led glows with a very less intensity and that is un-noticiable
	// Optional (but usually required if your driver expects it)
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OP_HS;
	GPIO_PClkCtrl(GPIOD, ENABLE);
	GPIO_Init(&GpioLed);


	GpioBtn.pGPIOx = GPIOA;
	GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;		//it is of no use here to activate internal pull resistor since , if u check the schematic of button u see there that there is already a external pull resistor given by the micro-controller
	// Optional (but usually required if your driver expects it)
	GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OP_HS;
	GPIO_PClkCtrl(GPIOA, ENABLE);
	GPIO_Init(&GpioBtn);
	while(1){
	if(GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0)== BTN_Pressed){
		delay();
		GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_12);
	}
	}

	return 0;



}
