
#include "RTE_Components.h"
#include CMSIS_device_header
#include "cmsis_os.h"
#include "stm32f4xx.h"
#include "Board_LED.h"

#define OS_ROBIN 0							//Disable round robin

#define LED1 0
#define Common_LED 1
#define LED2 2

#define p1 2
#define p2 3

#define hp 6

#define pe1 1
#define pe2 1

static void Ptimer_callback(void const *arg);
static uint32_t parg;

static osTimerId Ptimer;
static osTimerDef(Pt,Ptimer_callback);



void Child1(void const *argument);
void Child2(void const *argument);

void myDelay(int i){
	while(i--);
}

int count=0;
osThreadId ID1,ID2;

osThreadDef (Child1, osPriorityNormal, 1, 0);         // thread object
osThreadDef (Child2, osPriorityNormal, 1, 0);         // thread object


//osStatus shm;

static void Ptimer_callback(void const *arg)
{
	count++;
	osThreadTerminate(ID1);
	osThreadTerminate(ID2);
	
	
	if(count%p1 == 0)
	{
		ID1 = osThreadCreate(osThread(Child1),NULL);
	}
	else if(count%p2 == 0 || (count-pe1)%hp == 0)
	{
		ID2 = osThreadCreate(osThread(Child2),NULL);
	}
}

int main()
{
	
	LED_Initialize();
	SystemCoreClockUpdate();

	osKernelInitialize();
	Ptimer = osTimerCreate(osTimer(Pt),osTimerPeriodic,&parg);

	osKernelStart();
	osTimerStart(Ptimer,1000);
	while(1)
	{
		osThreadYield();
	}
	
	
	
}
void Child2 (void const *argument) { 
	for (;;) 
	{
		LED_On(LED1);
		myDelay(100000);
		LED_Off(LED1);
		myDelay(100000);
	}
}

void Child1(void const *arguments){
	for (;;) 
	{
		LED_On(LED2);
		myDelay(100000);
		LED_Off(LED2);
		myDelay(100000);
	}
}

