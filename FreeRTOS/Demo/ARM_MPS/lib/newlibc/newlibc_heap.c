/*
 * FreeRTOS
 * Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>

#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"

#ifdef __cplusplus
    extern "C" {
#endif

extern uint8_t __heap_start;
extern uint8_t __heap_end;

static uint8_t * pHeapEnd = &__heap_start;

#if defined( __PICOLIBC__ )
    void * sbrk( ptrdiff_t incr )
    {
        void * pResult = ( void * ) pHeapEnd;

        if( incr < 0 )
        {
            if( ( pHeapEnd - &__heap_start ) < ( -1 * incr ) )
            {
                pResult = ( void * ) ( ( ptrdiff_t ) -1 );
            }
            else
            {
                pHeapEnd += incr;
            }
        }
        else
        {
            if( ( &__heap_end - pHeapEnd ) < incr )
            {
                pResult = ( void * ) ( ( ptrdiff_t ) -1 );
            }
            else
            {
                pHeapEnd += incr;
            }
        }
        return pResult;
    }
#elif defined( __NEWLIB__ )
    void * _sbrk( int incr )
    {
        void * pResult = ( void * ) pHeapEnd;

        if( incr < 0 )
        {
            if( ( pHeapEnd - &__heap_start ) < ( -1 * incr ) )
            {
                pResult = ( void * ) ( ( ptrdiff_t ) -1 );
            }
            else
            {
                pHeapEnd += ( ptrdiff_t ) incr;
            }
        }
        else
        {
            if( ( &__heap_end - pHeapEnd ) < incr )
            {
                pResult = ( void * ) ( ( ptrdiff_t ) -1 );
            }
            else
            {
                pHeapEnd += ( ptrdiff_t ) incr;
            }
        }
        return pResult;
    }
#endif /* __NEWLIB__ */

void __malloc_lock( struct _reent * p_reent )
{
    ( void ) p_reent;

    configASSERT( xPortIsInsideInterrupt() == pdFALSE );
    if( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED )
    {
        vTaskSuspendAll();
    }
}

void __malloc_unlock( struct _reent * p_reent )
{
    ( void ) p_reent;

    configASSERT( xPortIsInsideInterrupt() == pdFALSE );

    if( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED )
    {
        ( void ) xTaskResumeAll();
    }
}

void * pvPortMalloc( size_t xWantedSize )
{
    void * pvReturn = malloc( xWantedSize );

    #if ( configUSE_MALLOC_FAILED_HOOK == 1 )
    {
        if( pvReturn == NULL )
        {
            vApplicationMallocFailedHook();
        }
    }
    #endif

    return pvReturn;
}

void * pvPortCalloc( size_t xNum, size_t xSize )
{
    void * pvReturn = calloc( xNum, xSize );

    #if( configUSE_MALLOC_FAILED_HOOK == 1 )
    {
        if( pvReturn == NULL )
        {
            vApplicationMallocFailedHook();
        }
    }
    #endif

    return pvReturn;
}

void vPortFree( void * pvData )
{
    if( pvData != NULL )
    {
        free( pvData );
    }
}

size_t xPortGetFreeHeapSize( void )
{
    struct mallinfo xMallocInfo = mallinfo();

    return( xMallocInfo.fordblks + ( &__heap_end - pHeapEnd ) );
}

void vPortInitialiseBlocks( void )
{
}

#ifdef __cplusplus
    }
#endif
