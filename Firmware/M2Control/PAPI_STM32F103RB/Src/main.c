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
  * COPYRIGHT(c) 2018 STMicroelectronics
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
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#include "tm1637.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/


/*****************************************************************************************************************/
unsigned char message[10]="ENJOYNG!!";              // outgoing message
//unsigned char outgoing_length = 20;
unsigned char msgCount = 0;            // count of outgoing messages
unsigned char localAddress = 0x71;     // address of this device
unsigned char destination = 0xFF;      // destination to send to
//long lastSendTime = 0;        // last send time
//int interval = 2000;          // interval between sends
unsigned char _packetIndex;
#pragma used+
unsigned char read_reg_single(unsigned char addr);
void write_reg_single(unsigned char addr,unsigned char data);
void read_all_mem(unsigned char *mem_buff);
#pragma used-
unsigned char rfm98w_begin(unsigned int frequency);
void rf_sleep(void);
void setFrequency(unsigned int frequency);
unsigned int _frequency;
void setTxPower(unsigned char level);
void idle();
//void sendMessage(char *outgoing);
int LoRa_beginPacket(void);
//void explicitHeaderMode();
//unsigned char _implicitHeaderMode;
//unsigned char LoRa_write( unsigned char *buffer,unsigned char size);
unsigned char LoRa_endPacket();

void sendmess(unsigned char *buff);
void LoRa_AddDataToSend(unsigned char des,unsigned char add,unsigned char count, unsigned char *buffer);
unsigned char LoRa_parsePacket(void);
void onReceive_(unsigned char packetSize);
unsigned char LoRa_available(void);
unsigned char LoRa_read(void);
/*****************************************************************************************************************/
//using 1 triger for some loops
#define LOOPSAME 200// 200 mean 200ms
uint16_t _loop_same = LOOPSAME;
uint32_t wait_for_cali_;
uint8_t _stt_lamp_ = 0;//LAMP_OK

const uint8_t _papi_id = PAPI_MASTER;
//const uint8_t _papi_id = PAPI_SLAVE1;

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



struct _s_sensor_ {
	int rawx;
	int rawy;
	int offset_x;
	int offset_y;
	int diff_x;
	int diff_y;
	int abs_diff_x;
	int abs_diff_y;
	uint16_t raw_lux;
	uint16_t ds18b20_temp;
};
struct _s_sensor_ ss1;

void _send_papi_info_(void){
	RS485_TX1;
	printf(">P%02X%02X<",_papi_id,_stt_lamp_);
}

uint8_t UART2_DATA[RX2SIZE];

// Cac trang thai cua cam bien
#define SEN_OK 0//Cam bien binh thuong, OK
#define SEN_WO 1//Cam bien dang bi lech goc WRONG
#define SEN_DI 2//Cam bien chet DIE
#define SEN_FI 3//Cam bien den chet FIRE
#define SEN_HO 4//Cam bien qua nhiet HOT
//for _loop4_check_sensor_
//Lost sensor : Uart error, sensor error
//if in 20 seconds has no right sensor massage => ERROR
#define LOOOP4RUN 20//20 for 20s
uint8_t _loop4 = LOOOP4RUN;
uint8_t _stt_sensor_ = SEN_OK;

// gia lap gia tri cam bien
#define EMU_ON  1
#define EMU_OFF 0
uint8_t _emu_mode_ = EMU_OFF;
uint8_t _emu_value_ = SEN_OK;

void _loop4_check_sensor_(void)
{
	if(_loop4 == ONTIME){// sau 1 phut ko nhan duoc du lieu tu cam bien
		_loop4 = STOP;
		_stt_sensor_ = SEN_DI; 
		//RS485_TX1;
		//printf("SENSOR DIED!!\r\n");
	}
	else if((_loop_same == ONTIME) &&(_loop4 != STOP)){//Khoang 200ms (loop same) vao day 1 lan
		//Check sensor dinh ky
		//_loop4 != STOP =>>> Phai nhan duoc ban tin tu cam bien thi moi xu ly
		//kiem tra xem du lieu cam bien dung hay sai <-----check co kiem tra anh sang
		//KIEM TRA ANH SANG O DAY
		//RS485_TX1;
		//if(ss1.diff_x>0x7FFF) ss1.diff_x = 0;
		//if(ss1.diff_y>0x7FFF) ss1.diff_y = 0;//ss1.diff_y-0x10000;
		//if(ss1.diff_x>0x7FFF) 
		//ss1.diff_x = (signed int)0xffff;
		//printf("X,Y,t,l :%d,%d,%d\r\n",ss1.diff_x,ss1.rawy,ss1.ds18b20_temp);
		
		//ss1.diff_x = ss1.rawx - ss1.offset_x;
		//ss1.diff_y = ss1.rawy - ss1.offset_y;
		
		ss1.diff_x = ss1.rawx;
		ss1.diff_y = ss1.rawy;
		
		if(ss1.diff_x > 0) ss1.abs_diff_x = ss1.diff_x;
		else ss1.abs_diff_x = -ss1.diff_x;
		
		if(ss1.diff_y > 0) ss1.abs_diff_y = ss1.diff_y;
		else ss1.abs_diff_y = -ss1.diff_y;
		//Doi sang do
		ss1.abs_diff_x = 7*ss1.abs_diff_x;
		ss1.abs_diff_y = 7*ss1.abs_diff_y;
		/*if((ss1.offset_x - ss1.rawx) > 7 )      _stt_sensor_ = SEN_WO;// goc x nho hon -0.25 do
		else if((ss1.rawx - ss1.offset_x) > 14) _stt_sensor_ = SEN_WO;  //goc x lon hon 0.5 do
		else if((ss1.offset_y - ss1.rawy) > 7)  _stt_sensor_ = SEN_WO; //goc y nho hon -0.25 do
		else if((ss1.rawy - ss1.offset_y) > 14) _stt_sensor_ = SEN_WO; //goc y lon hon 0.5 do
		else _stt_sensor_ = SEN_OK;*/
		//if(((ss1.offset_x - ss1.rawx) > 7 ) || ((ss1.rawx - ss1.offset_x) > 14) ||((ss1.offset_y - ss1.rawy) > 7)||((ss1.rawy - ss1.offset_y) > 14))
		if((ss1.abs_diff_x >249))//||(ss1.abs_diff_y >250))//0.25 do
		{
			_stt_sensor_ = SEN_WO;
			//RS485_TX1;
			//printf("SENSOR WRONG\r\n");
		}
		else if(ss1.raw_lux >710) _stt_sensor_ = SEN_FI;//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<Thêm quá nhi?t ? dây
		else if(ss1.ds18b20_temp >850) {
			_stt_sensor_ = SEN_HO;// Qua 85 do thi bao loi qua nhiet
			//RS485_TX1;
			//printf("SENSOR HOT\r\n");
			}
		else //<<check lai dieu kien nay nhe
		{
			_stt_sensor_ = SEN_OK;
			//RS485_TX1;
			//printf("SENSOR OK\r\n");
		}
		
	}
	if(_emu_mode_ == EMU_ON) {
		_stt_sensor_ = _emu_value_;
	}
}

// Boc tach du lieu tu cam bien gui ve
void _get_sensor_data(void)
{
	uint8_t temp1,temp2,temp3,temp4;
	uint8_t buff_temp1,buff_temp2;//ds18b20
  signed int x,y;
	uint16_t t,l;
	
	//RS485_TX1;
	//printf("SENSOR :%s\r\n",UART2_DATA);
	
	RS485_TX2;
	if(wait_for_cali_ == 1)
		{
			wait_for_cali_ = 0;
			HAL_UART_Transmit(&huart2, (uint8_t *)"CALI", 4, 10);
		}
	RS485_RX2;
	if((UART2_DATA[0] == 'S') &&(UART2_DATA[19] == '<'))
	{
		_loop4 = LOOOP4RUN;//Nhan duoc dung du lieu tu cam bien => Cam bien OK
		//RS485_TX1;
		//printf("SENSOR :%s\r\n",UART2_DATA);
		temp1 = HEXInStringToDec(UART2_DATA[2]);
		temp2 = HEXInStringToDec(UART2_DATA[3]);
		temp3 = HEXInStringToDec(UART2_DATA[4]);
		temp4 = HEXInStringToDec(UART2_DATA[5]);
		x= (temp1<<12) + (temp2<<8) + (temp3<<4) +temp4;
		
		temp1 = HEXInStringToDec(UART2_DATA[7]);
		temp2 = HEXInStringToDec(UART2_DATA[8]);
		temp3 = HEXInStringToDec(UART2_DATA[9]);
		temp4 = HEXInStringToDec(UART2_DATA[10]);
		y= (temp1<<12) + (temp2<<8) + (temp3<<4) +temp4;
		
		temp2 = HEXInStringToDec(UART2_DATA[12]);
		temp3 = HEXInStringToDec(UART2_DATA[13]);
		temp4 = HEXInStringToDec(UART2_DATA[14]);
		t= (temp2<<8) + (temp3<<4) +temp4;
		
		temp2 = HEXInStringToDec(UART2_DATA[16]);
		temp3 = HEXInStringToDec(UART2_DATA[17]);
		temp4 = HEXInStringToDec(UART2_DATA[18]);
		l= (temp2<<8) + (temp3<<4) +temp4;
		
		//RS485_TX1;
		//printf("X,Y,t,l :%04X,%04X,%d,%d\r\n",x,y,t,l);
		
		ss1.rawx = x;
		ss1.rawy = y;
		ss1.ds18b20_temp = t;
		ss1.raw_lux = l;
		if(ss1.rawx>0x7FFF) ss1.rawx = ss1.rawx - 0xffff - 1;
		if(ss1.rawy>0x7FFF) ss1.rawy = ss1.rawy - 0xffff - 1;
		//RS485_TX1;
		//printf("X,Y,t,l :%04X,%04X,%d,%d\r\n",ss1.rawx,ss1.rawy,ss1.ds18b20_temp,l);
		//printf("X,Y,t,l :%d,%d,%d,%d\r\n",ss1.rawx,ss1.rawy,ss1.ds18b20_temp,l);
		/*
		temp1 = HEXInStringToDec(UART2_DATA[2]);
		temp2 = HEXInStringToDec(UART2_DATA[3]);
		temp3 = HEXInStringToDec(UART2_DATA[4]);
		ss1.rawx = (temp1<<8) + (temp2<<4) + temp3;

		temp1 = HEXInStringToDec(UART2_DATA[6]);
		temp2 = HEXInStringToDec(UART2_DATA[7]);
		temp3 = HEXInStringToDec(UART2_DATA[8]);
		ss1.rawy = (temp1<<8) + (temp2<<4) + temp3;

		temp1 = HEXInStringToDec(UART2_DATA[10]);
		temp2 = HEXInStringToDec(UART2_DATA[11]);
		buff_temp1 = (temp1<<4) + temp2;

		temp1 = HEXInStringToDec(UART2_DATA[13]);
		temp2 = HEXInStringToDec(UART2_DATA[14]);
		buff_temp2 = (temp1<<4) + temp2;

		temp1 = HEXInStringToDec(UART2_DATA[16]);
		temp2 = HEXInStringToDec(UART2_DATA[17]);
		ss1.raw_lux = (temp1<<4) + temp2;
		*/
		//ss1.ds18b20_temp = ((float)(buff_temp1&0x0f))/16;            // Lay phan thuc cua gia tri nhiet //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		//buff_temp1 =((buff_temp1&0xf0)>>4)|((buff_temp2&0x0f)<<4) ;    // Lay phan nguyen cua gia tri nhiet do
		//ss1.ds18b20_temp = ss1.ds18b20_temp+buff_temp1;
	}
	

}

unsigned int bt_scan;
unsigned int check;
//for _loop1_tx1off_
unsigned char _loop1;
unsigned char _loop1_run = 25;// 25 mean 25ms
//processing loop1, turn uart 1 to rx mode.
void _loop1_tx1off_(void){
	if(_loop1 == ONTIME) {
		_loop1 = STOP;
		RS485_RX1;
	}
}

void _clear_mem(uint8_t *pData, uint8_t _size_)
{
	uint8_t i;
	for(i=0;i<_size_;i++)
	{
		pData[i] = 0;
	}
}

//for _loop2_rx1_process_
unsigned int _loop2;
uint8_t UART1_DATA[RX1SIZE];
//tu kiem tra ket noi cua ban than
uint8_t _my_conn = 0;
#define CONN_LIMIT 10// 10 means 10 times without any incoming message

// trang thai cua cac den papi trong he thong papi
uint8_t papi_info[4];
//khoang thoi gian timeout cua ban thong tin
#define F5TIMEOUT 20// 20 means 10 seconds
// do moi cua cac ban tin
uint8_t f5_papi_info[4];
//incoming message from master
uint8_t flag_incom_master = 0;
// Lay thong tin tu cac den PAPI khac
void _get_uart_info (void){
	if((UART1_DATA[0] == '>') && (UART1_DATA[1] == 'P') && (UART1_DATA[6] == '<'))
		{
			_my_conn = 0;//minh ket noi binh thuong;
			if( (UART1_DATA[3] -48) == PAPI_MASTER)  {papi_info[PAPI_MASTER] = (UART1_DATA[5] -48);f5_papi_info[PAPI_MASTER] = 0;flag_incom_master = 1;}
			if( (UART1_DATA[3] -48) == PAPI_SLAVE1)  {papi_info[PAPI_SLAVE1] = (UART1_DATA[5] -48);f5_papi_info[PAPI_SLAVE1] = 0;}
			if( (UART1_DATA[3] -48) == PAPI_SLAVE2)  {papi_info[PAPI_SLAVE2] = (UART1_DATA[5] -48);f5_papi_info[PAPI_SLAVE2] = 0;}
			if( (UART1_DATA[3] -48) == PAPI_SLAVE3)  {papi_info[PAPI_SLAVE3] = (UART1_DATA[5] -48);f5_papi_info[PAPI_SLAVE3] = 0;}
		}
}

void _loop2_rx1_process_(void)
{
	if(_loop2 == ONTIME){
		_loop2 = STOP;
		_get_uart_info();
		//Xu ly no
		//RS485_TX1;
		//printf("UART1:%s,RxSize :%d,RxCount:%d\r\n",UART1_DATA,huart1.RxXferSize,huart1.RxXferCount);
		huart1.RxXferCount = RX1SIZE;
		HAL_UART_Receive_IT(&huart1,UART1_DATA,RX1SIZE);
		huart1.pRxBuffPtr = UART1_DATA;
		_clear_mem(UART1_DATA,RX1SIZE-1);
	}
}





//for _loop3_rx2_process_
//Xu ly du lieu cam bien
unsigned int _loop3;
void _loop3_rx2_process_(void)
{
	if(_loop3 == ONTIME){
		_loop3 = STOP;
		//Xu ly no
		
		_get_sensor_data();
		//RS485_TX1;
		//printf("UART2:%s\r\n",UART2_DATA);//<< check raw data uart
		//printf("UART2:%s,RxSize :%d,RxCount:%d\r\n",UART2_DATA,huart2.RxXferSize,huart2.RxXferCount);
		//printf("Sensor:X:%d,Y:%d,L:%d\r\n",ss1.rawx,ss1.rawy,ss1.raw_lux);
		huart2.RxXferCount = RX2SIZE;
		HAL_UART_Receive_IT(&huart2,UART2_DATA,RX2SIZE);
		huart2.pRxBuffPtr = UART2_DATA;
		_clear_mem(UART2_DATA,RX1SIZE-1);
	}
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_NVIC_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
//for _loop5_button_
#define LOOOP5RUN 200// 200 mean 200ms
unsigned char _loop5 = LOOOP5RUN;
//processing loop5, check button
void _loop5_button_(void){
	if(_loop_same == ONTIME) {
		//_loop_same = LOOPSAME;// renew at the end loop
		//check button
		if(!HAL_GPIO_ReadPin(BT1_GPIO_Port,BT1_Pin)) {/*RS485_TX1;printf("BT1\r\n");*/
																										wait_for_cali_ = 1;
																										//TM1637_Disp_Dot(0x39,0x77,0x38,0x30);//CALI
																										//while(wait_for_cali_--);
																										//ss1.offset_x = ss1.rawx; 
																										//ss1.offset_y = ss1.rawy;
																										//TM1637_Disp_Dot(0,0x40,0x40,0);
		//wait_for_cali_ = 1000;																							
		//while(wait_for_cali_--);
		}
		if(!HAL_GPIO_ReadPin(BT2_GPIO_Port,BT2_Pin)) {/*RS485_TX1;printf("BT2\r\n");*/_emu_value_++;if(_emu_value_ == 5) _emu_value_ = SEN_OK;}
	}
}
//for _loop6_lampcheck_
#define LAMP_OK           0
#define LAMP_ERR1_SENSOR  1
#define LAMP_ERR2_FIRED   2
#define LAMP_ERR3_INCLIN  3
#define LAMP_ERR9_HOT     4

#define LOOOP6RUN 200// 200 mean 200ms
unsigned char _loop6 = LOOOP6RUN;
//uint8_t _stt_lamp_ = LAMP_OK;
//processing loop6, check sensor for lamp's error
void _loop6_lampcheck_(void){
	if(_loop_same == ONTIME) {
		if			(_stt_sensor_ == SEN_OK) _stt_lamp_ = LAMP_OK;//Den binh thuong
		else if	(_stt_sensor_ == SEN_WO) _stt_lamp_ = LAMP_ERR3_INCLIN;//Den bi nghieng
		else if	(_stt_sensor_ == SEN_DI) _stt_lamp_ = LAMP_ERR1_SENSOR;//Ko co phan hoi tu cam bien
		else if	(_stt_sensor_ == SEN_FI) _stt_lamp_ = LAMP_ERR2_FIRED;//Bong den bi chay
		else if	(_stt_sensor_ == SEN_HO) _stt_lamp_ = LAMP_ERR9_HOT;//Cam bien bi nong qua 85oC
		//RS485_TX1;
		//if			(_stt_lamp_ == LAMP_OK) 					printf("PAPI's status :LAMP_OK\r\n");//Den binh thuong
		//else if	(_stt_lamp_ == LAMP_ERR3_INCLIN) 	printf("PAPI's status :LAMP_ERR3_INCLIN\r\n");//Den bi nghieng
		//else if	(_stt_lamp_ == LAMP_ERR1_SENSOR) 	printf("PAPI's status :LAMP_ERR1_SENSOR\r\n");//Ko co phan hoi tu cam bien
		//else if	(_stt_lamp_ == LAMP_ERR2_FIRED) 	printf("PAPI's status :LAMP_ERR2_FIRED\r\n");//Bong den bi chay
		f5_papi_info[_papi_id] = 0;
		papi_info[_papi_id] = _stt_lamp_;
		//check lamp
	}
}
//Kiem tra ket noi den slave hoac master co on dinh hay ko?
//Cac trang thai
#define COM_GOOOD 0
#define COM_BAD   1
uint8_t _stt_comm_ = COM_GOOOD;
void _loop7_communication_(void){
	if(_loop_same == ONTIME) {
		if(_my_conn == 0) _stt_comm_ = COM_GOOOD;
		else if(_my_conn == CONN_LIMIT) _stt_comm_ = COM_BAD;
		
		if(f5_papi_info[PAPI_MASTER] >= F5TIMEOUT) papi_info[PAPI_MASTER] = 'x';//x means dont know
		if(f5_papi_info[PAPI_SLAVE1] >= F5TIMEOUT) papi_info[PAPI_SLAVE1] = 'x';//x means dont know
		if(f5_papi_info[PAPI_SLAVE2] >= F5TIMEOUT) papi_info[PAPI_SLAVE2] = 'x';//x means dont know
		if(f5_papi_info[PAPI_SLAVE3] >= F5TIMEOUT) papi_info[PAPI_SLAVE3] = 'x';//x means dont know
		
		//RS485_TX1;
		//if(_stt_comm_ == COM_GOOOD) printf("COM_GOOOD\r\n");
		//else if(_stt_comm_ == COM_BAD) printf("COM_BAD\r\n");
		
	}
}
//Kiem tra cac den khac co loi ko
//Cac trang thai
#define NEIGH_GOOD 0
#define NEIGH_BAD  1
uint8_t _stt_neigh_ = NEIGH_GOOD;
void _loop8_neigh_(void){
	if(_loop_same == ONTIME) {
		//_stt_neigh_ = NEIGH_GOOD;
		if((papi_info[0]+papi_info[1]+papi_info[2]+papi_info[3]) == 0) _stt_neigh_ = NEIGH_GOOD;//including me
		else _stt_neigh_ = NEIGH_BAD;
		//RS485_TX1;
		//if(_stt_neigh_ == NEIGH_GOOD) printf("NEIGH_GOOD\r\n");
		//else if(_stt_neigh_ == NEIGH_BAD) printf("NEIGH_BAD\r\n");
		
	}
}
//INPUT: LOCAL ERROR & COMM ERROR & NEIGH ERROR
//OUTPUT:GOOOD
#define SYSTEM_GOOOD 0
//LOCAL ERROR : 
#define ERR1_SEN     1 // Cam bien cua PAPI gap su co
#define ERR2_LAM     2 // lamp cua PAPI gap su co
#define ERR3_INC     3 // PAPI's sensor is inclined 
#define ERR9_HOT     9 // PAPI's sensor is hot
//COMM ERROR
#define ERR4_COM     4 // Communication got trouble
//NEIGH ERROR
#define ERR5_NEI     5 // One of neighbors got trouble
uint8_t system_status = SYSTEM_GOOOD;
void _loop9_systemcheck_(void){
	
	if(_loop_same == ONTIME) {
		_loop_same = LOOPSAME;//ket thuc cac vong lap tuong tu nhau
		
		if			(_stt_lamp_ == LAMP_ERR1_SENSOR) system_status = ERR1_SEN;
		else if	(_stt_lamp_ == LAMP_ERR2_FIRED ) system_status = ERR2_LAM;
		else if	(_stt_lamp_ == LAMP_ERR3_INCLIN) system_status = ERR3_INC;
		else if	(_stt_lamp_ == LAMP_ERR9_HOT)    system_status = ERR9_HOT;
		else if	(_stt_comm_ == COM_BAD         ) system_status = ERR4_COM;
		else if	(_stt_neigh_ == NEIGH_BAD      ) system_status = ERR5_NEI;
		else 																		 system_status = SYSTEM_GOOOD;
		/*
		RS485_TX1;
		if(system_status == ERR1_SEN) 		 			printf("ERR1_SEN\r\n");
		else if(system_status == ERR2_LAM) 			printf("ERR2_LAM\r\n");
		else if(system_status == ERR3_INC) 			printf("ERR3_INC\r\n");
		else if(system_status == ERR4_COM) 			printf("ERR4_COM\r\n");
		else if(system_status == ERR5_NEI) 			printf("ERR5_NEI\r\n");
		else if(system_status == SYSTEM_GOOOD) 	printf("SYSTEM_GOOOD\r\n");
		*/
		//printf("papi_info    :%d-%d-%d-%d\r\n",papi_info[0],papi_info[1],papi_info[2],papi_info[3]);
		//printf("f5_papi_info :%d-%d-%d-%d\r\n",f5_papi_info[0],f5_papi_info[1],f5_papi_info[2],f5_papi_info[3]);
	}
}



//for _loop_master_send_
#define LOOPMASTER 3000// 1000 mean 1000ms
uint16_t _loopm = LOOPMASTER;
//processing loop master, send papi's status to slaves
void _loop_master_send_(void){
	if(_papi_id == PAPI_MASTER){
	if(_loopm == ONTIME) {
		_loopm = LOOPMASTER;
		_send_papi_info_();
	}
	}
}

// phan hoi ban tin tu master
uint16_t _slave_respond ;
uint16_t _slave_time_to_rep;
void _slave_rep_(void)
{
	if(flag_incom_master == 1)// master hoi, tra loi thoi
	{
		flag_incom_master = 0;
		if( _papi_id == PAPI_SLAVE1) _slave_time_to_rep = 300;//300ms
		else if( _papi_id == PAPI_SLAVE2) _slave_time_to_rep = 600;//600ms
		else if( _papi_id == PAPI_SLAVE3) _slave_time_to_rep = 900;//900ms
	}
}
// slave gui ban tin
void _slave_send_(void)
{
	if(_slave_time_to_rep == ONTIME)
	{
		_slave_time_to_rep = STOP;
		_send_papi_info_();
	}
}
#define LOOP10RUN 3
uint8_t _loop10_ = LOOP10RUN;
uint8_t _display_label_ = 0;
uint8_t _display_conten[4] ;

unsigned char seg_temp1,seg_temp2,seg_temp3,seg_temp4;	

void _error_display_(void);
void _loop10_led7out_(void){
	if(_loop10_ == ONTIME){
		_loop10_ = LOOP10RUN;
		//RS485_TX1;
	
		switch(_display_label_)
		{
			
			
		case 0: {// Neu co loi, hien thi ma loi o day. Neu ko co loi, bo qua
					_error_display_();
					//printf("Label 0\r\n");
					break;
				}
				case 1: {
									//printf("abs_diff_x:%d\r\n",ss1.abs_diff_x);
								if(ss1.diff_x<0) seg_temp1 = 0xC0;
								else seg_temp1 = 0x80;
								//TM1637_Disp_Dot(seg_temp1,seg_temp2,seg_temp3,seg_temp4);
								seg_temp2 = ss1.abs_diff_x/100;
								seg_temp4 = ss1.abs_diff_x%10;
								seg_temp3 = ss1.abs_diff_x%100;
								seg_temp3 = seg_temp3/10;
								
								
								TM1637_Disp_Dot(seg_temp1,digitToSegment[seg_temp2],digitToSegment[seg_temp3],digitToSegment[seg_temp4]);
								
									break;
								}
				case 2: {/*//Turn off Y display
								if(ss1.diff_y<0) seg_temp1 = 0xC0;
								else seg_temp1 = 0x80;
								//TM1637_Disp_Dot(seg_temp1,seg_temp2,seg_temp3,seg_temp4);
								seg_temp2 = ss1.abs_diff_y/100;
								seg_temp4 = ss1.abs_diff_y%10;
								seg_temp3 = ss1.abs_diff_y%100;
								seg_temp3 = seg_temp3/10;
								
								
								TM1637_Disp_Dot(seg_temp1,digitToSegment[seg_temp2],digitToSegment[seg_temp3],digitToSegment[seg_temp4]);
								//printf("abs_diff_y:%d\r\n",ss1.abs_diff_y);
					*/
								break;
				}
		case 3: {printf("Label 3\r\n");break;}
		case 4: {printf("Label 4\r\n");break;}
		case 5: {printf("Label 5\r\n");break;}
		case 6: {printf("Label 6\r\n");break;}
		case 7: {printf("Label 7\r\n");break;}
		case 8: {printf("Label 8\r\n");break;}
		case 9: {printf("Label 9\r\n");break;}
		case 10: {printf("Label 10\r\n");break;}
		}
		_display_label_++;
		if(_display_label_>2) _display_label_ = 0;
	}
	
	
}

#define SYS_NOMAL 0
#define SYS_ERROR 10
uint8_t _loop10_flag_err = SYS_NOMAL;
void _error_display_(void){
		
		if(system_status != SYSTEM_GOOOD)// Neu ko co loi cuc bo => hien thi loi he thong
			{
				//RS485_TX1;
			if(system_status == ERR1_SEN) 		 			{TM1637_Disp_Dot(0x79,0x50,0x50,0x06);ERR1;}//RS485_TX1;printf("ERR1\r\n");}//printf("ERR1_SEN\r\n");
			else if(system_status == ERR2_LAM) 			{TM1637_Disp_Dot(0x79,0x50,0x50,0x5b);ERR2;}//RS485_TX1;printf("ERR2\r\n");}//printf("ERR2_LAM\r\n");
			else if(system_status == ERR3_INC) 			{TM1637_Disp_Dot(0x79,0x50,0x50,0x4f);ERR3;}//RS485_TX1;printf("ERR3\r\n");}//printf("ERR3_INC\r\n");
			else if(system_status == ERR4_COM) 			{TM1637_Disp_Dot(0x79,0x50,0x50,0x66);ERR4;}//RS485_TX1;printf("ERR4\r\n");}//printf("ERR4_COM\r\n");
			else if(system_status == ERR5_NEI) 			{TM1637_Disp_Dot(0x79,0x50,0x50,0x6d);ERR5;}//RS485_TX1;printf("ERR5\r\n");}//printf("ERR5_NEI\r\n");
			else if(system_status == ERR9_HOT) 			{TM1637_Disp_Dot(0x79,0x50,0x50,0x6f);ERR9;}//RS485_TX1;printf("ERR9\r\n");}//printf("ERR9_HOT\r\n");
			
				if(_loop10_flag_err < SYS_ERROR) _loop10_flag_err++;
				else if(_loop10_flag_err == SYS_ERROR) 
				{
					//RS485_TX1;printf("LAMP_OFF\r\n");
					//printf("LAMP_OFF\r\n");
					LAMP_OFF;
					//gui ban tin bao loi qua rf
					sendmess(message);
				}
			}
			else if(_stt_lamp_ != LAMP_OK) //Neu loi cuc bo, hien thi loi cuc bo
			{
				//RS485_TX1;
				if	(_stt_lamp_ == LAMP_ERR3_INCLIN) 	TM1637_Disp_Dot(0x79,0x50,0x50,0x7d);//printf("PAPI's status :LAMP_ERR3_INCLIN\r\n");//Den bi nghieng
				else if	(_stt_lamp_ == LAMP_ERR1_SENSOR) 	TM1637_Disp_Dot(0x79,0x50,0x50,0x07);//printf("PAPI's status :LAMP_ERR1_SENSOR\r\n");//Ko co phan hoi tu cam bien
				else if	(_stt_lamp_ == LAMP_ERR2_FIRED) 	TM1637_Disp_Dot(0x79,0x50,0x50,0x7f);//printf("PAPI's status :LAMP_ERR2_FIRED\r\n");//Bong den bi chay
				if(_loop10_flag_err < SYS_ERROR) _loop10_flag_err++;
				else if(_loop10_flag_err == SYS_ERROR) 
					{
					//RS485_TX1;
					//printf("LAMP_OFF\r\n");
					LAMP_OFF;
						//gui ban tin bao loi qua rf
						sendmess(message);
					}
				
			}
		else {
				//He thong hoat dong binh thuong
				_loop10_ = ONTIME;
				LAMP_ON;
				_loop10_flag_err = SYS_NOMAL;
				DRY_NO_ERR;
				//RS485_TX1;
					//printf("LAMP_ON\r\n");
			}
}
/************************************************************************************************************/

char LoRa_packetRssi(void)
{
  //return (readRegister(REG_PKT_RSSI_VALUE) - (_frequency < 868E6 ? 164 : 157));
  //return (read_reg_single(REG_PKT_RSSI_VALUE) - 164 );
  return read_reg_single(REG_PKT_RSSI_VALUE);
}

unsigned char LoRa_packetSnr(void)
{
  //return ((int8_t)read_reg_single(REG_PKT_SNR_VALUE)) * 0.25;
  return read_reg_single(REG_PKT_SNR_VALUE) ;
}
uint8_t singleTransfer(uint8_t address, uint8_t value)
{
  uint8_t response;

  //digitalWrite(_ss, LOW);
  
  RF_NSS(0);// = 0;
  HAL_Delay(1);
  //SPI.beginTransaction(_spiSettings);
  //SPI.transfer(address);
  //response = SPI.transfer(value);
  //spi(address);
	HAL_SPI_Transmit(&hspi1,&address,1,100);
  HAL_SPI_TransmitReceive(&hspi1,&value,&response,1,100);
  //SPI.endTransaction();

  //digitalWrite(_ss, HIGH);
  HAL_Delay(1);
  RF_NSS(1);
  return response;
}
uint8_t LoRa_readRegister(uint8_t address)
{
  return singleTransfer(address & 0x7f, 0x00);
}

void LoRa_writeRegister(uint8_t address, uint8_t value)
{
  singleTransfer(address | 0x80, value);
}
unsigned char read_reg_single(unsigned char addr)
{
//unsigned char data=0;
//addr = addr &0x7F;//read only
//RF_NSS(0);
//HAL_SPI_Transmit(&hspi1,&addr,1,100);
//HAL_SPI_TransmitReceive(&hspi1,0,&data,1,100);
//RF_NSS(1);
//return data;
	
	return singleTransfer(addr & 0x7f, 0x00);
}

void write_reg_single(unsigned char addr,unsigned char data)
{
addr = addr | 0x80;//write only
RF_NSS(0);
HAL_SPI_Transmit(&hspi1,&addr,1,100);
HAL_SPI_Transmit(&hspi1,&data,1,100);
RF_NSS(1);
}

void onReceive_(unsigned char packetSize) {
  
  // read packet header bytes:
  unsigned char recipient;          // recipient address
  unsigned char sender;            // sender address
  unsigned char incomingMsgId;     // incoming msg ID
  unsigned char incomingLength;    // incoming msg length
  char incoming[20];
  unsigned char length_of_incoming=0;                                                   
  
  if (packetSize == 0) return;          // if there's no packet, return
  recipient = LoRa_read();
  sender = LoRa_read();
  incomingMsgId = LoRa_read();
  incomingLength = LoRa_read();
  
  while (LoRa_available()) {
    incoming[length_of_incoming++] = LoRa_read();
  }

  if (incomingLength != length_of_incoming) {   // check length for error
    RS485_TX1;
    printf("error: message length does not match length\r\n");
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    RS485_TX1;
    printf("This message is not for me.\r\n");
    return;                             // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
  RS485_TX1;
  printf("Received from: 0x%x\r\n",sender);
  printf("Sent to: 0x%x\r\n",recipient);
  printf("Message ID: %d\r\n",incomingMsgId);
  printf("Message length: %d\r\n",incomingLength);
  printf("Message: %s\r\n",incoming);
  printf("RSSI: %d dBm\r\n",LoRa_packetRssi()-164);
  printf("Snr: %d/4\r\n",LoRa_packetSnr());

}
unsigned char LoRa_available(void)
{
  return (read_reg_single(REG_RX_NB_BYTES) - _packetIndex);
}

unsigned char LoRa_read(void)
{
  if (!LoRa_available()) {
    return -1;
  }

  _packetIndex++;
  return read_reg_single(REG_FIFO);
}

unsigned char LoRa_parsePacket(void)
{
  unsigned char packetLength = 0;
  unsigned char irqFlags = read_reg_single(REG_IRQ_FLAGS);


  //explicitHeaderMode();
  write_reg_single(REG_MODEM_CONFIG_1, read_reg_single(REG_MODEM_CONFIG_1) & 0xfe);

  // clear IRQ's
  write_reg_single(REG_IRQ_FLAGS, irqFlags);

  if ((irqFlags & IRQ_RX_DONE_MASK) && (irqFlags & IRQ_PAYLOAD_CRC_ERROR_MASK) == 0) {
    // received a packet
    _packetIndex = 0;
	
	packetLength = read_reg_single(REG_RX_NB_BYTES);
	
    // set FIFO address to current RX address
    write_reg_single(REG_FIFO_ADDR_PTR, read_reg_single(REG_FIFO_RX_CURRENT_ADDR));

    // put in standby mode
    idle();
  } 
  else if (read_reg_single(REG_OP_MODE) != (MODE_LONG_RANGE_MODE | MODE_RX_SINGLE)) {
    // not currently in RX mode
    // reset FIFO address
    write_reg_single(REG_FIFO_ADDR_PTR, 0);

    // put in single RX mode
    write_reg_single(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_SINGLE);
  }

  return packetLength;
}

void sendmess(unsigned char *buff)
{
  LoRa_beginPacket();                   // start packet
  LoRa_AddDataToSend(destination,localAddress,msgCount, buff);
  LoRa_endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
  }
 
void LoRa_AddDataToSend(unsigned char des,unsigned char add,unsigned char count, unsigned char *buffer)
{
	unsigned char currentLength = read_reg_single(REG_PAYLOAD_LENGTH);
	unsigned char i;
	// check size fix size is 10
	write_reg_single(REG_FIFO, des);// add destination address
	write_reg_single(REG_FIFO, add);// add sender address
	write_reg_single(REG_FIFO, count);// add message ID
	write_reg_single(REG_FIFO, 10);// add payload length => fix 10
	
	for (i = 0; i < 10; i++) {
    write_reg_single(REG_FIFO, buffer[i]);
	}
	// update length : size of mess + 4 mess above
	write_reg_single(REG_PAYLOAD_LENGTH, currentLength + 14);
  
}
 

/*
void explicitHeaderMode()
{
  _implicitHeaderMode = 0;

  write_reg_single(REG_MODEM_CONFIG_1, read_reg_single(REG_MODEM_CONFIG_1) & 0xfe);
}
*/

int LoRa_beginPacket(void)
{
  // put in standby mode
  idle();
  //explicitHeaderMode();
  write_reg_single(REG_MODEM_CONFIG_1, read_reg_single(REG_MODEM_CONFIG_1) & 0xfe);
  // reset FIFO address and paload length
  write_reg_single(REG_FIFO_ADDR_PTR, 0);
  write_reg_single(REG_PAYLOAD_LENGTH, 0);

  return 1;
}


unsigned char LoRa_endPacket()
{
 // put in TX mode
  write_reg_single(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);

  // wait for TX done
  while((read_reg_single(REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) == 0);

  // clear IRQ's
  write_reg_single(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);

  return 1;
}


void rf_sleep(void)
{
    write_reg_single(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);
}

void setFrequency(unsigned int frequency)
{
   unsigned int frf;
   
   _frequency = frequency;
   
  frf = (frequency << 6);

  write_reg_single(REG_FRF_MSB, (unsigned char)(frf >> 8));
  write_reg_single(REG_FRF_MID, (unsigned char)(frf >> 0));
  write_reg_single(REG_FRF_LSB, 0);
}
void setTxPower(unsigned char level)
{


    // PA BOOST
    if (level < 2) {
      level = 2;
    } else if (level > 17) {
      level = 17;
    }

    write_reg_single(REG_PA_CONFIG, PA_BOOST | (level - 2));
  
  
}
void idle()
{
  write_reg_single(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
}
unsigned char rfm98w_begin(unsigned int frequency) //unsigned long int 0 to 4294967295
{
	RF_NSS(1);
	RF_RST(0);
	HAL_Delay(100);
	RF_NSS(0);
	RF_RST(1);
	HAL_Delay(100);
//check version
	
	//RS485_TX1;
	//printf("Reg 0x%x:%x\r\n",REG_VERSION,read_reg_single(REG_VERSION));
	
if(read_reg_single(REG_VERSION) != 0x12) return 0;
// put in sleep mode
  rf_sleep(); 
// set frequency
  setFrequency(frequency);  
  // set base addresses
  write_reg_single(REG_FIFO_TX_BASE_ADDR, 0);
  write_reg_single(REG_FIFO_RX_BASE_ADDR, 0);

  // set LNA boost
  write_reg_single(REG_LNA, read_reg_single(REG_LNA) | 0x03);

  // set auto AGC
  write_reg_single(REG_MODEM_CONFIG_3, 0x04);

  // set output power to 17 dBm
  setTxPower(7);

  // put in standby mode
  idle();

  return 1;
}
/****************************************************************************************************************************/
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	//uint8_t UART1_DATA[RX1SIZE];
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
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();

  /* USER CODE BEGIN 2 */
	// Chay ngat timer
	if (HAL_TIM_Base_Start_IT(&htim1) != HAL_OK) {
        Error_Handler();
    }
	if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK) {
        Error_Handler();
    }
	HAL_UART_Receive_IT(&huart1,UART1_DATA,RX1SIZE);
	HAL_UART_Receive_IT(&huart2,UART2_DATA,RX2SIZE);
	//Khoi dong LED 7 thanh
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
	printf("PAPI master!\r\n");
	bt_scan = 0;
		check = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	ss1.offset_x = 0;
	ss1.offset_y = 0;
	
	//RS485_TX1;
	//printf("Reg 0x%x:%x\r\n",REG_VERSION,LoRa_readRegister(REG_VERSION));
	if(!rfm98w_begin(433)) {
				RS485_TX1;printf("LoRa init failed. Check your connections.\n\r!");
	}
	else {  
        RS485_TX1;
        printf("LoRa init succeeded.\r\n!");
	}
  while (1)
  {
		/*
		if(bt_scan >2)
		{
		bt_scan = 0;
			
			RS485_TX1;
			printf("UART1:%s, check :%d,RxXferSize :%d,RxXferCount:%d\r\n",UART1_DATA,check,huart1.RxXferSize,huart1.RxXferCount);
			//HAL_UART_Receive_IT(&huart1,UART1_DATA,20);
			//huart1.pRxBuffPtr = UART1_DATA;
			//huart1.RxXferSize = 20;
			if(huart1.RxXferCount <=1) {HAL_UART_Receive_IT(&huart1,UART1_DATA,20);}
		}
		
		if(check > 10)
		{
			bt_scan = 0;
			
			RS485_TX1;
			
			printf("UART:%s, check :%d\r\n",UART1_DATA,check);
			//HAL_UART_Receive(&huart1,UART1_DATA,20,100);
			//HAL_UART_Receive_IT(&huart1,UART1_DATA,20);
			check = 0;
		}
		
		if(check == ONTIME)
		{
			check = 0;
			RS485_TX1;
			printf("UART1:%s, check :%d,RxXferSize :%d,RxXferCount:%d\r\n",UART1_DATA,check,huart1.RxXferSize,huart1.RxXferCount);
			HAL_UART_Receive_IT(&huart1,UART1_DATA,20);
			huart1.pRxBuffPtr = UART1_DATA;
			huart1.RxXferCount = 20;
			//_clear_mem(UART1_DATA,20);
		}*/
		_loop1_tx1off_();
		_loop2_rx1_process_();
		_loop3_rx2_process_();
		_loop4_check_sensor_();
		_loop5_button_();
		_loop6_lampcheck_();
		_loop7_communication_();
		_loop8_neigh_();
		_loop9_systemcheck_();
		_loop_master_send_();
		_slave_rep_();
		_slave_send_();
		_loop10_led7out_();
		
		//HAL_Delay(1000);
		//onReceive_(LoRa_parsePacket());
		/*
		if(check == ONTIME){
		check = STOP;
		//Xu ly no
		RS485_TX1;
		printf("UART1:%s, check :%d,RxXferSize :%d,RxXferCount:%d\r\n",UART1_DATA,check,huart1.RxXferSize,huart1.RxXferCount);
		huart1.RxXferCount = RX1SIZE;
		HAL_UART_Receive_IT(&huart1,UART1_DATA,RX1SIZE);
		huart1.pRxBuffPtr = UART1_DATA;
		_clear_mem(UART1_DATA,RX1SIZE-1);
	}*/
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

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

/** NVIC Configuration
*/
static void MX_NVIC_Init(void)
{
  /* USART2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
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
  HAL_GPIO_WritePin(GPIOB, CRL3_Pin|CRL2_Pin|CRL1_Pin|CLK_Pin 
                          |DIO_Pin|RF_NSS_Pin|RF_RST_Pin, GPIO_PIN_RESET);

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

  /*Configure GPIO pins : CRL3_Pin CRL2_Pin CRL1_Pin RF_NSS_Pin 
                           RF_RST_Pin */
  GPIO_InitStruct.Pin = CRL3_Pin|CRL2_Pin|CRL1_Pin|RF_NSS_Pin 
                          |RF_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : CLK_Pin DIO_Pin */
  GPIO_InitStruct.Pin = CLK_Pin|DIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

unsigned char HEXInStringToDec(unsigned char data)
      {
            if((data>47)&&(data<58)) return (data-48);// 0...9
            else if((data>63)&&(data<71)) return (data-55);//A..F
						else return 0;
      }
			
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim3.Instance )//1ms
	{
		//counting for loop1
		if(_loop1 > ONTIME) _loop1--;
		//counting for loop2
		if(_loop2 > ONTIME) _loop2--;
		//counting for loop3
		if(_loop3 > ONTIME) _loop3--;
		//counting for loop5
		if(_loop5 > ONTIME) _loop5--;
		//counting for _loop_same
		if(_loop_same > ONTIME) _loop_same--;
		//counting for _loop_master
		if(_loopm > ONTIME) _loopm--;
		//counting for _slave_time_to_rep
		if(_slave_time_to_rep > ONTIME) _slave_time_to_rep--;
		
		if(check > ONTIME) check--;
	}
	if(htim->Instance == htim1.Instance )//1s
	{
		bt_scan++;
		//counting for _loop4
		if(_loop4 > ONTIME) _loop4--;
		//counting for _loop10_
		if(_loop10_ > ONTIME) _loop10_--;
		
		//kiem tra xem ket noi OK ko
		if(_my_conn < CONN_LIMIT) _my_conn++;
		//f5_papi_info
		if(f5_papi_info[PAPI_MASTER]<F5TIMEOUT) f5_papi_info[PAPI_MASTER]++;
		if(f5_papi_info[PAPI_SLAVE1]<F5TIMEOUT) f5_papi_info[PAPI_SLAVE1]++;
		if(f5_papi_info[PAPI_SLAVE2]<F5TIMEOUT) f5_papi_info[PAPI_SLAVE2]++;
		if(f5_papi_info[PAPI_SLAVE3]<F5TIMEOUT) f5_papi_info[PAPI_SLAVE3]++;
	}
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
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 10);
	//start loop1 
	_loop1 = _loop1_run;
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
