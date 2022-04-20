/**
************************************************************************************************
* @file   		tcp_demo.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		TCP ��ʾ����
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


uint8 buff[2048];				                              	     /*����һ��2KB�Ļ���*/
uint8 firstflag = 0;
void registerDevice(void);
void parseCmd(char *buff);
void Delay(__IO uint32_t nCount);
void ResetDevice(int num);

void do_tcp_server(void)
{	
	uint16 len=0;
	switch(getSn_SR(SOCK_TCPS))											            	/*��ȡsocket��״̬*/
	{
		case SOCK_CLOSED:													                  /*socket���ڹر�״̬*/
			socket(SOCK_TCPS ,Sn_MR_TCP,local_port,Sn_MR_ND);	        /*��socket*/
			break;     
    
		case SOCK_INIT:														                  /*socket�ѳ�ʼ��״̬*/
			listen(SOCK_TCPS);		/*socket��������*/
			break;
		
		case SOCK_ESTABLISHED:												              /*socket�������ӽ���״̬*/
			if(getSn_IR(SOCK_TCPS) & Sn_IR_CON)
			{
				setSn_IR(SOCK_TCPS, Sn_IR_CON);								          /*��������жϱ�־λ*/
			}
			
			len=getSn_RX_RSR(SOCK_TCPS); 							 /*����lenΪ�ѽ������ݵĳ���*/
			if(len>0) {
				recv(SOCK_TCPS,buff,len); 							 /*��������Server������*/
				buff[len]= '\0';  									 /*����ַ���������*/
				parseCmd((char *)buff);
			}
			break;
		
		case SOCK_CLOSE_WAIT:												                /*socket���ڵȴ��ر�״̬*/
			close(SOCK_TCPS);
			break;
	}
}

bool do_tcp_client(void)
{	

	switch(getSn_SR(SOCK_TCPC)) {			  				         /*��ȡsocket��״̬*/
		case SOCK_CLOSED:											 /*socket���ڹر�״̬*/
			socket(SOCK_TCPC,Sn_MR_TCP,local_port++,Sn_MR_ND);
			break;
		case SOCK_INIT:												 /*socket���ڳ�ʼ��״̬*/
			connect(SOCK_TCPC,remote_ip,remote_port);                /*socket���ӷ�����*/ 
			break;
		case SOCK_ESTABLISHED: 										 /*socket�������ӽ���״̬*/
			if(getSn_IR(SOCK_TCPC) & Sn_IR_CON) {
				setSn_IR(SOCK_TCPC, Sn_IR_CON); 					 /*��������жϱ�־λ*/
			}
			if(!firstflag) {
				registerDevice();  									 /*����ע����Ϣ*/
				firstflag = 1;
			}
			close(SOCK_TCPC);
			return FALSE;
			break;
		case SOCK_CLOSE_WAIT: 										 /*socket���ڵȴ��ر�״̬*/
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
		printf("���ݳ���Ϊ: %s", buff);
		Usart_Printf((uint8_t*)buff, strlen(buff), num);
		
		while(1) {
			if(getSn_IR( SOCK_TCPS) & Sn_IR_CON) {
				setSn_IR(SOCK_TCPS, Sn_IR_CON); 					 /*��������жϱ�־λ*/
			}
			len = getSn_RX_RSR(SOCK_TCPS); 							 /*����lenΪ�ѽ������ݵĳ���*/
			if(len > 0) {
				memset(buff, 0, 2048);
				recv(SOCK_TCPS, (uint8_t *)buff,len); 							 /*��������Server������*/
				Usart_Printf((uint8_t*)buff, len, num);
				break;
			}
		}
		while((len = getSn_RX_RSR(SOCK_TCPS)) > 0) {
			if(getSn_IR(SOCK_TCPS) & Sn_IR_CON) {
				setSn_IR(SOCK_TCPS, Sn_IR_CON); 					 /*��������жϱ�־λ*/
			}
			recv(SOCK_TCPS, (uint8_t *)buff, len); 							 /*��������Server������*/
			Usart_Printf((uint8_t*)buff, len, num);
		}
		printf("���ս���\n");
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
