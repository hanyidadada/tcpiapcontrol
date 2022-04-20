/**
******************************************************************************
* @file    			main.c
* @author  			WIZnet Software Team
* @version 			V1.0
* @date    			2015-02-14
* @brief   			��3.5.0�汾�⽨�Ĺ���ģ��
* @attention  	ʵ��ƽ̨��Ұ�� iSO-V3 STM32 ������ + Ұ��W5500���������
*
*               Ĭ��ʹ��Ұ�𿪷����SPI2�ӿ�
*							
*               �������ԣ��뱣֤W5500��IP�����PC����ͬһ�����ڣ��Ҳ���ͻ
*               ����������߸�PC��ֱ����������PC���������ӵ�ַIPΪ��̬IP
*
* ʵ��ƽ̨:Ұ�� iSO-V3 STM32 ������ 
* ��̳    :http://www.firebbs.cn
* �Ա�    :https://fire-stm32.taobao.com
******************************************************************************
*/ 
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_i2c_ee.h"
#include "bsp_i2c_gpio.h"
#include "bsp_led.h"

#include "w5500.h"
#include "W5500_conf.h"
#include "socket.h"
#include "utility.h"
#include "dhcp.h"
/*app����ͷ�ļ�*/
#include "tcp_demo.h"
//uint8 buff[2048];
int main(void)
{ 	
	systick_init(72);										/*��ʼ��Systick����ʱ��*/
	USART1_Config();										/*��ʼ������ͨ��:115200@8-n-1*/
	USART2_Config();										/*��ʼ������ͨ��:115200@8-n-1*/
	USART3_Config();
	GPIONRST_Config();
	i2c_CfgGpio();											/*��ʼ��eeprom*/
	gpio_for_w5500_config();						        /*��ʼ��MCU�������*/
	reset_w5500();											/*Ӳ��λW5500*/
	set_w5500_mac();										/*����MAC��ַ*/										/*����IP��ַ*/
	
	socket_buf_init(txsize, rxsize);		/*��ʼ��8��Socket�ķ��ͽ��ջ����С*/
	
	printf(" STM32����IAP \r\n");
	while(1) {
		do_dhcp();                      /*��DHCP��������ȡIP��ַ*/
		if(dhcp_ok==1) { 				/*��ȡIP��ַ֮��*/
			break;
		}
	}
	while(do_tcp_client());
	
	local_port = 5000;
	printf(" W5500 �����˿�:%d\r\n", local_port);
	while(1) {                          /*ѭ��ִ�еĺ���*/                 
		do_tcp_server();/*TCP_Client ���ݻػ����Գ���*/
	}
} 


