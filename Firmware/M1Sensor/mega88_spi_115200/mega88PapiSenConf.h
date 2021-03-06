#define RS485_TX(stt) PORTD.2 = stt;
#define LED(stt) PORTD.3 = stt;

unsigned char cali_flag;
volatile unsigned int count_cali;
//char ang_x[8],ang_y[8];



char count;
int newavgx,newavgy;
int avgx,avgy;
int send_avgx,send_avgy;
float X_err_measure = 2.0;
float X_err_estimate = 2.0;
float X_q = 0.01;
float X_current_estimate;
float X_last_estimate;
float X_kalman_gain;

int X_updateEstimate(float mea)
{
  X_kalman_gain = X_err_estimate/(X_err_estimate + X_err_measure);
  X_current_estimate = X_last_estimate + X_kalman_gain * (mea - X_last_estimate);
  X_err_estimate =  (1.0 - X_kalman_gain)*X_err_estimate + fabs(X_last_estimate-X_current_estimate)*X_q;
  X_last_estimate=X_current_estimate;

  return (int)(X_current_estimate);
}

float Y_err_measure = 2.0;
float Y_err_estimate = 2.0;
float Y_q = 0.01;
float Y_current_estimate;
float Y_last_estimate;
float Y_kalman_gain;

int Y_updateEstimate(float mea)
{
  Y_kalman_gain = Y_err_estimate/(Y_err_estimate + Y_err_measure);
  Y_current_estimate = Y_last_estimate + Y_kalman_gain * (mea - Y_last_estimate);
  Y_err_estimate =  (1.0 - Y_kalman_gain)*Y_err_estimate + fabs(Y_last_estimate-Y_current_estimate)*Y_q;
  Y_last_estimate=Y_current_estimate;

  return (int)(Y_current_estimate);
}


unsigned int lux;
unsigned char buff_temp1,buff_temp2;

unsigned int new_tempu;
#define ONTIME    1 // for the sticks timer
#define STOPTHIS  0 // for the sticks timer
volatile unsigned int usart_time_out;
volatile unsigned int send_sensor_info=1000;//1s

const unsigned char RDAX=16;
const unsigned char RDAY=17; 
unsigned int read_SCA100TX(void);
unsigned int read_SCA100TY(void);
unsigned int read_SCA100TXY(unsigned char thisValue);
//int rawx,rawy;
eeprom int osrawx,osrawy;
eeprom char check_eep1,check_eep2;
//unsigned char cal_abs(signed int num)
//{
//	if(num >=0) return (num&0x00ff);
//	else if(num<0) return (256+(num&0x00ff));
//}
int ds_temp;
float ds_err_measure = 2.0;
float ds_err_estimate = 2.0;
float ds_q = 0.01;
float ds_current_estimate;
float ds_last_estimate;
float ds_kalman_gain;
/*
int ds_updateEstimate(float mea)
{
  ds_kalman_gain = ds_err_estimate/(ds_err_estimate + ds_err_measure);
  ds_current_estimate = ds_last_estimate + ds_kalman_gain * (mea - ds_last_estimate);
  ds_err_estimate =  (1.0 - ds_kalman_gain)*ds_err_estimate + fabs(ds_last_estimate-ds_current_estimate)*ds_q;
  ds_last_estimate=ds_current_estimate;

  return (int)(ds_current_estimate);
}
*/
unsigned int cal_tempu(unsigned char b1,unsigned char b2)
{
	unsigned int donvi;
	unsigned int hangchuc;
	
	donvi = b1&0x0f;
    donvi = (donvi*5)/8;	
	hangchuc = 10*(((b1&0xf0)>>4)|((b2&0x0f)<<4)) ;
    new_tempu = hangchuc + donvi;
    ds_kalman_gain = ds_err_estimate/(ds_err_estimate + ds_err_measure);
    ds_current_estimate = ds_last_estimate + ds_kalman_gain * (new_tempu - ds_last_estimate);
    ds_err_estimate =  (1.0 - ds_kalman_gain)*ds_err_estimate + fabs(ds_last_estimate-ds_current_estimate)*ds_q;
    ds_last_estimate=ds_current_estimate;

    return (int)(ds_current_estimate);
}

#define DATA_REGISTER_EMPTY (1<<UDRE0)
#define RX_COMPLETE (1<<RXC0)
#define FRAMING_ERROR (1<<FE0)
#define PARITY_ERROR (1<<UPE0)
#define DATA_OVERRUN (1<<DOR0)

// USART Receiver buffer
#define RX_BUFFER_SIZE0 8
char rx_buffer0[RX_BUFFER_SIZE0];

#if RX_BUFFER_SIZE0 <= 256
unsigned char rx_wr_index0=0,rx_rd_index0=0;
#else
unsigned int rx_wr_index0=0,rx_rd_index0=0;
#endif

#if RX_BUFFER_SIZE0 < 256
unsigned char rx_counter0=0;
#else
unsigned int rx_counter0=0;
#endif

// This flag is set on USART Receiver buffer overflow
bit rx_buffer_overflow0;
// USART Transmitter buffer
#define TX_BUFFER_SIZE0 8
char tx_buffer0[TX_BUFFER_SIZE0];

#if TX_BUFFER_SIZE0 <= 256
unsigned char tx_wr_index0=0,tx_rd_index0=0;
#else
unsigned int tx_wr_index0=0,tx_rd_index0=0;
#endif

#if TX_BUFFER_SIZE0 < 256
unsigned char tx_counter0=0;
#else
unsigned int tx_counter0=0;
#endif
#ifndef _DEBUG_TERMINAL_IO_
// Get a character from the USART Receiver buffer
#define _ALTERNATE_GETCHAR_
#pragma used+
char getchar(void)
{
char data;
while (rx_counter0==0);
data=rx_buffer0[rx_rd_index0++];
#if RX_BUFFER_SIZE0 != 256
if (rx_rd_index0 == RX_BUFFER_SIZE0) rx_rd_index0=0;
#endif
#asm("cli")
--rx_counter0;
#asm("sei")
return data;
}
#pragma used-
#endif
#ifndef _DEBUG_TERMINAL_IO_
// Write a character to the USART Transmitter buffer
#define _ALTERNATE_PUTCHAR_
#pragma used+
void putchar(char c)
{
while (tx_counter0 == TX_BUFFER_SIZE0);
#asm("cli")
if (tx_counter0 || ((UCSR0A & DATA_REGISTER_EMPTY)==0))
   {
   tx_buffer0[tx_wr_index0++]=c;
#if TX_BUFFER_SIZE0 != 256
   if (tx_wr_index0 == TX_BUFFER_SIZE0) tx_wr_index0=0;
#endif
   ++tx_counter0;
   }
else
   UDR0=c;
#asm("sei")
}
#pragma used-
#endif

// Voltage Reference: AVCC pin
#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))

void process_uart_mess();
unsigned int read_SCA100TX(void);
unsigned int read_SCA100TY(void);
unsigned int read_SCA100TXY(unsigned char thisValue);
        /***************************************************************************************/
        // DS1820 Temperature Sensor functions
//#include <ds18b20.h>
// 1 Wire Bus initialization
// 1 Wire Data port: PORTC
// 1 Wire Data bit: 2
#define DS18B20_PORT_DQ_H PORTC.2 = 1;
#define DS18B20_PORT_DQ_L PORTC.2 = 0;


// Dinh nghia do phan giai su dung
#define DS18B20_9BIT_RES 0  // Do phan giai 9bit
#define DS18B20_10BIT_RES 1 // Do phan giai 10bit
#define DS18B20_11BIT_RES 2 // Do phan giai 11bit
#define DS18B20_12BIT_RES 3 // Do phan giai 12bit

// Dinh nghia cac lenh ROM
#define DS1820_CMD_SEARCHROM     0xF0   // Tim so luong DS18B20 tren bus va gia tri 64 bit ROM cua cac DS18B20   
#define DS1820_CMD_READROM       0x33   // Doc gia tri 8 Byte ma khac tren ROM DS18B20
#define DS1820_CMD_MATCHROM      0x55   // Chon DS18B20 can giao tiep, gui truoc 64 Bit ROM DS18B20 can chon
#define DS1820_CMD_SKIPROM       0xCC   // Truy cap thang den bo nho cua DS18B20
#define DS1820_CMD_ALARMSEARCH   0xEC   // Tim DS18B20 co co bao dong duoc bat

// Dinh nghia cac lenh chuc nang
#define DS1820_CMD_CONVERTTEMP   0x44   // Khoi dong qua trinh do va chuyen doi gia tri nhiet do thanh so nhi phan
#define DS1820_CMD_WRITESCRPAD   0x4E   // Ghi gia tri vao 3 thanh ghi: TH, TL, Configuration register (Ghi tu MSB->LSB)
#define DS1820_CMD_READSCRPAD    0xBE   // Doc noi dung bo nho DS18B20: doc tu Byte0 -> Byte9, doc tu MSB->LSB
#define DS1820_CMD_COPYSCRPAD    0x48   // Copy gia tri 3 thanh ghi TH, TL, Config vao bo nho EEPROM
#define DS1820_CMD_RECALLEE      0xB8
/*-----------------------------------------------------------------------------
Noi Dung    :   Doc 1 byte du lieu tu DS18B20 ra ngoai. 
Tham Bien   :   Khong. 
Tra Ve      :   Byte du lieu can doc.
  -----------------------------------------------------------------------------*/   
  
unsigned char DS18B20_ReadByte(void);	

/*-----------------------------------------------------------------------------
Noi Dung    :   Viet 1 byte du lieu vao DS18B20. 
Tham Bien   :   data: Byte du lieu can viet vao. 
Tra Ve      :   Khong.
  -----------------------------------------------------------------------------*/
  			
void DS18B20_WriteByte(unsigned char data);	

/*-----------------------------------------------------------------------------
Noi Dung    :   Khoi tao DS18B20. 
Tham Bien   :   Khong. 
Tra Ve      :   Gia tri tra ve khi khoi tao xong cua DS18B20 (bit).
  -----------------------------------------------------------------------------*/
  
void DS18B20_Init(void);

/*-----------------------------------------------------------------------------
Noi Dung    :   Ghi 1 byte lenh chuc nang vao DS18B20. 
Tham Bien   :   byte_func: byte lenh chuc nang cua DS18B20. 
Tra Ve      :   Khong.
  -----------------------------------------------------------------------------*/
  
void DS18B20_WriteFunc(unsigned char byte_func);

/*-----------------------------------------------------------------------------
Noi Dung    :   Cau hinh cho DS18B20. 
Tham Bien   :   temp_low: Gia tri nhiet do thap nhat de dua ra canh bao. 
                temp_high: Gia tri nhiet do cao nhat de dua ra canh bao.   
                resolution: Do phan giai cho DS18B20.
Tra Ve      :   Khong.
  -----------------------------------------------------------------------------*/
  
void DS18B20_Config(unsigned char temp_low, unsigned char temp_high, unsigned char resolution);	

/*-----------------------------------------------------------------------------
Noi Dung    :   Doc gia tri nhiet do do duoc cua DS18B20. 
Tham Bien   :   Khong. 
Tra Ve      :   Gia tri nhiet do do duoc.
  -----------------------------------------------------------------------------*/
  												
//float DS18B20_ReadTemp(void);
/***************************************************************************************/

void init_hw()
{
 // Input/Output Ports initialization
// Port B initialization
// Function: Bit7=In Bit6=In Bit5=Out Bit4=In Bit3=Out Bit2=Out Bit1=In Bit0=In 
DDRB=(0<<DDB7) | (0<<DDB6) | (1<<DDB5) | (0<<DDB4) | (1<<DDB3) | (1<<DDB2) | (0<<DDB1) | (0<<DDB0);
// State: Bit7=T Bit6=T Bit5=0 Bit4=T Bit3=0 Bit2=0 Bit1=T Bit0=T 
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

// Port C initialization
// Function: Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRC=(0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
// State: Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTC=(0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (1<<DDD3) | (1<<DDD2) | (0<<DDD1) | (0<<DDD0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 250.000 kHz
// Mode: Normal top=0xFF
// OC0A output: Disconnected
// OC0B output: Disconnected
// Timer Period: 1 ms
TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (0<<WGM00);
TCCR0B=(0<<WGM02) | (0<<CS02) | (1<<CS01) | (1<<CS00);
TCNT0=0x06;
OCR0A=0x00;
OCR0B=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer1 Stopped
// Mode: Normal top=0xFFFF
// OC1A output: Disconnected
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer2 Stopped
// Mode: Normal top=0xFF
// OC2A output: Disconnected
// OC2B output: Disconnected
ASSR=(0<<EXCLK) | (0<<AS2);
TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (0<<WGM21) | (0<<WGM20);
TCCR2B=(0<<WGM22) | (0<<CS22) | (0<<CS21) | (0<<CS20);
TCNT2=0x00;
OCR2A=0x00;
OCR2B=0x00;
// Timer/Counter 0 Interrupt(s) initialization
TIMSK0=(0<<OCIE0B) | (0<<OCIE0A) | (1<<TOIE0);

// Timer/Counter 1 Interrupt(s) initialization
TIMSK1=(0<<ICIE1) | (0<<OCIE1B) | (0<<OCIE1A) | (0<<TOIE1);

// Timer/Counter 2 Interrupt(s) initialization
TIMSK2=(0<<OCIE2B) | (0<<OCIE2A) | (0<<TOIE2);

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// Interrupt on any change on pins PCINT0-7: Off
// Interrupt on any change on pins PCINT8-14: Off
// Interrupt on any change on pins PCINT16-23: Off
EICRA=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
EIMSK=(0<<INT1) | (0<<INT0);
PCICR=(0<<PCIE2) | (0<<PCIE1) | (0<<PCIE0);

// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART0 Mode: Asynchronous
// USART Baud Rate: 115200 (Double Speed Mode)
UCSR0A=(0<<RXC0) | (0<<TXC0) | (0<<UDRE0) | (0<<FE0) | (0<<DOR0) | (0<<UPE0) | (1<<U2X0) | (0<<MPCM0);
UCSR0B=(1<<RXCIE0) | (1<<TXCIE0) | (0<<UDRIE0) | (1<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80);
UCSR0C=(0<<UMSEL01) | (0<<UMSEL00) | (0<<UPM01) | (0<<UPM00) | (0<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00) | (0<<UCPOL0);
UBRR0H=0x00;
UBRR0L=0x10;

// Analog Comparator initialization
// Analog Comparator: Off
// The Analog Comparator's positive input is
// connected to the AIN0 pin
// The Analog Comparator's negative input is
// connected to the AIN1 pin
ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);
// Digital input buffer on AIN0: On
// Digital input buffer on AIN1: On
DIDR1=(0<<AIN0D) | (0<<AIN1D);

// ADC initialization
// ADC Clock frequency: 1000.000 kHz
// ADC Voltage Reference: AVCC pin
// ADC Auto Trigger Source: ADC Stopped
// Digital input buffers on ADC0: On, ADC1: On, ADC2: On, ADC3: On
// ADC4: On, ADC5: On
DIDR0=(0<<ADC5D) | (0<<ADC4D) | (0<<ADC3D) | (0<<ADC2D) | (0<<ADC1D) | (0<<ADC0D);
ADMUX=ADC_VREF_TYPE;
ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (1<<ADPS2) | (0<<ADPS1) | (0<<ADPS0);
ADCSRB=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);

// SPI initialization
// SPI Type: Master
// SPI Clock Rate: 125.000 kHz
// SPI Clock Phase: Cycle Start
// SPI Clock Polarity: Low
// SPI Data Order: MSB First
SPCR=(0<<SPIE) | (1<<SPE) | (0<<DORD) | (1<<MSTR) | (0<<CPOL) | (0<<CPHA) | (1<<SPR1) | (1<<SPR0);
SPSR=(0<<SPI2X);

// TWI initialization
// TWI disabled
TWCR=(0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE);

// Global enable interrupts
#asm("sei")
}

