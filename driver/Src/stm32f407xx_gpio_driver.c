/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: Dec 19, 2025
 *      Author: mansisd
 */

#include <stdint.h>
#include "stm32f407xx_gpio_driver.h"


/************************************************************************************************************************************************************************
 * @fn								-	GPIO_Init
 *
 * @brief							-	Initializes the GPIO Pin
 *
 * @param[in]						-	GPIO_Handle_t
 * @param[in]						-
 * @param[in]						-
 *
 * @return							-	none
 *
 * @Note							-	none
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp = 0;

	// Peripheral Clock Enable

	GPIO_PClkCtrl(pGPIOHandle->pGPIOx, ENABLE);
	//1. configure the mode of GPIO pin
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		//the non interrupt mode
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber) );
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); //clearing
		pGPIOHandle->pGPIOx->MODER |= temp; //setting

	}else
	{
		//This we will code later(the interrupt mode)
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT){
			//1. Configure the FTSR
			EXTI->FTSR |= (0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//2. Clear the corresponding RTSR Bit
			EXTI->RTSR &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT){

			//1. Configure the RTSR
			EXTI->RTSR |= (0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//2. Clear the corresponding RTSR Bit
			EXTI->FTSR &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT){
			//1. Configure both FTSR AND RTSR
			EXTI->RTSR |= (0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR |= (0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		//2. configure the GPIO port selection in SYSCFG_EXTICR
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber/4;
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber%4;
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);		/*If you go to SYSCFG register and in that EXTICR you can see port code for each port A-I*/
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1]= portcode << (temp2 *4);
		//3. enable the EXTI interrupt delivery using IMR
		EXTI->IMR |= 0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
	}
	temp = 0;
	//2. configure the speed
	temp =	(pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber) );
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); //clearing
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;  //setting

	temp = 0;
	//3. configure the pu-pd settings
	temp =	(pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber) );
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); //clearing
	pGPIOHandle->pGPIOx->PUPDR |= temp; //setting

	temp = 0;
	//4. configure the output type
	temp =	(pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType<< pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber );
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); //clearing
	pGPIOHandle->pGPIOx->OTYPER |= temp; //setting

	temp = 0;
	//5. configure the alternate functionality
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		//configure the alternate registers
		uint8_t temp1, temp2;
		temp1 =pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber/8;									/* by dividing the pin number by 8 you will get the index position i.e AFR[0] means AFRL register and AFR[1] means AFRH register*/
		temp2 =pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber%8;									/*Logic is by dividing the number by 8 we get the number itself since we r taking mod values and total there are 8 pins in each AFR */
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0XF <<(4*temp2));	//clearing
		pGPIOHandle->pGPIOx->AFR[temp1] |= pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode<<(4*temp2);	//setting
	}

}

/************************************************************************************************************************************************************************
 * @fn								-	GPIO_DeInit
 *
 * @brief							-	Resets the GPIO peripherals using AHB1STR register for more details discover "stm32f407xx.h"
 *
 * @param[in]						-	base address of the GPIO peripheral
 * @param[in]						-
 * @param[in]						-
 *
 * @return							-	none
 *
 * @Note							-	none
 */

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if(pGPIOx == GPIOA){
		GPIOA_REG_RESET();
	}else if(pGPIOx == GPIOB){
		GPIOB_REG_RESET();
	}else if(pGPIOx == GPIOC){
		GPIOC_REG_RESET();
	}else if(pGPIOx == GPIOD){
		GPIOD_REG_RESET();
	}else if(pGPIOx == GPIOE){
		GPIOE_REG_RESET();
	}else if(pGPIOx == GPIOF){
		GPIOF_REG_RESET();
	}else if(pGPIOx == GPIOG){
		GPIOG_REG_RESET();
	}else if(pGPIOx == GPIOH){
		GPIOH_REG_RESET();
	}else{
		GPIOI_REG_RESET();
	}

}


/*
 * Peripheral Clock Setup
 */

/************************************************************************************************************************************************************************
 * @fn								-	GPIO_PClkCtrl
 *
 * @brief							-	This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]						-	base address of the GPIO peripheral
 * @param[in]						-	ENABLE or DISABLE macros
 * @param[in]						-
 *
 * @return							-	none
 *
 * @Note							-	none
 */

void GPIO_PClkCtrl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE){
		if(pGPIOx == GPIOA){
			GPIOA_PCLK_EN();
		}else if(pGPIOx == GPIOB){
			GPIOB_PCLK_EN();
		}else if(pGPIOx == GPIOC){
			GPIOC_PCLK_EN();
		}else if(pGPIOx == GPIOD){
			GPIOD_PCLK_EN();
		}else if(pGPIOx == GPIOE){
			GPIOE_PCLK_EN();
		}else if(pGPIOx == GPIOF){
			GPIOF_PCLK_EN();
		}else if(pGPIOx == GPIOG){
			GPIOG_PCLK_EN();
		}else if(pGPIOx == GPIOH){
			GPIOH_PCLK_EN();
		}else{
			GPIOI_PCLK_EN();
		}
	}else{
		if(pGPIOx == GPIOA){
			GPIOA_PCLK_DI();
		}else if(pGPIOx == GPIOB){
			GPIOB_PCLK_DI();
		}else if(pGPIOx == GPIOC){
			GPIOC_PCLK_DI();
		}else if(pGPIOx == GPIOD){
			GPIOD_PCLK_DI();
		}else if(pGPIOx == GPIOE){
			GPIOE_PCLK_DI();
		}else if(pGPIOx == GPIOF){
			GPIOF_PCLK_DI();
		}else if(pGPIOx == GPIOG){
			GPIOG_PCLK_DI();
		}else if(pGPIOx == GPIOH){
			GPIOH_PCLK_DI();
		}else{
			GPIOI_PCLK_DI();
		}
		//TO DO(disable clock)
	}

}

/*
 * Peripheral Read and Write
 */

/************************************************************************************************************************************************************************
 * @fn								-	GPIO_ReadFromInputPin
 *
 * @brief							-	Reading the data from the input data register
 *
 * @param[in]						-	base address of the GPIO peripheral
 * @param[in]						-	Pin Number of the respective data to be read
 * @param[in]						-
 *
 * @return							-	0 or 1
 *
 * @Note							-	none
 */

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	value = (uint8_t) ((pGPIOx->IDR >> PinNumber)& 0x00000001);
	return value;
}

/************************************************************************************************************************************************************************
 * @fn								-	GPIO_ReadFromInputPort
 *
 * @brief							-
 *
 * @param[in]						-	base address of the GPIO peripheral
 * @param[in]						-
 * @param[in]						-
 *
 * @return							-	0 or 1
 *
 * @Note							-	none
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;
	value = (uint16_t) pGPIOx->IDR;
	return value;

}

/************************************************************************************************************************************************************************
 * @fn								-	GPIO_WriteToOutputPin
 *
 * @brief							-
 *
 * @param[in]						-	base address of the GPIO peripheral
 * @param[in]						-	PinNumber
 * @param[in]						-	Value i.e 0 or 1
 *
 * @return							-	none
 *
 * @Note							-	none
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	if(Value == GPIO_PIN_SET){
		//write 1 to the output data register at the bit field corresponding to the pin number
		pGPIOx->ODR |= (1 << PinNumber);
	}else{
		//write 0
		pGPIOx->ODR &= ~(1 << PinNumber);
	}

}

/************************************************************************************************************************************************************************
 * @fn								-	GPIO_WriteToOutputPort
 *
 * @brief							-
 *
 * @param[in]						-	base address of the GPIO peripheral
 * @param[in]						-	Value i.e 0 or 1
 * @param[in]						-
 *
 * @return							-	none
 *
 * @Note							-	none
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx,uint16_t Value)
{
	pGPIOx->ODR = Value;

}

/*
 * Peripheral Output Toggling
 */

/************************************************************************************************************************************************************************
 * @fn								-	GPIO_ToggleOutputPin
 *
 * @brief							-	0 ing or 1 ing the O/P pin
 *
 * @param[in]						-	base address of the GPIO peripheral
 * @param[in]						-	PinNumber
 * @param[in]						-
 *
 * @return							-	none
 *
 * @Note							-	none
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1 << PinNumber);

}

/*
 * Peripheral Interrupt Handling and Configuration
 */

/************************************************************************************************************************************************************************
 * @fn								-	PIO_IRQConfig
 *
 * @brief							-
 *
 * @param[in]						-	IRQ number
 * @param[in]						-	IRQ Priority
 * @param[in]						-	Enable or Disable macros
 *
 * @return							-	none
 *
 * @Note							-	check the  Generic User Guide of Cortex M4 micro-controller
 * We are using here only 3 conditions since in our discovery board only 90 or less than 90 interrupt registers are present
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(IRQNumber <= 31 )
		{
			//program ISER0 Register
			*NVIC_ISER0 = ( 1<< IRQNumber );
		}else if(IRQNumber >= 32 && IRQNumber < 64){
			//program ISER1 Register
			*NVIC_ISER1 = ( 1<< (IRQNumber%32) );

		}else if(IRQNumber >= 64 && IRQNumber < 96){
			//program ISER2 Register
			*NVIC_ISER2 = ( 1<< (IRQNumber%64) );
		}

	}else
	{
		if(IRQNumber <= 31 )
		{
			//program ICER0 Register
			*NVIC_ICER0 = ( 1<< IRQNumber );
		}else if(IRQNumber >= 32 && IRQNumber < 64){
			//program ICER1 Register
			*NVIC_ICER1 = ( 1<< (IRQNumber % 32) );
		}else if(IRQNumber >= 64 && IRQNumber < 96){
			//program ICER2 Register
			*NVIC_ICER2 = ( 1<< (IRQNumber % 64) );
		}

	}
}

void GPIO_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//1. first lets find out the IPR Register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	uint8_t shift_amount = (8*iprx_section)+(8-NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << (shift_amount));
}

/************************************************************************************************************************************************************************
 * @fn								-	GPIO_IRQHandling
 *
 * @brief							-
 *
 * @param[in]						-	Pin Number
 * @param[in]						-
 * @param[in]						-
 *
 * @return							-	none
 *
 * @Note							-	none
 */
void GPIO_IRQHandling(uint8_t PinNumber)
{
// clear the EXTI PR register corresponding to the pin number
	if(EXTI->PR & (1 << PinNumber))
	{
		//clear
		EXTI->PR |= (1 << PinNumber); //This bit is cleared by programming it to ‘1’ given in the reference manual

	}

}



