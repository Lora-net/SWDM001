For the most part, this is a subset of the STM32CubeL4 project, taken
from:
	https://github.com/STMicroelectronics/STM32CubeL4/tree/v1.14.0

stm32l4xx_hal_spi.h and stm32l4xx_hal_spi.c have been modified by the
addition of the function:

/**
  * @brief  Same as HAL_SPI_Receive, but make sure zeros are sent on bus.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pRxData pointer to reception data buffer
  * @param  Size amount of data to be sent and received
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Receive0(SPI_HandleTypeDef *hspi, uint8_t *pRxData, uint16_t Size, uint32_t Timeout)
