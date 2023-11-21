/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "sd.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_SPI3_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
//  uint32_t NumberBack = 0;
    LCD_Init(WHITE);
//    LCD_ShowString(30,0,"SPI mode SD card test",BLACK,WHITE,16,1);
//
//    uint8_t i = 0;
//    while (SD_Init() != 0) {
//        LCD_ShowString(30,16 + 24*i,"No SD Card",BLACK,WHITE,24,1);
//        i++;
//        HAL_Delay(2000);
//    }
//    LCD_ShowString(30,16,"SD Init success",BLACK,WHITE,16,1);
//
//    //逻辑0扇区的物理扇区号
//    NumberBack = SD_GetLogic0();
//    LCD_ShowString(30,16*2,"Number:",BLACK,WHITE,16,1);
//    LCD_ShowHexNum(30 + 8* strlen("NUmber:"),16*2,NumberBack,8,BLACK,WHITE,16,1 );
//
//    //获取扇区总数
//    uint32_t SectorNum = 0;
//    SectorNum = GetSDCardSectorCount(); //SectorNum = 15564800 SD 7.421875GB

    //Write Data in SD
//    uint8_t Buff[512];
//    for(uint16_t j = 0;j < 512;j++) {
//        Buff[j] = 2*j;
//    }
//    SDCardWriteData(Buff,0x00000000,1);

//    uint8_t RxBuff[512];

    FATFS *fs;
    FRESULT ret;
    //apply malloc
    fs = (FATFS *)malloc(sizeof(FATFS));

    //mount SD device
    ret = f_mount(fs,"0:",1);
    if(ret) {
        LCD_ShowString(30,0,"mount fail",BLACK,WHITE,16,1);
    } else {
        LCD_ShowString(30, 0, "mount OK", BLACK, WHITE, 16, 1);
    }

    //open file
    FIL *fil;
    FRESULT res;
    fil = malloc(sizeof(FIL));
    res = f_open(fil,"test.txt",FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
    if(res) {
        LCD_ShowString(30,16,"open fail",BLACK,WHITE,16,1);
    } else {
        LCD_ShowString(30,16,"open OK",BLACK,WHITE,16,1);
    }

    //read file attention: read and write ptr
    uint8_t RxBuff[512] = {0};
    uint16_t FSize;
    uint16_t ReadCount;
    FSize = f_size(fil);
    f_read(fil,RxBuff,FSize,(UINT *)&ReadCount);
    if(ReadCount != 0) {
        for(uint8_t i = 0;i < FSize;i++) {
            LCD_ShowChar(30 + 8*i,16*2,RxBuff[i],BLACK,WHITE,16,1);
        }
    }

    //write file
//    uint16_t WriteCount;
//    f_write(fil,"Hello?",sizeof("Hello?"),(UINT *)&WriteCount);

    //close file
    f_close(fil);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//      //read SD
//      for(uint16_t m = 0;m < 512 ;m++) {
//          RxBuff[m] = 0;
//      }
//        SDCardReadData(RxBuff,0x00000000,1);
//        LCD_ShowIntNum(30,16*3,RxBuff[0],3,BLACK,WHITE,16);
//        LCD_ShowIntNum(30 + 5*8,16*3,RxBuff[1],3,BLACK,WHITE,16);
//        LCD_ShowIntNum(30 + 10*8,16*3,RxBuff[2],3,BLACK,WHITE,16);
//        LCD_ShowIntNum(30 + 15*8,16*3,RxBuff[3],3,BLACK,WHITE,16);
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
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
