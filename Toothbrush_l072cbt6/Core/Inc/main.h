/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DS1302_SCLK_Pin GPIO_PIN_5
#define DS1302_SCLK_GPIO_Port GPIOA
#define DS1302_CE_Pin GPIO_PIN_7
#define DS1302_CE_GPIO_Port GPIOA
#define W25Q16_CS_Pin GPIO_PIN_12
#define W25Q16_CS_GPIO_Port GPIOB
#define W25Q16_CLK_Pin GPIO_PIN_13
#define W25Q16_CLK_GPIO_Port GPIOB
#define W25Q16_DO_Pin GPIO_PIN_14
#define W25Q16_DO_GPIO_Port GPIOB
#define W25Q16_DI_Pin GPIO_PIN_15
#define W25Q16_DI_GPIO_Port GPIOB
#define MPU6050_SDA_Pin GPIO_PIN_8
#define MPU6050_SDA_GPIO_Port GPIOB
#define MPU6050_SCL_Pin GPIO_PIN_9
#define MPU6050_SCL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
