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

#include "ds18b20.h"
#include <mega88a.h>
#include <delay.h>


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
}
/*------------------------------KET THUC FILE-------------------------------
______________________________MinhHa R&D Team______________________________*/

