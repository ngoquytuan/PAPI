/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tm1637.h"
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#include "string.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
//enable/disable
#define ENABLE       1
#define DISABLE      0


uint32_t test1=0,t1=0,t2=0,bt_scan=0,tx1_count;
uint8_t UART_DATA[20];
uint8_t UART1_DATA[20];
char checku2=0,checku1=0;

char slave_debug = ENABLE;
#define DEBUG        'D'
#define MASTER       'M'
//#define SLAVE        'S'

#ifdef MASTER
	unsigned char typeOfdevice = MASTER; //'M' is master, 'S' is slave
	const unsigned char totalOfSlave = 2;// => slave 0, slave 1, slave 2
	unsigned char CurrentSlave = 0;
	unsigned char slave_com[totalOfSlave+1];// for checking communication between master and slave
	unsigned char slave_com_timeout[totalOfSlave+1];//// Phai cong 1 chu???
	unsigned char slave_error[totalOfSlave+1];// for checking slave's angle error
#endif

char idOfSlave = 2;





uint32_t time_to_check_slave = 5;
uint32_t time_to_check_sensor =5;//5s check sensor
uint32_t time_to_update_7seg = 0;
uint32_t flag_check_7seg = 1;

//printf("S>%03X-%03X-%02X-%02X-%02X<  ",rawx,rawy,buff_temp1,buff_temp2,lux);		
unsigned char system_error = NO_ERR;
//sensor error
unsigned char sen_err;
//slave error
unsigned char sla_err;			
int rawx,rawy,offsetrawx,offsetrawy;//SCA100
int deltax,deltay,abs_deltax,abs_deltay;
unsigned char buff_temp1,buff_temp2;//ds18b20
unsigned char lux;//cambienquang			
float temp_ds1820;			
uint8_t check=0;
uint32_t cnt; 


	
const uint8_t digitToSegment[10] = {
 // XGFEDCBA
  0x3f,//0b00111111,    // 0
  0x06,//0b00000110,    // 1
  0x5b,//0b01011011,    // 2
  0x4f,//0b01001111,    // 3
  0x66,//0b01100110,    // 4
  0x6d,//0b01101101,    // 5
  0x7d,//0b01111101,    // 6
  0x07,//0b00000111,    // 7
  0x7f,//0b01111111,    // 8
  0x6f,//0b01101111,    // 9
  };


unsigned char seg_temp1,seg_temp2,seg_temp3,seg_temp4;	
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);


/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
//check for lost sensor 
uint8_t time_out_lost_signal = 5;// = 0 is lost sensor 

unsigned char one_of_slave_err = 1;// 1 means contain an error





/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	char temp_id;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();

  /* USER CODE BEGIN 2 */
	/*if (HAL_TIM_Base_Start(&htim1) != HAL_OK) {
        Error_Handler();
    }*/

  if (HAL_TIM_Base_Start_IT(&htim1) != HAL_OK) {
        Error_Handler();
    }
	if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK) {
        Error_Handler();
    }
	//cnt = __HAL_TIM_GET_COUNTER(&htim1);
	tm1637Init();
	tm1637SetBrightness(4);
	//tm1637DisplayDecimal(6969,2);
		// Err
	TM1637_Disp_Dot(0,0x79,0x50,0x50);
		// Err
	TM1637_Disp_Dot(0,0x79,0x50,0x50);
		// -- 
	TM1637_Disp_Dot(0,0x40,0x40,0);
	//RS4851 TX
	RS485_TX1;
	//RS485 2 REC
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		if(typeOfdevice == MASTER) 
		{
			printf("PAPI master!\r\n");
			slave_stt_init();
		}
		else
		{
			printf("PAPI slave :%d\r\n",idOfSlave);
			if(slave_debug ==  ENABLE) printf("Slave emulator is ENABLE\r\n");
		}
	HAL_UART_Receive_DMA(&huart2,UART_DATA,20);
	//HAL_UART_Receive_IT(&huart1,UART_DATA,20);
	HAL_UART_Receive_DMA(&huart1,UART1_DATA,10);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		//turn tx1 to rx mode
		if(tx1_count == ONTIME) {RS485_RX1;tx1_count = STOP;}//------------------------------------------------------------------
		//checking the button
		if(bt_scan > 200)//scaning the buttons after per 200ms--------------------------------------------------------------------------------
		{
			bt_scan = 0;
			if(!HAL_GPIO_ReadPin(BT1_GPIO_Port,BT1_Pin)) {RS485_TX1;printf("BT1\r\n");offsetrawx = rawx; offsetrawy = rawy;}
			if(!HAL_GPIO_ReadPin(BT2_GPIO_Port,BT2_Pin)) {RS485_TX1;printf("BT2\r\n");}
		}
		
		//if((time_to_check_sensor == ONTIME) && (error != ERR_COMMUN)&& (error != ERR_SLAVE))//<--------------------------------------------------------------------
		if(checku2 == ONTIME) {//<----------------------------------------------------------Checking sensor
			checku2 = STOP;
			//RS485_TX1;
			//printf("Sensor: %s\r\n",UART_DATA);
			find_data();
			//printf("S>%03X-%03X-%02X-%02X-%02X<, temp: %2.1f, check:%dr\n",rawx,rawy,buff_temp1,buff_temp2,lux,temp_ds1820,check);
			//RS485_TX1;
			//printf("rawx:%d, offsetrawx :%d,rawy:%d, offsetrawy:%d\r\n",rawx,offsetrawx,rawy,offsetrawy);
			deltax = rawx - offsetrawx;
			deltay = rawy - offsetrawy;
			if(deltax > 0) abs_deltax = deltax;
			else abs_deltax = -deltax;
			
			if(deltay > 0) abs_deltay = deltay;
			else abs_deltay = -deltay;
			//RS485_TX1;
			//printf("Dx: %d,Dy: %d\r\n",deltax,deltay);
			memset(UART_DATA,0,20);
			HAL_UART_Receive_DMA(&huart2,UART_DATA,20);
			
		}	
		if(time_to_check_sensor == ONTIME)//<---------- always check sensor  <<<<< err1 & err2
		{
			if(time_out_lost_signal) time_out_lost_signal--;
			time_to_check_sensor = 5;// check sensor per every 5 seconds
			check_angle_err();
			if(time_out_lost_signal == 0) 
			{
				sen_err = NO_SEN ;
				rawx = 444;
				rawy = 444;
				offsetrawx = 0;
				offsetrawy = 0;
				abs_deltay = 444;
				abs_deltax = 444;
			}
			#ifdef DEBUG
			RS485_TX1;
			if(sen_err == NO_SEN) printf("No sensor\r\n");
			else if(sen_err == ANG_ERR) printf("Sensor alarm\r\n");
			else if(sen_err == NO_ERR) printf("Sensor fine\r\n");
			#endif
			//if(system_error) HAL_GPIO_WritePin(CRL8_GPIO_Port, CRL8_Pin, GPIO_PIN_SET);//<-----------------------------co chuyen dc di cho khac ko
			//else HAL_GPIO_WritePin(CRL8_GPIO_Port, CRL8_Pin, GPIO_PIN_RESET);
		}
		
		#ifdef MASTER//<--------------------------------------------------------------------------------------------------------------------------------------

			if(time_to_check_slave == ONTIME)
				{
					
					time_to_check_slave = 4;// =5 means after 5s check again
					// giam timeout neu slave ko phan hoi
					if(slave_com_timeout[CurrentSlave]) slave_com_timeout[CurrentSlave] --;
					else {	
									slave_com[CurrentSlave] = LOST_CON;
									sla_err = ERR_COMMUN;
									
									#ifdef DEBUG
									RS485_TX1;
									printf("Mat ket noi :%d-%d-%d\r\n",slave_com_timeout[0],slave_com_timeout[1],slave_com_timeout[2]);
									//MAT KET NOI SLAVE =>> ERR 3
									#endif
							}
					RS485_TX1;
					printf(">M0x%02X<",CurrentSlave++);
					if(CurrentSlave>totalOfSlave) CurrentSlave = 0; //fix total has only 3 slave
					
				}
		#endif
		
		
		if(flag_check_7seg == ONTIME)//<<<<----------------------------------------------------------------------------------------hien thi loi
		{
			flag_check_7seg = 2;//<-----------2s
			//if(system_error) HAL_GPIO_WritePin(CRL8_GPIO_Port, CRL8_Pin, GPIO_PIN_SET);
			//else HAL_GPIO_WritePin(CRL8_GPIO_Port, CRL8_Pin, GPIO_PIN_RESET);
			
			#ifdef MASTER
			
			RS485_TX1;
			printf("sen_err:%d;sla_err:%d\r\n",sen_err,sla_err);
			
			//No system_error
			if((sen_err == NO_ERR) && (sla_err == NO_ERR)) 	system_error = NO_ERR;
			//Err 1 : sensor of this lamp got trouble
			 if(sen_err == NO_SEN)											system_error = ERR_SENSOR ;
			// Err 2 : this lamp is wrong angle
			 if(sen_err == ANG_ERR) 										system_error = ERR_ANGLE  ;
			//Err 3 : lost connection to the slaves
			 if(sla_err == LOST_CON) 										system_error = ERR_COMMUN   ;
			// Err 4 : one or more of slaves got trouble
			 if(sla_err == ERR_SEN) 										system_error = ERR_SLAVE    ;
				#ifdef DEBUG
			RS485_TX1;
			if(system_error == NO_ERR) printf("system_error:NO_ERR\r\n");
			else if(system_error == ERR_SENSOR) printf("system_error:ERR_SENSOR\r\n");
			else if(system_error == ERR_ANGLE) printf("system_error:ERR_ANGLE\r\n");
			else if(system_error == ERR_COMMUN) printf("system_error:ERR_COMMUN\r\n");
			else if(system_error == ERR_SLAVE) printf("system_error:ERR_SLAVE\r\n");
				#endif
			#endif
			#ifdef SLAVE
			//No system_error
			if(sen_err == NO_ERR) 											system_error = NO_ERR;
			//Err 1 : sensor of this lamp got trouble
			else if(sen_err == NO_SEN)									system_error = ERR_SENSOR ;
			// Err 2 : this lamp is wrong angle
			else if(sen_err == ANG_ERR) 								system_error = ERR_ANGLE  ;
			#endif
			switch(time_to_update_7seg)
		{
			
			
			case 0: 
							{
								// Err 1 : sensor of this lamp got trouble
								// Err 2 : this lamp is wrong angle
								// Err 3 : lost connection to the slaves
								// Err 4 : one or more of slaves got trouble
								if (system_error == ERR_SLAVE ) TM1637_Disp_Dot(0x79,0x50,0x50,0x66); //err4
								else if (system_error == ERR_COMMUN) TM1637_Disp_Dot(0x79,0x50,0x50,0x4f); //err3
								else if (system_error == ERR_ANGLE ) TM1637_Disp_Dot(0x79,0x50,0x50,0x5b); //err2
								else if	(system_error == ERR_SENSOR) TM1637_Disp_Dot(0x79,0x50,0x50,0x06); //err1
								else time_to_update_7seg = 2;
								break;
								/*
								if(time_out_lost_signal ==0)TM1637_Disp_Dot(0x79,0x50,0x50,0x06);//err1
								else if((one_of_slave_err == 1) &&(typeOfdevice ==MASTER))  TM1637_Disp_Dot(0x79,0x50,0x50,0x5b); //err2
								else if(system_error) TM1637_Disp_Dot(0,0x79,0x50,0x50);//err
								else time_to_update_7seg = 2;*/
								
								
							}
			case 3: 
							{
								// -- 
								if(deltax<0) seg_temp1 = 0xC0;
								else seg_temp1 = 0x80;
								//TM1637_Disp_Dot(seg_temp1,seg_temp2,seg_temp3,seg_temp4);
								seg_temp2 = abs_deltax/100;
								seg_temp4 = abs_deltax%10;
								seg_temp3 = abs_deltax%100;
								seg_temp3 = seg_temp3/10;
								
								
								TM1637_Disp_Dot(seg_temp1,digitToSegment[seg_temp2],digitToSegment[seg_temp3],digitToSegment[seg_temp4]);
								
								//time_to_update_7seg++;
								break;
							}
			case 7: 
							{
								if(deltay<0) seg_temp1 = 0x40;
								else seg_temp1 = 0;
								//TM1637_Disp_Dot(seg_temp1,seg_temp2,seg_temp3,seg_temp4);
								seg_temp2 = abs_deltay/100;
								seg_temp4 = abs_deltay%10;
								seg_temp3 = abs_deltay%100;
								seg_temp3 = seg_temp3/10;
								TM1637_Disp_Dot(seg_temp1,digitToSegment[seg_temp2],digitToSegment[seg_temp3],digitToSegment[seg_temp4]);
								
								break;
							}
			
		}
		
			time_to_update_7seg++;
			if(time_to_update_7seg > 10) time_to_update_7seg = 0;
		}
		
		
		//if(checku2 ==1) {test1 = 50;checku2=0;}
		//time out for receiver enough data from U1 485
		if(checku1 == ONTIME) {//------------SLAVE, MASTER...
			checku1 = STOP;
			
			//RS485_TX1;
			//printf("U1RX:'%s'\r\n",UART1_DATA);
			
			#ifdef SLAVE
				if((UART1_DATA[0] =='>') && (UART1_DATA[1] == 'M') && (UART1_DATA[5] == (48+idOfSlave)) && (UART1_DATA[6] =='<'))
				{
					RS485_TX1;
					printf(">S0x%02X%02X<",idOfSlave,sen_err);
				}
				memset(UART1_DATA,' ',7);
				HAL_UART_Receive_DMA(&huart1,UART1_DATA,7);
			#endif
				
			#ifdef MASTER
				//check for right data from slave only master--> check for err4
				if((UART1_DATA[0] =='>') && (UART1_DATA[1] == 'S') && (UART1_DATA[8] =='<'))
				{
					temp_id = UART1_DATA[5] - 48;//get id
					slave_com[temp_id] = NO_ERR;// normal connection
					slave_com_timeout[temp_id] = 10;

					if			(UART1_DATA[7] == (NO_ERR			+48)) {
																												slave_error[temp_id] = NO_ERR;
																												//all of slaves must be no err						
																												if((slave_error[0] == NO_ERR) && (slave_error[1] == NO_ERR) && (slave_error[2] == NO_ERR)) sla_err = NO_ERR ;
																											}
					else if	(UART1_DATA[7] == (ERR_SENSOR	+48)) {	slave_error[temp_id] = ERR_SENSOR ;sla_err = ERR_SLAVE ; }//TM1637_Disp_Dot(0x79,0x50,0x50,0x66); time_to_update_7seg = 0;}
					else if	(UART1_DATA[7] == (ERR_ANGLE	+48)) {	slave_error[temp_id] = ERR_ANGLE  ;sla_err = ERR_SLAVE ; }//TM1637_Disp_Dot(0x79,0x50,0x50,0x66); time_to_update_7seg = 0;}
					//else {slave_error[temp_id] = ERR_COMMUN ;error = ERR_SLAVE ;TM1637_Disp_Dot(0x79,0x50,0x50,0x66); time_to_update_7seg = 0;}
					#ifdef DEBUG
					RS485_TX1;
					if(	slave_error[temp_id] == NO_ERR) printf("Slave %d NO_ERR,sla_err:%d\r\n",temp_id,sla_err);
					else if(	slave_error[temp_id] == ERR_SENSOR) printf("Slave %d ERR_SENSOR,sla_err:%d\r\n",temp_id,sla_err);
					else if(	slave_error[temp_id] == ERR_ANGLE) printf("Slave %d ERR_ANGLE,sla_err:%d\r\n",temp_id,sla_err);
					#endif
					//system_error = 1;
					//printf("temp_id:%d!\r\n",temp_id);
					//one_of_slave_err = 1;
					//slave_com[i] = LOST_CON;
					//slave_error[i] = ANG_ERR;
				}
				//>>>> Neu no ko vao day thi uart bi tat????>> phai co cach bat no len???check no neu ko vao day thi tu bat no len
				memset(UART1_DATA,0,9);
				HAL_UART_Receive_DMA(&huart1,UART1_DATA,9);
			#endif	
			
		}
		
		
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM1 init function */
static void MX_TIM1_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
	// making 1s 
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 1124;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 63999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_ENABLE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
	//making 1ms
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 35999;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RS485_2RD_Pin|CRL8_Pin|CRL7_Pin|CRL6_Pin 
                          |RS485_1RD_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, CRL5_Pin|CRL4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, CRL3_Pin|CRL2_Pin|CRL1_Pin|GPIO_PIN_12 
                          |GPIO_PIN_13|RF_NSS_Pin|RF_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BT1_Pin BT2_Pin */
  GPIO_InitStruct.Pin = BT1_Pin|BT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : RS485_2RD_Pin CRL8_Pin CRL7_Pin CRL6_Pin 
                           RS485_1RD_Pin */
  GPIO_InitStruct.Pin = RS485_2RD_Pin|CRL8_Pin|CRL7_Pin|CRL6_Pin 
                          |RS485_1RD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : CRL5_Pin CRL4_Pin */
  GPIO_InitStruct.Pin = CRL5_Pin|CRL4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : CRL3_Pin CRL2_Pin CRL1_Pin PB12 
                           PB13 RF_NSS_Pin RF_RST_Pin */
  GPIO_InitStruct.Pin = CRL3_Pin|CRL2_Pin|CRL1_Pin|GPIO_PIN_12 
                          |GPIO_PIN_13|RF_NSS_Pin|RF_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim3.Instance )//1ms
	{
		//HAL_GPIO_TogglePin(GPIOB,CRL3_Pin);
		bt_scan++;
		t1++;t2++;
		if(checku2 > ONTIME ) checku2--;//rx2 time out
		if(checku1 > ONTIME ) checku1--;//rx1 time out 
		if(tx1_count > ONTIME) tx1_count--;//control tx1 rs485
	}
	if(htim->Instance == htim1.Instance )//1s
	{
		//HAL_GPIO_TogglePin(GPIOB,CRL2_Pin);
		if(time_to_check_slave > ONTIME) time_to_check_slave--;
		if(time_to_check_sensor > ONTIME) time_to_check_sensor--;
		if(flag_check_7seg > ONTIME) flag_check_7seg--;
		 
		//time_to_update_7seg ++;
	}
}
// Boc tach du lieu tu cam bien gui ve
void find_data(void)
{
	uint8_t temp1,temp2,temp3;
	

	if((UART_DATA[0] == 'S') &&(UART_DATA[18] == '<'))
	{
		time_out_lost_signal = 5;//<--------------------------------------------------------------
		
		temp1 = HEXInStringToDec(UART_DATA[2]);
		temp2 = HEXInStringToDec(UART_DATA[3]);
		temp3 = HEXInStringToDec(UART_DATA[4]);
		rawx = (temp1<<8) + (temp2<<4) + temp3;

		temp1 = HEXInStringToDec(UART_DATA[6]);
		temp2 = HEXInStringToDec(UART_DATA[7]);
		temp3 = HEXInStringToDec(UART_DATA[8]);
		rawy = (temp1<<8) + (temp2<<4) + temp3;

		temp1 = HEXInStringToDec(UART_DATA[10]);
		temp2 = HEXInStringToDec(UART_DATA[11]);
		buff_temp1 = (temp1<<4) + temp2;

		temp1 = HEXInStringToDec(UART_DATA[13]);
		temp2 = HEXInStringToDec(UART_DATA[14]);
		buff_temp2 = (temp1<<4) + temp2;

		temp1 = HEXInStringToDec(UART_DATA[16]);
		temp2 = HEXInStringToDec(UART_DATA[17]);
		lux = (temp1<<4) + temp2;
		
		temp_ds1820 = ((float)(buff_temp1&0x0f))/16;            // Lay phan thuc cua gia tri nhiet 
		buff_temp1 =((buff_temp1&0xf0)>>4)|((buff_temp2&0x0f)<<4) ;    // Lay phan nguyen cua gia tri nhiet do
		
		temp_ds1820=temp_ds1820+buff_temp1;
	}

}
unsigned char HEXInStringToDec(unsigned char data)
      {
            if((data>47)&&(data<58)) return (data-48);// 0...9
            else if((data>63)&&(data<71)) return (data-55);//A..F
						else return 0;
      }
			
void check_angle_err(void)
{
	if((offsetrawx - rawx) > 7 )      sen_err = ANG_ERR;// goc x nho hon -0.25 do
	else if((rawx - offsetrawx) > 14) sen_err = ANG_ERR; //goc x lon hon 0.5 do
	else if((offsetrawy - rawy) > 7)  sen_err = ANG_ERR; //goc y nho hon -0.25 do
	else if((rawy - offsetrawy) > 14) sen_err = ANG_ERR; //goc y lon hon 0.5 do
	else sen_err = NO_ERR;
}

void slave_stt_init(void)
{
	char i;
	for(i=0;i<totalOfSlave+1;i++)
	{
		slave_com[i] 					= NO_ERR;	//Ket noi binh thuong
		slave_error[i] 				= NO_ERR;	// goc nghieng binh thong
		slave_com_timeout[i] 	= 5;			// timeout ket noi
	}
	//RS485_TX1;
	//printf("slave_com_timeout:%d-%d-%d\r\n",slave_com_timeout[0],slave_com_timeout[1],slave_com_timeout[2]);
	//printf("slave_error:%d-%d-%d\r\n",slave_error[0],slave_error[1],slave_error[2]);
	//printf("slave_com:%d-%d-%d\r\n",slave_com[0],slave_com[1],slave_com[2]);
}


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);
	tx1_count = 3;
	return ch;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
