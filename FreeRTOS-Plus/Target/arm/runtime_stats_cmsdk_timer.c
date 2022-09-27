/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*
 * Utility functions required to gather run time statistics.  See:
 * https://www.FreeRTOS.org/rtos-run-time-stats.html
 *
 * Note that this is a simulated port, where simulated time is a lot slower than
 * real time, therefore the run time counter values have no real meaningful
 * units.
 *
 * Also note that it is assumed this demo is going to be used for short periods
 * of time only, and therefore timer overflows are not handled.
 */

#include <time.h>

/* FreeRTOS includes. */
#include <FreeRTOS.h>

#include "CMSDK_CM3.h"

/* Time at start of day (in ns). */
static volatile unsigned long ulRunTimeOverflowCount = 0U;

#define TIMER_POSTSCALER    ( 8UL )

/*-----------------------------------------------------------*/

void TIMER0_Handler( void )
{
    ulRunTimeOverflowCount++;

    /* Reload the timer and clear the flag */
    CMSDK_TIMER0->RELOAD = 0xFFFFFFFF;

    /* Clear interrupt flag */
    CMSDK_TIMER0->INTCLEAR |= ~CMSDK_TIMER_INTCLEAR_Msk;
}

/*-----------------------------------------------------------*/

void vConfigureTimerForRunTimeStats( void )
{
    /* PCLK / SystemCoreClock is 25MHz, Timer clock is always PCLK */

    CMSDK_TIMER0->CTRL &= ~( CMSDK_TIMER_CTRL_EN_Msk );

    CMSDK_TIMER0->RELOAD = 0xFFFFFFFF;

    /* Enable overflow interrupt and start the timer */
    CMSDK_TIMER0->CTRL |= CMSDK_TIMER_CTRL_IRQEN_Msk;
    CMSDK_TIMER0->CTRL |= CMSDK_TIMER_CTRL_EN_Msk;

}

/*-----------------------------------------------------------*/

unsigned long ulGetRunTimeCounterValue( void )
{
    unsigned long ulTimerValue = CMSDK_TIMER0->RELOAD - CMSDK_TIMER0->VALUE;

    /*
     * 32 bits will overflow after ~ ( 2**32 / 25000000 ) == 171 seconds,
     * So we remove the lower 8 bits and borrow 8 bits from the overflow counter.
     */

    ulTimerValue = ( ulTimerValue >> TIMER_POSTSCALER );

    /* Add remaining 8 bits from ulRunTimeOverflowCount */
    ulTimerValue |= ( ulRunTimeOverflowCount << ( 32UL - TIMER_POSTSCALER ) );

    return ulTimerValue;
}

/*-----------------------------------------------------------*/
