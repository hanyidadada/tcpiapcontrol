#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Config(void);
void USART2_Config(void);
void USART3_Config(void);
void GPIONRST_Config(void);
void Usart_Printf(uint8_t *str, uint32_t len, int num);
//int fputc(int ch, FILE *f);
//void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);

#endif /* __USART1_H */
