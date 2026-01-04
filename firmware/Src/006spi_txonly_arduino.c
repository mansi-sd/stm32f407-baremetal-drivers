/*
 * 006spi_txonly_arduino.c
 *
 *  Created on: Jan 4, 2026
 *      Author: mansisd
 */


/*
 * 005spi_tx_testing.c
 *
 *  Created on: Dec 30, 2025
 *      Author: mansisd
 */
/*
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * PB13 --> SPI2_SCLK
 * PB12 --> SPI2_NSS
 * ALT function mode : 5
 */
#include "stm32f407xx.h"
#include <string.h>

void delay()
{
 for(uint32_t i = 0; i<500000/2 ; i++);
}
void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;
	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = 	GPIO_OP_HS;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//MISO
	//SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	//GPIOInit(&SPIPins);

	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);

}

void SPI2_Inits(void)
{
	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPI_PinConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPI_PinConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPI_PinConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8; // generates SCLK of 2 MHz
	SPI2handle.SPI_PinConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPI_PinConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPI_PinConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPI_PinConfig.SPI_SSM = SPI_SSM_DI; // Hardware slave management enabled for NSS pin


	SPI_Init(&SPI2handle);
}

void GPIO_Button_Init(void)
{
	GPIO_Handle_t GpioBtn;
	// This is the button GPIO configuration
	GpioBtn.pGPIOx = GPIOA;
	GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD	;		//it is of no use here to activate internal pull resistor since , if u check the schematic of button u see there that there is already a external pull resistor given by the micro-controller
	// Optional (but usually required if your driver expects it)
	GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OP_HS;

	GPIO_Init(&GpioBtn);
}
int main(void)
{
	char user_data[]= "Hello Mansi Sharad Dongare";
	//this function is used to initialize the GPIO pins to behave a SPI2 pins
	GPIO_Button_Init();
	SPI2_GPIOInits();

	//this function is used to initialize the SPI2 peripheral parameters
	SPI2_Inits();

	/*
	 * Making SSOE 1 does NSS output enable
	 * The NSS pin is automatically managed by the hardware.
	 * i.e when SPE = 1, NSS will be pulled to low
	 * and NSS pin will be high when SPE = 0
	 */

	SPI_SSOEConfig(SPI2, ENABLE);
	while (1)
	{
		// wait till the button is pressed
		while(! (GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0 )));

		// to avoid button de-bouncing related issues 200ms of delay
		delay();

		//enable the SPI2peripheral
		SPI_PeripheralControl(SPI2, ENABLE);

		//first lets send length information
		uint8_t dataLen = strlen(user_data);
		SPI_SendData(SPI2,&dataLen,1);
		//to send data
		SPI_SendData(SPI2,(uint8_t*)user_data,strlen(user_data));


		// lets confirm that SPI flag is not busy
		while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

		//disable the SPI2peripheral
		SPI_PeripheralControl(SPI2, DISABLE);
	}

	return 0;

}
