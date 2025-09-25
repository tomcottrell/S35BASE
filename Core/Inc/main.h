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
#include "stm32l4xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define nBUTTON1_Pin GPIO_PIN_2
#define nBUTTON1_GPIO_Port GPIOE
#define LCD_HEATER_Pin GPIO_PIN_3
#define LCD_HEATER_GPIO_Port GPIOE
#define ENABLE_USB_SUPPLY_Pin GPIO_PIN_6
#define ENABLE_USB_SUPPLY_GPIO_Port GPIOE
#define ENABLE_CURRENT_AIN2_Pin GPIO_PIN_13
#define ENABLE_CURRENT_AIN2_GPIO_Port GPIOC
#define ADC_HW_REF_ID1_Pin GPIO_PIN_0
#define ADC_HW_REF_ID1_GPIO_Port GPIOC
#define ADC_HW_REF_ID0_Pin GPIO_PIN_1
#define ADC_HW_REF_ID0_GPIO_Port GPIOC
#define uC_FREQ_IN_Pin GPIO_PIN_2
#define uC_FREQ_IN_GPIO_Port GPIOC
#define uC_FREQ_IN_EXTI_IRQn EXTI2_IRQn
#define ADC_VIN_MEAS_Pin GPIO_PIN_3
#define ADC_VIN_MEAS_GPIO_Port GPIOC
#define ADC_DISP_TEMP_Pin GPIO_PIN_0
#define ADC_DISP_TEMP_GPIO_Port GPIOA
#define LTDC_PWM_BACKLIGHT_Pin GPIO_PIN_1
#define LTDC_PWM_BACKLIGHT_GPIO_Port GPIOA
#define nCAN_RESISTOR_ENABLE_Pin GPIO_PIN_2
#define nCAN_RESISTOR_ENABLE_GPIO_Port GPIOA
#define RS485_RX_Pin GPIO_PIN_3
#define RS485_RX_GPIO_Port GPIOA
#define FLASH_NCS_Pin GPIO_PIN_4
#define FLASH_NCS_GPIO_Port GPIOA
#define ADC_AIN_2_Pin GPIO_PIN_5
#define ADC_AIN_2_GPIO_Port GPIOA
#define FLASH_SIO3_Pin GPIO_PIN_6
#define FLASH_SIO3_GPIO_Port GPIOA
#define FLASH_SIO2_Pin GPIO_PIN_7
#define FLASH_SIO2_GPIO_Port GPIOA
#define ADC_AIN_1_Pin GPIO_PIN_4
#define ADC_AIN_1_GPIO_Port GPIOC
#define LTDC_RESET_Pin GPIO_PIN_5
#define LTDC_RESET_GPIO_Port GPIOC
#define FLASH_IO1_Pin GPIO_PIN_0
#define FLASH_IO1_GPIO_Port GPIOB
#define FLASH_IO0_Pin GPIO_PIN_1
#define FLASH_IO0_GPIO_Port GPIOB
#define nBUTTON2_Pin GPIO_PIN_2
#define nBUTTON2_GPIO_Port GPIOB
#define FLASH_CLK_Pin GPIO_PIN_10
#define FLASH_CLK_GPIO_Port GPIOB
#define RTC_SDA_2_Pin GPIO_PIN_13
#define RTC_SDA_2_GPIO_Port GPIOB
#define RTC_SCL_2_Pin GPIO_PIN_14
#define RTC_SCL_2_GPIO_Port GPIOB
#define nBUTTON3_Pin GPIO_PIN_15
#define nBUTTON3_GPIO_Port GPIOB
#define DIGITAL_OUT1_Pin GPIO_PIN_6
#define DIGITAL_OUT1_GPIO_Port GPIOC
#define DIGITAL_OUT2_Pin GPIO_PIN_7
#define DIGITAL_OUT2_GPIO_Port GPIOC
#define nBUTTON0_Pin GPIO_PIN_8
#define nBUTTON0_GPIO_Port GPIOC
#define USB_VBUS_SENSE_Pin GPIO_PIN_9
#define USB_VBUS_SENSE_GPIO_Port GPIOA
#define USB_OTG_ID_Pin GPIO_PIN_10
#define USB_OTG_ID_GPIO_Port GPIOA
#define USB_D_N_Pin GPIO_PIN_11
#define USB_D_N_GPIO_Port GPIOA
#define USB_D_P_Pin GPIO_PIN_12
#define USB_D_P_GPIO_Port GPIOA
#define JTMS_SWDIO_Pin GPIO_PIN_13
#define JTMS_SWDIO_GPIO_Port GPIOA
#define JTCK_SWCLK_Pin GPIO_PIN_14
#define JTCK_SWCLK_GPIO_Port GPIOA
#define DOUT_nRED_LED_Pin GPIO_PIN_10
#define DOUT_nRED_LED_GPIO_Port GPIOC
#define DOUT_nRUN_LED_Pin GPIO_PIN_11
#define DOUT_nRUN_LED_GPIO_Port GPIOC
#define FW_UPDATE_ENABLE_Pin GPIO_PIN_12
#define FW_UPDATE_ENABLE_GPIO_Port GPIOC
#define ENABLE_RESISTIVE_AIN2_Pin GPIO_PIN_2
#define ENABLE_RESISTIVE_AIN2_GPIO_Port GPIOD
#define RS485_IO_Pin GPIO_PIN_4
#define RS485_IO_GPIO_Port GPIOD
#define RS485_TX_Pin GPIO_PIN_5
#define RS485_TX_GPIO_Port GPIOD
#define ENABLE_RESISTIVE_AIN1_Pin GPIO_PIN_7
#define ENABLE_RESISTIVE_AIN1_GPIO_Port GPIOD
#define JTDO_TRACESWO_Pin GPIO_PIN_3
#define JTDO_TRACESWO_GPIO_Port GPIOB
#define NJTRST_Pin GPIO_PIN_4
#define NJTRST_GPIO_Port GPIOB
#define ENABLE_CURRENT_AIN1_Pin GPIO_PIN_5
#define ENABLE_CURRENT_AIN1_GPIO_Port GPIOB
#define USART_TX_Pin GPIO_PIN_6
#define USART_TX_GPIO_Port GPIOB
#define USART_RX_Pin GPIO_PIN_7
#define USART_RX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define TIMER2_COUNTER_PERIOD		16383

#define VERSION_STRING_LENGTH		24
extern char firmware_version[VERSION_STRING_LENGTH];

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
