/**
************************************************************************************************
* @file   		tcp_demo.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		TCP 演示函数
* @attention  
************************************************************************************************
**/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bsp_usart1.h"
#include "tcp_demo.h"
#include "W5500_conf.h"
#include "w5500.h"
#include "socket.h"
#include "bsp_iap.h"
#include "message.h"


uint8 buff[2048];				                              	     /*定义一个2KB的缓存*/
uint8 firstflag = 0;
void registerDevice(void);
void parseCmd(char *buff);
void Delay(__IO uint32_t nCount);
void ResetDevice(int num);

void do_tcp_server(void)
{	
	uint16 len=0;
	switch(getSn_SR(SOCK_TCPS))											            	/*获取socket的状态*/
	{
		case SOCK_CLOSED:													                  /*socket处于关闭状态*/
			socket(SOCK_TCPS ,Sn_MR_TCP,local_port,Sn_MR_ND);	        /*打开socket*/
			break;     
    
		case SOCK_INIT:														                  /*socket已初始化状态*/
			listen(SOCK_TCPS);		/*socket建立监听*/
			break;
		
		case SOCK_ESTABLISHED:												              /*socket处于连接建立状态*/
			if(getSn_IR(SOCK_TCPS) & Sn_IR_CON)
			{
				setSn_IR(SOCK_TCPS, Sn_IR_CON);								          /*清除接收中断标志位*/
			}
			
			len=getSn_RX_RSR(SOCK_TCPS); 							 /*定义len为已接收数据的长度*/
			if(len>0) {
				recv(SOCK_TCPS,buff,len); 							 /*接收来自Server的数据*/
				buff[len]= '\0';  									 /*添加字符串结束符*/
				parseCmd((char *)buff);
			}
			break;
		
		case SOCK_CLOSE_WAIT:												                /*socket处于等待关闭状态*/
			close(SOCK_TCPS);
			break;
	}
}

bool do_tcp_client(void)
{	

	switch(getSn_SR(SOCK_TCPC)) {			  				         /*获取socket的状态*/
		case SOCK_CLOSED:											 /*socket处于关闭状态*/
			socket(SOCK_TCPC,Sn_MR_TCP,local_port++,Sn_MR_ND);
			break;
		case SOCK_INIT:												 /*socket处于初始化状态*/
			connect(SOCK_TCPC,remote_ip,remote_port);                /*socket连接服务器*/ 
			break;
		case SOCK_ESTABLISHED: 										 /*socket处于连接建立状态*/
			if(getSn_IR(SOCK_TCPC) & Sn_IR_CON) {
				setSn_IR(SOCK_TCPC, Sn_IR_CON); 					 /*清除接收中断标志位*/
			}
			if(!firstflag) {
				registerDevice();  									 /*发送注册信息*/
				firstflag = 1;
			}
			close(SOCK_TCPC);
			return FALSE;
			break;
		case SOCK_CLOSE_WAIT: 										 /*socket处于等待关闭状态*/
			close(SOCK_TCPC);
			firstflag = 0;
			break;
	}
	return TRUE;
}

void registerDevice(void)
{
	char *message = NULL;
	getRegisterMessage("STM32", 2, "10.2.25.108:5000", "DeviceType", &message);
	send(SOCK_TCPC, (uint8_t*)message, strlen(message));
	free(message);
}

void parseCmd(char *buff)
{
	char cmd[6];
	int num;
	int length;
	sscanf(buff, cmdMessageTemplate, cmd, &num, &length);
	if(strcmp(cmd, "bin") == 0) {
		int len;
		ResetDevice(num);
		memset(buff, 0, 2048);
		sprintf(buff, "%d\n", length);
		printf("数据长度为: %s", buff);
		Usart_Printf((uint8_t*)buff, strlen(buff), num);
		
		while(1) {
			if(getSn_IR( SOCK_TCPS) & Sn_IR_CON) {
				setSn_IR(SOCK_TCPS, Sn_IR_CON); 					 /*清除接收中断标志位*/
			}
			len = getSn_RX_RSR(SOCK_TCPS); 							 /*定义len为已接收数据的长度*/
			if(len > 0) {
				memset(buff, 0, 2048);
				recv(SOCK_TCPS, (uint8_t *)buff,len); 							 /*接收来自Server的数据*/
				Usart_Printf((uint8_t*)buff, len, num);
				break;
			}
		}
		while((len = getSn_RX_RSR(SOCK_TCPS)) > 0) {
			if(getSn_IR(SOCK_TCPS) & Sn_IR_CON) {
				setSn_IR(SOCK_TCPS, Sn_IR_CON); 					 /*清除接收中断标志位*/
			}
			recv(SOCK_TCPS, (uint8_t *)buff, len); 							 /*接收来自Server的数据*/
			Usart_Printf((uint8_t*)buff, len, num);
		}
		printf("接收结束\n");
	} else if(strcmp(cmd, "monitor") == 0) {
		send(SOCK_TCPS, "1,1\r\n", 7);
	}
	return;
}

void ResetDevice(int num)
{
	switch(num) {
		case 1:
			GPIO_ResetBits(GPIOD, GPIO_Pin_1);
			Delay(0xfffff);
			GPIO_SetBits(GPIOD, GPIO_Pin_1);
			Delay(0xfffff);
			break;
		case 2:
			GPIO_ResetBits(GPIOD, GPIO_Pin_2);
			Delay(0xfffff);
			GPIO_SetBits(GPIOD, GPIO_Pin_2);
			Delay(0xfffff);
			break;
		default:
			printf("Please check your device number\n");
			break;
	}
	return;
}

void Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}
