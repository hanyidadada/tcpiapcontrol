#ifndef __BSP_LED_H
#define __BSP_LED_H

#define LED_CLK       RCC_APB2Periph_GPIOC
#define LED_PORT      GPIOC
#define LED_PIN       GPIO_Pin_13


#define digitalHi(p,i)      {p->BSRR = i;}
#define digitalLo(p,i)      {p->BRR = i;}
#define digitalToggle(p,i)  {p->ODR ^= i;}

#define LED_ON          digitalLo(LED_PORT, LED_PIN)
#define LED_OFF         digitalHi(LED_PORT, LED_PIN)
#define LED_TOGGLE      digitalToggle(LED_PORT, LED_PIN)


void LED_Config(void);
#endif
