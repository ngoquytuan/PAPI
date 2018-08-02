/*****************************************************************************
    *Ten Tep                :     ds18b20.h
    *Ngay                   :     10/10/2017
    *Tac Gia                :     Tuan swat
    Chip type               : ATmega88PA
    Program type            : Application
    AVR Core Clock frequency: 16.000000 MHz
    Memory model            : Small
    External RAM size       : 0
    Data Stack size         : 256
    *Tom Tat                :     Khai bao cac ham giao tiep DS18B20
  ******************************************************************************
  * Chu Y        :   Phai dinh nghia chan DQ cua cam bien DS18B20 vao file main.h  
                     Chon do phan giai cho DS18B20 trong file ds18b20.h
    #define DS18B20_DDR_DQ  DDRC_1  // Cau hinh huong IN/OUT cho chan DQ
    #define DS18B20_PIN_DQ  PINC_1  // Xuat du lieu ra cho chan DQ
    #define DS18B20_PORT_DQ PORTC_1 // Doc du lieu vao chan DQ   
    
  * Vi Du        : 
    float32_t temp=0;   // Khai bao bien luu gia tri nhiet do
    DS18B20_Config(15,40,DS18B20_9BIT_RES); // Cau hinh cho DS18B20
    LCD_Init(); // Khoi tao LCD16x2
    
    temp=DS18B20_ReadTemp();    // Doc nhiet do tu DS18B20
    sprintf(str,"temp: %2.2f\xdfC  ",temp); // Hien thi len LCD16x2
    LCD_Gotoxy(0,1);
    LCD_Puts(str);                            
  ******************************************************************************
**/
#ifndef __DS18B20_H
#define __DS18B20_H


//#include "user.h"
void mekiep(unsigned char lsb,unsigned char msb);
//#include <stdio.h>

 
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
               
#endif 
/*------------------------------KET THUC FILE-------------------------------






