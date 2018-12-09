#include "system_stm32f10x.h"
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
//#include "timers.h"
#include "semphr.h"

#include "lwip/netif.h"
#include "lwipopts.h"
#include "drv_ethernet.h"
#include "tcpecho_raw.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/init.h"
#include "ethernet.h" 
#include "drv_ethernet.h"
#include "tcp_server_demo.h" 
#include "tcpecho.h"


////为LWIP提供计时
extern u32 lwip_localtime;

#if LWIP_DHCP
extern u8  dhcpstatus;
#endif

void TASK_MAIN(void* pvParameters);

xTaskHandle x_Handle_MAIN;
xTaskHandle x_Handle_TcpEcho;
int main(void)
{
	  SystemInit();	/* 系统初始化 */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭JTAG调试接口打开ＳＷＤ调试接口
		#if LWIP_DHCP
		while(dhcpstatus!=2&&dhcpstatus!=0xFF);
		{
			lwip_periodic_handle();
		}
		#endif
		xTaskCreate(TASK_MAIN, "Main", 500, NULL, 3, &x_Handle_MAIN);//创建任务
		//tcpecho_raw_init();//初始化lwip
		vTaskStartScheduler();
	  
			
		return 0;																						
}

void TASK_MAIN(void* pvParameters)
{
	while(lwip_comm_init());
	tcpecho_init();
	while(1)
	{	
		lwip_localtime = xTaskGetTickCount();
		//tcp_server_test();  	//TCP Server模式
		//lwip_periodic_handle();
	}

}

