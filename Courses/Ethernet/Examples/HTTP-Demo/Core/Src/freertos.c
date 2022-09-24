/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lwip/apps/httpd.h"
#include "lwip/apps/fs.h"
#include <string.h>
#include "smm_hd44780.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for mainTask */
osThreadId_t mainTaskHandle;
uint32_t mainTaskBuffer[ 128 ];
osStaticThreadDef_t mainTaskControlBlock;
const osThreadAttr_t mainTask_attributes = {
  .name = "mainTask",
  .stack_mem = &mainTaskBuffer[0],
  .stack_size = sizeof(mainTaskBuffer),
  .cb_mem = &mainTaskControlBlock,
  .cb_size = sizeof(mainTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static const char* WEB_SSI_TAGS[] = {
	"sw1",
	"sw2",
	"sw3",
};

static const char HIGH[] = "High";
static const int HIGH_LEN = sizeof(HIGH) - 1;
static const char LOW[] = "Low";
static const int LOW_LEN = sizeof(LOW) - 1;

static const char* swStates[] = { 
	HIGH,
	HIGH,
	HIGH,
};



static const int WEB_SSI_TAGS_LEN = sizeof(WEB_SSI_TAGS) / sizeof(WEB_SSI_TAGS[0]);

static const char MY_PAGE[] = "This is My Page";
static const int MY_PAGE_LEN = sizeof(MY_PAGE) - 1;

static const char PAGE_DONE[] = "Done";
static const int PAGE_DONE_LEN = sizeof(PAGE_DONE) - 1;

static const char PAGE_ERROR[] = "Error";
static const int PAGE_ERROR_LEN = sizeof(PAGE_ERROR) - 1;

u16_t Web_ssiHandler(
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
      
														 ) {
															 
	strcpy(pcInsert, swStates[iIndex]);
	return swStates[iIndex] == HIGH ? HIGH_LEN : LOW_LEN;
}
														 
const char* Web_ledHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
	if (iNumParams == 2) {
		if (strcmp(pcParam[0], "num") == 0 && strcmp(pcParam[1], "state") == 0) {
			int num;
			int state;
			sscanf(pcValue[0], "%d", &num);
			sscanf(pcValue[1], "%d", &state);
			if ((num >= 0 && num <= 2) && (state == 0 || state == 1)) {
				switch (num) {
					case 0:
						HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, (GPIO_PinState) state);
						break;
					case 1:
						HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, (GPIO_PinState) state);
						break;
					case 2:
						HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, (GPIO_PinState) state);
						break;
				}
				return "/done";
			}
		}
		return "/error";
	}
	else {
		return "/404.html";
	}
}
					
const char* Web_alcdHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
	if (iNumParams == 1) {
		if (strcmp(pcParam[0], "text") == 0) {
			HD44780_Clear();
			HD44780_Puts(0, 0, pcValue[0]);
			return "/done";
		}
		else {
			return "/error";
		}
	}
	return "/404.html";
}

int fs_open_custom(struct fs_file *file, const char *name) {
	if (strcmp(name, "/mypage.html") == 0 || strcmp(name, "/mypage") == 0) {
		file->index = MY_PAGE_LEN;
		file->data = MY_PAGE;
		file->len = MY_PAGE_LEN;
		file->pextension = NULL;
		file->flags = 0;
		return 1;
	}
	else if (strcmp(name, "/done") == 0) {
		file->index = PAGE_DONE_LEN;
		file->data = PAGE_DONE;
		file->len = PAGE_DONE_LEN;
		file->pextension = NULL;
		file->flags = 0;
		return 1;
	}
	else if (strcmp(name, "/error") == 0) {
		file->index = PAGE_ERROR_LEN;
		file->data = PAGE_ERROR;
		file->len = PAGE_ERROR_LEN;
		file->pextension = NULL;
		file->flags = 0;
		return 1;
	}
	return 0;
}
void fs_close_custom(struct fs_file *file) {
	/*file->index = 0;
	file->data = NULL;
	file->len = 0;
	file->pextension = NULL;
	file->flags = 0;*/
}
int fs_read_custom(struct fs_file *file, char *buffer, int count) {
	if (file->index < file->len) {
		int left = fs_bytes_left(file);
		if (count < left) {
			left = count;
		}
		memcpy(buffer, &file->data[file->index], count);
		file->index += left;
		return fs_bytes_left(file);
	}
	else {
		return FS_READ_EOF;
	}
}

static const tCGI WEB_CGI_HANDLERS[] = {
	{"/led", Web_ledHandler},
	{"/alcd", Web_alcdHandler},
};
static const int WEB_CGI_HANDLERS_LEN = sizeof(WEB_CGI_HANDLERS) / sizeof(WEB_CGI_HANDLERS[0]);

/* USER CODE END FunctionPrototypes */

void StartMainTask(void *argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of mainTask */
  mainTaskHandle = osThreadNew(StartMainTask, NULL, &mainTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartMainTask */
/**
  * @brief  Function implementing the mainTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartMainTask */
void StartMainTask(void *argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartMainTask */
  /* Infinite loop */
	HD44780_Init(16, 2);
	httpd_init();
	http_set_ssi_handler(Web_ssiHandler, WEB_SSI_TAGS, WEB_SSI_TAGS_LEN);
	http_set_cgi_handlers(WEB_CGI_HANDLERS, WEB_CGI_HANDLERS_LEN);
  for(;;)
  {
		swStates[0] = HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) ? HIGH : LOW;
		swStates[1] = HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) ? HIGH : LOW;
		swStates[2] = HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) ? HIGH : LOW;
    osDelay(50);
  }
  /* USER CODE END StartMainTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
