/*

*	文件名：GPU.c
*    	定义GPU函数
**/

#include "gpio.h"
#include "GPU.h"
#include "Global.h"

//初始化
unsigned int init_GPU(unsigned int width, unsigned int height, unsigned int bitDepth)
{
	volatile unsigned int GPU_MSG = 0;
	
	/*参数检查*/
	if(width > 4096 || height > 4096 || bitDepth > 32)
	{
		return 0;
	}
	struct FrameBufferInfoS *bufInfo = (struct FrameBufferInfoS *) &FrameBufferInfo;

	bufInfo->phyWidth = width;
	bufInfo->phyHeight = height;
	bufInfo->virWidth = width;
	bufInfo->virHeight = height;
	bufInfo->bitDepth = bitDepth;
	
	/*禁用cache，读取内存值，防止产生误差。0x40000000 */
	GPU_MSG = (unsigned int) bufInfo + 0x40000000; 
	GPU_SendMail(GPU_MSG, 1);
	return GPU_RecMail(1);
}



int GPU_SendMail(unsigned int GPU_MSG, unsigned int channel)
{
	volatile unsigned int *Status = (unsigned int *)GPU_Status;
	volatile unsigned int *Write = (unsigned int *) GPU_Write;
	
	/*参数检查*/
	if((GPU_MSG & 0xF) != 0)
	{
		return -1;
	}

	if((channel > 15) || (channel < 0))
	{
		return -1;
	}

	/*循环等待状态寄存器的最高为为0.则可以发送消息*/
	while((*Status & 0x80000000) != 0);

	/*按照格式发送消息到GPU*/
	GPU_MSG = GPU_MSG + channel;  
	*Write = GPU_MSG;
	
	return 0;
}



unsigned int GPU_RecMail(unsigned int channel)
{
	volatile unsigned int *Status = (unsigned int *)GPU_Status;
	volatile unsigned int *Read = (unsigned int *) GPU_Read;
	volatile unsigned int GPU_MSG = 0;
	
	/*参数检查*/
	if((channel > 15) || (channel < 0))
	{
		return 0;
	}
	do{
		/*循环等待状态寄存器30bit为0.则可以接收消息*/
		while((*Status & 0x40000000) != 0);
		
		GPU_MSG = *Read;
		/*通过channel判断是否是所需的消息*/
		if((GPU_MSG & 0xF) == channel)
		{
			/*GPU_MSG = 0 表示成功发送消息，否则失败*/
			GPU_MSG = (GPU_MSG & 0xFFFFFFF0);
			if(GPU_MSG == 0)
			{
				return (unsigned int)&FrameBufferInfo;				
			}
		}
	}while(1);
}

