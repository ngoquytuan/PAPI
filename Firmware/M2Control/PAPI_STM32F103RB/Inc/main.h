/**
  ******************************************************************************
  * File Name          : main.hpp
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
#define CLK_Pin GPIO_PIN_12
#define CLK_GPIO_Port GPIOB
#define DIO_Pin GPIO_PIN_13
#define DIO_GPIO_Port GPIOB
#define RS485_1RD_Pin GPIO_PIN_8
#define RS485_1RD_GPIO_Port GPIOA
#define RF_NSS_Pin GPIO_PIN_6
#define RF_NSS_GPIO_Port GPIOB
#define RF_RST_Pin GPIO_PIN_7
#define RF_RST_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */
#define RF_NSS(stage)  HAL_GPIO_WritePin(RF_NSS_GPIO_Port, RF_NSS_Pin, stage)
#define RF_RST(stage)  HAL_GPIO_WritePin(RF_RST_GPIO_Port, RF_RST_Pin, stage)

#define LORA_DEFAULT_SS_PIN    RFM_NSS
#define LORA_DEFAULT_RESET_PIN RFM_RST
#define LORA_DEFAULT_DIO0_PIN  RFM_DIO0

#define PA_OUTPUT_RFO_PIN      0
#define PA_OUTPUT_PA_BOOST_PIN 1
#define	nIRQ0				(PINE&(4<<0))			//RFMxx nIRQ0
#define	HI(x)				PORTB |= (1<<(x))		//Output high
#define	LO(x)				PORTB &= ~(1<<(x))		//Output low
#define	nCS					4						//RFMxx module CS pin
#define	SCK					1						//RFMxx sck
#define	MISO				3						//RFMxx MISO
#define READbit             PINB.3
#define	MOSI				2						//RFMxx MOSI
#define	HOLD_ON_RESET		PORTB&=~(1<<5)
#define	RELEASE_FROM_RESET	PORTB|=(1<<5)
/*--------------------------------------------*/
// registers
#define REG_FIFO                 0x00
#define REG_OP_MODE              0x01
#define REG_FRF_MSB              0x06
#define REG_FRF_MID              0x07
#define REG_FRF_LSB              0x08
#define REG_PA_CONFIG            0x09
#define REG_LNA                  0x0c
#define REG_FIFO_ADDR_PTR        0x0d
#define REG_FIFO_TX_BASE_ADDR    0x0e
#define REG_FIFO_RX_BASE_ADDR    0x0f
#define REG_FIFO_RX_CURRENT_ADDR 0x10
#define REG_IRQ_FLAGS            0x12
#define REG_RX_NB_BYTES          0x13
#define REG_PKT_RSSI_VALUE       0x1a
#define REG_PKT_SNR_VALUE        0x1b
#define REG_MODEM_CONFIG_1       0x1d
#define REG_MODEM_CONFIG_2       0x1e
#define REG_PREAMBLE_MSB         0x20
#define REG_PREAMBLE_LSB         0x21
#define REG_PAYLOAD_LENGTH       0x22
#define REG_MODEM_CONFIG_3       0x26
#define REG_RSSI_WIDEBAND        0x2c
#define REG_DETECTION_OPTIMIZE   0x31
#define REG_DETECTION_THRESHOLD  0x37
#define REG_SYNC_WORD            0x39
#define REG_DIO_MAPPING_1        0x40
#define REG_VERSION              0x42

// modes
#define MODE_LONG_RANGE_MODE     0x80
#define MODE_SLEEP               0x00
#define MODE_STDBY               0x01
#define MODE_TX                  0x03
#define MODE_RX_CONTINUOUS       0x05
#define MODE_RX_SINGLE           0x06

// PA config
#define PA_BOOST                 0x80

// IRQ masks
#define IRQ_TX_DONE_MASK           0x08
#define IRQ_PAYLOAD_CRC_ERROR_MASK 0x20
#define IRQ_RX_DONE_MASK           0x40

#define MAX_PKT_LENGTH           255
/*--------------------------------------------*/


#define RS485_TX1 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
#define RS485_RX1 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

#define RS485_TX2 HAL_GPIO_WritePin(GPIOA, RS485_2RD_Pin, GPIO_PIN_SET);
#define RS485_RX2 HAL_GPIO_WritePin(GPIOA, RS485_2RD_Pin, GPIO_PIN_RESET);

#define RX1SIZE 20
#define LOOP2RUN 100

#define RX2SIZE 22
#define LOOP3RUN 100

#define LAMP_OFF HAL_GPIO_WritePin(CRL8_GPIO_Port, CRL8_Pin, GPIO_PIN_SET);
#define LAMP_ON  HAL_GPIO_WritePin(CRL8_GPIO_Port, CRL8_Pin, GPIO_PIN_RESET);
//for looping
#define ONTIME        1
#define STOP          0

//PAPI ID
#define PAPI_MASTER 0x00
#define PAPI_SLAVE1 0x01
#define PAPI_SLAVE2 0x02
#define PAPI_SLAVE3 0x03

unsigned char HEXInStringToDec(unsigned char data);
void _send_papi_info_(void);

#define LAMP_OFF HAL_GPIO_WritePin(CRL8_GPIO_Port, CRL8_Pin, GPIO_PIN_SET);
#define LAMP_ON  HAL_GPIO_WritePin(CRL8_GPIO_Port, CRL8_Pin, GPIO_PIN_RESET);

#define ERR1   HAL_GPIO_WritePin(CRL7_GPIO_Port, CRL7_Pin, GPIO_PIN_SET);
#define NERR1  HAL_GPIO_WritePin(CRL7_GPIO_Port, CRL7_Pin, GPIO_PIN_RESET);

#define ERR2   HAL_GPIO_WritePin(CRL6_GPIO_Port, CRL6_Pin, GPIO_PIN_SET);
#define NERR2  HAL_GPIO_WritePin(CRL6_GPIO_Port, CRL6_Pin, GPIO_PIN_RESET);

#define ERR3   HAL_GPIO_WritePin(CRL5_GPIO_Port, CRL5_Pin, GPIO_PIN_SET);
#define NERR3  HAL_GPIO_WritePin(CRL5_GPIO_Port, CRL5_Pin, GPIO_PIN_RESET);

#define ERR4   HAL_GPIO_WritePin(CRL4_GPIO_Port, CRL4_Pin, GPIO_PIN_SET);
#define NERR4  HAL_GPIO_WritePin(CRL4_GPIO_Port, CRL4_Pin, GPIO_PIN_RESET);

#define ERR5   HAL_GPIO_WritePin(CRL3_GPIO_Port, CRL3_Pin, GPIO_PIN_SET);
#define NERR5  HAL_GPIO_WritePin(CRL3_GPIO_Port, CRL3_Pin, GPIO_PIN_RESET);

#define ERR9   HAL_GPIO_WritePin(CRL2_GPIO_Port, CRL2_Pin, GPIO_PIN_SET);
#define NERR9  HAL_GPIO_WritePin(CRL2_GPIO_Port, CRL2_Pin, GPIO_PIN_RESET);

#define DRY_NO_ERR NERR1;NERR2;NERR3;NERR4;NERR5;NERR9;


/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
