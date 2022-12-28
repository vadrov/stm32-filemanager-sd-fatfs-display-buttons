/* USER CODE BEGIN Header */
/**
  *
  * Copyright (C) 2022, VadRov, all right reserved.
  *
  * Проект файлового менеджера с использованием библиотеки FATFS
  *
  * Допускается свободное распространение без целей коммерческого использования.
  * При коммерческом использовании необходимо согласование с автором.
  * Распространятся по типу "как есть", то есть использование осуществляете на свой страх и риск.
  * Автор не предоставляет никаких гарантий.
  *
  * Основные возможности файлового менеджера:
  * - Вывод менеджера в окне, параметры (координата верхнего левого угла, ширина и высота окна, шрифт, цвет)
  *   которого задает пользователь;
  * - Сокращение имен файлов и каталогов (если они не помещаются в окне) по типу менеджера Total Commander;
  * - Нет ограничений на количество файлов и подкаталогов в каталоге;
  * - Запоминание позиции курсора в вышестоящем каталоге и восстановление этой позиции при возврате на него
  *   с вложенного подкаталога;
  * - Вывод иконок (значков) файлов и каталогов;
  * - Поддержка вложенных каталогов с уровнем вложенности, ограниченным размером оперативной памяти
  *   микроконтроллера.
  *
  *   Проект базируется на библиотеках автора:
  *   - драйвер работы с SD-картой по SPI c использованием DMA (Авторы: ChaN (2014), kiwih (2017), VadRov (2019),
  *     каталог FATFS/Target)
  *     https://www.youtube.com/watch?v=z99bLhlnukM
  *   - драйвер для работы с дисплеями по SPI с использованием DMA (каталог Display_spi_LL)
  *     https://www.youtube.com/watch?v=8tIJ16riJqo
  *   - модуль для работы с кнопками с фильтром дребезга, автоповтором и буферризацией ввода (каталог Keyboard)
  *     https://www.youtube.com/watch?v=e-w5HS75neg
  *   - процедуры работы со строками (каталог MyString).
  *
  * Автор: VadRov
  *
  * https://www.youtube.com/@VadRov
  * https://dzen.ru/vadrov
  * https://vk.com/vadrov
  * https://t.me/vadrov_channel
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
/*--------- Подключение пользовательских модулей и драйверов -----------------*/
#include "../../Keyboard/keyboard.h" 	   //модуль работы с кнопками
#include "../../Display/display.h"  	   //драйвер работы с дисплеями по SPI
#include "../../Display/st7789.h"	  	   //драйвер для дисплея ST7789
#include "../../Filemanager/filemanager.h" //модуль файлового менеджера
#include "../../MyString/mystring.h"	   //библиотека работы со строками
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
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM10_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM3_Init(void);
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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_TIM10_Init();
  MX_SPI2_Init();
  MX_TIM3_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

  /* ---------------------------- Настройка кнопок ------------------------------- */
  KEYB_Add_Button(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin, KEYB_LEFT, KEYB_BUTTON_ACTIVE);
  KEYB_Add_Button(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin, KEYB_RIGHT, KEYB_BUTTON_ACTIVE);
  KEYB_Add_Button(KEY_UP_GPIO_Port, KEY_UP_Pin, KEYB_UP, KEYB_BUTTON_ACTIVE);
  KEYB_Add_Button(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin, KEYB_DOWN, KEYB_BUTTON_ACTIVE);

  TIM10->DIER |= TIM_DIER_UIE;
  TIM10->CR1  |= TIM_CR1_CEN;
  /* ------------------------------------------------------------------------------- */

  /* ---------------------------- Настройка дисплея -------------------------------- */
  LCD_DMA_TypeDef dma_tx = { DMA2,
    		  	  	  	  	 LL_DMA_STREAM_3 };

  LCD_BackLight_data bkl_data = { TIM3,
    		  	  	  	  	  	  LL_TIM_CHANNEL_CH1,
								  0,
								  0,
								  50  };

  LCD_SPI_Connected_data spi_con = { SPI1,
    		  	  	  	  	  	  	 dma_tx,
									 LCD_RES_GPIO_Port,
									 LCD_RES_Pin,
									 LCD_DC_GPIO_Port,
									 LCD_DC_Pin,
									 LCD_CS_GPIO_Port,
									 LCD_CS_Pin         };

  #ifndef LCD_DYNAMIC_MEM
  LCD_Handler lcd1;
#endif
  LCD = LCD_DisplayAdd( LCD,
#ifndef LCD_DYNAMIC_MEM
		   	   	   	   	&lcd1,
#endif
						240,
     					240,
						ST7789_CONTROLLER_WIDTH,
						ST7789_CONTROLLER_HEIGHT,
						PAGE_ORIENTATION_PORTRAIT,
						//PAGE_ORIENTATION_LANDSCAPE,
						ST7789_Init,
						ST7789_SetWindow,
						ST7789_SleepIn,
						ST7789_SleepOut,
						&spi_con,
						LCD_DATA_16BIT_BUS,
						bkl_data				   );
  LCD_Handler *lcd = LCD; //указатель на первый дисплей в списке
  LCD_Init(lcd);
  LCD_Fill(lcd, COLOR_BLACK);
  /* ------------------------------------------------------------------------------ */

  /* ----------------------------- Монтирование диска ----------------------------- */
  (void)f_mount(&USERFatFS, USERPath, 0);
  /* ------------------------------------------------------------------------------ */

  /* ----------------------- Настройка файлового менеджера ------------------------ */
  /* Определение цветовых схем. Варианты для примера. */
  /* Вариант 1. Цветовая схема по типу TotalCommander */
  File_Manager_Color_Scheme color_scheme_tc = { COLOR_BLACK, /* цвет текста наименования каталога */
		  	  	  	  	  	  	  	  	  	  	COLOR_BLACK, /* цвет текста наименования файла */
												COLOR_BLACK, /* цвет текста выбранного файла/каталога */
												COLOR_WHITE, /* цвет фона окна менеджера (для пустых строк) */
												COLOR_WHITE, /* цвет фона четных строк */
												0xF5F5F5,    /* цвет фона нечетных строк */
												COLOR_BLACK, /* цвет курсора текущего файла/каталога */
												0xDBDBDB,	 /* цвет ползунка вертикальной прокрутки */
												0xF5F5F5,	 /* цвет полосы вертикальной прокрутки */
												0			 /* флаг закрашивания курсора:
															    0 - текст выбранного файла/каталога обводится
															    прямоугольником цвета курсора (cursor_color).
													 	 	    >0 - фон текста выбранного файла/каталога приравнивается
													 	 	    цвету курсора (cursor_color) */
											 	 };
  /* Вариант 2. Цветовая схема по типу FAR */
  File_Manager_Color_Scheme color_scheme_far = { COLOR_WHITE,
		  	  	  	  	  	  	  	  	  	  	 COLOR_CYAN,
												 COLOR_BLACK,
		  	  	  	  	  	  	  	  	  	  	 COLOR_BLUE,
												 COLOR_BLUE,
												 COLOR_BLUE,
												 0x00CCCC,
												 COLOR_CYAN,
												 COLOR_BLUE,
												 1};
  /* Вариант 3. Цветовая схема по типу Frigate */
  File_Manager_Color_Scheme color_scheme_frigate = { 0x19389E,
		  	  	  	  	  	  	  	  	  	  	  	 0x19389E,
													 COLOR_BLACK,
													 COLOR_WHITE,
													 COLOR_WHITE,
													 COLOR_WHITE,
													 0xA6CBEE,
													 0xA6CBEE,
												 	 0xF8F8F5,
													 1};
  /* Вариант 4. Цветовая схема по типу Double Commander */
  File_Manager_Color_Scheme color_scheme_dc = { COLOR_GREEN,
		  	  	  	  	  	  	  	  	  	  	COLOR_GREEN,
												COLOR_BLACK,
		  	  	  	  	  	  	  	  	  	  	COLOR_BLACK,
												COLOR_BLACK,
												COLOR_BLACK,
												0x0A246A,
												0xD4D0C8,
												0xF0F0F0,
												1};

  /* Объявление массива указателей на цветовые схемы */
  File_Manager_Color_Scheme *color_scheme[] = {&color_scheme_tc, &color_scheme_far, &color_scheme_frigate, &color_scheme_dc};

/* Константы, соответствующие позиции указателя на цветовую схему в массиве *color_scheme[] */
#define COLOR_SCHEME_TC			0
#define COLOR_SCHEME_FAR		1
#define COLOR_SCHEME_FRIGATE	2
#define COLOR_SCHEME_DC			3

  File_Manager_Handler *fm = FileManagerNew();			/* Создание обработчика файлового менеджера */
  fm->SetDisplay(fm, lcd);								/* Дисплей, на который выводится файловый менеджер (указатель) */
  fm->SetWin(fm, 0, 0, lcd->Width, lcd->Height);		/* Параметры окна файлового менеджера:
  	  	  	  	  	  	  	  	  	  	  	  	  	  	  - позиция левого верхнего угла окна по горизонтали;
  	  	  	  	  	  	  	  	  	  	  	  	  	  	  - позиция левого верхнего угла окна по вертикали;
  	  	  	  	  	  	  	  	  	  	  	  	  	  	  - ширина окна;
  	  	  	  	  	  	  	  	  	  	  	  	  	  	  - высота окна. */
  fm->SetColor(fm, color_scheme[COLOR_SCHEME_TC]);		/* Цветовая схема (указатель) */
  fm->SetFont(fm, &Font_12x20);							/* Шрифт (указатель) */
  fm->SetKeys(fm, KEYB_UP, KEYB_DOWN, KEYB_RIGHT);		/* Кнопки управления:
   	   	   	   	   	   	   	   	   	   	   	   	   	   	   - номер бита кнопки вверх;
   	   	   	   	   	   	   	   	   	   	   	   	   	   	   - номер бита кнопки вниз;
   	   	   	   	   	   	   	   	   	   	   	   	   	   	   - номер бита кнопки ввод/выбор. */
  //--------------------------------------------------------------------------------

  while (1)
  {
	  /* "Всплывающее" окно файлового менеджера */
	  FileManagerPopupWin(fm, 5);
	  /* Запуск менеджера файлов */
	  fm->Show(fm);
	  /* Проверка статуса завершения работы менеджера файлов */
	  if (fm->GetStatus(fm) == File_Manager_OK) /* Завершение без ошибок с выбором файла? */
	  {
		  /* "Склеиваем" путь до файла с именем файла */
		  char *tmp = my_strsplice(2, "/", fm->GetFilePath(fm), fm->GetFileName(fm));
		  if (f_open(&USERFile, tmp, FA_READ) == FR_OK) /* Файл открыт успешно? */
		  {
			  /* Выводим сообщение о том, что файл открыт успешно. */
			  LCD_WriteString(lcd, 0, 0, "File opened succesfully!", fm->font, fm->color_scheme->text_color_file, fm->color_scheme->bg_color, LCD_SYMBOL_PRINT_FAST);
			  /* Закрываем файл */
			  f_close(&USERFile);
		  }
		  else
		  {
			  /* Выводим сообщение о том, что возникла ошибка при открытии файла */
			  LCD_WriteString(lcd, 0, 0, "File opening error!", fm->font, fm->color_scheme->text_color_file, fm->color_scheme->bg_color, LCD_SYMBOL_PRINT_FAST);
		  }
	  }
	  else
	  {
		  /* Выводим сообщение, что работа файлового менеджера завершена с ошибкой */
		  LCD_WriteString(lcd, 0, 0, "Error FileManager!", fm->font, fm->color_scheme->text_color_file, fm->color_scheme->bg_color, LCD_SYMBOL_PRINT_FAST);
	  }
	  /* Ожидаем нажатия любой кнопки */
	  while(!KEYB_Inkeys()) ;
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 168, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_SetSystemCoreClock(84000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
  LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**SPI1 GPIO Configuration
  PA5   ------> SPI1_SCK
  PA7   ------> SPI1_MOSI
  */
  GPIO_InitStruct.Pin = LCD_SCL_Pin|LCD_SDA_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* SPI1 DMA Init */

  /* SPI1_TX Init */
  LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_3, LL_DMA_CHANNEL_3);

  LL_DMA_SetDataTransferDirection(DMA2, LL_DMA_STREAM_3, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetStreamPriorityLevel(DMA2, LL_DMA_STREAM_3, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA2, LL_DMA_STREAM_3, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA2, LL_DMA_STREAM_3, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA2, LL_DMA_STREAM_3, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA2, LL_DMA_STREAM_3, LL_DMA_PDATAALIGN_HALFWORD);

  LL_DMA_SetMemorySize(DMA2, LL_DMA_STREAM_3, LL_DMA_MDATAALIGN_HALFWORD);

  LL_DMA_DisableFifoMode(DMA2, LL_DMA_STREAM_3);

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_HIGH;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 10;
  LL_SPI_Init(SPI1, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI1, LL_SPI_PROTOCOL_MOTOROLA);
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**SPI2 GPIO Configuration
  PB13   ------> SPI2_SCK
  PB14   ------> SPI2_MISO
  PB15   ------> SPI2_MOSI
  */
  GPIO_InitStruct.Pin = SD_CLK_Pin|SD_MISO_Pin|SD_MOSI_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* SPI2 DMA Init */

  /* SPI2_RX Init */
  LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_3, LL_DMA_CHANNEL_0);

  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_3, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_3, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_STREAM_3, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_3, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_3, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_3, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_3, LL_DMA_MDATAALIGN_BYTE);

  LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_3);

  /* SPI2_TX Init */
  LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_4, LL_DMA_CHANNEL_0);

  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_4, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_4, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_STREAM_4, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_4, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_4, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_4, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_4, LL_DMA_MDATAALIGN_BYTE);

  LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_4);

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV128;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 10;
  LL_SPI_Init(SPI2, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI2, LL_SPI_PROTOCOL_MOTOROLA);
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  TIM_InitStruct.Prescaler = 999;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 209;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM3, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM3);
  LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH1);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 109;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM3, LL_TIM_CHANNEL_CH1);
  LL_TIM_SetTriggerOutput(TIM3, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM3);
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**TIM3 GPIO Configuration
  PA6   ------> TIM3_CH1
  */
  GPIO_InitStruct.Pin = LCD_BLK_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(LCD_BLK_GPIO_Port, &GPIO_InitStruct);

}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM10);

  /* TIM10 interrupt Init */
  NVIC_SetPriority(TIM1_UP_TIM10_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),2, 0));
  NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  TIM_InitStruct.Prescaler = 999;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 419;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM10, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM10);
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* DMA interrupt init */
  /* DMA1_Stream3_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Stream3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(DMA1_Stream3_IRQn);
  /* DMA1_Stream4_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Stream4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  /* DMA2_Stream3_IRQn interrupt configuration */
  NVIC_SetPriority(DMA2_Stream3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(DMA2_Stream3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_SetOutputPin(SD_LED_GPIO_Port, SD_LED_Pin);

  /**/
  LL_GPIO_SetOutputPin(GPIOA, LCD_DC_Pin|LCD_RES_Pin|LCD_CS_Pin);

  /**/
  LL_GPIO_SetOutputPin(SD_CS_GPIO_Port, SD_CS_Pin);

  /**/
  GPIO_InitStruct.Pin = SD_LED_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(SD_LED_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LCD_DC_Pin|LCD_RES_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LCD_CS_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(LCD_CS_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = SD_CS_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(SD_CS_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = KEY_LEFT_Pin|KEY_RIGHT_Pin|KEY_UP_Pin|KEY_DOWN_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
