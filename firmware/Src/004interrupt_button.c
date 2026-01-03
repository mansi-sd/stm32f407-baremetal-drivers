/*
 * 002led_button.c
 *
 *  Created on: Dec 23, 2025
 *      Author: mansisd
 */

#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx.h"
#include <string.h>
#define HIGH ENABLE
#define BTN_Pressed HIGH
void delay()
{
	// delay implemented of 200ms
	for(uint32_t i = 0; i<500000/2 ; i++);
}
int main(void)
{
	GPIO_Handle_t GpioLed, GpioBtn;
	memset(&GpioLed,0,sizeof(GpioLed));
	memset(&GpioBtn,0,sizeof(GpioBtn));
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;		//if you use it in the pull up mode led glows with a very less intensity and that is un-noticiable
	// Optional (but usually required if your driver expects it)
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OP_HS;
	GPIO_PClkCtrl(GPIOD, ENABLE);
	GPIO_Init(&GpioLed);

	//this is button GPIO configuration
	GpioBtn.pGPIOx = GPIOD;
	GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;		//it is of no use here to activate internal pull resistor since , if u check the schematic of button u see there that there is already a external pull resistor given by the micro-controller
	// Optional (but usually required if your driver expects it)
	GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OP_HS;
	GPIO_PClkCtrl(GPIOD, ENABLE);
	GPIO_Init(&GpioBtn);

	//IRQ Configuration
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5,NVIC_IRQ_PRI15);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5,ENABLE);		// IRQ_NO_EXTI9_5 SINCE we are connecting our button on pin 5

	while(1);

}
void EXTI9_5_IRQHandler(void)
{
	delay();
	GPIO_IRQHandling(GPIO_PIN_NO_5);
	GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_12);
}
