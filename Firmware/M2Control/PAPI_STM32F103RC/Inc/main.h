/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define BT1_Pin GPIO_PIN_0
#define BT1_GPIO_Port GPIOC
#define BT2_Pin GPIO_PIN_1
#define BT2_GPIO_Port GPIOC
#define RS485_2RD_Pin GPIO_PIN_4
#define RS485_2RD_GPIO_Port GPIOA
#define CRL8_Pin GPIO_PIN_5
#define CRL8_GPIO_Port GPIOA
#define CRL7_Pin GPIO_PIN_6
#define CRL7_GPIO_Port GPIOA
#define CRL6_Pin GPIO_PIN_7
#define CRL6_GPIO_Port GPIOA
#define CRL5_Pin GPIO_PIN_4
#define CRL5_GPIO_Port GPIOC
#define CRL4_Pin GPIO_PIN_5
#define CRL4_GPIO_Port GPIOC
#define CRL3_Pin GPIO_PIN_0
#define CRL3_GPIO_Port GPIOB
#define CRL2_Pin GPIO_PIN_1
#define CRL2_GPIO_Port GPIOB
#define CRL1_Pin GPIO_PIN_2
#define CRL1_GPIO_Port GPIOB
#define RS485_1RD_Pin GPIO_PIN_8
#define RS485_1RD_GPIO_Port GPIOA
#define RF_NSS_Pin GPIO_PIN_6
#define RF_NSS_GPIO_Port GPIOB
#define RF_RST_Pin GPIO_PIN_7
#define RF_RST_GPIO_Port GPIOB

#define RS485_TX1 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
#define RS485_RX1 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

#define LAMP_OFF HAL_GPIO_WritePin(CRL8_GPIO_Port, CRL8_Pin, GPIO_PIN_SET);
#define LAMP_ON  HAL_GPIO_WritePin(CRL8_GPIO_Port, CRL8_Pin, GPIO_PIN_RESET);

//time sticker..
#define ONTIME        1
#define STOP          0


#define NO_ERR     0  // Ko co loi
#define ERR_SENSOR 1	// Loi ko co cam bien
#define ERR_ANGLE  2  // Loi cam bien sai gia tri
#define ERR_COMMUN 3  // Loi mat ket noi den slave
#define ERR_SLAVE  4  // Loi slave gap su co ( mat cam bien hoac cam bien sai)

//slave error for master
#define LOST_CON       1 //Mat ket noi den slave
#define ERR_SEN        2 //Slave gap van de voi cam bien

//sensor error
#define ANG_ERR        1 //Cam bien loi
#define NO_SEN         2 //Ko co cam bien

extern unsigned char system_error;//(NO_ERR,ERR_SENSOR,ERR_ANGLE,ERR_COMMUN,ERR_SLAVE)
//sensor error
extern unsigned char sen_err;//(NO_ERR,ANG_ERR,NO_SEN)
//slave error
extern unsigned char sla_err;//(NO_ERR,LOST_CON,ERR_SEN)

//Khoi tao cac gia tri error ban dau cho cac den slave
void slave_stt_init(void);
//kiem tra den co bi loi angle ko?
void check_angle_err(void);
//boc tach du lieu tu cam bien gui ve
void find_data(void);

unsigned char HEXInStringToDec(unsigned char data);


/* USER CODE BEGIN Private defines */
extern char checku2;
/* USER CODE END Private defines */

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
