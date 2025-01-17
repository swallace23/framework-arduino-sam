/**************************************************************************//**
 * @file     system_<Device>.c
 * @brief    CMSIS Cortex-M# Device Peripheral Access Layer Source File
 *           for the Device Series ...
 * @version  V2.10
 * @date     20. July 2011
 *
 * @note
 * Copyright (C) 2010-2011 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M 
 * processor based microcontrollers.  This file can be freely distributed 
 * within development tools that are supporting such ARM based processors. 
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/


#include <stdint.h>
#include "<Device>.h"


/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/
    
/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
/* ToDo: add here your necessary defines for device initialization     
         following is an example for different system frequencies             */
#define __HSI             ( 6000000UL)
#define __XTAL            (12000000UL)    /* Oscillator frequency             */
#define __SYS_OSC_CLK     (    ___HSI)    /* Main oscillator frequency        */

#define __SYSTEM_CLOCK    (4*__XTAL)


/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
/* ToDo: initialize SystemCoreClock with the system core clock frequency value
         achieved after system intitialization.
         This means system core clock frequency after call to SystemInit()    */
uint32_t SystemCoreClock = __SYSTEM_CLOCK;  /*!< System Clock Frequency (Core Clock)*/


/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
/* ToDo: add code to calculate the system frequency based upon the current
         register settings.
         This function can be used to retrieve the system core clock frequeny
         after user changed register sittings.                                */
  EFC0->EEFC_FMR = EEFC_FMR_FWS(4);
	EFC1->EEFC_FMR = EEFC_FMR_FWS(4);

	/*MODIFIED FOR 317 LAB EXTERNAL CLOCK - Sean*/
	/* Initialize main oscillator */
	if (!(PMC->CKGR_MOR & CKGR_MOR_MOSCSEL)) {
		/*PMC->CKGR_MOR = SYS_CKGR_MOR_KEY_VALUE | SYS_BOARD_OSCOUNT | 
			                     CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTBY;*/
		PMC->CKGR_MOR = SYS_CKGR_MOR_KEY_VALUE | SYS_BOARD_OSCOUNT | 
			                     CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN;
		while (!(PMC->PMC_SR & PMC_SR_MOSCXTS)) {
		}
	}
	/*MODIFIED FOR 317 LAB EXTERNAL CLOCK - Sean*/
	/* Switch to 3-20MHz Xtal oscillator */
	/*PMC->CKGR_MOR = SYS_CKGR_MOR_KEY_VALUE | SYS_BOARD_OSCOUNT | 
                CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTBY | CKGR_MOR_MOSCSEL;*/

	PMC->CKGR_MOR = SYS_CKGR_MOR_KEY_VALUE | SYS_BOARD_OSCOUNT | 
	                           CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCSEL;

	while (!(PMC->PMC_SR & PMC_SR_MOSCSELS)) {
	}
 	PMC->PMC_MCKR = (PMC->PMC_MCKR & ~(uint32_t)PMC_MCKR_CSS_Msk) | 
		                     PMC_MCKR_CSS_MAIN_CLK;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY)) {
	}

	/* Initialize PLLA */
	PMC->CKGR_PLLAR = SYS_BOARD_PLLAR;
	while (!(PMC->PMC_SR & PMC_SR_LOCKA)) {
	}

	/* Switch to main clock */
	PMC->PMC_MCKR = (SYS_BOARD_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY)) {
	}

	/* Switch to PLLA */
	PMC->PMC_MCKR = SYS_BOARD_MCKR;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY)) {
	}

	SystemCoreClock = CHIP_FREQ_CPU_MAX;
  //SystemCoreClock = __SYSTEM_CLOCK;
}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void)
{
/* ToDo: add code to initialize the system
         do not use global variables because this function is called before
         reaching pre-main. RW section maybe overwritten afterwards.          */
  SystemCoreClock = __SYSTEM_CLOCK;
}
