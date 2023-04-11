/*
 * FreeRTOS
 * Copyright (C) 2023 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

/* newlibc / picolibc includes */
#include "newlib.h"
#include <sys/lock.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "portmacro.h"

#if( _RETARGETABLE_LOCKING == 1 )
    struct __lock
    {
        SemaphoreHandle_t xHandle;
        StaticSemaphore_t xSemaphoreBuffer;
    };

    struct __lock __lock___sinit_recursive_mutex;
    struct __lock __lock___sfp_recursive_mutex;
    struct __lock __lock___atexit_recursive_mutex;
    struct __lock __lock___at_quick_exit_mutex;
    struct __lock __lock___malloc_recursive_mutex;
    struct __lock __lock___env_recursive_mutex;
    struct __lock __lock___tz_mutex;
    struct __lock __lock___dd_hash_mutex;
    struct __lock __lock___arc4random_mutex;

    __attribute__((constructor))
    void newlib_retarget_locks_init( void )
    {
        __lock___sinit_recursive_mutex.xHandle = xSemaphoreCreateRecursiveMutexStatic( &( __lock___sinit_recursive_mutex.xSemaphoreBuffer ) );
        __lock___sfp_recursive_mutex.xHandle = xSemaphoreCreateRecursiveMutexStatic( &( __lock___sfp_recursive_mutex.xSemaphoreBuffer ) );
        __lock___atexit_recursive_mutex.xHandle = xSemaphoreCreateRecursiveMutexStatic( &( __lock___atexit_recursive_mutex.xSemaphoreBuffer ) );
        __lock___at_quick_exit_mutex.xHandle = xSemaphoreCreateMutexStatic( &( __lock___at_quick_exit_mutex.xSemaphoreBuffer ) );
        __lock___malloc_recursive_mutex.xHandle = xSemaphoreCreateRecursiveMutexStatic( &( __lock___malloc_recursive_mutex.xSemaphoreBuffer ) );
        __lock___env_recursive_mutex.xHandle = xSemaphoreCreateRecursiveMutexStatic( &( __lock___env_recursive_mutex.xSemaphoreBuffer ) );
        __lock___tz_mutex.xHandle = xSemaphoreCreateMutexStatic( &( __lock___tz_mutex.xSemaphoreBuffer ) );
        __lock___dd_hash_mutex.xHandle = xSemaphoreCreateMutexStatic( &( __lock___dd_hash_mutex.xSemaphoreBuffer ) );
        __lock___arc4random_mutex.xHandle = xSemaphoreCreateMutexStatic( &( __lock___arc4random_mutex.xSemaphoreBuffer ) );
    }

    void __retarget_lock_init( struct __lock * * ppxLock )
    {
        if( ppxLock )
        {
            if( *ppxLock == NULL )
            {
                *ppxLock = pvPortMalloc( sizeof( struct __lock ) );
            }

            if( *ppxLock != NULL )
            {
                ( *ppxLock )->xHandle = xSemaphoreCreateMutexStatic( &( ( *ppxLock )->xSemaphoreBuffer ) );

                configASSERT( ( *ppxLock )->xHandle != NULL );
            }
        }
    }

    void __retarget_lock_close( struct __lock * pxLock )
    {
        configASSERT( pxLock != NULL );

        if( ( pxLock != NULL ) &&
            ( pxLock->xHandle != NULL ) )
        {
            /* Mutex should not be held when released */
            configASSERT( xSemaphoreGetMutexHolder( pxLock->xHandle ) == NULL );

            vSemaphoreDelete( pxLock->xHandle );

            pxLock->xHandle = NULL;
        }
    }

    void __retarget_lock_acquire( struct __lock * pxLock )
    {
        if( ( pxLock != NULL ) &&
            ( pxLock->xHandle != NULL ) &&
            ( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED ) &&
            ( xPortIsInsideInterrupt() == pdFALSE ) )
        {
            ( void ) xSemaphoreTake( pxLock->xHandle, portMAX_DELAY );
        }
    }

    int __retarget_lock_try_acquire( struct __lock * pxLock )
    {
        BaseType_t xResult = pdFAIL;

        if( ( pxLock != NULL ) &&
            ( pxLock->xHandle != NULL ) &&
            ( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED ) &&
            ( xPortIsInsideInterrupt() == pdFALSE ) )
        {
            xResult = xSemaphoreTake( pxLock->xHandle, 0U );
        }

        return( xResult == pdPASS );
    }

    void __retarget_lock_release( struct __lock * pxLock )
    {
        if( ( pxLock != NULL ) &&
            ( pxLock->xHandle != NULL ) &&
            ( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED ) &&
            ( xPortIsInsideInterrupt() == pdFALSE ) )
        {
            ( void ) xSemaphoreGive( pxLock->xHandle );
        }
    }

    void __retarget_lock_init_recursive( struct __lock * * ppxLock )
    {
        if( ppxLock )
        {
            if( *ppxLock == NULL )
            {
                *ppxLock = pvPortMalloc( sizeof( struct __lock ) );
            }

            if( *ppxLock != NULL )
            {
                ( *ppxLock )->xHandle = xSemaphoreCreateRecursiveMutexStatic( &( ( *ppxLock )->xSemaphoreBuffer ) );

                configASSERT( ( *ppxLock )->xHandle != NULL );
            }
        }
    }

    void __retarget_lock_close_recursive( struct __lock * pxLock )
    {
        if( ( pxLock != NULL ) &&
            ( pxLock->xHandle != NULL ) )
        {
            /* Mutex should not be held when released */
            configASSERT( xSemaphoreGetMutexHolder( pxLock->xHandle ) == NULL );

            vSemaphoreDelete( pxLock->xHandle );

            pxLock->xHandle = NULL;
        }
    }

    void __retarget_lock_acquire_recursive( struct __lock * pxLock )
    {
        if( ( pxLock != NULL ) &&
            ( pxLock->xHandle != NULL ) &&
            ( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED ) &&
            ( xPortIsInsideInterrupt() == pdFALSE ) )
        {
            ( void ) xSemaphoreTakeRecursive( pxLock->xHandle, portMAX_DELAY );
        }
    }

    int __retarget_lock_try_acquire_recursive( struct __lock * pxLock )
    {
        BaseType_t xResult = pdFAIL;

        if( ( pxLock != NULL ) &&
            ( pxLock->xHandle != NULL ) &&
            ( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED ) &&
            ( xPortIsInsideInterrupt() == pdFALSE ) )
        {
            xResult = xSemaphoreTakeRecursive( pxLock->xHandle, 0U );
        }

        return( xResult == pdPASS );
    }

    void __retarget_lock_release_recursive( struct __lock * pxLock )
    {
        if( ( pxLock != NULL ) &&
            ( pxLock->xHandle != NULL ) &&
            ( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED ) &&
            ( xPortIsInsideInterrupt() == pdFALSE ) )
        {
            ( void ) xSemaphoreGiveRecursive( pxLock->xHandle );
        }
    }
#endif /* _RETARGETABLE_LOCKING == 1 */
