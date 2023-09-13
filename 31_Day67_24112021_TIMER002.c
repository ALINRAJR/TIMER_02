/*
===============================================================================
 Name        : 31_Day67_24112021_TIMER002.c
 Date		 : 24-11-2021

 Description : Program to demonstrate the TIMER using interrupt Method

After cycle 2 the timer can stop or reset
 Layered Architecture used for this project
 ************************************
 Application layer-31_Day67_24112021_TIMER002.c
 ************************************
 Board layer -  configboard.h, led.c/.h
 ************************************
 Low level drivers or chip level - pinmux.c/.h,timer.c/.h, gpio.c/.h
 ************************************
 Hardware
 ************************************
===============================================================================
*/

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Private includes ----------------------------------------------------------*/
#include "pinmux.h"
#include "timer.h"
#include "led.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {DEFAULTSTATE=0,STATE0,STATE1,STATE2}states_t;
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
volatile states_t currentstate =DEFAULTSTATE;
/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/
void vLedHandler(uint32_t interruptsource)
{
		switch (interruptsource) {
		case MR0INT:
			currentstate = STATE0;
			break;
		case MR1INT:
			currentstate = STATE1;
			break;
		case MR2INT:
			currentstate = STATE2;
			break;
		case MR3INT:
			currentstate = DEFAULTSTATE;
			break;
		case CAP0INT:
			/* DO NOTHING*/
			break;
		case CAP1INT:
			/* DO NOTHING*/
			break;
		}
}

/**
  * @brief  Initialize all the hardware connected
  * @retval none
  */
void vAppHardwareInit(void)
{
	vPinmuxInitialize();
	vLedInitialize();
	/* Basic configurations of Timer */
	vTimerInitialize(TIMER_0);
	/* Call back registration of vLedHandler with Timer IRQ Handler */
	vTimerIntAttachCallback(TIMER_0,vLedHandler);
	/* Setting the prescalar for making TC count every 1ms */
	vTimerPrescalarSel(TIMER0,PRESCALAR_MS);
	/* Load the respective match registers with required delay in ms */
	vTimerLoadMatchRegister(TIMER0,MATCH0,2000);
	vTimerLoadMatchRegister(TIMER0,MATCH1,5000);
	vTimerLoadMatchRegister(TIMER0,MATCH2,8000);
	vTimerLoadMatchRegister(TIMER0,MATCH3,10000);
	/* Enabling to Reset the timer when TC matches with MR3 register value */
	vTimerMatchReset(TIMER0,MATCH3);
	/* Enabling interrupts for all match registers */
	vTimerInterruptEnable(TIMER0,MATCH0);
	vTimerInterruptEnable(TIMER0,MATCH1);
	vTimerInterruptEnable(TIMER0,MATCH2);
	vTimerInterruptEnable(TIMER0,MATCH3);
	/* Enabling the Timer0 interrupt in NVIC */
	NVIC_EnableIRQ(TIMER0_IRQn);


    /* Enabling the timer and counting starts */
	vTimerControl(TIMER0,TIMER_COUNTER_ENABLE);

}


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Initialize all configured peripherals */
   vAppHardwareInit();
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)  // for(;;)
  {

		switch (currentstate) {
		case DEFAULTSTATE:
			vLedOff(LED_0);
			vLedOff(LED_1);
			vLedOff(LED_2);
			break;
		case STATE0:
			vLedOn(LED_0);
			break;
		case STATE1:
			vLedOn(LED_1);
			break;
		case STATE2:
			vLedOn(LED_2);
			break;

		default:
			/* DO NOTHING */
			break;
		}



  }
  /* End of Application entry point */
}



