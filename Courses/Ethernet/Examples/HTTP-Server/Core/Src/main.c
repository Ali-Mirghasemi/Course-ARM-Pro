/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "lwip/apps/httpd.h"
#include "lwip/apps/fs.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
  Tag_Timestamp,
  Tag_Led0,
  Tag_Led1,
  Tag_Led2,
} Tag_Index;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */


static const char* SSI_TAGS[] = {
  "ts",
  "led0",
  "led1",
  "led2",
};
static const uint32_t SSI_TAGS_LEN = sizeof(SSI_TAGS) / sizeof(SSI_TAGS[0]);

static char timestampBuffer[32] = "";

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
u16_t WebServer_onSSI(
#if LWIP_HTTPD_SSI_RAW
                             const char* ssi_tag_name,
#else /* LWIP_HTTPD_SSI_RAW */
                             int iIndex,
#endif /* LWIP_HTTPD_SSI_RAW */
                             char *pcInsert, int iInsertLen
#if LWIP_HTTPD_SSI_MULTIPART
                             , u16_t current_tag_part, u16_t *next_tag_part
#endif /* LWIP_HTTPD_SSI_MULTIPART */
#if defined(LWIP_HTTPD_FILE_STATE) && LWIP_HTTPD_FILE_STATE
                             , void *connection_state
#endif /* LWIP_HTTPD_FILE_STATE */
                             ) 
{
  switch ((Tag_Index) iIndex) {
    case Tag_Timestamp:
      return snprintf(pcInsert, iInsertLen, "%u", HAL_GetTick());
    case Tag_Led0:
      return snprintf(pcInsert, iInsertLen, "%s", (LED0_GPIO_Port->ODR & LED0_Pin) ? "On" : "Off");
    case Tag_Led1:
      return snprintf(pcInsert, iInsertLen, "%s", (LED1_GPIO_Port->ODR & LED1_Pin) ? "On" : "Off");
    case Tag_Led2:
      return snprintf(pcInsert, iInsertLen, "%s", (LED2_GPIO_Port->ODR & LED2_Pin) ? "On" : "Off");
    default:
      return 0;
  }
}

const char* WebServer_onLed(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
  if (iNumParams != 2) {
    return "/notfound";
  }
  else {
    uint8_t led;
    uint8_t state;
    while (iNumParams-- > 0) {
      if (strcmp(*pcParam, "led") == 0) {
        led = atoi(*pcValue);
      }
      else if (strcmp(*pcParam, "state") == 0) {
        state = atoi(*pcValue);
      }
      pcParam++;
      pcValue++;
    }
   
    if (led <= 2 && state <= 1) {
      switch (led) {
        case 0:
          HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, (GPIO_PinState) state);
          break;
        case 1:
          HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, (GPIO_PinState) state);
          break;
        case 2:
          HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, (GPIO_PinState) state);
          break;
      }
      return "/index.html";
    }
    else {
      return "/notfound";
    }
  }
}


static const tCGI CGI_TAGS[] = {
  {"/led", WebServer_onLed},
};
static const uint32_t CGI_TAGS_LEN = sizeof(CGI_TAGS) / sizeof(CGI_TAGS[0]);

int fs_open_custom(struct fs_file *file, const char *name) {
  if (strcmp(name, "/timestamp.html") == 0) {
    file->len = snprintf(timestampBuffer, sizeof(timestampBuffer), "%u", HAL_GetTick());
    file->index = 0;
    file->flags = 0;
    file->data = timestampBuffer;
    return 1;
  }
  else {
    return 0;
  }
}
void fs_close_custom(struct fs_file *file) {
  file->data = NULL;
  file->len = 0;
}
int fs_read_custom(struct fs_file *file, char *buffer, int count) {
  int remainingBytes = file->len - file->index;
  if (remainingBytes > 0) {
    if (remainingBytes > count) {
      remainingBytes = count;
    }
    memcpy(buffer, &file->data[file->index], remainingBytes);
    file->index += remainingBytes;
    return remainingBytes;
  }
  else {
    return FS_READ_EOF;
  }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LWIP_Init();
  /* USER CODE BEGIN 2 */
  httpd_init();
  http_set_ssi_handler(WebServer_onSSI, SSI_TAGS, SSI_TAGS_LEN);
  http_set_cgi_handlers(CGI_TAGS, CGI_TAGS_LEN);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    MX_LWIP_Process();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED0_Pin|LED1_Pin|LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED0_Pin LED1_Pin LED2_Pin */
  GPIO_InitStruct.Pin = LED0_Pin|LED1_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
