#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_usart.h"
#include "bsp_flash.h"
#include "bsp_iap.h"

uint8_t buff[10*1024] __attribute__ ((at(0X20002000)));
void Delay(__IO u32 nCount); 
void systick_init(u8 sysclk);
int main(void)
{
	int len;
	uint16_t i;
	/* LED 端口初始化 */
	systick_init(72);
	LED_Config();	 
	LED_ON;
	/* 初始化串口 */
	USART_Config();
	USART2_Config();
	while (1) {
		len = 0;
		scanf("%d", &len);
		sprintf(buff, "%d\n", len);
		Usart_Printf(buff, 4);
		for (i =0; i < len; i++) {
			buff[i] = getchar();
		}
		IAP_WRITEAPP2FLASH(FLASH_APP1_ADDR, buff, len);
		
		IAP_LoadAPP(FLASH_APP1_ADDR);
	}
}


void Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}

static u8  fac_us=0;
static u16 fac_ms=0;
void systick_init (u8 sysclk)
{
	SysTick->CTRL&=0xfffffffb;																						/*bit2清空,选择外部时钟  HCLK/8*/
	fac_us=sysclk/8;		    
	fac_ms=(u16)fac_us*1000;
}
