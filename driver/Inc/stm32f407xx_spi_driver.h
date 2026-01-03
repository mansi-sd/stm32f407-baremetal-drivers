/*
 * stm32f407xx_spi_driver.h
 *
 *  Created on: Dec 29, 2025
 *      Author: mansisd
 */

#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_
#include "stm32f407xx.h"
/*
 * This is a configuration structure for SPI
 */

typedef struct
{
	uint8_t SPI_DeviceMode;					/*POSSIBLE VALUES FROM 							@SPI_DeviceMode 	*/
	uint8_t SPI_BusConfig;					/*POSSIBLE VALUES FROM 							@SPI_BusConfig 		*/
	uint8_t SPI_SclkSpeed;					/*POSSIBLE VALUES FROM 							@SPI_SclkSpeed 		*/
	uint8_t SPI_DFF;						/*POSSIBLE VALUES FROM 							@SPI_DFF		 	*/
	uint8_t SPI_CPHA;						/*POSSIBLE VALUES FROM 							@CPOL 				*/
	uint8_t SPI_CPOL;						/*POSSIBLE VALUES FROM 							@CPHA 				*/
	uint8_t SPI_SSM;						/*POSSIBLE VALUES FROM 							@SSM 				*/

}SPI_PinConfig_t;

/*
 * This is a handle structure for SPI
 */
typedef struct
{
	SPI_RegDef_t *pSPIx;
	SPI_PinConfig_t	SPI_PinConfig;
}SPI_Handle_t;


/*
 * @SPI_DeviceMode
 */

#define SPI_DEVICE_MODE_MASTER				1
#define SPI_DEVICE_MODE_SLAVE				0

/*
 * @SPI_BusConfig
 */
#define SPI_BUS_CONFIG_FD					1
#define SPI_BUS_CONFIG_HD					2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY		3

/*
 * @SPI_SclkSpeed
 */

#define	SPI_SCLK_SPEED_DIV2					0
#define SPI_SCLK_SPEED_DIV4					1
#define SPI_SCLK_SPEED_DIV8					2
#define SPI_SCLK_SPEED_DIV16				3
#define SPI_SCLK_SPEED_DIV32				4
#define SPI_SCLK_SPEED_DIV64				5
#define SPI_SCLK_SPEED_DIV128				6
#define SPI_SCLK_SPEED_DIV256				7

/*
 * @SPI_DFF
 */
#define SPI_DFF_8BITS						0
#define SPI_DFF_16BITS						1

/*
 * @CPOL
 */
#define SPI_CPOL_HIGH						1
#define SPI_CPOL_LOW						0

/*
 * @CPHA
 */
#define SPI_CPHA_HIGH						1
#define SPI_CPHA_LOW						0

/*
 * @SSM
 */
#define SPI_SSM_DI						0
#define SPI_SSM_EN						1

/*
 * SPI related Status flags definitions
 */

#define SPI_TXE_FLAG						(1 << SPI_SR_TXE)
#define SPI_RXNE_FLAG						(1 << SPI_SR_RXNE)
#define SPI_BUSY_FLAG						(1 << SPI_SR_BSY)
/*********************************************************************************************************************************************************************
 *
 * 																APIs supported by this driver
 * 												For more information of the APIs check the function definitions
 *********************************************************************************************************************************************************************/

/*
 * Peripheral Clock Setup
 */
void SPI_PClkCtrl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/*
 *  In-it and De-Init
 */

void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

/*
 * Data Send and Receive
 */
void SPI_SendData(SPI_RegDef_t *pSPIx,uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx,uint8_t *pRxBuffer, uint32_t Len);

/*
 * IRQ Configuration and ISR Handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle);

/*
 * Other Peripheral Control APIs
 */


void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);





#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
