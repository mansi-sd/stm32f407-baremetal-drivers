/*
 * stm32f407xx_gpio_driver.h
 *
 *  Created on: Dec 19, 2025
 *      Author: mansisd
 */

#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_

#include "stm32f407xx.h"

/*
 * This is a configuration structure for GPIO pin
 */

typedef struct
{
	uint8_t GPIO_PinNumber;						/*POSSIBLE VALUES FROM @GPIO_PIN_NOS			*/
	uint8_t GPIO_PinMode;						/*POSSIBLE VALUES FROM @GPIO_PIN_MODES			*/
	uint8_t GPIO_PinSpeed;						/*Possible values from @GPIO_SPEED_MODES		*/
	uint8_t GPIO_PinPuPdControl;				/*Possible values from @GPIO_PULLUP_PULLDOWN	*/
	uint8_t GPIO_PinOPType;						/*Possible values from @GPIO_OUTPUT_TYPES		*/
	uint8_t GPIO_PinAltFunMode;
}GPIO_PinConfig_t;

/*
 *  This is a Handle Structure for a GPIO pin
 */

typedef struct
{
	GPIO_RegDef_t 		*pGPIOx;				/*This holds the base address of the GPIO port to which pin belongs to*/
	GPIO_PinConfig_t	GPIO_PinConfig;			/*This hold GPIO pin configuration settings*/
}GPIO_Handle_t;

/*
 * @GPIO_PIN_NOS
 * GPIO Pin Numbers
 */

#define GPIO_PIN_NO_0		0
#define GPIO_PIN_NO_1		1
#define GPIO_PIN_NO_2		2
#define GPIO_PIN_NO_3		3
#define GPIO_PIN_NO_4		4
#define GPIO_PIN_NO_5		5
#define GPIO_PIN_NO_6		6
#define GPIO_PIN_NO_7		7
#define GPIO_PIN_NO_8		8
#define GPIO_PIN_NO_9		9
#define GPIO_PIN_NO_10		10
#define GPIO_PIN_NO_11		11
#define GPIO_PIN_NO_12		12
#define GPIO_PIN_NO_13		13
#define GPIO_PIN_NO_14		14
#define GPIO_PIN_NO_15		15

/*
 * @GPIO_PIN_MODES
 * GPIO Pins possible Modes
 */

#define GPIO_MODE_IN		0
#define GPIO_MODE_OUT		1
#define GPIO_MODE_ALTFN		2
#define GPIO_MODE_ANALOG 	3
#define GPIO_MODE_IT_FT		4					/*Input falling Trigger			*/
#define GPIO_MODE_IT_RT		5					/*Input rising Trigger			*/
#define GPIO_MODE_IT_RFT	6					/*Input falling rising Trigger	*/

/*
 * @GPIO_OUTPUT_TYPES
 * GPIO Pins Output Types
 */

#define GPIO_OP_TYPE_PP		0					/*		Push Pull				*/
#define GPIO_OP_TYPE_OD		1					/*		Open Drain				*/

/*
 * @GPIO_SPEED_MODES
 * GPIO Pins Output Speed
 */

#define GPIO_OP_LS			0					/*		Low Speed				*/
#define GPIO_OP_MS			1					/*		Medium Speed			*/
#define GPIO_OP_HS			2					/*		High Speed				*/
#define GPIO_OP_VHS			3					/*		Very High Speed			*/

/*
 * @GPIO_PULLUP_PULLDOWN
 * GPIO Pin Pull up Pull Down
 */

#define GPIO_NO_PUPD		0					/*		No Pull up/Pull down	*/
#define GPIO_PIN_PU			1					/*		Pull up					*/
#define GPIO_PIN_PD			2					/*		Pull down				*/
/***************************************************************************************************************
 * 									APIs supported by this driver
 *  				For more information about the APIs check the function definitions
 ***************************************************************************************************************/

/*
 * Peripheral Initialization and De-Init
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);


/*
 * Peripheral Clock Setup
 */

void GPIO_PClkCtrl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

/*
 * Peripheral Read and Write
 */

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx,uint16_t Value);

/*
 * Peripheral Output Toggling
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/*
 * Peripheral Interrupt Handling and Configuration
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);




















#endif /* INC_STM32F407XX_GPIO_DRIVER_H_ */
