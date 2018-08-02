#define RS485_TX1 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
#define RS485_RX1 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

#define ONTIME        1
#define STOP          0
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

//start loop1 
_loop1 = _loop1_run;

//counting for loop1
if(_loop1 > ONTIME) _loop1--;

HAL_UART_Receive_IT(&huart1,UART1_DATA,20);
>>Du lieu duoc luu vao UART1_DATA khi du 20 thi dung lai.

#define RX1SIZE 20
#define LOOP2RUN 50
uint8_t UART1_DATA[RX1SIZE];
HAL_UART_Receive_IT(&huart1,UART1_DATA,RX1SIZE);

//for _loop2_rx1_process_
unsigned char _loop2;
//start loop2 
	_loop2 = LOOP2RUN;
void _loop2_rx1_process_(void)
{
	if(_loop2 == ONTIME){
		_loop2 = STOP;
		//Xu ly no
		HAL_UART_Receive_IT(&huart1,UART1_DATA,RX1SIZE);
		_clear_mem(UART1_DATA,RX1SIZE);
	}
}

//counting for loop2
		if(_loop2 > ONTIME) _loop2--;
		

void _clear_mem(uint8_t *pData, uint8_t _size)
{
	uint8_t i;
	for(i=0;i<_size;i++)
	{
		*(pData+i) = 0;
	}
}

//for _loop2_rx1_process_
unsigned int _loop2;
uint8_t UART1_DATA[RX1SIZE];

void _loop2_rx1_process_(void)
{
	if(_loop2 == ONTIME){
		_loop2 = STOP;
		//Xu ly no
		RS485_TX1;
		printf("UART1:%s,RxSize :%d,RxCount:%d\r\n",UART1_DATA,huart1.RxXferSize,huart1.RxXferCount);
		huart1.RxXferCount = RX1SIZE;
		HAL_UART_Receive_IT(&huart1,UART1_DATA,RX1SIZE);
		huart1.pRxBuffPtr = UART1_DATA;
		_clear_mem(UART1_DATA,RX1SIZE-1);
	}
}

//for _loop3_rx2_process_
unsigned int _loop3;
uint8_t UART2_DATA[RX2SIZE];

void _loop3_rx2_process_(void)
{
	if(_loop3 == ONTIME){
		_loop3 = STOP;
		//Xu ly no
		//RS485_TX1;
		//printf("UART2:%s,RxSize :%d,RxCount:%d\r\n",UART2_DATA,huart2.RxXferSize,huart2.RxXferCount);
		_get_sensor_data();
		huart2.RxXferCount = RX2SIZE;
		HAL_UART_Receive_IT(&huart2,UART2_DATA,RX2SIZE);
		huart2.pRxBuffPtr = UART2_DATA;
		_clear_mem(UART2_DATA,RX1SIZE-1);
	}
}

> xu ly du lieu tu cam bien?
// Cac trang thai cua cam bien
#define SEN_OK 0//Cam bien binh thuong, OK
#define SEN_WO 1//Cam bien dang bi lech goc WRONG
#define SEN_DI 2//Cam bien chet DIE
#define SEN_FI 3//Cam bien den chet FIRE
//for _loop4_check_sensor_
//Lost sensor : Uart error, sensor error
//if in 60 seconds has no right sensor massage => ERROR
#define LOOOP4RUN 10
uint8_t _loop4 = LOOOP4RUN;
uint8_t _stt_sensor_ = SEN_OK;

void _loop4_check_sensor_(void)
{
	if(_loop4 == ONTIME){// sau 1 phut ko nhan duoc du lieu tu cam bien
		_loop4 = STOP;
		_stt_sensor_ = SEN_DI; 
		RS485_TX1;
		printf("Mat cam bien:SEN_DI\r\n");
	}
	else{//kiem tra xem du lieu cam bien dung hay sai <-----check co kiem tra anh sang
		//KIEM TRA ANH SANG O DAY
		ss1.diff_x = ss1.rawx - ss1.offset_x;
		ss1.diff_y = ss1.rawy - ss1.offset_y;
		
		if(ss1.diff_x > 0) ss1.abs_diff_x = ss1.diff_x;
		else ss1.abs_diff_x = -ss1.diff_x;
		
		if(ss1.diff_y > 0) ss1.abs_diff_y = ss1.diff_y;
		else ss1.abs_diff_y = -ss1.diff_y;
		
		/*if((ss1.offset_x - ss1.rawx) > 7 )      _stt_sensor_ = SEN_WO;// goc x nho hon -0.25 do
		else if((ss1.rawx - ss1.offset_x) > 14) _stt_sensor_ = SEN_WO;  //goc x lon hon 0.5 do
		else if((ss1.offset_y - ss1.rawy) > 7)  _stt_sensor_ = SEN_WO; //goc y nho hon -0.25 do
		else if((ss1.rawy - ss1.offset_y) > 14) _stt_sensor_ = SEN_WO; //goc y lon hon 0.5 do
		else _stt_sensor_ = SEN_OK;*/
		if(((ss1.offset_x - ss1.rawx) > 7 ) || ((ss1.rawx - ss1.offset_x) > 14) ||((ss1.offset_y - ss1.rawy) > 7)||((ss1.rawy - ss1.offset_y) > 14))
		{
			_stt_sensor_ = SEN_WO;
			//RS485_TX1;
			//printf("Sai cam bien:SEN_WO\r\n");
		}
		else 
		{
			_stt_sensor_ = SEN_OK;
			//RS485_TX1;
			//printf("Sai cam bien:SEN_OK\r\n");
		}
		
		
	}
}

struct _s_sensor_ {
	int rawx;
	int rawy;
	int offset_x;
	int offset_y;
	int diff_x;
	int diff_y;
	int abs_diff_x;
	int abs_diff_y;
	uint8_t raw_lux;
};
struct _s_sensor_ ss1;
// Boc tach du lieu tu cam bien gui ve
void _get_sensor_data(void)
{
	uint8_t temp1,temp2,temp3;
	

	if((UART2_DATA[0] == 'S') &&(UART2_DATA[18] == '<'))
	{
		_loop4 = LOOOP4RUN;//Nhan duoc dung du lieu tu cam bien => Cam bien OK
		
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
		
		//temp_ds1820 = ((float)(buff_temp1&0x0f))/16;            // Lay phan thuc cua gia tri nhiet 
		//buff_temp1 =((buff_temp1&0xf0)>>4)|((buff_temp2&0x0f)<<4) ;    // Lay phan nguyen cua gia tri nhiet do
		
		//temp_ds1820=temp_ds1820+buff_temp1;
	}

}

//Cac trang thai cua den
Den BT
Den Hong ss
Den hong lamp

//Cac trang thai cua he thong
He thong bt
he thong loi den
he thong loi cam bien
he thong loi ket noi
#define LOOPSAME 200// 200 mean 200ms
unsigned char _loop_same = LOOPSAME;

//for _loop5_button_
#define LOOOP5RUN 200// 200 mean 200ms
unsigned char _loop5 = LOOOP5RUN;
//processing loop5, check button
void _loop5_button_(void){
	if(_loop5 == ONTIME) {
		_loop5 = LOOOP5RUN;
		//check button
		if(!HAL_GPIO_ReadPin(BT1_GPIO_Port,BT1_Pin)) {RS485_TX1;printf("BT1\r\n");ss1.offset_x = ss1.rawx; ss1.offset_y = ss1.rawy;}
		if(!HAL_GPIO_ReadPin(BT2_GPIO_Port,BT2_Pin)) {RS485_TX1;printf("BT2\r\n");}
	}
}

//for _loop6_lampcheck_
#define LAMP_OK           0
#define LAMP_ERR1_SENSOR  1
#define LAMP_ERR2_FIRED   2
#define LAMP_ERR2_INCLIN  3

#define LOOOP6RUN 200// 200 mean 200ms
unsigned char _loop6 = LOOOP6RUN;
//processing loop6, check sensor for lamp's error
void _loop6_lampcheck_(void){
	if(_loop_same == ONTIME) {
		if			(_stt_sensor_ == SEN_OK) _stt_lamp_ = LAMP_OK;//Den binh thuong
		else if	(_stt_sensor_ == SEN_WO) _stt_lamp_ = LAMP_ERR3_INCLIN;//Den bi nghieng
		else if	(_stt_sensor_ == SEN_DI) _stt_lamp_ = LAMP_ERR1_SENSOR;//Ko co phan hoi tu cam bien
		else if	(_stt_sensor_ == SEN_FI) _stt_lamp_ = LAMP_ERR2_FIRED;//Bong den bi chay
		RS485_TX1;
		if			(_stt_lamp_ == LAMP_OK) 					printf("PAPI's status :LAMP_OK\r\n");//Den binh thuong
		else if	(_stt_lamp_ == LAMP_ERR3_INCLIN) 	printf("PAPI's status :LAMP_ERR3_INCLIN\r\n");//Den bi nghieng
		else if	(_stt_lamp_ == LAMP_ERR1_SENSOR) 	printf("PAPI's status :LAMP_ERR1_SENSOR\r\n");//Ko co phan hoi tu cam bien
		else if	(_stt_lamp_ == LAMP_ERR2_FIRED) 	printf("PAPI's status :LAMP_ERR2_FIRED\r\n");//Bong den bi chay
		
		//check lamp
	}
}
//Kiem tra ket noi den slave hoac master co on dinh hay ko?
//Cac trang thai
#define COM_GOOOD 0
#define COM_BAD   1
void _loop7_communication_(void){
	
}
//Kiem tra cac den khac co loi ko
//Cac trang thai
#define NEIGH_GOOD 0
#define NEIGH_BAD  1
void _loop8_neigh_(void){
	
}
//INPUT: LOCAL ERROR & COMM ERROR & NEIGH ERROR
//OUTPUT:GOOOD
#define SYSTEM_GOOOD 0
//LOCAL ERROR : 
#define ERR1_SEN     1 // Cam bien cua PAPI gap su co
#define ERR2_LAM     2 // lamp cua PAPI gap su co
#define ERR3_INC     3 // PAPI's sensor is inclined 
//COMM ERROR
#define ERR4_COM     4 // Communication got trouble
//NEIGH ERROR
#define ERR5_NEI     5 // One of neighbors got trouble
uint8_t system_status = SYSTEM_GOOOD;
void _loop9_systemcheck_(void){
	
	if(_loop_same == ONTIME) {
		_loop_same = LOOPSAME;//ket thuc cac vong lap tuong tu nhau
		//RS485_TX1;
		//printf("Check system cai nao\r\n");
		//check lamp
	}
}
#define LOOP10RUN 3
uint8_t _loop10_ = LOOP10RUN;
uint8_t _display_ = 0;
uint8_t _display_conten[4] ;
void _loop10_led7out_(void){
	if(_loop10_ == ONTIME){
		_loop10_ = LOOP10RUN;
		
	
		switch(_display_)
		{
			
			
		case 0: {break;}
		}
		_display_++;
		if(_display_>10) _display_ = 0;
	}
	
	
}
		_loop1_tx1off_();
		_loop2_rx1_process_();
		_loop3_rx2_process_();
		_loop4_check_sensor_();
		_loop5_button_();
		_loop6_lampcheck_();
		_loop7_communication_();
		_loop8_neigh_();
		_loop9_systemcheck_();
		_loop10_led7out_();

		//PAPI ID
#define PAPI_MASTER 0x00
#define PAPI_SLAVE1 0x01
#define PAPI_SLAVE2 0x02
#define PAPI_SLAVE3 0x03
uint8_t _papi_id = PAPI_MASTER;
//uint8_t _papi_id = PAPI_SLAVE1;
void _send_papi_info_(void){
	RS485_TX1;
	printf(">P0x%02X%02X<",_papi_id,_stt_lamp_);
}



//for _loop_master_send_
#define LOOPMASTER 1000// 1000 mean 1000ms
uint16_t _loopm = LOOPMASTER;
//processing loop master, send papi's status to slaves
void _loop_master_send_(void){
	if(_loopm == ONTIME) {
		_loopm = LOOPMASTER;
		_send_papi_info_();
	}
}
//tu kiem tra ket noi cua ban than
uint8_t _my_conn = 0;
#define CONN_LIMIT 10// 10 means 10 times without any incoming message

// trang thai cua cac den papi trong he thong papi
uint8_t papi_info[4];
//khoang thoi gian timeout cua ban thong tin
#define F5TIMEOUT 20;// 20 means 10 seconds
// do moi cua cac ban tin
uint8_t f5_papi_info[4];
// Lay thong tin tu cac den PAPI khac
void _get_uart_info (void){
	if((UART1_DATA[0] == '>') && (UART1_DATA[1] == 'P') && (UART1_DATA[6] == '<'))
		{
			_my_conn = 0;//minh ket noi binh thuong;
		}
}


if(f5_papi_info[PAPI_MASTER] >= F5TIMEOUT) papi_info[PAPI_MASTER] = 'x';//x means dont know
if(f5_papi_info[PAPI_SLAVE1] >= F5TIMEOUT) papi_info[PAPI_SLAVE1] = 'x';//x means dont know
if(f5_papi_info[PAPI_SLAVE2] >= F5TIMEOUT) papi_info[PAPI_SLAVE2] = 'x';//x means dont know
if(f5_papi_info[PAPI_SLAVE2] >= F5TIMEOUT) papi_info[PAPI_SLAVE3] = 'x';//x means dont know


#define SEN_OK 0//Cam bien binh thuong, OK
#define SEN_WO 1//Cam bien dang bi lech goc WRONG
#define SEN_DI 2//Cam bien chet DIE
#define SEN_FI 3//Cam bien den chet FIRE

// gia lap gia tri cam bien
#define EMU_ON  0
#define EMU_OFF 1
uint8_t _emu_mode_ = EMU_ON;
uint8_t _emu_value_ = SEN_OK;
if(_emu_mode_ == EMU_ON) {
	_stt_sensor_ = _emu_value_;
}

#define LOOP10RUN 3
uint8_t _loop10_ = LOOP10RUN;
uint8_t _display_label_ = 0;
uint8_t _display_conten[4] ;
void _error_display_(void);
void _loop10_led7out_(void){
	if(_loop10_ == ONTIME){
		_loop10_ = LOOP10RUN;
		RS485_TX1;
	
		switch(_display_label_)
		{
			
			
		case 0: {// Neu co loi, hien thi ma loi o day. Neu ko co loi, bo qua
					_error_display_();
					printf("Label 0\r\n");
					break;
				}
		case 1: {printf("Label 1\r\n");break;}
		case 2: {printf("Label 2\r\n");break;}
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
		if(_display_label_>10) _display_label_ = 0;
	}
	
	
}

void _error_display_(void){
		
		if(_stt_lamp_ != LAMP_OK) //Neu loi cuc bo, hien thi loi cuc bo
			{
				RS485_TX1;
				if	(_stt_lamp_ == LAMP_ERR3_INCLIN) 	printf("PAPI's status :LAMP_ERR3_INCLIN\r\n");//Den bi nghieng
				else if	(_stt_lamp_ == LAMP_ERR1_SENSOR) 	printf("PAPI's status :LAMP_ERR1_SENSOR\r\n");//Ko co phan hoi tu cam bien
				else if	(_stt_lamp_ == LAMP_ERR2_FIRED) 	printf("PAPI's status :LAMP_ERR2_FIRED\r\n");//Bong den bi chay
			}
		else if(system_status != SYSTEM_GOOOD)// Neu ko co loi cuc bo => hien thi loi he thong
			{
				RS485_TX1;
				if(system_status == ERR1_SEN) 		 			printf("ERR1_SEN\r\n");
			else if(system_status == ERR2_LAM) 			printf("ERR2_LAM\r\n");
			else if(system_status == ERR3_INC) 			printf("ERR3_INC\r\n");
			else if(system_status == ERR4_COM) 			printf("ERR4_COM\r\n");
			else if(system_status == ERR5_NEI) 			printf("ERR5_NEI\r\n");
			
			}
		else _display_label_++;
}
