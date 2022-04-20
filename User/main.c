/**
******************************************************************************
* @file    			main.c
* @author  			WIZnet Software Team
* @version 			V1.0
* @date    			2015-02-14
* @brief   			用3.5.0版本库建的工程模板
* @attention  	实验平台：野火 iSO-V3 STM32 开发板 + 野火W5500网络适配板
*
*               默认使用野火开发板的SPI2接口
*							
*               内网测试，请保证W5500的IP与测试PC机在同一网段内，且不冲突
*               如果是用网线跟PC机直连，请设置PC机本地连接地址IP为静态IP
*
* 实验平台:野火 iSO-V3 STM32 开发板 
* 论坛    :http://www.firebbs.cn
* 淘宝    :https://fire-stm32.taobao.com
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
/*app函数头文件*/
#include "tcp_demo.h"
//uint8 buff[2048];
int main(void)
{ 	
	systick_init(72);										/*初始化Systick工作时钟*/
	USART1_Config();										/*初始化串口通信:115200@8-n-1*/
	USART2_Config();										/*初始化串口通信:115200@8-n-1*/
	USART3_Config();
	GPIONRST_Config();
	i2c_CfgGpio();											/*初始化eeprom*/
	gpio_for_w5500_config();						        /*初始化MCU相关引脚*/
	reset_w5500();											/*硬复位W5500*/
	set_w5500_mac();										/*配置MAC地址*/										/*配置IP地址*/
	
	socket_buf_init(txsize, rxsize);		/*初始化8个Socket的发送接收缓存大小*/
	
	printf(" STM32网络IAP \r\n");
	while(1) {
		do_dhcp();                      /*从DHCP服务器获取IP地址*/
		if(dhcp_ok==1) { 				/*获取IP地址之后*/
			break;
		}
	}
	while(do_tcp_client());
	
	local_port = 5000;
	printf(" W5500 监听端口:%d\r\n", local_port);
	while(1) {                          /*循环执行的函数*/                 
		do_tcp_server();/*TCP_Client 数据回环测试程序*/
	}
} 


