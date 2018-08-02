
;CodeVisionAVR C Compiler V3.12 Advanced
;(C) Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Build configuration    : Debug
;Chip type              : ATmega88PA
;Program type           : Application
;Clock frequency        : 16.000000 MHz
;Memory model           : Small
;Optimize for           : Speed
;(s)printf features     : int, width
;(s)scanf features      : int, width
;External RAM size      : 0
;Data Stack size        : 256 byte(s)
;Heap size              : 0 byte(s)
;Promote 'char' to 'int': Yes
;'char' is unsigned     : Yes
;8 bit enums            : Yes
;Global 'const' stored in FLASH: Yes
;Enhanced function parameter passing: Yes
;Enhanced core instructions: On
;Automatic register allocation for global variables: On
;Smart register allocation: On

	#define _MODEL_SMALL_

	#pragma AVRPART ADMIN PART_NAME ATmega88PA
	#pragma AVRPART MEMORY PROG_FLASH 8192
	#pragma AVRPART MEMORY EEPROM 512
	#pragma AVRPART MEMORY INT_SRAM SIZE 1024
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x100

	.LISTMAC
	.EQU EERE=0x0
	.EQU EEWE=0x1
	.EQU EEMWE=0x2
	.EQU UDRE=0x5
	.EQU RXC=0x7
	.EQU EECR=0x1F
	.EQU EEDR=0x20
	.EQU EEARL=0x21
	.EQU EEARH=0x22
	.EQU SPSR=0x2D
	.EQU SPDR=0x2E
	.EQU SMCR=0x33
	.EQU MCUSR=0x34
	.EQU MCUCR=0x35
	.EQU WDTCSR=0x60
	.EQU UCSR0A=0xC0
	.EQU UDR0=0xC6
	.EQU SPL=0x3D
	.EQU SPH=0x3E
	.EQU SREG=0x3F
	.EQU GPIOR0=0x1E

	.DEF R0X0=R0
	.DEF R0X1=R1
	.DEF R0X2=R2
	.DEF R0X3=R3
	.DEF R0X4=R4
	.DEF R0X5=R5
	.DEF R0X6=R6
	.DEF R0X7=R7
	.DEF R0X8=R8
	.DEF R0X9=R9
	.DEF R0XA=R10
	.DEF R0XB=R11
	.DEF R0XC=R12
	.DEF R0XD=R13
	.DEF R0XE=R14
	.DEF R0XF=R15
	.DEF R0X10=R16
	.DEF R0X11=R17
	.DEF R0X12=R18
	.DEF R0X13=R19
	.DEF R0X14=R20
	.DEF R0X15=R21
	.DEF R0X16=R22
	.DEF R0X17=R23
	.DEF R0X18=R24
	.DEF R0X19=R25
	.DEF R0X1A=R26
	.DEF R0X1B=R27
	.DEF R0X1C=R28
	.DEF R0X1D=R29
	.DEF R0X1E=R30
	.DEF R0X1F=R31

	.EQU __SRAM_START=0x0100
	.EQU __SRAM_END=0x04FF
	.EQU __DSTACK_SIZE=0x0100
	.EQU __HEAP_SIZE=0x0000
	.EQU __CLEAR_SRAM_SIZE=__SRAM_END-__SRAM_START+1

	.MACRO __CPD1N
	CPI  R30,LOW(@0)
	LDI  R26,HIGH(@0)
	CPC  R31,R26
	LDI  R26,BYTE3(@0)
	CPC  R22,R26
	LDI  R26,BYTE4(@0)
	CPC  R23,R26
	.ENDM

	.MACRO __CPD2N
	CPI  R26,LOW(@0)
	LDI  R30,HIGH(@0)
	CPC  R27,R30
	LDI  R30,BYTE3(@0)
	CPC  R24,R30
	LDI  R30,BYTE4(@0)
	CPC  R25,R30
	.ENDM

	.MACRO __CPWRR
	CP   R@0,R@2
	CPC  R@1,R@3
	.ENDM

	.MACRO __CPWRN
	CPI  R@0,LOW(@2)
	LDI  R30,HIGH(@2)
	CPC  R@1,R30
	.ENDM

	.MACRO __ADDB1MN
	SUBI R30,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDB2MN
	SUBI R26,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDW1MN
	SUBI R30,LOW(-@0-(@1))
	SBCI R31,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW2MN
	SUBI R26,LOW(-@0-(@1))
	SBCI R27,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	SBCI R22,BYTE3(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1N
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	SBCI R22,BYTE3(-@0)
	SBCI R23,BYTE4(-@0)
	.ENDM

	.MACRO __ADDD2N
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	SBCI R24,BYTE3(-@0)
	SBCI R25,BYTE4(-@0)
	.ENDM

	.MACRO __SUBD1N
	SUBI R30,LOW(@0)
	SBCI R31,HIGH(@0)
	SBCI R22,BYTE3(@0)
	SBCI R23,BYTE4(@0)
	.ENDM

	.MACRO __SUBD2N
	SUBI R26,LOW(@0)
	SBCI R27,HIGH(@0)
	SBCI R24,BYTE3(@0)
	SBCI R25,BYTE4(@0)
	.ENDM

	.MACRO __ANDBMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ANDWMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ANDI R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ANDD1N
	ANDI R30,LOW(@0)
	ANDI R31,HIGH(@0)
	ANDI R22,BYTE3(@0)
	ANDI R23,BYTE4(@0)
	.ENDM

	.MACRO __ANDD2N
	ANDI R26,LOW(@0)
	ANDI R27,HIGH(@0)
	ANDI R24,BYTE3(@0)
	ANDI R25,BYTE4(@0)
	.ENDM

	.MACRO __ORBMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ORWMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ORI  R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ORD1N
	ORI  R30,LOW(@0)
	ORI  R31,HIGH(@0)
	ORI  R22,BYTE3(@0)
	ORI  R23,BYTE4(@0)
	.ENDM

	.MACRO __ORD2N
	ORI  R26,LOW(@0)
	ORI  R27,HIGH(@0)
	ORI  R24,BYTE3(@0)
	ORI  R25,BYTE4(@0)
	.ENDM

	.MACRO __DELAY_USB
	LDI  R24,LOW(@0)
__DELAY_USB_LOOP:
	DEC  R24
	BRNE __DELAY_USB_LOOP
	.ENDM

	.MACRO __DELAY_USW
	LDI  R24,LOW(@0)
	LDI  R25,HIGH(@0)
__DELAY_USW_LOOP:
	SBIW R24,1
	BRNE __DELAY_USW_LOOP
	.ENDM

	.MACRO __GETD1S
	LDD  R30,Y+@0
	LDD  R31,Y+@0+1
	LDD  R22,Y+@0+2
	LDD  R23,Y+@0+3
	.ENDM

	.MACRO __GETD2S
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	LDD  R24,Y+@0+2
	LDD  R25,Y+@0+3
	.ENDM

	.MACRO __PUTD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R31
	STD  Y+@0+2,R22
	STD  Y+@0+3,R23
	.ENDM

	.MACRO __PUTD2S
	STD  Y+@0,R26
	STD  Y+@0+1,R27
	STD  Y+@0+2,R24
	STD  Y+@0+3,R25
	.ENDM

	.MACRO __PUTDZ2
	STD  Z+@0,R26
	STD  Z+@0+1,R27
	STD  Z+@0+2,R24
	STD  Z+@0+3,R25
	.ENDM

	.MACRO __CLRD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R30
	STD  Y+@0+2,R30
	STD  Y+@0+3,R30
	.ENDM

	.MACRO __POINTB1MN
	LDI  R30,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW1MN
	LDI  R30,LOW(@0+(@1))
	LDI  R31,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTD1M
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __POINTW1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	LDI  R22,BYTE3(2*@0+(@1))
	LDI  R23,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTB2MN
	LDI  R26,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW2MN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTW2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	LDI  R24,BYTE3(2*@0+(@1))
	LDI  R25,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTBRM
	LDI  R@0,LOW(@1)
	.ENDM

	.MACRO __POINTWRM
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __POINTBRMN
	LDI  R@0,LOW(@1+(@2))
	.ENDM

	.MACRO __POINTWRMN
	LDI  R@0,LOW(@2+(@3))
	LDI  R@1,HIGH(@2+(@3))
	.ENDM

	.MACRO __POINTWRFN
	LDI  R@0,LOW(@2*2+(@3))
	LDI  R@1,HIGH(@2*2+(@3))
	.ENDM

	.MACRO __GETD1N
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __GETD2N
	LDI  R26,LOW(@0)
	LDI  R27,HIGH(@0)
	LDI  R24,BYTE3(@0)
	LDI  R25,BYTE4(@0)
	.ENDM

	.MACRO __GETB1MN
	LDS  R30,@0+(@1)
	.ENDM

	.MACRO __GETB1HMN
	LDS  R31,@0+(@1)
	.ENDM

	.MACRO __GETW1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	.ENDM

	.MACRO __GETD1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	LDS  R22,@0+(@1)+2
	LDS  R23,@0+(@1)+3
	.ENDM

	.MACRO __GETBRMN
	LDS  R@0,@1+(@2)
	.ENDM

	.MACRO __GETWRMN
	LDS  R@0,@2+(@3)
	LDS  R@1,@2+(@3)+1
	.ENDM

	.MACRO __GETWRZ
	LDD  R@0,Z+@2
	LDD  R@1,Z+@2+1
	.ENDM

	.MACRO __GETD2Z
	LDD  R26,Z+@0
	LDD  R27,Z+@0+1
	LDD  R24,Z+@0+2
	LDD  R25,Z+@0+3
	.ENDM

	.MACRO __GETB2MN
	LDS  R26,@0+(@1)
	.ENDM

	.MACRO __GETW2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	.ENDM

	.MACRO __GETD2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	LDS  R24,@0+(@1)+2
	LDS  R25,@0+(@1)+3
	.ENDM

	.MACRO __PUTB1MN
	STS  @0+(@1),R30
	.ENDM

	.MACRO __PUTW1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	.ENDM

	.MACRO __PUTD1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	STS  @0+(@1)+2,R22
	STS  @0+(@1)+3,R23
	.ENDM

	.MACRO __PUTB1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	RCALL __EEPROMWRB
	.ENDM

	.MACRO __PUTW1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	RCALL __EEPROMWRW
	.ENDM

	.MACRO __PUTD1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	RCALL __EEPROMWRD
	.ENDM

	.MACRO __PUTBR0MN
	STS  @0+(@1),R0
	.ENDM

	.MACRO __PUTBMRN
	STS  @0+(@1),R@2
	.ENDM

	.MACRO __PUTWMRN
	STS  @0+(@1),R@2
	STS  @0+(@1)+1,R@3
	.ENDM

	.MACRO __PUTBZR
	STD  Z+@1,R@0
	.ENDM

	.MACRO __PUTWZR
	STD  Z+@2,R@0
	STD  Z+@2+1,R@1
	.ENDM

	.MACRO __GETW1R
	MOV  R30,R@0
	MOV  R31,R@1
	.ENDM

	.MACRO __GETW2R
	MOV  R26,R@0
	MOV  R27,R@1
	.ENDM

	.MACRO __GETWRN
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __PUTW1R
	MOV  R@0,R30
	MOV  R@1,R31
	.ENDM

	.MACRO __PUTW2R
	MOV  R@0,R26
	MOV  R@1,R27
	.ENDM

	.MACRO __ADDWRN
	SUBI R@0,LOW(-@2)
	SBCI R@1,HIGH(-@2)
	.ENDM

	.MACRO __ADDWRR
	ADD  R@0,R@2
	ADC  R@1,R@3
	.ENDM

	.MACRO __SUBWRN
	SUBI R@0,LOW(@2)
	SBCI R@1,HIGH(@2)
	.ENDM

	.MACRO __SUBWRR
	SUB  R@0,R@2
	SBC  R@1,R@3
	.ENDM

	.MACRO __ANDWRN
	ANDI R@0,LOW(@2)
	ANDI R@1,HIGH(@2)
	.ENDM

	.MACRO __ANDWRR
	AND  R@0,R@2
	AND  R@1,R@3
	.ENDM

	.MACRO __ORWRN
	ORI  R@0,LOW(@2)
	ORI  R@1,HIGH(@2)
	.ENDM

	.MACRO __ORWRR
	OR   R@0,R@2
	OR   R@1,R@3
	.ENDM

	.MACRO __EORWRR
	EOR  R@0,R@2
	EOR  R@1,R@3
	.ENDM

	.MACRO __GETWRS
	LDD  R@0,Y+@2
	LDD  R@1,Y+@2+1
	.ENDM

	.MACRO __PUTBSR
	STD  Y+@1,R@0
	.ENDM

	.MACRO __PUTWSR
	STD  Y+@2,R@0
	STD  Y+@2+1,R@1
	.ENDM

	.MACRO __MOVEWRR
	MOV  R@0,R@2
	MOV  R@1,R@3
	.ENDM

	.MACRO __INWR
	IN   R@0,@2
	IN   R@1,@2+1
	.ENDM

	.MACRO __OUTWR
	OUT  @2+1,R@1
	OUT  @2,R@0
	.ENDM

	.MACRO __CALL1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	ICALL
	.ENDM

	.MACRO __CALL1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	RCALL __GETW1PF
	ICALL
	.ENDM

	.MACRO __CALL2EN
	PUSH R26
	PUSH R27
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	RCALL __EEPROMRDW
	POP  R27
	POP  R26
	ICALL
	.ENDM

	.MACRO __CALL2EX
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	RCALL __EEPROMRDD
	ICALL
	.ENDM

	.MACRO __GETW1STACK
	IN   R30,SPL
	IN   R31,SPH
	ADIW R30,@0+1
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1STACK
	IN   R30,SPL
	IN   R31,SPH
	ADIW R30,@0+1
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z
	MOVW R30,R0
	.ENDM

	.MACRO __NBST
	BST  R@0,@1
	IN   R30,SREG
	LDI  R31,0x40
	EOR  R30,R31
	OUT  SREG,R30
	.ENDM


	.MACRO __PUTB1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RNS
	MOVW R26,R@0
	ADIW R26,@1
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	RCALL __PUTDP1
	.ENDM


	.MACRO __GETB1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R30,Z
	.ENDM

	.MACRO __GETB1HSX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	.ENDM

	.MACRO __GETW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z+
	LD   R23,Z
	MOVW R30,R0
	.ENDM

	.MACRO __GETB2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R26,X
	.ENDM

	.MACRO __GETW2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	.ENDM

	.MACRO __GETD2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R1,X+
	LD   R24,X+
	LD   R25,X
	MOVW R26,R0
	.ENDM

	.MACRO __GETBRSX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	LD   R@0,Z
	.ENDM

	.MACRO __GETWRSX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	LD   R@0,Z+
	LD   R@1,Z
	.ENDM

	.MACRO __GETBRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	LD   R@0,X
	.ENDM

	.MACRO __GETWRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	LD   R@0,X+
	LD   R@1,X
	.ENDM

	.MACRO __LSLW8SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	CLR  R30
	.ENDM

	.MACRO __PUTB1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __CLRW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __CLRD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R30
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __PUTB2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R26
	.ENDM

	.MACRO __PUTW2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z,R27
	.ENDM

	.MACRO __PUTD2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z+,R27
	ST   Z+,R24
	ST   Z,R25
	.ENDM

	.MACRO __PUTBSRX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	ST   Z,R@0
	.ENDM

	.MACRO __PUTWSRX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	ST   Z+,R@0
	ST   Z,R@1
	.ENDM

	.MACRO __PUTB1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __MULBRR
	MULS R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRRU
	MUL  R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRR0
	MULS R@0,R@1
	.ENDM

	.MACRO __MULBRRU0
	MUL  R@0,R@1
	.ENDM

	.MACRO __MULBNWRU
	LDI  R26,@2
	MUL  R26,R@0
	MOVW R30,R0
	MUL  R26,R@1
	ADD  R31,R0
	.ENDM

;NAME DEFINITIONS FOR GLOBAL VARIABLES ALLOCATED TO REGISTERS
	.DEF _cali_flag=R4
	.DEF _count=R3
	.DEF _newavgx=R5
	.DEF _newavgx_msb=R6
	.DEF _newavgy=R7
	.DEF _newavgy_msb=R8
	.DEF _avgx=R9
	.DEF _avgx_msb=R10
	.DEF _avgy=R11
	.DEF _avgy_msb=R12
	.DEF _send_avgx=R13
	.DEF _send_avgx_msb=R14

;GPIOR0 INITIALIZATION VALUE
	.EQU __GPIOR0_INIT=0x00

	.CSEG
	.ORG 0x00

;START OF CODE MARKER
__START_OF_CODE:

;INTERRUPT VECTORS
	RJMP __RESET
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP _timer0_ovf_isr
	RJMP 0x00
	RJMP _usart_rx_isr
	RJMP 0x00
	RJMP _usart_tx_isr
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00

_tbl10_G101:
	.DB  0x10,0x27,0xE8,0x3,0x64,0x0,0xA,0x0
	.DB  0x1,0x0
_tbl16_G101:
	.DB  0x0,0x10,0x0,0x1,0x10,0x0,0x1,0x0

_0x3:
	.DB  0x0,0x0,0x0,0x40
_0x4:
	.DB  0x0,0x0,0x0,0x40
_0x5:
	.DB  0xA,0xD7,0x23,0x3C
_0x6:
	.DB  0x0,0x0,0x0,0x40
_0x7:
	.DB  0x0,0x0,0x0,0x40
_0x8:
	.DB  0xA,0xD7,0x23,0x3C
_0x9:
	.DB  0xE8,0x3
_0xA:
	.DB  0x0,0x0,0x0,0x40
_0xB:
	.DB  0x0,0x0,0x0,0x40
_0xC:
	.DB  0xA,0xD7,0x23,0x3C
_0x0:
	.DB  0x50,0x41,0x50,0x49,0x20,0x73,0x65,0x6E
	.DB  0x73,0x6F,0x72,0x20,0xD,0xA,0x0,0x45
	.DB  0x45,0x50,0x52,0x4F,0x4D,0x20,0x69,0x73
	.DB  0x20,0x67,0x6F,0x6F,0x64,0x21,0x20,0x4F
	.DB  0x66,0x66,0x73,0x65,0x74,0x20,0x78,0x2C
	.DB  0x79,0x3A,0x25,0x64,0x2C,0x25,0x64,0xD
	.DB  0xA,0x20,0x0,0x45,0x45,0x50,0x52,0x4F
	.DB  0x4D,0x20,0x69,0x73,0x20,0x42,0x41,0x44
	.DB  0x21,0x20,0x4F,0x66,0x66,0x73,0x65,0x74
	.DB  0x20,0x78,0x2C,0x79,0x3A,0x25,0x64,0x2C
	.DB  0x25,0x64,0xD,0xA,0x20,0x0,0x53,0x3E
	.DB  0x25,0x30,0x34,0x58,0x2D,0x25,0x30,0x34
	.DB  0x58,0x2D,0x25,0x30,0x33,0x58,0x2D,0x25
	.DB  0x30,0x33,0x58,0x3C,0xD,0xA,0x20,0x20
	.DB  0x0,0x4F,0x66,0x66,0x73,0x65,0x74,0x20
	.DB  0x64,0x6F,0x6E,0x65,0x21,0x20,0x78,0x2C
	.DB  0x79,0x3A,0x25,0x64,0x2C,0x25,0x64,0xD
	.DB  0xA,0x20,0x0,0x43,0x61,0x6E,0x6E,0x6F
	.DB  0x74,0x20,0x6F,0x66,0x66,0x73,0x65,0x74
	.DB  0x20,0x78,0x2C,0x79,0x3A,0x25,0x64,0x2C
	.DB  0x25,0x64,0xD,0xA,0x20,0x0
_0x2000060:
	.DB  0x1
_0x2000000:
	.DB  0x2D,0x4E,0x41,0x4E,0x0,0x49,0x4E,0x46
	.DB  0x0

__GLOBAL_INI_TBL:
	.DW  0x04
	.DW  _X_err_measure
	.DW  _0x3*2

	.DW  0x04
	.DW  _X_err_estimate
	.DW  _0x4*2

	.DW  0x04
	.DW  _X_q
	.DW  _0x5*2

	.DW  0x04
	.DW  _Y_err_measure
	.DW  _0x6*2

	.DW  0x04
	.DW  _Y_err_estimate
	.DW  _0x7*2

	.DW  0x04
	.DW  _Y_q
	.DW  _0x8*2

	.DW  0x02
	.DW  _send_sensor_info
	.DW  _0x9*2

	.DW  0x04
	.DW  _ds_err_measure
	.DW  _0xA*2

	.DW  0x04
	.DW  _ds_err_estimate
	.DW  _0xB*2

	.DW  0x04
	.DW  _ds_q
	.DW  _0xC*2

	.DW  0x01
	.DW  __seed_G100
	.DW  _0x2000060*2

_0xFFFFFFFF:
	.DW  0

#define __GLOBAL_INI_TBL_PRESENT 1

__RESET:
	CLI
	CLR  R30
	OUT  EECR,R30

;INTERRUPT VECTORS ARE PLACED
;AT THE START OF FLASH
	LDI  R31,1
	OUT  MCUCR,R31
	OUT  MCUCR,R30

;CLEAR R2-R14
	LDI  R24,(14-2)+1
	LDI  R26,2
	CLR  R27
__CLEAR_REG:
	ST   X+,R30
	DEC  R24
	BRNE __CLEAR_REG

;CLEAR SRAM
	LDI  R24,LOW(__CLEAR_SRAM_SIZE)
	LDI  R25,HIGH(__CLEAR_SRAM_SIZE)
	LDI  R26,LOW(__SRAM_START)
	LDI  R27,HIGH(__SRAM_START)
__CLEAR_SRAM:
	ST   X+,R30
	SBIW R24,1
	BRNE __CLEAR_SRAM

;GLOBAL VARIABLES INITIALIZATION
	LDI  R30,LOW(__GLOBAL_INI_TBL*2)
	LDI  R31,HIGH(__GLOBAL_INI_TBL*2)
__GLOBAL_INI_NEXT:
	LPM  R24,Z+
	LPM  R25,Z+
	SBIW R24,0
	BREQ __GLOBAL_INI_END
	LPM  R26,Z+
	LPM  R27,Z+
	LPM  R0,Z+
	LPM  R1,Z+
	MOVW R22,R30
	MOVW R30,R0
__GLOBAL_INI_LOOP:
	LPM  R0,Z+
	ST   X+,R0
	SBIW R24,1
	BRNE __GLOBAL_INI_LOOP
	MOVW R30,R22
	RJMP __GLOBAL_INI_NEXT
__GLOBAL_INI_END:

;GPIOR0 INITIALIZATION
	LDI  R30,__GPIOR0_INIT
	OUT  GPIOR0,R30

;HARDWARE STACK POINTER INITIALIZATION
	LDI  R30,LOW(__SRAM_END-__HEAP_SIZE)
	OUT  SPL,R30
	LDI  R30,HIGH(__SRAM_END-__HEAP_SIZE)
	OUT  SPH,R30

;DATA STACK POINTER INITIALIZATION
	LDI  R28,LOW(__SRAM_START+__DSTACK_SIZE)
	LDI  R29,HIGH(__SRAM_START+__DSTACK_SIZE)

	RJMP _main

	.ESEG
	.ORG 0

	.DSEG
	.ORG 0x200

	.CSEG
;/*******************************************************
;This program was created by the
;CodeWizardAVR V3.12 Advanced
;Automatic Program Generator
;© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com
;
;Project : PAPI sensor
;Version : Final
;Date    : 3/12/2018
;Author  :
;Company :
;Comments:
;BAUD = 115200;
;Send CALI to calibrate the sensors, offset value is saved to eeprom after calibrate
;If eeprom is bad, LED is always light
;Mat 1 ky tu cuoi khi send uart do tx rs485
;Chip type               : ATmega88PA
;Program type            : Application
;AVR Core Clock frequency: 16.000000 MHz
;Memory model            : Small
;External RAM size       : 0
;Data Stack size         : 256
;*******************************************************/
;
;#include <mega88a.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x01
	.EQU __sm_mask=0x0E
	.EQU __sm_adc_noise_red=0x02
	.EQU __sm_powerdown=0x04
	.EQU __sm_powersave=0x06
	.EQU __sm_standby=0x0C
	.EQU __sm_ext_standby=0x0E
	.SET power_ctrl_reg=smcr
	#endif
;#include <stdlib.h>
;#include "mega88PapiSenConf.h"

	.DSEG

	.CSEG
_X_updateEstimate:
; .FSTART _X_updateEstimate
	RCALL __PUTPARD2
;	mea -> Y+0
	LDS  R30,_X_err_measure
	LDS  R31,_X_err_measure+1
	LDS  R22,_X_err_measure+2
	LDS  R23,_X_err_measure+3
	LDS  R26,_X_err_estimate
	LDS  R27,_X_err_estimate+1
	LDS  R24,_X_err_estimate+2
	LDS  R25,_X_err_estimate+3
	RCALL __ADDF12
	LDS  R26,_X_err_estimate
	LDS  R27,_X_err_estimate+1
	LDS  R24,_X_err_estimate+2
	LDS  R25,_X_err_estimate+3
	RCALL __DIVF21
	STS  _X_kalman_gain,R30
	STS  _X_kalman_gain+1,R31
	STS  _X_kalman_gain+2,R22
	STS  _X_kalman_gain+3,R23
	LDS  R26,_X_last_estimate
	LDS  R27,_X_last_estimate+1
	LDS  R24,_X_last_estimate+2
	LDS  R25,_X_last_estimate+3
	RCALL __GETD1S0
	RCALL __SUBF12
	LDS  R26,_X_kalman_gain
	LDS  R27,_X_kalman_gain+1
	LDS  R24,_X_kalman_gain+2
	LDS  R25,_X_kalman_gain+3
	RCALL __MULF12
	LDS  R26,_X_last_estimate
	LDS  R27,_X_last_estimate+1
	LDS  R24,_X_last_estimate+2
	LDS  R25,_X_last_estimate+3
	RCALL __ADDF12
	STS  _X_current_estimate,R30
	STS  _X_current_estimate+1,R31
	STS  _X_current_estimate+2,R22
	STS  _X_current_estimate+3,R23
	LDS  R26,_X_kalman_gain
	LDS  R27,_X_kalman_gain+1
	LDS  R24,_X_kalman_gain+2
	LDS  R25,_X_kalman_gain+3
	__GETD1N 0x3F800000
	RCALL __SUBF12
	LDS  R26,_X_err_estimate
	LDS  R27,_X_err_estimate+1
	LDS  R24,_X_err_estimate+2
	LDS  R25,_X_err_estimate+3
	RCALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	LDS  R26,_X_current_estimate
	LDS  R27,_X_current_estimate+1
	LDS  R24,_X_current_estimate+2
	LDS  R25,_X_current_estimate+3
	LDS  R30,_X_last_estimate
	LDS  R31,_X_last_estimate+1
	LDS  R22,_X_last_estimate+2
	LDS  R23,_X_last_estimate+3
	RCALL __SUBF12
	MOVW R26,R30
	MOVW R24,R22
	RCALL _fabs
	LDS  R26,_X_q
	LDS  R27,_X_q+1
	LDS  R24,_X_q+2
	LDS  R25,_X_q+3
	RCALL __MULF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	RCALL __ADDF12
	STS  _X_err_estimate,R30
	STS  _X_err_estimate+1,R31
	STS  _X_err_estimate+2,R22
	STS  _X_err_estimate+3,R23
	LDS  R30,_X_current_estimate
	LDS  R31,_X_current_estimate+1
	LDS  R22,_X_current_estimate+2
	LDS  R23,_X_current_estimate+3
	STS  _X_last_estimate,R30
	STS  _X_last_estimate+1,R31
	STS  _X_last_estimate+2,R22
	STS  _X_last_estimate+3,R23
	LDS  R30,_X_current_estimate
	LDS  R31,_X_current_estimate+1
	LDS  R22,_X_current_estimate+2
	LDS  R23,_X_current_estimate+3
	RCALL __CFD1
	RJMP _0x20C0003
; .FEND

	.DSEG

	.CSEG
_Y_updateEstimate:
; .FSTART _Y_updateEstimate
	RCALL __PUTPARD2
;	mea -> Y+0
	LDS  R30,_Y_err_measure
	LDS  R31,_Y_err_measure+1
	LDS  R22,_Y_err_measure+2
	LDS  R23,_Y_err_measure+3
	LDS  R26,_Y_err_estimate
	LDS  R27,_Y_err_estimate+1
	LDS  R24,_Y_err_estimate+2
	LDS  R25,_Y_err_estimate+3
	RCALL __ADDF12
	LDS  R26,_Y_err_estimate
	LDS  R27,_Y_err_estimate+1
	LDS  R24,_Y_err_estimate+2
	LDS  R25,_Y_err_estimate+3
	RCALL __DIVF21
	STS  _Y_kalman_gain,R30
	STS  _Y_kalman_gain+1,R31
	STS  _Y_kalman_gain+2,R22
	STS  _Y_kalman_gain+3,R23
	LDS  R26,_Y_last_estimate
	LDS  R27,_Y_last_estimate+1
	LDS  R24,_Y_last_estimate+2
	LDS  R25,_Y_last_estimate+3
	RCALL __GETD1S0
	RCALL __SUBF12
	LDS  R26,_Y_kalman_gain
	LDS  R27,_Y_kalman_gain+1
	LDS  R24,_Y_kalman_gain+2
	LDS  R25,_Y_kalman_gain+3
	RCALL __MULF12
	LDS  R26,_Y_last_estimate
	LDS  R27,_Y_last_estimate+1
	LDS  R24,_Y_last_estimate+2
	LDS  R25,_Y_last_estimate+3
	RCALL __ADDF12
	STS  _Y_current_estimate,R30
	STS  _Y_current_estimate+1,R31
	STS  _Y_current_estimate+2,R22
	STS  _Y_current_estimate+3,R23
	LDS  R26,_Y_kalman_gain
	LDS  R27,_Y_kalman_gain+1
	LDS  R24,_Y_kalman_gain+2
	LDS  R25,_Y_kalman_gain+3
	__GETD1N 0x3F800000
	RCALL __SUBF12
	LDS  R26,_Y_err_estimate
	LDS  R27,_Y_err_estimate+1
	LDS  R24,_Y_err_estimate+2
	LDS  R25,_Y_err_estimate+3
	RCALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	LDS  R26,_Y_current_estimate
	LDS  R27,_Y_current_estimate+1
	LDS  R24,_Y_current_estimate+2
	LDS  R25,_Y_current_estimate+3
	LDS  R30,_Y_last_estimate
	LDS  R31,_Y_last_estimate+1
	LDS  R22,_Y_last_estimate+2
	LDS  R23,_Y_last_estimate+3
	RCALL __SUBF12
	MOVW R26,R30
	MOVW R24,R22
	RCALL _fabs
	LDS  R26,_Y_q
	LDS  R27,_Y_q+1
	LDS  R24,_Y_q+2
	LDS  R25,_Y_q+3
	RCALL __MULF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	RCALL __ADDF12
	STS  _Y_err_estimate,R30
	STS  _Y_err_estimate+1,R31
	STS  _Y_err_estimate+2,R22
	STS  _Y_err_estimate+3,R23
	LDS  R30,_Y_current_estimate
	LDS  R31,_Y_current_estimate+1
	LDS  R22,_Y_current_estimate+2
	LDS  R23,_Y_current_estimate+3
	STS  _Y_last_estimate,R30
	STS  _Y_last_estimate+1,R31
	STS  _Y_last_estimate+2,R22
	STS  _Y_last_estimate+3,R23
	LDS  R30,_Y_current_estimate
	LDS  R31,_Y_current_estimate+1
	LDS  R22,_Y_current_estimate+2
	LDS  R23,_Y_current_estimate+3
	RCALL __CFD1
	RJMP _0x20C0003
; .FEND

	.DSEG

	.CSEG
_cal_tempu:
; .FSTART _cal_tempu
	ST   -Y,R26
	RCALL __SAVELOCR4
;	b1 -> Y+5
;	b2 -> Y+4
;	donvi -> R16,R17
;	hangchuc -> R18,R19
	LDD  R30,Y+5
	LDI  R31,0
	ANDI R30,LOW(0xF)
	ANDI R31,HIGH(0xF)
	MOVW R16,R30
	__MULBNWRU 16,17,5
	RCALL __LSRW3
	MOVW R16,R30
	LDD  R30,Y+5
	LDI  R31,0
	ANDI R30,LOW(0xF0)
	ANDI R31,HIGH(0xF0)
	RCALL __ASRW4
	MOVW R26,R30
	LDD  R30,Y+4
	LDI  R31,0
	ANDI R30,LOW(0xF)
	ANDI R31,HIGH(0xF)
	SWAP R30
	ANDI R30,0xF0
	OR   R30,R26
	OR   R31,R27
	LDI  R26,LOW(10)
	LDI  R27,HIGH(10)
	RCALL __MULW12
	MOVW R18,R30
	MOVW R30,R16
	ADD  R30,R18
	ADC  R31,R19
	STS  _new_tempu,R30
	STS  _new_tempu+1,R31
	LDS  R30,_ds_err_measure
	LDS  R31,_ds_err_measure+1
	LDS  R22,_ds_err_measure+2
	LDS  R23,_ds_err_measure+3
	LDS  R26,_ds_err_estimate
	LDS  R27,_ds_err_estimate+1
	LDS  R24,_ds_err_estimate+2
	LDS  R25,_ds_err_estimate+3
	RCALL __ADDF12
	LDS  R26,_ds_err_estimate
	LDS  R27,_ds_err_estimate+1
	LDS  R24,_ds_err_estimate+2
	LDS  R25,_ds_err_estimate+3
	RCALL __DIVF21
	STS  _ds_kalman_gain,R30
	STS  _ds_kalman_gain+1,R31
	STS  _ds_kalman_gain+2,R22
	STS  _ds_kalman_gain+3,R23
	LDS  R30,_ds_last_estimate
	LDS  R31,_ds_last_estimate+1
	LDS  R22,_ds_last_estimate+2
	LDS  R23,_ds_last_estimate+3
	LDS  R26,_new_tempu
	LDS  R27,_new_tempu+1
	CLR  R24
	CLR  R25
	RCALL __CDF2
	RCALL __SWAPD12
	RCALL __SUBF12
	LDS  R26,_ds_kalman_gain
	LDS  R27,_ds_kalman_gain+1
	LDS  R24,_ds_kalman_gain+2
	LDS  R25,_ds_kalman_gain+3
	RCALL __MULF12
	LDS  R26,_ds_last_estimate
	LDS  R27,_ds_last_estimate+1
	LDS  R24,_ds_last_estimate+2
	LDS  R25,_ds_last_estimate+3
	RCALL __ADDF12
	STS  _ds_current_estimate,R30
	STS  _ds_current_estimate+1,R31
	STS  _ds_current_estimate+2,R22
	STS  _ds_current_estimate+3,R23
	LDS  R26,_ds_kalman_gain
	LDS  R27,_ds_kalman_gain+1
	LDS  R24,_ds_kalman_gain+2
	LDS  R25,_ds_kalman_gain+3
	__GETD1N 0x3F800000
	RCALL __SUBF12
	LDS  R26,_ds_err_estimate
	LDS  R27,_ds_err_estimate+1
	LDS  R24,_ds_err_estimate+2
	LDS  R25,_ds_err_estimate+3
	RCALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	LDS  R26,_ds_current_estimate
	LDS  R27,_ds_current_estimate+1
	LDS  R24,_ds_current_estimate+2
	LDS  R25,_ds_current_estimate+3
	LDS  R30,_ds_last_estimate
	LDS  R31,_ds_last_estimate+1
	LDS  R22,_ds_last_estimate+2
	LDS  R23,_ds_last_estimate+3
	RCALL __SUBF12
	MOVW R26,R30
	MOVW R24,R22
	RCALL _fabs
	LDS  R26,_ds_q
	LDS  R27,_ds_q+1
	LDS  R24,_ds_q+2
	LDS  R25,_ds_q+3
	RCALL __MULF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	RCALL __ADDF12
	STS  _ds_err_estimate,R30
	STS  _ds_err_estimate+1,R31
	STS  _ds_err_estimate+2,R22
	STS  _ds_err_estimate+3,R23
	LDS  R30,_ds_current_estimate
	LDS  R31,_ds_current_estimate+1
	LDS  R22,_ds_current_estimate+2
	LDS  R23,_ds_current_estimate+3
	STS  _ds_last_estimate,R30
	STS  _ds_last_estimate+1,R31
	STS  _ds_last_estimate+2,R22
	STS  _ds_last_estimate+3,R23
	LDS  R30,_ds_current_estimate
	LDS  R31,_ds_current_estimate+1
	LDS  R22,_ds_current_estimate+2
	LDS  R23,_ds_current_estimate+3
	RCALL __CFD1
	RCALL __LOADLOCR4
	ADIW R28,6
	RET
; .FEND
;	data -> R17
_putchar:
; .FSTART _putchar
	ST   -Y,R26
;	c -> Y+0
_0x11:
	LDS  R26,_tx_counter0
	CPI  R26,LOW(0x8)
	BREQ _0x11
	cli
	LDS  R30,_tx_counter0
	CPI  R30,0
	BRNE _0x15
	LDS  R30,192
	ANDI R30,LOW(0x20)
	BRNE _0x14
_0x15:
	LDS  R30,_tx_wr_index0
	SUBI R30,-LOW(1)
	STS  _tx_wr_index0,R30
	SUBI R30,LOW(1)
	LDI  R31,0
	SUBI R30,LOW(-_tx_buffer0)
	SBCI R31,HIGH(-_tx_buffer0)
	LD   R26,Y
	STD  Z+0,R26
	LDS  R26,_tx_wr_index0
	CPI  R26,LOW(0x8)
	BRNE _0x17
	LDI  R30,LOW(0)
	STS  _tx_wr_index0,R30
_0x17:
	LDS  R30,_tx_counter0
	SUBI R30,-LOW(1)
	STS  _tx_counter0,R30
	RJMP _0x18
_0x14:
	LD   R30,Y
	STS  198,R30
_0x18:
	sei
	RJMP _0x20C0001
; .FEND
_init_hw:
; .FSTART _init_hw
	LDI  R30,LOW(44)
	OUT  0x4,R30
	LDI  R30,LOW(0)
	OUT  0x5,R30
	OUT  0x7,R30
	OUT  0x8,R30
	LDI  R30,LOW(12)
	OUT  0xA,R30
	LDI  R30,LOW(0)
	OUT  0xB,R30
	OUT  0x24,R30
	LDI  R30,LOW(3)
	OUT  0x25,R30
	LDI  R30,LOW(6)
	OUT  0x26,R30
	LDI  R30,LOW(0)
	OUT  0x27,R30
	OUT  0x28,R30
	STS  128,R30
	STS  129,R30
	STS  133,R30
	STS  132,R30
	STS  135,R30
	STS  134,R30
	STS  137,R30
	STS  136,R30
	STS  139,R30
	STS  138,R30
	STS  182,R30
	STS  176,R30
	STS  177,R30
	STS  178,R30
	STS  179,R30
	STS  180,R30
	LDI  R30,LOW(1)
	STS  110,R30
	LDI  R30,LOW(0)
	STS  111,R30
	STS  112,R30
	STS  105,R30
	OUT  0x1D,R30
	STS  104,R30
	LDI  R30,LOW(2)
	STS  192,R30
	LDI  R30,LOW(216)
	STS  193,R30
	LDI  R30,LOW(6)
	STS  194,R30
	LDI  R30,LOW(0)
	STS  197,R30
	LDI  R30,LOW(16)
	STS  196,R30
	LDI  R30,LOW(128)
	OUT  0x30,R30
	LDI  R30,LOW(0)
	STS  127,R30
	STS  126,R30
	LDI  R30,LOW(64)
	STS  124,R30
	LDI  R30,LOW(132)
	STS  122,R30
	LDI  R30,LOW(0)
	STS  123,R30
	LDI  R30,LOW(83)
	OUT  0x2C,R30
	LDI  R30,LOW(0)
	OUT  0x2D,R30
	STS  188,R30
	sei
	RET
; .FEND
;#include "SimpleKalmanFilter.h"
;#include <delay.h>
;// Standard Input/Output functions
;#include <stdio.h>
;// SPI functions
;#include <spi.h>
;// Declare your global variables here
;
;
;// Timer 0 overflow interrupt service routine
;interrupt [TIM0_OVF] void timer0_ovf_isr(void)
; 0000 0028 {
_timer0_ovf_isr:
; .FSTART _timer0_ovf_isr
	ST   -Y,R26
	ST   -Y,R27
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
; 0000 0029 // Reinitialize Timer 0 value
; 0000 002A TCNT0=0x06;
	LDI  R30,LOW(6)
	OUT  0x26,R30
; 0000 002B // Place your code here
; 0000 002C if(usart_time_out > ONTIME) usart_time_out--;
	LDS  R26,_usart_time_out
	LDS  R27,_usart_time_out+1
	SBIW R26,2
	BRLO _0x19
	LDI  R26,LOW(_usart_time_out)
	LDI  R27,HIGH(_usart_time_out)
	LD   R30,X+
	LD   R31,X+
	SBIW R30,1
	ST   -X,R31
	ST   -X,R30
; 0000 002D if(send_sensor_info > ONTIME) send_sensor_info--;
_0x19:
	LDS  R26,_send_sensor_info
	LDS  R27,_send_sensor_info+1
	SBIW R26,2
	BRLO _0x1A
	LDI  R26,LOW(_send_sensor_info)
	LDI  R27,HIGH(_send_sensor_info)
	LD   R30,X+
	LD   R31,X+
	SBIW R30,1
	ST   -X,R31
	ST   -X,R30
; 0000 002E if(count_cali > ONTIME) count_cali--;
_0x1A:
	LDS  R26,_count_cali
	LDS  R27,_count_cali+1
	SBIW R26,2
	BRLO _0x1B
	LDI  R26,LOW(_count_cali)
	LDI  R27,HIGH(_count_cali)
	LD   R30,X+
	LD   R31,X+
	SBIW R30,1
	ST   -X,R31
	ST   -X,R30
; 0000 002F }
_0x1B:
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	LD   R27,Y+
	LD   R26,Y+
	RETI
; .FEND
;
;
;
;// USART Receiver interrupt service routine
;interrupt [USART_RXC] void usart_rx_isr(void)
; 0000 0035 {
_usart_rx_isr:
; .FSTART _usart_rx_isr
	ST   -Y,R26
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
; 0000 0036 char status,data;
; 0000 0037 status=UCSR0A;
	RCALL __SAVELOCR2
;	status -> R17
;	data -> R16
	LDS  R17,192
; 0000 0038 data=UDR0;
	LDS  R16,198
; 0000 0039 if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
	MOV  R30,R17
	ANDI R30,LOW(0x1C)
	BRNE _0x1C
; 0000 003A    {
; 0000 003B    usart_time_out=10;// 10ms after the last character received will processing  rx_buffer0
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	STS  _usart_time_out,R30
	STS  _usart_time_out+1,R31
; 0000 003C    rx_buffer0[rx_wr_index0++]=data;
	LDS  R30,_rx_wr_index0
	SUBI R30,-LOW(1)
	STS  _rx_wr_index0,R30
	SUBI R30,LOW(1)
	LDI  R31,0
	SUBI R30,LOW(-_rx_buffer0)
	SBCI R31,HIGH(-_rx_buffer0)
	ST   Z,R16
; 0000 003D #if RX_BUFFER_SIZE0 == 256
; 0000 003E    // special case for receiver buffer size=256
; 0000 003F    if (++rx_counter0 == 0) rx_buffer_overflow0=1;
; 0000 0040 #else
; 0000 0041    if (rx_wr_index0 == RX_BUFFER_SIZE0) rx_wr_index0=0;
	LDS  R26,_rx_wr_index0
	CPI  R26,LOW(0x8)
	BRNE _0x1D
	LDI  R30,LOW(0)
	STS  _rx_wr_index0,R30
; 0000 0042    if (++rx_counter0 == RX_BUFFER_SIZE0)
_0x1D:
	LDS  R26,_rx_counter0
	SUBI R26,-LOW(1)
	STS  _rx_counter0,R26
	CPI  R26,LOW(0x8)
	BRNE _0x1E
; 0000 0043       {
; 0000 0044       rx_counter0=0;
	LDI  R30,LOW(0)
	STS  _rx_counter0,R30
; 0000 0045       rx_buffer_overflow0=1;
	SBI  0x1E,0
; 0000 0046       }
; 0000 0047 #endif
; 0000 0048    }
_0x1E:
; 0000 0049 }
_0x1C:
	LD   R16,Y+
	LD   R17,Y+
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	LD   R26,Y+
	RETI
; .FEND
;
;
;
;
;
;// USART Transmitter interrupt service routine
;interrupt [USART_TXC] void usart_tx_isr(void)
; 0000 0051 {
_usart_tx_isr:
; .FSTART _usart_tx_isr
	ST   -Y,R26
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
; 0000 0052 if (tx_counter0)
	LDS  R30,_tx_counter0
	CPI  R30,0
	BREQ _0x21
; 0000 0053    {
; 0000 0054    --tx_counter0;
	SUBI R30,LOW(1)
	STS  _tx_counter0,R30
; 0000 0055    UDR0=tx_buffer0[tx_rd_index0++];
	LDS  R30,_tx_rd_index0
	SUBI R30,-LOW(1)
	STS  _tx_rd_index0,R30
	SUBI R30,LOW(1)
	LDI  R31,0
	SUBI R30,LOW(-_tx_buffer0)
	SBCI R31,HIGH(-_tx_buffer0)
	LD   R30,Z
	STS  198,R30
; 0000 0056 #if TX_BUFFER_SIZE0 != 256
; 0000 0057    if (tx_rd_index0 == TX_BUFFER_SIZE0) tx_rd_index0=0;
	LDS  R26,_tx_rd_index0
	CPI  R26,LOW(0x8)
	BRNE _0x22
	LDI  R30,LOW(0)
	STS  _tx_rd_index0,R30
; 0000 0058 #endif
; 0000 0059    }
_0x22:
; 0000 005A if (!tx_counter0) RS485_TX(0);//Turn RS485 TX to RS485 RX
_0x21:
	LDS  R30,_tx_counter0
	CPI  R30,0
	BRNE _0x23
	CBI  0xB,2
; 0000 005B }
_0x23:
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	LD   R26,Y+
	RETI
; .FEND
;
;
;
;
;
;
;
;// Read the AD conversion result
;unsigned int read_adc(unsigned char adc_input)
; 0000 0065 {
_read_adc:
; .FSTART _read_adc
; 0000 0066 ADMUX=adc_input | ADC_VREF_TYPE;
	ST   -Y,R26
;	adc_input -> Y+0
	LD   R30,Y
	ORI  R30,0x40
	STS  124,R30
; 0000 0067 // Delay needed for the stabilization of the ADC input voltage
; 0000 0068 delay_us(10);
	__DELAY_USB 53
; 0000 0069 // Start the AD conversion
; 0000 006A ADCSRA|=(1<<ADSC);
	LDS  R30,122
	ORI  R30,0x40
	STS  122,R30
; 0000 006B // Wait for the AD conversion to complete
; 0000 006C while ((ADCSRA & (1<<ADIF))==0);
_0x26:
	LDS  R30,122
	ANDI R30,LOW(0x10)
	BREQ _0x26
; 0000 006D ADCSRA|=(1<<ADIF);
	LDS  R30,122
	ORI  R30,0x10
	STS  122,R30
; 0000 006E return ADCW;
	LDS  R30,120
	LDS  R31,120+1
	RJMP _0x20C0001
; 0000 006F }
; .FEND
;
;void get_sensor_data(void)
; 0000 0072 {
_get_sensor_data:
; .FSTART _get_sensor_data
; 0000 0073       DS18B20_WriteFunc(DS1820_CMD_CONVERTTEMP);  // Khoi dong qua trinh do va chuyen doi nhiet do ra so nhi phan
	LDI  R26,LOW(68)
	RCALL _DS18B20_WriteFunc
; 0000 0074       delay_us(200);
	__DELAY_USW 800
; 0000 0075       DS18B20_WriteFunc(DS1820_CMD_READSCRPAD);   // Doc du lieu tu bo nho DS18b20
	LDI  R26,LOW(190)
	RCALL _DS18B20_WriteFunc
; 0000 0076 
; 0000 0077       buff_temp1 = DS18B20_ReadByte();
	RCALL _DS18B20_ReadByte
	STS  _buff_temp1,R30
; 0000 0078       buff_temp2 = DS18B20_ReadByte();
	RCALL _DS18B20_ReadByte
	STS  _buff_temp2,R30
; 0000 0079       ds_temp = cal_tempu(buff_temp1,buff_temp2);
	LDS  R30,_buff_temp1
	ST   -Y,R30
	LDS  R26,_buff_temp2
	RCALL _cal_tempu
	STS  _ds_temp,R30
	STS  _ds_temp+1,R31
; 0000 007A       newavgx = read_SCA100TX();
	RCALL _read_SCA100TX
	__PUTW1R 5,6
; 0000 007B       avgx = X_updateEstimate(newavgx);
	RCALL __CWD1
	RCALL __CDF1
	MOVW R26,R30
	MOVW R24,R22
	RCALL _X_updateEstimate
	__PUTW1R 9,10
; 0000 007C       newavgy = read_SCA100TY();
	RCALL _read_SCA100TY
	__PUTW1R 7,8
; 0000 007D       avgy = Y_updateEstimate(newavgy);
	RCALL __CWD1
	RCALL __CDF1
	MOVW R26,R30
	MOVW R24,R22
	RCALL _Y_updateEstimate
	__PUTW1R 11,12
; 0000 007E       lux = read_adc(3);
	LDI  R26,LOW(3)
	RCALL _read_adc
	STS  _lux,R30
	STS  _lux+1,R31
; 0000 007F       lux = (int)(updateEstimate(lux));
	CLR  R22
	CLR  R23
	RCALL __CDF1
	MOVW R26,R30
	MOVW R24,R22
	RCALL _updateEstimate
	RCALL __CFD1
	STS  _lux,R30
	STS  _lux+1,R31
; 0000 0080 }
	RET
; .FEND
;void main(void)
; 0000 0082 {
_main:
; .FSTART _main
; 0000 0083 // Declare your local variables here
; 0000 0084 
; 0000 0085 // Crystal Oscillator division factor: 1
; 0000 0086 #pragma optsize-
; 0000 0087 CLKPR=(1<<CLKPCE);
	LDI  R30,LOW(128)
	STS  97,R30
; 0000 0088 CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
	LDI  R30,LOW(0)
	STS  97,R30
; 0000 0089 #ifdef _OPTIMIZE_SIZE_
; 0000 008A #pragma optsize+
; 0000 008B #endif
; 0000 008C 
; 0000 008D init_hw();
	RCALL _init_hw
; 0000 008E RS485_TX(1);
	SBI  0xB,2
; 0000 008F printf("PAPI sensor \r\n");
	__POINTW1FN _0x0,0
	ST   -Y,R31
	ST   -Y,R30
	LDI  R24,0
	RCALL _printf
	ADIW R28,2
; 0000 0090 if((check_eep1 =='O')&&(check_eep2 =='K'))//eeprom is good
	LDI  R26,LOW(_check_eep1)
	LDI  R27,HIGH(_check_eep1)
	RCALL __EEPROMRDB
	CPI  R30,LOW(0x4F)
	BRNE _0x2C
	LDI  R26,LOW(_check_eep2)
	LDI  R27,HIGH(_check_eep2)
	RCALL __EEPROMRDB
	CPI  R30,LOW(0x4B)
	BREQ _0x2D
_0x2C:
	RJMP _0x2B
_0x2D:
; 0000 0091 {
; 0000 0092  RS485_TX(1);
	SBI  0xB,2
; 0000 0093  printf("EEPROM is good! Offset x,y:%d,%d\r\n ",osrawx,osrawy);
	__POINTW1FN _0x0,15
	RJMP _0x7C
; 0000 0094 }
; 0000 0095 else
_0x2B:
; 0000 0096 {
; 0000 0097  RS485_TX(1);
	SBI  0xB,2
; 0000 0098  printf("EEPROM is BAD! Offset x,y:%d,%d\r\n ",osrawx,osrawy);
	__POINTW1FN _0x0,51
_0x7C:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R26,LOW(_osrawx)
	LDI  R27,HIGH(_osrawx)
	RCALL __EEPROMRDW
	RCALL __CWD1
	RCALL __PUTPARD1
	LDI  R26,LOW(_osrawy)
	LDI  R27,HIGH(_osrawy)
	RCALL __EEPROMRDW
	RCALL __CWD1
	RCALL __PUTPARD1
	LDI  R24,8
	RCALL _printf
	ADIW R28,10
; 0000 0099 
; 0000 009A }
; 0000 009B 
; 0000 009C DS18B20_Init();
	RCALL _DS18B20_Init
; 0000 009D DS18B20_Config(15,40,DS18B20_9BIT_RES); // Cau hinh cho DS18B20
	LDI  R30,LOW(15)
	ST   -Y,R30
	LDI  R30,LOW(40)
	ST   -Y,R30
	LDI  R26,LOW(0)
	RCALL _DS18B20_Config
; 0000 009E 
; 0000 009F count = 0 ;
	CLR  R3
; 0000 00A0 //avgx = read_SCA100TX();
; 0000 00A1 //avgy = read_SCA100TY();
; 0000 00A2 SimpleKalmanFilter(2.0,2.0,0.01);
	__GETD1N 0x40000000
	RCALL __PUTPARD1
	RCALL __PUTPARD1
	__GETD2N 0x3C23D70A
	RCALL _SimpleKalmanFilter
; 0000 00A3 get_sensor_data();
	RCALL _get_sensor_data
; 0000 00A4 get_sensor_data();
	RCALL _get_sensor_data
; 0000 00A5 get_sensor_data();
	RCALL _get_sensor_data
; 0000 00A6 get_sensor_data();
	RCALL _get_sensor_data
; 0000 00A7 get_sensor_data();
	RCALL _get_sensor_data
; 0000 00A8 get_sensor_data();
	RCALL _get_sensor_data
; 0000 00A9 get_sensor_data();
	RCALL _get_sensor_data
; 0000 00AA get_sensor_data();
	RCALL _get_sensor_data
; 0000 00AB get_sensor_data();
	RCALL _get_sensor_data
; 0000 00AC get_sensor_data();
	RCALL _get_sensor_data
; 0000 00AD while (1)
_0x33:
; 0000 00AE       {
; 0000 00AF       // Place your code here
; 0000 00B0       //If eeprom is bad, LED is always light
; 0000 00B1       if((check_eep1 !='O')&&(check_eep2 !='K'))//eeprom is good
	LDI  R26,LOW(_check_eep1)
	LDI  R27,HIGH(_check_eep1)
	RCALL __EEPROMRDB
	CPI  R30,LOW(0x4F)
	BREQ _0x37
	LDI  R26,LOW(_check_eep2)
	LDI  R27,HIGH(_check_eep2)
	RCALL __EEPROMRDB
	CPI  R30,LOW(0x4B)
	BRNE _0x38
_0x37:
	RJMP _0x36
_0x38:
; 0000 00B2       {
; 0000 00B3         LED(1);
	SBI  0xB,3
; 0000 00B4       }
; 0000 00B5       if(send_sensor_info == ONTIME) {
_0x36:
	LDS  R26,_send_sensor_info
	LDS  R27,_send_sensor_info+1
	SBIW R26,1
	BREQ PC+2
	RJMP _0x3B
; 0000 00B6       //check all sensors after send_sensor_info time
; 0000 00B7         send_sensor_info = 100;//0.1s
	LDI  R30,LOW(100)
	LDI  R31,HIGH(100)
	STS  _send_sensor_info,R30
	STS  _send_sensor_info+1,R31
; 0000 00B8         LED(1);
	SBI  0xB,3
; 0000 00B9         count++;
	INC  R3
; 0000 00BA         get_sensor_data();
	RCALL _get_sensor_data
; 0000 00BB         //tinh gia tri trung binh 10 lan do
; 0000 00BC         if(count == 10)
	LDI  R30,LOW(10)
	CP   R30,R3
	BREQ PC+2
	RJMP _0x3E
; 0000 00BD         {
; 0000 00BE 
; 0000 00BF             count = 0;
	CLR  R3
; 0000 00C0             send_avgx = (avgx-osrawx)*5;
	LDI  R26,LOW(_osrawx)
	LDI  R27,HIGH(_osrawx)
	RCALL __EEPROMRDW
	MOVW R26,R30
	__GETW1R 9,10
	SUB  R30,R26
	SBC  R31,R27
	LDI  R26,LOW(5)
	LDI  R27,HIGH(5)
	RCALL __MULW12
	__PUTW1R 13,14
; 0000 00C1             send_avgy = (avgy-osrawy)*5;
	LDI  R26,LOW(_osrawy)
	LDI  R27,HIGH(_osrawy)
	RCALL __EEPROMRDW
	MOVW R26,R30
	__GETW1R 11,12
	SUB  R30,R26
	SBC  R31,R27
	LDI  R26,LOW(5)
	LDI  R27,HIGH(5)
	RCALL __MULW12
	STS  _send_avgy,R30
	STS  _send_avgy+1,R31
; 0000 00C2             //>> du lieu da x5 => de ra goc thi chi can x7 vi 5x7 =35
; 0000 00C3             RS485_TX(1);
	SBI  0xB,2
; 0000 00C4             //truyen thang gia tri co dau
; 0000 00C5             //printf("S>%d-%d-%d-%d<\r\n  ",(send_avgx),(send_avgy),cal_tempu(buff_temp1,buff_temp2),lux);
; 0000 00C6             printf("S>%04X-%04X-%03X-%03X<\r\n  ",(send_avgx),(send_avgy),cal_tempu(buff_temp1,buff_temp2),lux);
	__POINTW1FN _0x0,86
	ST   -Y,R31
	ST   -Y,R30
	__GETW1R 13,14
	RCALL __CWD1
	RCALL __PUTPARD1
	LDS  R30,_send_avgy
	LDS  R31,_send_avgy+1
	RCALL __CWD1
	RCALL __PUTPARD1
	LDS  R30,_buff_temp1
	ST   -Y,R30
	LDS  R26,_buff_temp2
	RCALL _cal_tempu
	CLR  R22
	CLR  R23
	RCALL __PUTPARD1
	LDS  R30,_lux
	LDS  R31,_lux+1
	CLR  R22
	CLR  R23
	RCALL __PUTPARD1
	LDI  R24,16
	RCALL _printf
	ADIW R28,18
; 0000 00C7         }
; 0000 00C8         //RS485_TX(1);
; 0000 00C9         //truyen thang gia tri co dau
; 0000 00CA         //printf("S>%04X-%04X-%03X-%03X<\r\n  ",(send_avgx),(send_avgy),cal_tempu(buff_temp1,buff_temp2),lux);
; 0000 00CB         //printf("%d,%d\r\n ",lux,(int)(u_kalman));
; 0000 00CC         //ds_temp = cal_tempu(buff_temp1,buff_temp2);
; 0000 00CD         //printf("%d\r\n ",ds_temp);
; 0000 00CE         //printf("%d,%d\r\n ",newavgx,avgx);
; 0000 00CF         //printf("%d,%d,%d,%d\r\n ",avgx,avgy,ds_temp,(int)(u_kalman));
; 0000 00D0         LED(0);
_0x3E:
	CBI  0xB,3
; 0000 00D1       }
; 0000 00D2       if(usart_time_out == ONTIME) {
_0x3B:
	LDS  R26,_usart_time_out
	LDS  R27,_usart_time_out+1
	SBIW R26,1
	BRNE _0x43
; 0000 00D3         usart_time_out = STOPTHIS;
	LDI  R30,LOW(0)
	STS  _usart_time_out,R30
	STS  _usart_time_out+1,R30
; 0000 00D4         process_uart_mess();
	RCALL _process_uart_mess
; 0000 00D5         //RS485_TX(1);
; 0000 00D6         //printf("????\r\n ");
; 0000 00D7       }
; 0000 00D8       if(cali_flag == ONTIME)//hieu chinh cam bien
_0x43:
	LDI  R30,LOW(1)
	CP   R30,R4
	BRNE _0x44
; 0000 00D9       {
; 0000 00DA         count_cali = 2000;
	LDI  R30,LOW(2000)
	LDI  R31,HIGH(2000)
	STS  _count_cali,R30
	STS  _count_cali+1,R31
; 0000 00DB         cali_flag = STOPTHIS;
	CLR  R4
; 0000 00DC         LED(1);
	SBI  0xB,3
; 0000 00DD         send_sensor_info = STOPTHIS;
	LDI  R30,LOW(0)
	STS  _send_sensor_info,R30
	STS  _send_sensor_info+1,R30
; 0000 00DE         //RS485_TX(1);
; 0000 00DF         //printf("Begin cali\r\n ");
; 0000 00E0         check_eep1 = '?';
	LDI  R26,LOW(_check_eep1)
	LDI  R27,HIGH(_check_eep1)
	LDI  R30,LOW(63)
	RCALL __EEPROMWRB
; 0000 00E1         check_eep2 = '?';
	LDI  R26,LOW(_check_eep2)
	LDI  R27,HIGH(_check_eep2)
	RCALL __EEPROMWRB
; 0000 00E2         delay_ms(100);
	LDI  R26,LOW(100)
	LDI  R27,0
	RCALL _delay_ms
; 0000 00E3       };
_0x44:
; 0000 00E4       if(count_cali > ONTIME)
	LDS  R26,_count_cali
	LDS  R27,_count_cali+1
	SBIW R26,2
	BRSH PC+2
	RJMP _0x47
; 0000 00E5       {
; 0000 00E6         delay_ms(1000);
	LDI  R26,LOW(1000)
	LDI  R27,HIGH(1000)
	RCALL _delay_ms
; 0000 00E7         get_sensor_data();delay_ms(100);
	RCALL _get_sensor_data
	LDI  R26,LOW(100)
	LDI  R27,0
	RCALL _delay_ms
; 0000 00E8         get_sensor_data();delay_ms(100);
	RCALL _get_sensor_data
	LDI  R26,LOW(100)
	LDI  R27,0
	RCALL _delay_ms
; 0000 00E9         get_sensor_data();delay_ms(100);
	RCALL _get_sensor_data
	LDI  R26,LOW(100)
	LDI  R27,0
	RCALL _delay_ms
; 0000 00EA         get_sensor_data();delay_ms(100);
	RCALL _get_sensor_data
	LDI  R26,LOW(100)
	LDI  R27,0
	RCALL _delay_ms
; 0000 00EB         get_sensor_data();delay_ms(100);
	RCALL _get_sensor_data
	LDI  R26,LOW(100)
	LDI  R27,0
	RCALL _delay_ms
; 0000 00EC         get_sensor_data();delay_ms(100);
	RCALL _get_sensor_data
	LDI  R26,LOW(100)
	LDI  R27,0
	RCALL _delay_ms
; 0000 00ED         get_sensor_data();delay_ms(100);
	RCALL _get_sensor_data
	LDI  R26,LOW(100)
	LDI  R27,0
	RCALL _delay_ms
; 0000 00EE         get_sensor_data();delay_ms(100);
	RCALL _get_sensor_data
	LDI  R26,LOW(100)
	LDI  R27,0
	RCALL _delay_ms
; 0000 00EF         get_sensor_data();delay_ms(100);
	RCALL _get_sensor_data
	LDI  R26,LOW(100)
	LDI  R27,0
	RCALL _delay_ms
; 0000 00F0         get_sensor_data();delay_ms(100);
	RCALL _get_sensor_data
	LDI  R26,LOW(100)
	LDI  R27,0
	RCALL _delay_ms
; 0000 00F1         osrawx = avgx;
	__GETW1R 9,10
	LDI  R26,LOW(_osrawx)
	LDI  R27,HIGH(_osrawx)
	RCALL __EEPROMWRW
; 0000 00F2         osrawy = avgy;
	__GETW1R 11,12
	LDI  R26,LOW(_osrawy)
	LDI  R27,HIGH(_osrawy)
	RCALL __EEPROMWRW
; 0000 00F3         count_cali = ONTIME;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	STS  _count_cali,R30
	STS  _count_cali+1,R31
; 0000 00F4         RS485_TX(1);
	SBI  0xB,2
; 0000 00F5         printf("Offset done! x,y:%d,%d\r\n ",osrawx,osrawy);
	__POINTW1FN _0x0,113
	ST   -Y,R31
	ST   -Y,R30
	LDI  R26,LOW(_osrawx)
	LDI  R27,HIGH(_osrawx)
	RCALL __EEPROMRDW
	RCALL __CWD1
	RCALL __PUTPARD1
	LDI  R26,LOW(_osrawy)
	LDI  R27,HIGH(_osrawy)
	RCALL __EEPROMRDW
	RCALL __CWD1
	RCALL __PUTPARD1
	LDI  R24,8
	RCALL _printf
	ADIW R28,10
; 0000 00F6         check_eep1 = 'O';
	LDI  R26,LOW(_check_eep1)
	LDI  R27,HIGH(_check_eep1)
	LDI  R30,LOW(79)
	RCALL __EEPROMWRB
; 0000 00F7         check_eep2 = 'K';
	LDI  R26,LOW(_check_eep2)
	LDI  R27,HIGH(_check_eep2)
	LDI  R30,LOW(75)
	RCALL __EEPROMWRB
; 0000 00F8       };
_0x47:
; 0000 00F9 
; 0000 00FA       if(count_cali == ONTIME)
	LDS  R26,_count_cali
	LDS  R27,_count_cali+1
	SBIW R26,1
	BRNE _0x4A
; 0000 00FB       {
; 0000 00FC         count_cali= STOPTHIS;
	LDI  R30,LOW(0)
	STS  _count_cali,R30
	STS  _count_cali+1,R30
; 0000 00FD         LED(0);
	CBI  0xB,3
; 0000 00FE         send_sensor_info = 100;
	LDI  R30,LOW(100)
	LDI  R31,HIGH(100)
	STS  _send_sensor_info,R30
	STS  _send_sensor_info+1,R31
; 0000 00FF 
; 0000 0100         if( check_eep1 != 'O'){
	LDI  R26,LOW(_check_eep1)
	LDI  R27,HIGH(_check_eep1)
	RCALL __EEPROMRDB
	CPI  R30,LOW(0x4F)
	BREQ _0x4D
; 0000 0101         RS485_TX(1);
	SBI  0xB,2
; 0000 0102         printf("Cannot offset x,y:%d,%d\r\n ",osrawx,osrawy); }
	__POINTW1FN _0x0,139
	ST   -Y,R31
	ST   -Y,R30
	LDI  R26,LOW(_osrawx)
	LDI  R27,HIGH(_osrawx)
	RCALL __EEPROMRDW
	RCALL __CWD1
	RCALL __PUTPARD1
	LDI  R26,LOW(_osrawy)
	LDI  R27,HIGH(_osrawy)
	RCALL __EEPROMRDW
	RCALL __CWD1
	RCALL __PUTPARD1
	LDI  R24,8
	RCALL _printf
	ADIW R28,10
; 0000 0103 
; 0000 0104 
; 0000 0105       }
_0x4D:
; 0000 0106       //delay_ms(200);
; 0000 0107       }
_0x4A:
	RJMP _0x33
; 0000 0108 }
_0x50:
	RJMP _0x50
; .FEND
;
;
;
;
;
;unsigned int read_SCA100TX(void){
; 0000 010E unsigned int read_SCA100TX(void){
_read_SCA100TX:
; .FSTART _read_SCA100TX
; 0000 010F   unsigned int result = 0;
; 0000 0110   result=read_SCA100TXY(RDAX);
	RCALL __SAVELOCR2
;	result -> R16,R17
	__GETWRN 16,17,0
	LDI  R26,LOW(16)
	RCALL _read_SCA100TXY
	MOVW R16,R30
; 0000 0111   return(result);
	RJMP _0x20C0004
; 0000 0112 }
; .FEND
;
;unsigned int read_SCA100TY(void){
; 0000 0114 unsigned int read_SCA100TY(void){
_read_SCA100TY:
; .FSTART _read_SCA100TY
; 0000 0115   unsigned int result = 0;
; 0000 0116   result=read_SCA100TXY(RDAY);
	RCALL __SAVELOCR2
;	result -> R16,R17
	__GETWRN 16,17,0
	LDI  R26,LOW(17)
	RCALL _read_SCA100TXY
	MOVW R16,R30
; 0000 0117   return(result);
	RJMP _0x20C0004
; 0000 0118 }
; .FEND
;
;//Read SCA100TXY from the SCA100T
;unsigned int read_SCA100TXY(unsigned char thisValue){
; 0000 011B unsigned int read_SCA100TXY(unsigned char thisValue){
_read_SCA100TXY:
; .FSTART _read_SCA100TXY
; 0000 011C     unsigned char inByte = 0;           // incoming byte from the SPI
; 0000 011D     unsigned int result = 0;   // result to return
; 0000 011E 
; 0000 011F     // digitalWrite(chipSelectPin, LOW);
; 0000 0120     PORTB &= ~(1<<2);
	ST   -Y,R26
	RCALL __SAVELOCR4
;	thisValue -> Y+4
;	inByte -> R17
;	result -> R18,R19
	LDI  R17,0
	__GETWRN 18,19,0
	CBI  0x5,2
; 0000 0121     //SPI.transfer(thisValue);
; 0000 0122     spi(thisValue);
	LDD  R26,Y+4
	RCALL _spi
; 0000 0123     // delay(100);
; 0000 0124     result = spi(0);//SPI.transfer(0x00);
	LDI  R26,LOW(0)
	RCALL _spi
	MOV  R18,R30
	CLR  R19
; 0000 0125     inByte = spi(0);//SPI.transfer(0x00);
	LDI  R26,LOW(0)
	RCALL _spi
	MOV  R17,R30
; 0000 0126     //digitalWrite(chipSelectPin, HIGH);
; 0000 0127     PORTB |= (1<<2);
	SBI  0x5,2
; 0000 0128     result = result << 8;
	MOV  R19,R18
	CLR  R18
; 0000 0129     result = result | inByte;
	LDI  R31,0
	__ORWRR 18,19,30,31
; 0000 012A     result = result >> 5;
	MOVW R26,R18
	LDI  R30,LOW(5)
	RCALL __LSRW12
	MOVW R18,R30
; 0000 012B     return(result);
	RCALL __LOADLOCR4
	ADIW R28,5
	RET
; 0000 012C 
; 0000 012D }
; .FEND
;//processing data from master, if received "CALI" => taking ofset for x,y
;void process_uart_mess()
; 0000 0130 {
_process_uart_mess:
; .FSTART _process_uart_mess
; 0000 0131     char usart_count;
; 0000 0132 
; 0000 0133     //RS485_TX(1);
; 0000 0134     //printf("%s\r\n ",rx_buffer0);
; 0000 0135     if((rx_buffer0[0]=='C')&&(rx_buffer0[1]=='A')&&(rx_buffer0[3]=='I'))
	ST   -Y,R17
;	usart_count -> R17
	LDS  R26,_rx_buffer0
	CPI  R26,LOW(0x43)
	BRNE _0x52
	__GETB2MN _rx_buffer0,1
	CPI  R26,LOW(0x41)
	BRNE _0x52
	__GETB2MN _rx_buffer0,3
	CPI  R26,LOW(0x49)
	BREQ _0x53
_0x52:
	RJMP _0x51
_0x53:
; 0000 0136     {
; 0000 0137     cali_flag=1;
	LDI  R30,LOW(1)
	MOV  R4,R30
; 0000 0138                        /*
; 0000 0139     osrawx = rawx;
; 0000 013A     osrawy = rawy;
; 0000 013B     RS485_TX(1);
; 0000 013C     printf("Ofset x,y:%d,%d\r\n ",osrawx,osrawy);
; 0000 013D     check_eep1 = 'O';
; 0000 013E     check_eep2 = 'K';*/
; 0000 013F     }
; 0000 0140     //reset memorry
; 0000 0141     //memset(rx_buffer0,0,RX_BUFFER_SIZE0);
; 0000 0142     for(usart_count=0;usart_count<RX_BUFFER_SIZE0;usart_count++)
_0x51:
	LDI  R17,LOW(0)
_0x55:
	CPI  R17,8
	BRSH _0x56
; 0000 0143                             {
; 0000 0144                             rx_buffer0[usart_count]=0                   ;
	MOV  R30,R17
	LDI  R31,0
	SUBI R30,LOW(-_rx_buffer0)
	SBCI R31,HIGH(-_rx_buffer0)
	LDI  R26,LOW(0)
	STD  Z+0,R26
; 0000 0145                             }
	SUBI R17,-1
	RJMP _0x55
_0x56:
; 0000 0146     rx_wr_index0=0;
	LDI  R30,LOW(0)
	STS  _rx_wr_index0,R30
; 0000 0147 }
	LD   R17,Y+
	RET
; .FEND
;
;/***********************************************************************************/
;/*-----------------------------------------------------------------------------
;Noi Dung    :   Doc 1 byte du lieu tu DS18B20 ra ngoai.
;Tham Bien   :   Khong.
;Tra Ve      :   Byte du lieu can doc.
;  -----------------------------------------------------------------------------*/
;
;
;unsigned char DS18B20_ReadByte(void)
; 0000 0152 {
_DS18B20_ReadByte:
; .FSTART _DS18B20_ReadByte
; 0000 0153     unsigned char i=0;
; 0000 0154     unsigned char data=0;
; 0000 0155 
; 0000 0156     for(i=8;i>0;i--)
	RCALL __SAVELOCR2
;	i -> R17
;	data -> R16
	LDI  R17,0
	LDI  R16,0
	LDI  R17,LOW(8)
_0x58:
	CPI  R17,1
	BRLO _0x59
; 0000 0157     {
; 0000 0158         //GPIO_SetState(GPIOA,GPIO_Pin_15,GPIO_Mode_Out_PP);   // Cau hinh chan DQ la OUPUT
; 0000 0159         DDRC.2 = 1;
	SBI  0x7,2
; 0000 015A         DS18B20_PORT_DQ_L;  // Keo chan DQ xuong muc '0'
	CBI  0x8,2
; 0000 015B         data>>=1;
	LSR  R16
; 0000 015C         DS18B20_PORT_DQ_H;  // Keo chan DQ len muc '1'
	SBI  0x8,2
; 0000 015D         //GPIO_SetState(GPIOA,GPIO_Pin_15,GPIO_Mode_IPU);   // Cau hinh chan DQ la INPUT
; 0000 015E         DDRC.2 = 0;
	CBI  0x7,2
; 0000 015F         PORTC.2 =1;
	SBI  0x8,2
; 0000 0160         if(PINC.2) data|=0x80;   // Nhan du lieu tra ve tu DS18B20
	SBIC 0x6,2
	ORI  R16,LOW(128)
; 0000 0161         delay_us(120);
	__DELAY_USW 480
; 0000 0162     }
	SUBI R17,1
	RJMP _0x58
_0x59:
; 0000 0163     return(data);
	MOV  R30,R16
_0x20C0004:
	LD   R16,Y+
	LD   R17,Y+
	RET
; 0000 0164 }
; .FEND
;
;/*-----------------------------------------------------------------------------
;Noi Dung    :   Viet 1 byte du lieu vao DS18B20.
;Tham Bien   :   data: Byte du lieu can viet vao.
;Tra Ve      :   Khong.
;  -----------------------------------------------------------------------------*/
;
;void DS18B20_WriteByte(unsigned char data)
; 0000 016D {
_DS18B20_WriteByte:
; .FSTART _DS18B20_WriteByte
; 0000 016E     unsigned char i=0;
; 0000 016F     //GPIO_SetState(GPIOA,GPIO_Pin_15,GPIO_Mode_Out_PP);   // Cau hinh chan DQ la OUTPUT
; 0000 0170     DDRC.2 = 1;
	ST   -Y,R26
	ST   -Y,R17
;	data -> Y+1
;	i -> R17
	LDI  R17,0
	SBI  0x7,2
; 0000 0171     for (i=8;i>0;i--)
	LDI  R17,LOW(8)
_0x68:
	CPI  R17,1
	BRLO _0x69
; 0000 0172     {
; 0000 0173         DS18B20_PORT_DQ_L;  // Keo chan DQ xuong muc '0'
	CBI  0x8,2
; 0000 0174         //DS18B20_PORT_DQ=(data&0x01); // Viet du lieu vao DS18B20
; 0000 0175                 if(data&0x01) DS18B20_PORT_DQ_H;
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x6C
	SBI  0x8,2
; 0000 0176                 if(!(data&0x01)) DS18B20_PORT_DQ_L;
_0x6C:
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BRNE _0x6F
	CBI  0x8,2
; 0000 0177         delay_us(60);
_0x6F:
	__DELAY_USW 240
; 0000 0178         DS18B20_PORT_DQ_H;  // Keo chan DQ len muc '1'
	SBI  0x8,2
; 0000 0179         data>>=1;
	LDD  R30,Y+1
	LSR  R30
	STD  Y+1,R30
; 0000 017A     }
	SUBI R17,1
	RJMP _0x68
_0x69:
; 0000 017B }
	LDD  R17,Y+0
	ADIW R28,2
	RET
; .FEND
;
;/*-----------------------------------------------------------------------------
;Noi Dung    :   Khoi tao DS18B20.
;Tham Bien   :   Khong.
;Tra Ve      :   Gia tri tra ve khi khoi tao xong cua DS18B20 (bit).
;  -----------------------------------------------------------------------------*/
;
;void DS18B20_Init(void)
; 0000 0184 {
_DS18B20_Init:
; .FSTART _DS18B20_Init
; 0000 0185     //GPIO_SetState(GPIOA,GPIO_Pin_15,GPIO_Mode_Out_PP);   // Cau hinh chan DQ la OUTPUT
; 0000 0186     DDRC.2 = 1;
	SBI  0x7,2
; 0000 0187     DS18B20_PORT_DQ_L;  // Keo DQ xuong muc '0' trong khoang 480us
	CBI  0x8,2
; 0000 0188     delay_us(500);
	__DELAY_USW 2000
; 0000 0189     //GPIO_SetState(GPIOA,GPIO_Pin_15,GPIO_Mode_IPU);   // Cau hinh chan DQ la INPUT trong khoang 480us
; 0000 018A     DDRC.2 = 0;
	CBI  0x7,2
; 0000 018B     PORTC.2 =1;
	SBI  0x8,2
; 0000 018C     delay_us(500);
	__DELAY_USW 2000
; 0000 018D }
	RET
; .FEND
;
;/*-----------------------------------------------------------------------------
;Noi Dung    :   Ghi 1 byte lenh chuc nang vao DS18B20.
;Tham Bien   :   byte_func: byte lenh chuc nang cua DS18B20.
;Tra Ve      :   Khong.
;  -----------------------------------------------------------------------------*/
;
;void DS18B20_WriteFunc(unsigned char byte_func)
; 0000 0196 {
_DS18B20_WriteFunc:
; .FSTART _DS18B20_WriteFunc
; 0000 0197     DS18B20_Init();                 // Khoi tao DS18B20
	ST   -Y,R26
;	byte_func -> Y+0
	RCALL _DS18B20_Init
; 0000 0198     DS18B20_WriteByte(DS1820_CMD_SKIPROM);  // Truy cap thang den cac lenh chuc nang bo nho cua DS18B20
	LDI  R26,LOW(204)
	RCALL _DS18B20_WriteByte
; 0000 0199     DS18B20_WriteByte(byte_func);   // Viet ma lenh chuc nang
	LD   R26,Y
	RCALL _DS18B20_WriteByte
; 0000 019A }
	RJMP _0x20C0001
; .FEND
;
;/*-----------------------------------------------------------------------------
;Noi Dung    :   Cau hinh cho DS18B20.
;Tham Bien   :   temp_low: Gia tri nhiet do thap nhat de dua ra canh bao.
;                temp_high: Gia tri nhiet do cao nhat de dua ra canh bao.
;                resolution: Do phan giai cho DS18B20.(1|R1|R0|1|1|1|1|1)
;Tra Ve      :   Khong.
;  -----------------------------------------------------------------------------*/
;
;void DS18B20_Config(unsigned char temp_low, unsigned char temp_high, unsigned char resolution)
; 0000 01A5 {
_DS18B20_Config:
; .FSTART _DS18B20_Config
; 0000 01A6     resolution = (resolution<<5)|0x1f;
	ST   -Y,R26
;	temp_low -> Y+2
;	temp_high -> Y+1
;	resolution -> Y+0
	LD   R30,Y
	SWAP R30
	ANDI R30,0xF0
	LSL  R30
	ORI  R30,LOW(0x1F)
	ST   Y,R30
; 0000 01A7     DS18B20_WriteFunc(DS1820_CMD_WRITESCRPAD);        // Cho phep ghi 3 byte vao bo nho nhap:
	LDI  R26,LOW(78)
	RCALL _DS18B20_WriteFunc
; 0000 01A8     DS18B20_WriteByte(temp_high);   // byte 2: Th
	LDD  R26,Y+1
	RCALL _DS18B20_WriteByte
; 0000 01A9     DS18B20_WriteByte(temp_low);    // byte 3: Tl
	LDD  R26,Y+2
	RCALL _DS18B20_WriteByte
; 0000 01AA     DS18B20_WriteByte(resolution);  // byte 4: configuration register
	LD   R26,Y
	RCALL _DS18B20_WriteByte
; 0000 01AB     DS18B20_WriteFunc(DS1820_CMD_COPYSCRPAD);        // Ghi vao EEPROM
	LDI  R26,LOW(72)
	RCALL _DS18B20_WriteFunc
; 0000 01AC }
	RJMP _0x20C0002
; .FEND
;
;/*-----------------------------------------------------------------------------
;Noi Dung    :   Doc gia tri nhiet do do duoc cua DS18B20.
;Tham Bien   :   Khong.
;Tra Ve      :   Gia tri nhiet do do duoc.
;  -----------------------------------------------------------------------------*/
;
;//float DS18B20_ReadTemp(void)
;//{
;//    float temp;
;//    unsigned char buff_temp1,buff_temp2;
;//
;//    DS18B20_WriteFunc(DS1820_CMD_CONVERTTEMP);  // Khoi dong qua trinh do va chuyen doi nhiet do ra so nhi phan
;//    delay_us(200);
;//    DS18B20_WriteFunc(DS1820_CMD_READSCRPAD);   // Doc du lieu tu bo nho DS18b20
;//
;//    buff_temp1 = DS18B20_ReadByte();
;//    temp=((float)(buff_temp1&0x0f))/16;            // Lay phan thuc cua gia tri nhiet do
;//    buff_temp2 = DS18B20_ReadByte();
;//    buff_temp1 =((buff_temp1&0xf0)>>4)|((buff_temp2&0x0f)<<4) ;    // Lay phan nguyen cua gia tri nhiet do
;//    temp=temp+buff_temp1;
;//    return temp;
;//}
;/*
; * SimpleKalmanFilter - a Kalman Filter implementation for single variable models.
; * Created by Denys Sene, January, 1, 2017.
; * Released under MIT License - see LICENSE file for details.
; */
;#include "SimpleKalmanFilter.h"
;#include <math.h>
;#include <stdlib.h>
;//float u_kalman,u;
;float _err_measure;
;float _err_estimate;
;float _q;
;float _current_estimate;
;float _last_estimate;
;float _kalman_gain;
;
;void SimpleKalmanFilter(float mea_e, float est_e, float q)
; 0001 0012 {

	.CSEG
_SimpleKalmanFilter:
; .FSTART _SimpleKalmanFilter
; 0001 0013   _err_measure=mea_e;
	RCALL __PUTPARD2
;	mea_e -> Y+8
;	est_e -> Y+4
;	q -> Y+0
	__GETD1S 8
	STS  __err_measure,R30
	STS  __err_measure+1,R31
	STS  __err_measure+2,R22
	STS  __err_measure+3,R23
; 0001 0014   _err_estimate=est_e;
	__GETD1S 4
	STS  __err_estimate,R30
	STS  __err_estimate+1,R31
	STS  __err_estimate+2,R22
	STS  __err_estimate+3,R23
; 0001 0015   _q = q;
	RCALL __GETD1S0
	STS  __q,R30
	STS  __q+1,R31
	STS  __q+2,R22
	STS  __q+3,R23
; 0001 0016 }
	ADIW R28,12
	RET
; .FEND
;
;float updateEstimate(float mea)
; 0001 0019 {
_updateEstimate:
; .FSTART _updateEstimate
; 0001 001A   _kalman_gain = _err_estimate/(_err_estimate + _err_measure);
	RCALL __PUTPARD2
;	mea -> Y+0
	LDS  R30,__err_measure
	LDS  R31,__err_measure+1
	LDS  R22,__err_measure+2
	LDS  R23,__err_measure+3
	LDS  R26,__err_estimate
	LDS  R27,__err_estimate+1
	LDS  R24,__err_estimate+2
	LDS  R25,__err_estimate+3
	RCALL __ADDF12
	LDS  R26,__err_estimate
	LDS  R27,__err_estimate+1
	LDS  R24,__err_estimate+2
	LDS  R25,__err_estimate+3
	RCALL __DIVF21
	STS  __kalman_gain,R30
	STS  __kalman_gain+1,R31
	STS  __kalman_gain+2,R22
	STS  __kalman_gain+3,R23
; 0001 001B   _current_estimate = _last_estimate + _kalman_gain * (mea - _last_estimate);
	LDS  R26,__last_estimate
	LDS  R27,__last_estimate+1
	LDS  R24,__last_estimate+2
	LDS  R25,__last_estimate+3
	RCALL __GETD1S0
	RCALL __SUBF12
	LDS  R26,__kalman_gain
	LDS  R27,__kalman_gain+1
	LDS  R24,__kalman_gain+2
	LDS  R25,__kalman_gain+3
	RCALL __MULF12
	LDS  R26,__last_estimate
	LDS  R27,__last_estimate+1
	LDS  R24,__last_estimate+2
	LDS  R25,__last_estimate+3
	RCALL __ADDF12
	STS  __current_estimate,R30
	STS  __current_estimate+1,R31
	STS  __current_estimate+2,R22
	STS  __current_estimate+3,R23
; 0001 001C   _err_estimate =  (1.0 - _kalman_gain)*_err_estimate + fabs(_last_estimate-_current_estimate)*_q;
	LDS  R26,__kalman_gain
	LDS  R27,__kalman_gain+1
	LDS  R24,__kalman_gain+2
	LDS  R25,__kalman_gain+3
	__GETD1N 0x3F800000
	RCALL __SUBF12
	LDS  R26,__err_estimate
	LDS  R27,__err_estimate+1
	LDS  R24,__err_estimate+2
	LDS  R25,__err_estimate+3
	RCALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	LDS  R26,__current_estimate
	LDS  R27,__current_estimate+1
	LDS  R24,__current_estimate+2
	LDS  R25,__current_estimate+3
	LDS  R30,__last_estimate
	LDS  R31,__last_estimate+1
	LDS  R22,__last_estimate+2
	LDS  R23,__last_estimate+3
	RCALL __SUBF12
	MOVW R26,R30
	MOVW R24,R22
	RCALL _fabs
	LDS  R26,__q
	LDS  R27,__q+1
	LDS  R24,__q+2
	LDS  R25,__q+3
	RCALL __MULF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	RCALL __ADDF12
	STS  __err_estimate,R30
	STS  __err_estimate+1,R31
	STS  __err_estimate+2,R22
	STS  __err_estimate+3,R23
; 0001 001D   _last_estimate=_current_estimate;
	LDS  R30,__current_estimate
	LDS  R31,__current_estimate+1
	LDS  R22,__current_estimate+2
	LDS  R23,__current_estimate+3
	STS  __last_estimate,R30
	STS  __last_estimate+1,R31
	STS  __last_estimate+2,R22
	STS  __last_estimate+3,R23
; 0001 001E 
; 0001 001F   return _current_estimate;
	LDS  R30,__current_estimate
	LDS  R31,__current_estimate+1
	LDS  R22,__current_estimate+2
	LDS  R23,__current_estimate+3
_0x20C0003:
	ADIW R28,4
	RET
; 0001 0020 }
; .FEND
;
;

	.CSEG
_fabs:
; .FSTART _fabs
	RCALL __PUTPARD2
    ld   r30,y+
    ld   r31,y+
    ld   r22,y+
    ld   r23,y+
    cbr  r23,0x80
    ret
; .FEND

	.DSEG

	.CSEG
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x01
	.EQU __sm_mask=0x0E
	.EQU __sm_adc_noise_red=0x02
	.EQU __sm_powerdown=0x04
	.EQU __sm_powersave=0x06
	.EQU __sm_standby=0x0C
	.EQU __sm_ext_standby=0x0E
	.SET power_ctrl_reg=smcr
	#endif

	.CSEG
_put_usart_G101:
; .FSTART _put_usart_G101
	ST   -Y,R27
	ST   -Y,R26
	LDD  R26,Y+2
	RCALL _putchar
	LD   R26,Y
	LDD  R27,Y+1
	LD   R30,X+
	LD   R31,X+
	ADIW R30,1
	ST   -X,R31
	ST   -X,R30
_0x20C0002:
	ADIW R28,3
	RET
; .FEND
__print_G101:
; .FSTART __print_G101
	ST   -Y,R27
	ST   -Y,R26
	SBIW R28,6
	RCALL __SAVELOCR6
	LDI  R17,0
	LDD  R26,Y+12
	LDD  R27,Y+12+1
	LDI  R30,LOW(0)
	LDI  R31,HIGH(0)
	ST   X+,R30
	ST   X,R31
_0x2020016:
	LDD  R30,Y+18
	LDD  R31,Y+18+1
	ADIW R30,1
	STD  Y+18,R30
	STD  Y+18+1,R31
	SBIW R30,1
	LPM  R30,Z
	MOV  R18,R30
	CPI  R30,0
	BRNE PC+2
	RJMP _0x2020018
	MOV  R30,R17
	CPI  R30,0
	BRNE _0x202001C
	CPI  R18,37
	BRNE _0x202001D
	LDI  R17,LOW(1)
	RJMP _0x202001E
_0x202001D:
	ST   -Y,R18
	LDD  R26,Y+13
	LDD  R27,Y+13+1
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ICALL
_0x202001E:
	RJMP _0x202001B
_0x202001C:
	CPI  R30,LOW(0x1)
	BRNE _0x202001F
	CPI  R18,37
	BRNE _0x2020020
	ST   -Y,R18
	LDD  R26,Y+13
	LDD  R27,Y+13+1
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ICALL
	RJMP _0x20200CC
_0x2020020:
	LDI  R17,LOW(2)
	LDI  R20,LOW(0)
	LDI  R16,LOW(0)
	CPI  R18,45
	BRNE _0x2020021
	LDI  R16,LOW(1)
	RJMP _0x202001B
_0x2020021:
	CPI  R18,43
	BRNE _0x2020022
	LDI  R20,LOW(43)
	RJMP _0x202001B
_0x2020022:
	CPI  R18,32
	BRNE _0x2020023
	LDI  R20,LOW(32)
	RJMP _0x202001B
_0x2020023:
	RJMP _0x2020024
_0x202001F:
	CPI  R30,LOW(0x2)
	BRNE _0x2020025
_0x2020024:
	LDI  R21,LOW(0)
	LDI  R17,LOW(3)
	CPI  R18,48
	BRNE _0x2020026
	ORI  R16,LOW(128)
	RJMP _0x202001B
_0x2020026:
	RJMP _0x2020027
_0x2020025:
	CPI  R30,LOW(0x3)
	BREQ PC+2
	RJMP _0x202001B
_0x2020027:
	CPI  R18,48
	BRLO _0x202002A
	CPI  R18,58
	BRLO _0x202002B
_0x202002A:
	RJMP _0x2020029
_0x202002B:
	LDI  R26,LOW(10)
	MUL  R21,R26
	MOV  R21,R0
	MOV  R30,R18
	SUBI R30,LOW(48)
	ADD  R21,R30
	RJMP _0x202001B
_0x2020029:
	MOV  R30,R18
	CPI  R30,LOW(0x63)
	BRNE _0x202002F
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	SBIW R30,4
	STD  Y+16,R30
	STD  Y+16+1,R31
	LDD  R26,Z+4
	ST   -Y,R26
	LDD  R26,Y+13
	LDD  R27,Y+13+1
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ICALL
	RJMP _0x2020030
_0x202002F:
	CPI  R30,LOW(0x73)
	BRNE _0x2020032
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	SBIW R30,4
	STD  Y+16,R30
	STD  Y+16+1,R31
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	ADIW R26,4
	RCALL __GETW1P
	STD  Y+6,R30
	STD  Y+6+1,R31
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	RCALL _strlen
	MOV  R17,R30
	RJMP _0x2020033
_0x2020032:
	CPI  R30,LOW(0x70)
	BRNE _0x2020035
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	SBIW R30,4
	STD  Y+16,R30
	STD  Y+16+1,R31
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	ADIW R26,4
	RCALL __GETW1P
	STD  Y+6,R30
	STD  Y+6+1,R31
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	RCALL _strlenf
	MOV  R17,R30
	ORI  R16,LOW(8)
_0x2020033:
	ORI  R16,LOW(2)
	ANDI R16,LOW(127)
	LDI  R19,LOW(0)
	RJMP _0x2020036
_0x2020035:
	CPI  R30,LOW(0x64)
	BREQ _0x2020039
	CPI  R30,LOW(0x69)
	BRNE _0x202003A
_0x2020039:
	ORI  R16,LOW(4)
	RJMP _0x202003B
_0x202003A:
	CPI  R30,LOW(0x75)
	BRNE _0x202003C
_0x202003B:
	LDI  R30,LOW(_tbl10_G101*2)
	LDI  R31,HIGH(_tbl10_G101*2)
	STD  Y+6,R30
	STD  Y+6+1,R31
	LDI  R17,LOW(5)
	RJMP _0x202003D
_0x202003C:
	CPI  R30,LOW(0x58)
	BRNE _0x202003F
	ORI  R16,LOW(8)
	RJMP _0x2020040
_0x202003F:
	CPI  R30,LOW(0x78)
	BREQ PC+2
	RJMP _0x2020071
_0x2020040:
	LDI  R30,LOW(_tbl16_G101*2)
	LDI  R31,HIGH(_tbl16_G101*2)
	STD  Y+6,R30
	STD  Y+6+1,R31
	LDI  R17,LOW(4)
_0x202003D:
	SBRS R16,2
	RJMP _0x2020042
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	SBIW R30,4
	STD  Y+16,R30
	STD  Y+16+1,R31
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	ADIW R26,4
	RCALL __GETW1P
	STD  Y+10,R30
	STD  Y+10+1,R31
	LDD  R26,Y+11
	TST  R26
	BRPL _0x2020043
	RCALL __ANEGW1
	STD  Y+10,R30
	STD  Y+10+1,R31
	LDI  R20,LOW(45)
_0x2020043:
	CPI  R20,0
	BREQ _0x2020044
	SUBI R17,-LOW(1)
	RJMP _0x2020045
_0x2020044:
	ANDI R16,LOW(251)
_0x2020045:
	RJMP _0x2020046
_0x2020042:
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	SBIW R30,4
	STD  Y+16,R30
	STD  Y+16+1,R31
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	ADIW R26,4
	RCALL __GETW1P
	STD  Y+10,R30
	STD  Y+10+1,R31
_0x2020046:
_0x2020036:
	SBRC R16,0
	RJMP _0x2020047
_0x2020048:
	CP   R17,R21
	BRSH _0x202004A
	SBRS R16,7
	RJMP _0x202004B
	SBRS R16,2
	RJMP _0x202004C
	ANDI R16,LOW(251)
	MOV  R18,R20
	SUBI R17,LOW(1)
	RJMP _0x202004D
_0x202004C:
	LDI  R18,LOW(48)
_0x202004D:
	RJMP _0x202004E
_0x202004B:
	LDI  R18,LOW(32)
_0x202004E:
	ST   -Y,R18
	LDD  R26,Y+13
	LDD  R27,Y+13+1
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ICALL
	SUBI R21,LOW(1)
	RJMP _0x2020048
_0x202004A:
_0x2020047:
	MOV  R19,R17
	SBRS R16,1
	RJMP _0x202004F
_0x2020050:
	CPI  R19,0
	BREQ _0x2020052
	SBRS R16,3
	RJMP _0x2020053
	LDD  R30,Y+6
	LDD  R31,Y+6+1
	LPM  R18,Z+
	STD  Y+6,R30
	STD  Y+6+1,R31
	RJMP _0x2020054
_0x2020053:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LD   R18,X+
	STD  Y+6,R26
	STD  Y+6+1,R27
_0x2020054:
	ST   -Y,R18
	LDD  R26,Y+13
	LDD  R27,Y+13+1
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ICALL
	CPI  R21,0
	BREQ _0x2020055
	SUBI R21,LOW(1)
_0x2020055:
	SUBI R19,LOW(1)
	RJMP _0x2020050
_0x2020052:
	RJMP _0x2020056
_0x202004F:
_0x2020058:
	LDI  R18,LOW(48)
	LDD  R30,Y+6
	LDD  R31,Y+6+1
	RCALL __GETW1PF
	STD  Y+8,R30
	STD  Y+8+1,R31
	LDD  R30,Y+6
	LDD  R31,Y+6+1
	ADIW R30,2
	STD  Y+6,R30
	STD  Y+6+1,R31
_0x202005A:
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	LDD  R26,Y+10
	LDD  R27,Y+10+1
	CP   R26,R30
	CPC  R27,R31
	BRLO _0x202005C
	SUBI R18,-LOW(1)
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	SUB  R30,R26
	SBC  R31,R27
	STD  Y+10,R30
	STD  Y+10+1,R31
	RJMP _0x202005A
_0x202005C:
	CPI  R18,58
	BRLO _0x202005D
	SBRS R16,3
	RJMP _0x202005E
	SUBI R18,-LOW(7)
	RJMP _0x202005F
_0x202005E:
	SUBI R18,-LOW(39)
_0x202005F:
_0x202005D:
	SBRC R16,4
	RJMP _0x2020061
	CPI  R18,49
	BRSH _0x2020063
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	SBIW R26,1
	BRNE _0x2020062
_0x2020063:
	RJMP _0x20200CD
_0x2020062:
	CP   R21,R19
	BRLO _0x2020067
	SBRS R16,0
	RJMP _0x2020068
_0x2020067:
	RJMP _0x2020066
_0x2020068:
	LDI  R18,LOW(32)
	SBRS R16,7
	RJMP _0x2020069
	LDI  R18,LOW(48)
_0x20200CD:
	ORI  R16,LOW(16)
	SBRS R16,2
	RJMP _0x202006A
	ANDI R16,LOW(251)
	ST   -Y,R20
	LDD  R26,Y+13
	LDD  R27,Y+13+1
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ICALL
	CPI  R21,0
	BREQ _0x202006B
	SUBI R21,LOW(1)
_0x202006B:
_0x202006A:
_0x2020069:
_0x2020061:
	ST   -Y,R18
	LDD  R26,Y+13
	LDD  R27,Y+13+1
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ICALL
	CPI  R21,0
	BREQ _0x202006C
	SUBI R21,LOW(1)
_0x202006C:
_0x2020066:
	SUBI R19,LOW(1)
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	SBIW R26,2
	BRLO _0x2020059
	RJMP _0x2020058
_0x2020059:
_0x2020056:
	SBRS R16,0
	RJMP _0x202006D
_0x202006E:
	CPI  R21,0
	BREQ _0x2020070
	SUBI R21,LOW(1)
	LDI  R30,LOW(32)
	ST   -Y,R30
	LDD  R26,Y+13
	LDD  R27,Y+13+1
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ICALL
	RJMP _0x202006E
_0x2020070:
_0x202006D:
_0x2020071:
_0x2020030:
_0x20200CC:
	LDI  R17,LOW(0)
_0x202001B:
	RJMP _0x2020016
_0x2020018:
	LDD  R26,Y+12
	LDD  R27,Y+12+1
	RCALL __GETW1P
	RCALL __LOADLOCR6
	ADIW R28,20
	RET
; .FEND
_printf:
; .FSTART _printf
	PUSH R15
	MOV  R15,R24
	SBIW R28,6
	RCALL __SAVELOCR2
	MOVW R26,R28
	ADIW R26,4
	RCALL __ADDW2R15
	MOVW R16,R26
	LDI  R30,LOW(0)
	STD  Y+4,R30
	STD  Y+4+1,R30
	STD  Y+6,R30
	STD  Y+6+1,R30
	MOVW R26,R28
	ADIW R26,8
	RCALL __ADDW2R15
	RCALL __GETW1P
	ST   -Y,R31
	ST   -Y,R30
	ST   -Y,R17
	ST   -Y,R16
	LDI  R30,LOW(_put_usart_G101)
	LDI  R31,HIGH(_put_usart_G101)
	ST   -Y,R31
	ST   -Y,R30
	MOVW R26,R28
	ADIW R26,8
	RCALL __print_G101
	RCALL __LOADLOCR2
	ADIW R28,8
	POP  R15
	RET
; .FEND
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x01
	.EQU __sm_mask=0x0E
	.EQU __sm_adc_noise_red=0x02
	.EQU __sm_powerdown=0x04
	.EQU __sm_powersave=0x06
	.EQU __sm_standby=0x0C
	.EQU __sm_ext_standby=0x0E
	.SET power_ctrl_reg=smcr
	#endif

	.CSEG
_spi:
; .FSTART _spi
	ST   -Y,R26
	LD   R30,Y
	OUT  0x2E,R30
_0x2040003:
	IN   R30,0x2D
	SBRS R30,7
	RJMP _0x2040003
	IN   R30,0x2E
_0x20C0001:
	ADIW R28,1
	RET
; .FEND

	.CSEG

	.CSEG

	.CSEG
_strlen:
; .FSTART _strlen
	ST   -Y,R27
	ST   -Y,R26
    ld   r26,y+
    ld   r27,y+
    clr  r30
    clr  r31
strlen0:
    ld   r22,x+
    tst  r22
    breq strlen1
    adiw r30,1
    rjmp strlen0
strlen1:
    ret
; .FEND
_strlenf:
; .FSTART _strlenf
	ST   -Y,R27
	ST   -Y,R26
    clr  r26
    clr  r27
    ld   r30,y+
    ld   r31,y+
strlenf0:
	lpm  r0,z+
    tst  r0
    breq strlenf1
    adiw r26,1
    rjmp strlenf0
strlenf1:
    movw r30,r26
    ret
; .FEND

	.DSEG
_count_cali:
	.BYTE 0x2
_send_avgy:
	.BYTE 0x2
_X_err_measure:
	.BYTE 0x4
_X_err_estimate:
	.BYTE 0x4
_X_q:
	.BYTE 0x4
_X_current_estimate:
	.BYTE 0x4
_X_last_estimate:
	.BYTE 0x4
_X_kalman_gain:
	.BYTE 0x4
_Y_err_measure:
	.BYTE 0x4
_Y_err_estimate:
	.BYTE 0x4
_Y_q:
	.BYTE 0x4
_Y_current_estimate:
	.BYTE 0x4
_Y_last_estimate:
	.BYTE 0x4
_Y_kalman_gain:
	.BYTE 0x4
_lux:
	.BYTE 0x2
_buff_temp1:
	.BYTE 0x1
_buff_temp2:
	.BYTE 0x1
_new_tempu:
	.BYTE 0x2
_usart_time_out:
	.BYTE 0x2
_send_sensor_info:
	.BYTE 0x2

	.ESEG
_osrawx:
	.BYTE 0x2
_osrawy:
	.BYTE 0x2
_check_eep1:
	.BYTE 0x1
_check_eep2:
	.BYTE 0x1

	.DSEG
_ds_temp:
	.BYTE 0x2
_ds_err_measure:
	.BYTE 0x4
_ds_err_estimate:
	.BYTE 0x4
_ds_q:
	.BYTE 0x4
_ds_current_estimate:
	.BYTE 0x4
_ds_last_estimate:
	.BYTE 0x4
_ds_kalman_gain:
	.BYTE 0x4
_rx_buffer0:
	.BYTE 0x8
_rx_wr_index0:
	.BYTE 0x1
_rx_rd_index0:
	.BYTE 0x1
_rx_counter0:
	.BYTE 0x1
_tx_buffer0:
	.BYTE 0x8
_tx_wr_index0:
	.BYTE 0x1
_tx_rd_index0:
	.BYTE 0x1
_tx_counter0:
	.BYTE 0x1
__err_measure:
	.BYTE 0x4
__err_estimate:
	.BYTE 0x4
__q:
	.BYTE 0x4
__current_estimate:
	.BYTE 0x4
__last_estimate:
	.BYTE 0x4
__kalman_gain:
	.BYTE 0x4
__seed_G100:
	.BYTE 0x4

	.CSEG

	.CSEG
_delay_ms:
	adiw r26,0
	breq __delay_ms1
__delay_ms0:
	__DELAY_USW 0xFA0
	wdr
	sbiw r26,1
	brne __delay_ms0
__delay_ms1:
	ret

	.equ __w1_port=0x08
	.equ __w1_bit=0x02

__ROUND_REPACK:
	TST  R21
	BRPL __REPACK
	CPI  R21,0x80
	BRNE __ROUND_REPACK0
	SBRS R30,0
	RJMP __REPACK
__ROUND_REPACK0:
	ADIW R30,1
	ADC  R22,R25
	ADC  R23,R25
	BRVS __REPACK1

__REPACK:
	LDI  R21,0x80
	EOR  R21,R23
	BRNE __REPACK0
	PUSH R21
	RJMP __ZERORES
__REPACK0:
	CPI  R21,0xFF
	BREQ __REPACK1
	LSL  R22
	LSL  R0
	ROR  R21
	ROR  R22
	MOV  R23,R21
	RET
__REPACK1:
	PUSH R21
	TST  R0
	BRMI __REPACK2
	RJMP __MAXRES
__REPACK2:
	RJMP __MINRES

__UNPACK:
	LDI  R21,0x80
	MOV  R1,R25
	AND  R1,R21
	LSL  R24
	ROL  R25
	EOR  R25,R21
	LSL  R21
	ROR  R24

__UNPACK1:
	LDI  R21,0x80
	MOV  R0,R23
	AND  R0,R21
	LSL  R22
	ROL  R23
	EOR  R23,R21
	LSL  R21
	ROR  R22
	RET

__CFD1U:
	SET
	RJMP __CFD1U0
__CFD1:
	CLT
__CFD1U0:
	PUSH R21
	RCALL __UNPACK1
	CPI  R23,0x80
	BRLO __CFD10
	CPI  R23,0xFF
	BRCC __CFD10
	RJMP __ZERORES
__CFD10:
	LDI  R21,22
	SUB  R21,R23
	BRPL __CFD11
	NEG  R21
	CPI  R21,8
	BRTC __CFD19
	CPI  R21,9
__CFD19:
	BRLO __CFD17
	SER  R30
	SER  R31
	SER  R22
	LDI  R23,0x7F
	BLD  R23,7
	RJMP __CFD15
__CFD17:
	CLR  R23
	TST  R21
	BREQ __CFD15
__CFD18:
	LSL  R30
	ROL  R31
	ROL  R22
	ROL  R23
	DEC  R21
	BRNE __CFD18
	RJMP __CFD15
__CFD11:
	CLR  R23
__CFD12:
	CPI  R21,8
	BRLO __CFD13
	MOV  R30,R31
	MOV  R31,R22
	MOV  R22,R23
	SUBI R21,8
	RJMP __CFD12
__CFD13:
	TST  R21
	BREQ __CFD15
__CFD14:
	LSR  R23
	ROR  R22
	ROR  R31
	ROR  R30
	DEC  R21
	BRNE __CFD14
__CFD15:
	TST  R0
	BRPL __CFD16
	RCALL __ANEGD1
__CFD16:
	POP  R21
	RET

__CDF1U:
	SET
	RJMP __CDF1U0
__CDF1:
	CLT
__CDF1U0:
	SBIW R30,0
	SBCI R22,0
	SBCI R23,0
	BREQ __CDF10
	CLR  R0
	BRTS __CDF11
	TST  R23
	BRPL __CDF11
	COM  R0
	RCALL __ANEGD1
__CDF11:
	MOV  R1,R23
	LDI  R23,30
	TST  R1
__CDF12:
	BRMI __CDF13
	DEC  R23
	LSL  R30
	ROL  R31
	ROL  R22
	ROL  R1
	RJMP __CDF12
__CDF13:
	MOV  R30,R31
	MOV  R31,R22
	MOV  R22,R1
	PUSH R21
	RCALL __REPACK
	POP  R21
__CDF10:
	RET

__SWAPACC:
	PUSH R20
	MOVW R20,R30
	MOVW R30,R26
	MOVW R26,R20
	MOVW R20,R22
	MOVW R22,R24
	MOVW R24,R20
	MOV  R20,R0
	MOV  R0,R1
	MOV  R1,R20
	POP  R20
	RET

__UADD12:
	ADD  R30,R26
	ADC  R31,R27
	ADC  R22,R24
	RET

__NEGMAN1:
	COM  R30
	COM  R31
	COM  R22
	SUBI R30,-1
	SBCI R31,-1
	SBCI R22,-1
	RET

__SUBF12:
	PUSH R21
	RCALL __UNPACK
	CPI  R25,0x80
	BREQ __ADDF129
	LDI  R21,0x80
	EOR  R1,R21

	RJMP __ADDF120

__ADDF12:
	PUSH R21
	RCALL __UNPACK
	CPI  R25,0x80
	BREQ __ADDF129

__ADDF120:
	CPI  R23,0x80
	BREQ __ADDF128
__ADDF121:
	MOV  R21,R23
	SUB  R21,R25
	BRVS __ADDF1211
	BRPL __ADDF122
	RCALL __SWAPACC
	RJMP __ADDF121
__ADDF122:
	CPI  R21,24
	BRLO __ADDF123
	CLR  R26
	CLR  R27
	CLR  R24
__ADDF123:
	CPI  R21,8
	BRLO __ADDF124
	MOV  R26,R27
	MOV  R27,R24
	CLR  R24
	SUBI R21,8
	RJMP __ADDF123
__ADDF124:
	TST  R21
	BREQ __ADDF126
__ADDF125:
	LSR  R24
	ROR  R27
	ROR  R26
	DEC  R21
	BRNE __ADDF125
__ADDF126:
	MOV  R21,R0
	EOR  R21,R1
	BRMI __ADDF127
	RCALL __UADD12
	BRCC __ADDF129
	ROR  R22
	ROR  R31
	ROR  R30
	INC  R23
	BRVC __ADDF129
	RJMP __MAXRES
__ADDF128:
	RCALL __SWAPACC
__ADDF129:
	RCALL __REPACK
	POP  R21
	RET
__ADDF1211:
	BRCC __ADDF128
	RJMP __ADDF129
__ADDF127:
	SUB  R30,R26
	SBC  R31,R27
	SBC  R22,R24
	BREQ __ZERORES
	BRCC __ADDF1210
	COM  R0
	RCALL __NEGMAN1
__ADDF1210:
	TST  R22
	BRMI __ADDF129
	LSL  R30
	ROL  R31
	ROL  R22
	DEC  R23
	BRVC __ADDF1210

__ZERORES:
	CLR  R30
	CLR  R31
	CLR  R22
	CLR  R23
	POP  R21
	RET

__MINRES:
	SER  R30
	SER  R31
	LDI  R22,0x7F
	SER  R23
	POP  R21
	RET

__MAXRES:
	SER  R30
	SER  R31
	LDI  R22,0x7F
	LDI  R23,0x7F
	POP  R21
	RET

__MULF12:
	PUSH R21
	RCALL __UNPACK
	CPI  R23,0x80
	BREQ __ZERORES
	CPI  R25,0x80
	BREQ __ZERORES
	EOR  R0,R1
	SEC
	ADC  R23,R25
	BRVC __MULF124
	BRLT __ZERORES
__MULF125:
	TST  R0
	BRMI __MINRES
	RJMP __MAXRES
__MULF124:
	PUSH R0
	PUSH R17
	PUSH R18
	PUSH R19
	PUSH R20
	CLR  R17
	CLR  R18
	CLR  R25
	MUL  R22,R24
	MOVW R20,R0
	MUL  R24,R31
	MOV  R19,R0
	ADD  R20,R1
	ADC  R21,R25
	MUL  R22,R27
	ADD  R19,R0
	ADC  R20,R1
	ADC  R21,R25
	MUL  R24,R30
	RCALL __MULF126
	MUL  R27,R31
	RCALL __MULF126
	MUL  R22,R26
	RCALL __MULF126
	MUL  R27,R30
	RCALL __MULF127
	MUL  R26,R31
	RCALL __MULF127
	MUL  R26,R30
	ADD  R17,R1
	ADC  R18,R25
	ADC  R19,R25
	ADC  R20,R25
	ADC  R21,R25
	MOV  R30,R19
	MOV  R31,R20
	MOV  R22,R21
	MOV  R21,R18
	POP  R20
	POP  R19
	POP  R18
	POP  R17
	POP  R0
	TST  R22
	BRMI __MULF122
	LSL  R21
	ROL  R30
	ROL  R31
	ROL  R22
	RJMP __MULF123
__MULF122:
	INC  R23
	BRVS __MULF125
__MULF123:
	RCALL __ROUND_REPACK
	POP  R21
	RET

__MULF127:
	ADD  R17,R0
	ADC  R18,R1
	ADC  R19,R25
	RJMP __MULF128
__MULF126:
	ADD  R18,R0
	ADC  R19,R1
__MULF128:
	ADC  R20,R25
	ADC  R21,R25
	RET

__DIVF21:
	PUSH R21
	RCALL __UNPACK
	CPI  R23,0x80
	BRNE __DIVF210
	TST  R1
__DIVF211:
	BRPL __DIVF219
	RJMP __MINRES
__DIVF219:
	RJMP __MAXRES
__DIVF210:
	CPI  R25,0x80
	BRNE __DIVF218
__DIVF217:
	RJMP __ZERORES
__DIVF218:
	EOR  R0,R1
	SEC
	SBC  R25,R23
	BRVC __DIVF216
	BRLT __DIVF217
	TST  R0
	RJMP __DIVF211
__DIVF216:
	MOV  R23,R25
	PUSH R17
	PUSH R18
	PUSH R19
	PUSH R20
	CLR  R1
	CLR  R17
	CLR  R18
	CLR  R19
	CLR  R20
	CLR  R21
	LDI  R25,32
__DIVF212:
	CP   R26,R30
	CPC  R27,R31
	CPC  R24,R22
	CPC  R20,R17
	BRLO __DIVF213
	SUB  R26,R30
	SBC  R27,R31
	SBC  R24,R22
	SBC  R20,R17
	SEC
	RJMP __DIVF214
__DIVF213:
	CLC
__DIVF214:
	ROL  R21
	ROL  R18
	ROL  R19
	ROL  R1
	ROL  R26
	ROL  R27
	ROL  R24
	ROL  R20
	DEC  R25
	BRNE __DIVF212
	MOVW R30,R18
	MOV  R22,R1
	POP  R20
	POP  R19
	POP  R18
	POP  R17
	TST  R22
	BRMI __DIVF215
	LSL  R21
	ROL  R30
	ROL  R31
	ROL  R22
	DEC  R23
	BRVS __DIVF217
__DIVF215:
	RCALL __ROUND_REPACK
	POP  R21
	RET

__ADDW2R15:
	CLR  R0
	ADD  R26,R15
	ADC  R27,R0
	RET

__ANEGW1:
	NEG  R31
	NEG  R30
	SBCI R31,0
	RET

__ANEGD1:
	COM  R31
	COM  R22
	COM  R23
	NEG  R30
	SBCI R31,-1
	SBCI R22,-1
	SBCI R23,-1
	RET

__LSRW12:
	TST  R30
	MOV  R0,R30
	MOVW R30,R26
	BREQ __LSRW12R
__LSRW12L:
	LSR  R31
	ROR  R30
	DEC  R0
	BRNE __LSRW12L
__LSRW12R:
	RET

__ASRW4:
	ASR  R31
	ROR  R30
__ASRW3:
	ASR  R31
	ROR  R30
__ASRW2:
	ASR  R31
	ROR  R30
	ASR  R31
	ROR  R30
	RET

__LSRW3:
	LSR  R31
	ROR  R30
__LSRW2:
	LSR  R31
	ROR  R30
	LSR  R31
	ROR  R30
	RET

__CWD1:
	MOV  R22,R31
	ADD  R22,R22
	SBC  R22,R22
	MOV  R23,R22
	RET

__CWD2:
	MOV  R24,R27
	ADD  R24,R24
	SBC  R24,R24
	MOV  R25,R24
	RET

__MULW12U:
	MUL  R31,R26
	MOV  R31,R0
	MUL  R30,R27
	ADD  R31,R0
	MUL  R30,R26
	MOV  R30,R0
	ADD  R31,R1
	RET

__MULW12:
	RCALL __CHKSIGNW
	RCALL __MULW12U
	BRTC __MULW121
	RCALL __ANEGW1
__MULW121:
	RET

__CHKSIGNW:
	CLT
	SBRS R31,7
	RJMP __CHKSW1
	RCALL __ANEGW1
	SET
__CHKSW1:
	SBRS R27,7
	RJMP __CHKSW2
	COM  R26
	COM  R27
	ADIW R26,1
	BLD  R0,0
	INC  R0
	BST  R0,0
__CHKSW2:
	RET

__GETW1P:
	LD   R30,X+
	LD   R31,X
	SBIW R26,1
	RET

__GETW1PF:
	LPM  R0,Z+
	LPM  R31,Z
	MOV  R30,R0
	RET

__GETD1S0:
	LD   R30,Y
	LDD  R31,Y+1
	LDD  R22,Y+2
	LDD  R23,Y+3
	RET

__PUTPARD1:
	ST   -Y,R23
	ST   -Y,R22
	ST   -Y,R31
	ST   -Y,R30
	RET

__PUTPARD2:
	ST   -Y,R25
	ST   -Y,R24
	ST   -Y,R27
	ST   -Y,R26
	RET

__CDF2U:
	SET
	RJMP __CDF2U0
__CDF2:
	CLT
__CDF2U0:
	RCALL __SWAPD12
	RCALL __CDF1U0

__SWAPD12:
	MOV  R1,R24
	MOV  R24,R22
	MOV  R22,R1
	MOV  R1,R25
	MOV  R25,R23
	MOV  R23,R1

__SWAPW12:
	MOV  R1,R27
	MOV  R27,R31
	MOV  R31,R1

__SWAPB12:
	MOV  R1,R26
	MOV  R26,R30
	MOV  R30,R1
	RET

__EEPROMRDW:
	ADIW R26,1
	RCALL __EEPROMRDB
	MOV  R31,R30
	SBIW R26,1

__EEPROMRDB:
	SBIC EECR,EEWE
	RJMP __EEPROMRDB
	PUSH R31
	IN   R31,SREG
	CLI
	OUT  EEARL,R26
	OUT  EEARH,R27
	SBI  EECR,EERE
	IN   R30,EEDR
	OUT  SREG,R31
	POP  R31
	RET

__EEPROMWRW:
	RCALL __EEPROMWRB
	ADIW R26,1
	PUSH R30
	MOV  R30,R31
	RCALL __EEPROMWRB
	POP  R30
	SBIW R26,1
	RET

__EEPROMWRB:
	SBIS EECR,EEWE
	RJMP __EEPROMWRB1
	WDR
	RJMP __EEPROMWRB
__EEPROMWRB1:
	IN   R25,SREG
	CLI
	OUT  EEARL,R26
	OUT  EEARH,R27
	SBI  EECR,EERE
	IN   R24,EEDR
	CP   R30,R24
	BREQ __EEPROMWRB0
	OUT  EEDR,R30
	SBI  EECR,EEMWE
	SBI  EECR,EEWE
__EEPROMWRB0:
	OUT  SREG,R25
	RET

__SAVELOCR6:
	ST   -Y,R21
__SAVELOCR5:
	ST   -Y,R20
__SAVELOCR4:
	ST   -Y,R19
__SAVELOCR3:
	ST   -Y,R18
__SAVELOCR2:
	ST   -Y,R17
	ST   -Y,R16
	RET

__LOADLOCR6:
	LDD  R21,Y+5
__LOADLOCR5:
	LDD  R20,Y+4
__LOADLOCR4:
	LDD  R19,Y+3
__LOADLOCR3:
	LDD  R18,Y+2
__LOADLOCR2:
	LDD  R17,Y+1
	LD   R16,Y
	RET

;END OF CODE MARKER
__END_OF_CODE:
