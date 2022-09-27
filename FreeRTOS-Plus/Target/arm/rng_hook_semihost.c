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

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* For Semihost open */
#include <fcntl.h>
#include <unistd.h>

#include "FreeRTOS.h"

/*-----------------------------------------------------------*/

UBaseType_t uxRand( void )
{
    int lFileId = -1;
    UBaseType_t uxRandValue = 0U;

    lFileId = open( "/dev/urandom", O_RDONLY );

    configASSERT( lFileId >= 0 );

    if( lFileId >= 0 )
    {
        ssize_t xBytesRead = 0U;
        xBytesRead = read( lFileId, &uxRandValue, sizeof( UBaseType_t ) );

        ( void ) close( lFileId );

        configASSERT( xBytesRead == sizeof( UBaseType_t ) );
    }

    return uxRandValue;
}

/*-----------------------------------------------------------*/