/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: Dec 29, 2025
 *      Author: mansisd
 */


#include "stm32f407xx_spi_driver.h"

/************************************************************************************************************************************************************************
 * @fn								-	SPI_PClkCtrl
 *
 * @brief							-
 *
 * @param[in]						-
 * @param[in]						-
 * @param[in]						-
 *
 * @return							-
 *
 * @Note							-
 */

void SPI_PClkCtrl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{

	if(EnorDi == ENABLE){
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}else if(pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}else if(pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}
	}else{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_DI();
		}else if(pSPIx == SPI2)
		{
			SPI2_PCLK_DI();
		}else if(pSPIx == SPI3)
		{
			SPI3_PCLK_DI();
		}
		//TO DO(disable clock)
	}


}

/************************************************************************************************************************************************************************
 * @fn								-	SPI_Init
 *
 * @brief							-
 *
 * @param[in]						-
 * @param[in]						-
 * @param[in]						-
 *
 * @return							-
 *
 * @Note							-
 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{

	//PERIPHERAL CLOCK ENABLE
	SPI_PClkCtrl(pSPIHandle->pSPIx,ENABLE);

	//first lets configure the SPI_CR1 register
	uint32_t tempreg = 0;
	//1. configure the device mode
	tempreg |= pSPIHandle->SPI_PinConfig.SPI_DeviceMode << SPI_CR1_MSTR;

	//2. configure the bus configuration
	if(pSPIHandle->SPI_PinConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		//bi-di mode should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
	}else if(pSPIHandle->SPI_PinConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//bi-di mode should be set
		tempreg |=  (1 << SPI_CR1_BIDIMODE);
	}else if(pSPIHandle->SPI_PinConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		//bi-di mode should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
		//Rx only mode should be enabled
		tempreg |= (1 << SPI_CR1_RXONLY);
	}

	//3. Configure the SPI serial clock speed
	tempreg |= (pSPIHandle->SPI_PinConfig.SPI_SclkSpeed << SPI_CR1_BR);

	//4. Configure the DFF
	tempreg |= (pSPIHandle->SPI_PinConfig.SPI_DFF << SPI_CR1_DFF);

	//5. Configure the CPOL
	tempreg |= (pSPIHandle->SPI_PinConfig.SPI_CPOL << SPI_CR1_CPOL);

	//6. Configure the CPHA
	tempreg |= (pSPIHandle->SPI_PinConfig.SPI_CPHA << SPI_CR1_CPHA);

	//7. Configure the SSM
	tempreg |= (pSPIHandle->SPI_PinConfig.SPI_SSM << SPI_CR1_SSM);

	pSPIHandle ->pSPIx ->CR1 = tempreg;
}

/************************************************************************************************************************************************************************
 * @fn								-	SPI_DeInit
 *
 * @brief							-
 *
 * @param[in]						-
 * @param[in]						-
 * @param[in]						-
 *
 * @return							-
 *
 * @Note							-
 */

void SPI_DeInit(SPI_RegDef_t *pSPIx)
{

	if(pSPIx == SPI1)
	{
		SPI1_REG_RESET();
	}else if(pSPIx == SPI2)
	{
		SPI2_REG_RESET();
	}else if(pSPIx == SPI3)
	{
		SPI3_REG_RESET();
	}


}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/************************************************************************************************************************************************************************
 * @fn								-	SPI_SendData
 *
 * @brief							-	known as blocking API
 *
 * @param[in]						-
 * @param[in]						-
 * @param[in]						-
 *
 * @return							-
 *
 * @Note							- This is a blocking call
 */

void SPI_SendData(SPI_RegDef_t *pSPIx,uint8_t *pTxBuffer, uint32_t Len)
{

	while(Len >0)
	{
		//1. wait until TXE is set
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

		//2. Check the DFF bit in CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF) )
		{
			//16 bit DFF
			//1. load the data in to the DR
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			Len--;
			Len--;
			(uint16_t*)pTxBuffer++;
		}else
		{
			//8 bit DFF
			pSPIx->DR = *pTxBuffer;
			Len--;
			pTxBuffer++;
		}
	}


}
/************************************************************************************************************************************************************************
 * @fn								-	SPI_ReceiveData
 *
 * @brief							-
 *
 * @param[in]						-
 * @param[in]						-
 * @param[in]						-
 *
 * @return							-
 *
 * @Note							-
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE );
	}
	else
	{
		pSPIx ->CR1 &= ~(1 << SPI_CR1_SPE);
	}

}
/************************************************************************************************************************************************************************
 * @fn								-	SPI_ReceiveData
 *
 * @brief							-
 *
 * @param[in]						-
 * @param[in]						-
 * @param[in]						-
 *
 * @return							-
 *
 * @Note							-
 */
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SSI );
	}
	else
	{
		pSPIx ->CR1 &= ~(1 << SPI_CR1_SSI);
	}

}

/************************************************************************************************************************************************************************
 * @fn								-	SPI_ReceiveData
 *
 * @brief							-
 *
 * @param[in]						-
 * @param[in]						-
 * @param[in]						-
 *
 * @return							-
 *
 * @Note							-
 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx,uint8_t *pRxBuffer, uint32_t Len)
{




}

/************************************************************************************************************************************************************************
 * @fn								-	SPI_IRQInterruptConfig
 *
 * @brief							-
 *
 * @param[in]						-
 * @param[in]						-
 * @param[in]						-
 *
 * @return							-
 *
 * @Note							-
 */

void SPI_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi)
{




}

/************************************************************************************************************************************************************************
 * @fn								-	SPI_IRQPriorityConfig
 *
 * @brief							-
 *
 * @param[in]						-
 * @param[in]						-
 * @param[in]						-
 *
 * @return							-
 *
 * @Note							-
 */
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{




}

/************************************************************************************************************************************************************************
 * @fn								-	SPI_IRQHandling
 *
 * @brief							-
 *
 * @param[in]						-
 * @param[in]						-
 * @param[in]						-
 *
 * @return							-
 *
 * @Note							-
 */

void SPI_IRQHandling(SPI_Handle_t *pSPIHandle)
{




}
