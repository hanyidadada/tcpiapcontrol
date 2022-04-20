#include "stm32f10x.h"
#include "bsp_usart.h"

void USART_Config(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(DEBUG_USART_CLK | DEBUG_USART_TX_CLK | DEBUG_USART_TX_CLK, ENABLE);
    //TX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = DEBUG_USART_TX_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DEBUG_USART_TX_PORT, &GPIO_InitStruct);
    //RX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = DEBUG_USART_RX_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DEBUG_USART_RX_PORT, &GPIO_InitStruct);

    USART_InitTypeDef USART_InitStruct;

    USART_InitStruct.USART_BaudRate = DEBUG_USART_BAUDRATE;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(DEBUG_USARTx, &USART_InitStruct);

    USART_Cmd(DEBUG_USARTx, ENABLE);
}

void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure); 
	USART_Cmd(USART2, ENABLE);
}

void Usart_Printf(uint8_t *str, uint32_t len){
		
	uint32_t data=0;
	do{
	  USART_SendData(USART2,str[data]);//发送一个字节
	  while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);//等待单字发送完成
		data++;
	}
	while(data < len);  //判断数据是否发送完成                                                  
		
}

int fputc(int ch, FILE *fp) {
    USART_SendData(DEBUG_USARTx, ch);
    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET){
    }
    return ch;
}

int fgetc(FILE *fp) {
    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET){
    }
    return (int)(USART_ReceiveData(DEBUG_USARTx));
}
