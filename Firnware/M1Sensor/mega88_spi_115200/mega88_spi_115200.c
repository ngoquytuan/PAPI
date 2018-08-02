/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 9/28/2017
Author  : 
Company : 
Comments: 


Chip type               : ATmega88PA
Program type            : Application
AVR Core Clock frequency: 16.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

#include <mega88a.h>
#include <math.h>
#include <delay.h>
#include "SimpleKalmanFilter.h"
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
  												
float DS18B20_ReadTemp(void);
//void mekiep(unsigned char lsb,unsigned char msb);	
// Standard Input/Output functions
#include <stdio.h>
/* include the abs function prototype */
#include <stdlib.h>
// SPI functions
#include <spi.h>
// Declare your global variables here
#define RS485_TX(stt) PORTD.2 = stt;
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

// USART Receiver interrupt service routine
interrupt [USART_RXC] void usart_rx_isr(void)
{
char status,data;
status=UCSR0A;
data=UDR0;
if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
   {
   rx_buffer0[rx_wr_index0++]=data;
#if RX_BUFFER_SIZE0 == 256
   // special case for receiver buffer size=256
   if (++rx_counter0 == 0) rx_buffer_overflow0=1;
#else
   if (rx_wr_index0 == RX_BUFFER_SIZE0) rx_wr_index0=0;
   if (++rx_counter0 == RX_BUFFER_SIZE0)
      {
      rx_counter0=0;
      rx_buffer_overflow0=1;
      }
#endif
   }
}

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

// USART Transmitter interrupt service routine
interrupt [USART_TXC] void usart_tx_isr(void)
{
if (tx_counter0)
   {
   --tx_counter0;
   UDR0=tx_buffer0[tx_rd_index0++];
#if TX_BUFFER_SIZE0 != 256
   if (tx_rd_index0 == TX_BUFFER_SIZE0) tx_rd_index0=0;
#endif
   }
if(!tx_counter0) RS485_TX(0); 
}

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

/*
MEAS 00000000 Measure mode (normal operation mode after power on)
RWTR 00001000 Read and write temperature data register
RDSR 00001010 Read status register
RLOAD 00001011 Reload NV data to memory output register
STX 00001110 Activate Self test for X-channel
STY 00001111 Activate Self test for Y-channel
RDAX 00010000 Read X-channel acceleration through SPI
RDAY 00010001 Read Y-channel acceleration through SPI
*/
/*
const unsigned char MEAS=0;
const unsigned char RWTR=8;
const unsigned char RDSR=10;
const unsigned char RLOAD=11;
const unsigned char STX=14;
const unsigned char STY=15;  */
const unsigned char RDAX=16;
const unsigned char RDAY=17; 
//void write_SCA100T(unsigned char thisValue);
unsigned int read_SCA100TX(void);
unsigned int read_SCA100TY(void);
unsigned int read_SCA100TXY(unsigned char thisValue);
//unsigned char read_SCA100TT(unsigned char thisValue);
//unsigned int read_SCA100T(void );
//void write_SCA100T(unsigned char thisValue);


unsigned int read_SCA100TX(void){
  unsigned int result = 0;
  result=read_SCA100TXY(RDAX);
  return(result);
}

unsigned int read_SCA100TY(void){
  unsigned int result = 0;
  result=read_SCA100TXY(RDAY);
  return(result);
}

//Read SCA100TXY from the SCA100T
unsigned int read_SCA100TXY(unsigned char thisValue){
    unsigned char inByte = 0;           // incoming byte from the SPI
    unsigned int result = 0;   // result to return
      
    // digitalWrite(chipSelectPin, LOW);
    PORTB &= ~(1<<2);
    //SPI.transfer(thisValue);
    spi(thisValue);  
    // delay(100);
    result = spi(0);//SPI.transfer(0x00);
    inByte = spi(0);//SPI.transfer(0x00);
    //digitalWrite(chipSelectPin, HIGH);
    PORTB |= (1<<2);
    result = result << 8;
    result = result | inByte;
    result = result >> 5;
    return(result);

}
/*
unsigned char read_SCA100TT(unsigned char thisValue){
  unsigned char result = 0;   // result to return
  PORTB &= ~(1<<2);
  spi(thisValue);//SPI.transfer(thisValue); 
  result = spi(0);//SPI.transfer(0x00);
  PORTB |= (1<<2);
  return(result);
}

//Read from the SCA100T
unsigned int read_SCA100T(void ) {
  unsigned char inByte = 0;           // incoming byte from the SPI
  unsigned int result = 0;   // result to return
  //digitalWrite(chipSelectPin, LOW);
  PORTB &= ~(1<<2);
  result = spi(0);//SPI.transfer(0x00);
  result = result << 8;
  inByte = spi(0);//SPI.transfer(0x00);
  //digitalWrite(chipSelectPin, HIGH);
   PORTB |= (1<<2);
   result = result | inByte;
   result = result >> 5;
   return(result);
}

//Sends a write command to SCP1000
void write_SCA100T(unsigned char thisValue) {
  //digitalWrite(chipSelectPin, LOW);
  PORTB &= ~(1<<2);
  spi(thisValue);  
  //digitalWrite(chipSelectPin, HIGH);
   PORTB |= (1<<2);
}
*/
/* maximum number of DS1820/DS18S20 connected to the bus */
#define MAX_DEVICES 8

// Voltage Reference: AVCC pin
#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (1<<ADLAR))

// Read the 8 most significant bits
// of the AD conversion result
unsigned char read_adc(unsigned char adc_input)
{
ADMUX=adc_input | ADC_VREF_TYPE;
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=(1<<ADSC);
// Wait for the AD conversion to complete
while ((ADCSRA & (1<<ADIF))==0);
ADCSRA|=(1<<ADIF);
return ADCH;
}

unsigned int ADC_GetSampleAvgNDeleteX(unsigned char N , unsigned char X);
void Sort_tab(unsigned int tab[], unsigned char lenght);
void main(void)
{
// Declare your local variables here
int rawx,rawy;
//float frawx,frawy;
//float alphax,alphay;
//float temp;
unsigned char buff_temp1,buff_temp2;
unsigned char lux=255;//cambienquang
/* DS1820/DS18S20 devices ROM code storage area,
   9 bytes are used for each device
   (see the w1_search function description),
   but only the first 8 bytes contain the ROM code
   and CRC */
// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif
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
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=Out Bit2=Out Bit1=In Bit0=In 
DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (1<<DDD3) | (1<<DDD2) | (0<<DDD1) | (0<<DDD0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=0 Bit2=0 Bit1=T Bit0=T 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=0xFF
// OC0A output: Disconnected
// OC0B output: Disconnected
TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (0<<WGM00);
TCCR0B=(0<<WGM02) | (0<<CS02) | (0<<CS01) | (0<<CS00);
TCNT0=0x00;
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
TIMSK0=(0<<OCIE0B) | (0<<OCIE0A) | (0<<TOIE0);

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
ADCSRB=(0<<ACME);
// Digital input buffer on AIN0: On
// Digital input buffer on AIN1: On
DIDR1=(0<<AIN0D) | (0<<AIN1D);

// ADC initialization
// ADC Clock frequency: 1000.000 kHz
// ADC Voltage Reference: AVCC pin
// ADC Auto Trigger Source: ADC Stopped
// Only the 8 most significant bits of
// the AD conversion result are used
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
PORTD.3 = 1;
RS485_TX(1);
DS18B20_Init();
DS18B20_Config(15,40,DS18B20_9BIT_RES); // Cau hinh cho DS18B20
SimpleKalmanFilter(2.0,2.0,0.001);
while (1)
      {
      // Place your code here 
      //lux = read_adc(3);    
      lux = ADC_GetSampleAvgNDeleteX(8,2);
      rawx=read_SCA100TX();
      rawy=read_SCA100TY(); 
      //frawx = (rawx-1024.0)/1638.0;   
      //frawy = (rawy-1024.0)/1638.0; 
      //alphax =  57*asin(frawx);
      //alphay =  57*asin(frawy);
      //temp=ds1820_temperature_10(&rom_codes[0][0]);
      
      //printf("rom_codes:%d-%d-%d-%d",rom_codes[0][0],rom_codes[0][1],rom_codes[0][2],rom_codes[0][3]); 
      //float asin(float x)
      //printf("Ahihi AX:%3.5f AY:%1.5f \r\n ",frawx,frawy);
      //temp=DS18B20_ReadTemp();    // Doc nhiet do tu DS18B20 
           
      DS18B20_WriteFunc(DS1820_CMD_CONVERTTEMP);  // Khoi dong qua trinh do va chuyen doi nhiet do ra so nhi phan
      delay_us(200);
      DS18B20_WriteFunc(DS1820_CMD_READSCRPAD);   // Doc du lieu tu bo nho DS18b20 
    
      buff_temp1 = DS18B20_ReadByte(); 
      //xxx=(125*(buff_temp1&0x0f))/2;            // Lay phan thuc cua gia tri nhiet do
      buff_temp2 = DS18B20_ReadByte();                 
      //buff_temp1 =((buff_temp1&0xf0)>>4)|((buff_temp2&0x0f)<<4) ;    // Lay phan nguyen cua gia tri nhiet do
      //temp=temp+buff_temp1;
    
    
    //tL = DS18B20_ReadByte(); 
    //temp=((float)(buff_temp1&0x0f))/16;            // Lay phan thuc cua gia tri nhiet do
    //tH = DS18B20_ReadByte();
      RS485_TX(1);
      printf("S>%03X-%03X-%02X-%02X-%02X<  ",rawx,rawy,buff_temp1,buff_temp2,lux);
      delay_ms(2500);  
      }
}

/**
* @brief Get the average of N-X ADC samples
* @param Number of ADC samples to be averaged
* @param Number of ADC samples to be averaged
* @retval The average value
*/
unsigned int ADC_GetSampleAvgNDeleteX(unsigned char N , unsigned char X)
{
unsigned long int avg_sample =0x00;
unsigned int adc_sample[8]={0,0,0,0,0,0,0,0};
unsigned char index=0x00;
for (index=0x00; index<N; index++)
{
/* ADC start conv */
//ADC_SoftwareStartConv(ADC1);
/* Wait end of conversion */
//while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);
/* Store ADC samples */
adc_sample[index] = read_adc(3);
}
/* Sort the N-X ADC samples */
Sort_tab(adc_sample,N);
/* Add the N ADC samples */
for (index=X/2; index<N-X/2; index++)
{
avg_sample += adc_sample[index];
}
/* Compute the average of N-X ADC sample */
avg_sample /= N-X;
/* Return average value */
return avg_sample;
}
/**
Averaging of N-X ADC samples: 
* @brief Sort the N ADC samples
* @param ADC samples to be sorted
* @param Number of ADC samples to be sorted
* @retval None
*/
void Sort_tab(unsigned int tab[], unsigned char lenght)
{
unsigned char l=0x00, exchange =0x01;
unsigned int tmp=0x00;
/* Sort tab */
while(exchange==1)
{
exchange=0;
for(l=0; l<lenght-1; l++)
{
if( tab[l] > tab[l+1] )
{
tmp = tab[l];
tab[l] = tab[l+1];
tab[l+1] = tmp;
exchange=1;
}
}
}
}

/*-----------------------------------------------------------------------------
Noi Dung    :   Doc 1 byte du lieu tu DS18B20 ra ngoai. 
Tham Bien   :   Khong. 
Tra Ve      :   Byte du lieu can doc.
  -----------------------------------------------------------------------------*/ 
  
    
unsigned char DS18B20_ReadByte(void)
{   
    unsigned char i=0;
    unsigned char data=0;      
      
    for(i=8;i>0;i--)
    {    
        //GPIO_SetState(GPIOA,GPIO_Pin_15,GPIO_Mode_Out_PP);   // Cau hinh chan DQ la OUPUT 
        DDRC.2 = 1;      
        DS18B20_PORT_DQ_L;  // Keo chan DQ xuong muc '0'
        data>>=1;
        DS18B20_PORT_DQ_H;  // Keo chan DQ len muc '1'      
        //GPIO_SetState(GPIOA,GPIO_Pin_15,GPIO_Mode_IPU);   // Cau hinh chan DQ la INPUT  
        DDRC.2 = 0;
        PORTC.2 =1;
        if(PINC.2) data|=0x80;   // Nhan du lieu tra ve tu DS18B20
        delay_us(120);                                                 
    }
    return(data);    
}    

/*-----------------------------------------------------------------------------
Noi Dung    :   Viet 1 byte du lieu vao DS18B20. 
Tham Bien   :   data: Byte du lieu can viet vao. 
Tra Ve      :   Khong.
  -----------------------------------------------------------------------------*/
          
void DS18B20_WriteByte(unsigned char data)
{
    unsigned char i=0;
    //GPIO_SetState(GPIOA,GPIO_Pin_15,GPIO_Mode_Out_PP);   // Cau hinh chan DQ la OUTPUT  
    DDRC.2 = 1;
    for (i=8;i>0;i--)
    {
        DS18B20_PORT_DQ_L;  // Keo chan DQ xuong muc '0'
        //DS18B20_PORT_DQ=(data&0x01); // Viet du lieu vao DS18B20
                if(data&0x01) DS18B20_PORT_DQ_H;
                if(!(data&0x01)) DS18B20_PORT_DQ_L;
        delay_us(60);
        DS18B20_PORT_DQ_H;  // Keo chan DQ len muc '1'
        data>>=1;
    }    
}

/*-----------------------------------------------------------------------------
Noi Dung    :   Khoi tao DS18B20. 
Tham Bien   :   Khong. 
Tra Ve      :   Gia tri tra ve khi khoi tao xong cua DS18B20 (bit).
  -----------------------------------------------------------------------------*/
      
void DS18B20_Init(void)
{
    //GPIO_SetState(GPIOA,GPIO_Pin_15,GPIO_Mode_Out_PP);   // Cau hinh chan DQ la OUTPUT  
    DDRC.2 = 1;
    DS18B20_PORT_DQ_L;  // Keo DQ xuong muc '0' trong khoang 480us
    delay_us(500);       
    //GPIO_SetState(GPIOA,GPIO_Pin_15,GPIO_Mode_IPU);   // Cau hinh chan DQ la INPUT trong khoang 480us
    DDRC.2 = 0;
    PORTC.2 =1;
    delay_us(500);         
}

/*-----------------------------------------------------------------------------
Noi Dung    :   Ghi 1 byte lenh chuc nang vao DS18B20. 
Tham Bien   :   byte_func: byte lenh chuc nang cua DS18B20. 
Tra Ve      :   Khong.
  -----------------------------------------------------------------------------*/
  
void DS18B20_WriteFunc(unsigned char byte_func)
{
    DS18B20_Init();                 // Khoi tao DS18B20
    DS18B20_WriteByte(DS1820_CMD_SKIPROM);  // Truy cap thang den cac lenh chuc nang bo nho cua DS18B20
    DS18B20_WriteByte(byte_func);   // Viet ma lenh chuc nang
}

/*-----------------------------------------------------------------------------
Noi Dung    :   Cau hinh cho DS18B20. 
Tham Bien   :   temp_low: Gia tri nhiet do thap nhat de dua ra canh bao. 
                temp_high: Gia tri nhiet do cao nhat de dua ra canh bao.   
                resolution: Do phan giai cho DS18B20.(1|R1|R0|1|1|1|1|1)
Tra Ve      :   Khong.
  -----------------------------------------------------------------------------*/
  
void DS18B20_Config(unsigned char temp_low, unsigned char temp_high, unsigned char resolution)
{   
    resolution = (resolution<<5)|0x1f;  
    DS18B20_WriteFunc(DS1820_CMD_WRITESCRPAD);        // Cho phep ghi 3 byte vao bo nho nhap:    
    DS18B20_WriteByte(temp_high);   // byte 2: Th
    DS18B20_WriteByte(temp_low);    // byte 3: Tl 
    DS18B20_WriteByte(resolution);  // byte 4: configuration register
    DS18B20_WriteFunc(DS1820_CMD_COPYSCRPAD);        // Ghi vao EEPROM
}                       

/*-----------------------------------------------------------------------------
Noi Dung    :   Doc gia tri nhiet do do duoc cua DS18B20. 
Tham Bien   :   Khong. 
Tra Ve      :   Gia tri nhiet do do duoc.
  -----------------------------------------------------------------------------*/
                                              
float DS18B20_ReadTemp(void)
{
    float temp;
    unsigned char buff_temp1,buff_temp2; 
    
    DS18B20_WriteFunc(DS1820_CMD_CONVERTTEMP);  // Khoi dong qua trinh do va chuyen doi nhiet do ra so nhi phan
    delay_us(200);
    DS18B20_WriteFunc(DS1820_CMD_READSCRPAD);   // Doc du lieu tu bo nho DS18b20 
    
    buff_temp1 = DS18B20_ReadByte(); 
    temp=((float)(buff_temp1&0x0f))/16;            // Lay phan thuc cua gia tri nhiet do
    buff_temp2 = DS18B20_ReadByte();                 
    buff_temp1 =((buff_temp1&0xf0)>>4)|((buff_temp2&0x0f)<<4) ;    // Lay phan nguyen cua gia tri nhiet do
    temp=temp+buff_temp1;
    return temp;       
}    
  /*
void mekiep(unsigned char lsb,unsigned char msb)
{
    //float temp;
    //uint8_t buff_temp1,buff_temp2; 
    
    DS18B20_WriteFunc(DS1820_CMD_CONVERTTEMP);  // Khoi dong qua trinh do va chuyen doi nhiet do ra so nhi phan
    delay_us(200);
    DS18B20_WriteFunc(DS1820_CMD_READSCRPAD);   // Doc du lieu tu bo nho DS18b20 
    
    lsb = DS18B20_ReadByte(); 
    //temp=((float)(buff_temp1&0x0f))/16;            // Lay phan thuc cua gia tri nhiet do
    msb = DS18B20_ReadByte();                 
    //buff_temp1 =((buff_temp1&0xf0)>>4)|((buff_temp2&0x0f)<<4) ;    // Lay phan nguyen cua gia tri nhiet do
    //temp=temp+buff_temp1;       
}    */
