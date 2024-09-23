#include "FreeRtos_demo.h"
#include "FreeRtos.h"
#include "task.h"
#include "queue.h"
#include "IIC.h"
#include "timers.h"
#include "queue.h"
#include "menu.h"
#define START_TASK_PRIO         1
#define START_TASK_STACK_SIZE   64
TaskHandle_t   start_task_handler;


#define TASK1_PRIO         2
#define TASK1_STACK_SIZE   64
TaskHandle_t   task1_handler;



#define TASK2_PRIO        3
#define TASK2_STACK_SIZE   64
TaskHandle_t task2_handler;

#define TASK3_PRIO        4
#define TASK3_STACK_SIZE   32
TaskHandle_t task3_handler;


uint16_t time_count;  //记录任务1的持续时间，用来实现睡眠模式，当任务1执行4s时，会让stm32进入睡眠模式
TimerHandle_t timer1_handle =0;  //定时器的句柄
uint8_t rec=0;  //从队列1读出来的按键值 有 1 2  3  4 
extern Function * menu_second_list[6]; //二级菜单列表
static uint8_t time_flag=0;   //定时器的反转flag
QueueHandle_t queue1;   //消息队列1 
/**
 * @description: 任务1函数：每隔1s显示一下图片
 * @return {*}
 */
void Task1(void * pvParameters)
	{
		xTimerStart(timer1_handle, portMAX_DELAY);  //开启定时器
    while(1)
    {
		time_count++;
		xQueueReceive(queue1,&rec,NULL);
		if(rec==3) //如果按下按键3 代表需要执行任务2
		{ 
			
			rec=0;
			//vTaskSuspend(task1_handler);		 这里不能挂起自己，挂起自己就不能恢复任务2 	
			vTaskResume(task2_handler);  //执行任务2不需要关闭什么
		//vTaskSuspend(task1_handler);		 这里不能挂起自己，恢复任务2就直接跳转到任务2了因为任务2优先级高，
		}
		if(time_count>4)  //如果任务1执行时间超过4s
		{
			
		SysTick -> CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭systick时钟
		OLED_Clear();                                 //清屏
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,DISABLE); //关闭oled时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,DISABLE);

			__WFI();	 //执行睡眠模式
		//醒了以后要继续执行，所以要把时钟都开
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		SysTick -> CTRL |= SysTick_CTRL_ENABLE_Msk;		//开启定时器



		time_count=0;  //重置任务1持续时间
		}
		


			OLED_Showtime();
      vTaskDelay(1000);
    }
}
	/**
* @description: 实现一级菜单的上下切换，并实现一级菜单到二级菜单的切换
 * @return {*}
 */
void Task2(void *pvParameters)
{	
	vTaskSuspend(task1_handler);
	xTimerStop(timer1_handle, portMAX_DELAY); //暂停软件定时器1 定时器1会执行oledshow 实现呼吸灯
//这里只会执行一次，所以后面还要再加开启和关闭软件定时器
	uint8_t flag=1;  //判断当前是一级菜单还是二级菜单 0是二级菜单 1是一级菜单
	OLED_Clear(); 
	Function * current=menu_list[0];
	OLED_ShowIMG(1,30,96,8,current->img);  //显示主菜单
	while(1)
	{
		
			xQueueReceive(queue1,&rec,NULL);
			
			if(rec==1)  //如果等于1
			{
				OLED_Clear(); //清屏然后切换下一个菜单并且显示图片
				current=current->Next;
				if(flag)
				OLED_ShowIMG(1,30,96,8,current->img);
				
			}
			else if(rec==2)
				{
						OLED_Clear();
						current=current->Last;//清屏然后切换上一个菜单并且显示图片
						if(flag)
						{
								OLED_ShowIMG(1,30,96,8,current->img);
						}

				}else if(rec==3)
						{
								OLED_Clear(); //清屏，判断是否在二级菜单且二级菜单有任务句柄 有就挂起自己然后执行对应的任务函数
								if(current->task_handler!=NULL&&(flag==0))
									{	
											vTaskResume(current->task_handler);
											vTaskSuspend(NULL);
										//执行完成后记得清屏
											OLED_Clear();
				
									}
									//如果当前的目录有下级目录，就会将current切换到下级目录并且将flag置为0
								if(current->down!=NULL)
									{
									
									current=current->down;
									flag=0;
									}
						}else if(rec==4) //按键4 如果flag=1也就是在一级菜单，恢复任务1 开启软件定时1，挂起自己 
										{
												OLED_Clear();
												if(flag){
																		vTaskResume(task1_handler);
																		xTimerStart(timer1_handle, portMAX_DELAY);
																		vTaskSuspend(NULL);
													//从任务1恢复过来以后清屏，显示当前的菜单，然后挂起任务1 关闭软件定时器
																		OLED_Clear();
																		OLED_ShowIMG(1,30,96,8,current->img);
																		vTaskSuspend(task1_handler);
																		xTimerStop(timer1_handle, portMAX_DELAY);
															}	else
												{//如果flag=0在二级菜单，将current切换到上一级就行，然后将flag置1
																		current=current->up;
																		flag=1;
																		OLED_ShowIMG(1,30,96,8,current->img);
																}
											}
			if(flag==0) //flag=0 只显示二级菜单
			{ //里层菜单 
					OLED_ShowString(1,1,(char *)current->Fname); 
					OLED_ShowChar(1,15,'<');
					OLED_ShowString(2,1,(char *)current->Next->Fname);
					OLED_ShowString(3,1,(char *)current->Next->Next->Fname);
					OLED_ShowString(4,1,(char *)current->Next->Next->Next->Fname);
			}

			 rec=0;
	     vTaskDelay(500);
			
	}
	
}

/**
 * @description: 任务3函数：显示当前还有多少内存剩余
 * @return {*}
 */
void Task3(void * pvParameters)
{
	OLED_Clear();
	while(1)
	{
		
		xQueueReceive(queue1,&rec,NULL); //当获取到按键4时，释放task2，挂起自己，如果恢复任务3，第一时间清屏
		if(rec==4)
		{
					rec=0;
					vTaskResume(task2_handler);
					vTaskSuspend(NULL);
			    OLED_Clear();
			
		}
		OLED_ShowString(1,1,"last memory is ");
		OLED_ShowNum(2,1,xPortGetFreeHeapSize(),4);
		vTaskDelay(1000);
	}
	
}
/**
 * @description: 软件定时器1的回调函数：用来实现呼吸灯的效果 每隔一秒亮灭图片
 * @return {*}
 */

void timer1_callback(TimerHandle_t pxTimer)
{


		if(time_flag)
		OLED_ShowIMG_zero();
		else
		OLED_ShowIMG_point();
		time_flag=!time_flag;
 
}
/**
 * @description: 开始函数：创建软件定时器1 开启定时器1 并且创建task1任务用来显示时间 
 * @return {*}
 */
void Start_Task(void *pvParameters)
{
    taskENTER_CRITICAL(); /* 进入临界区 关中断*/ 

		timer1_handle = xTimerCreate("timer1", 1000, pdTRUE,(void *)1,timer1_callback);
		xTaskCreate((TaskFunction_t)Task1,
                (char *)"Task1",
                (configSTACK_DEPTH_TYPE)TASK1_STACK_SIZE ,
                (void *)NULL,
                (UBaseType_t)TASK1_PRIO ,
                (TaskHandle_t *)&task1_handler);
		xTaskCreate((TaskFunction_t)Task2,
                (char *)"Task2",
                (configSTACK_DEPTH_TYPE)TASK2_STACK_SIZE ,
                (void *)NULL,
                (UBaseType_t)TASK2_PRIO ,
                (TaskHandle_t *)&task2_handler);
		xTaskCreate((TaskFunction_t)Task3,
                (char *)"Task3",
                (configSTACK_DEPTH_TYPE)TASK3_STACK_SIZE ,
                (void *)NULL,
                (UBaseType_t)TASK3_PRIO ,
                (TaskHandle_t *)&task3_handler);
		queue1 = xQueueCreate(1, sizeof(uint8_t));		 //创建信号队列 大小为一		
		menu_second_list[0]->task_handler=task3_handler;  //让二级菜单列表绑定对应的任务
    vTaskDelete(start_task_handler);  
		vTaskSuspend(task2_handler);  //挂起任务2
	  vTaskSuspend(task3_handler); //挂起任务3
		//只执行任务1 就是显示时间
    taskEXIT_CRITICAL(); /* 退出临界区 */
}



/**
 * @description: FreeRTOS入口函数：创建任务函数并开始调度
 * @return {*}
 */
void FreeRTOS_Start(void)
{
		xTaskCreate((TaskFunction_t)Start_Task,
         (char *)"Start_Task",
         (configSTACK_DEPTH_TYPE)START_TASK_STACK_SIZE,
         (void *)NULL,
         (UBaseType_t)START_TASK_PRIO ,
         (TaskHandle_t *)&start_task_handler);
   vTaskStartScheduler();

}


