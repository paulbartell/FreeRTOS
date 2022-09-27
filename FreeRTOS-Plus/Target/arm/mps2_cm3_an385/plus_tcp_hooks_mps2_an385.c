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

/* Logging */
#include "logging_levels.h"
#define LIBRARY_LOG_NAME "TCP_HOOKS"
#define LIBRARY_LOG_LEVEL LOG_INFO
#include "logging_stack.h"

/* Standard includes. */
#include <stdio.h>
#include <time.h>
#include <unistd.h>

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include "task.h"

/* TCP Stack */
#include "FreeRTOSIPConfig.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

/* Ethernet Driver */
#include "smsc9220_eth_drv.h"
#include "SMM_MPS2.h"
#include "CMSDK_CM3.h"

#define __weak__    __attribute__( ( weak ) )

/* Called by FreeRTOS+TCP when the network connects or disconnects.  Disconnect
 * events are only received if implemented in the MAC driver. */
void __weak__ vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent )
{
    uint32_t ulIPAddress, ulNetMask, ulGatewayAddress, ulDNSServerAddress;
    char cBuffer[ 16 ];

    /* If the network has just come up...*/
    if( eNetworkEvent == eNetworkUp )
    {
        /* Print out the network configuration, which may have come from a DHCP
         * server. */
        FreeRTOS_GetAddressConfiguration( &ulIPAddress, &ulNetMask, &ulGatewayAddress, &ulDNSServerAddress );
        FreeRTOS_inet_ntoa( ulIPAddress, cBuffer );
        FreeRTOS_printf( ( "\r\n\r\nIP Address: %s\r\n", cBuffer ) );

        FreeRTOS_inet_ntoa( ulNetMask, cBuffer );
        FreeRTOS_printf( ( "Subnet Mask: %s\r\n", cBuffer ) );

        FreeRTOS_inet_ntoa( ulGatewayAddress, cBuffer );
        FreeRTOS_printf( ( "Gateway Address: %s\r\n", cBuffer ) );

        FreeRTOS_inet_ntoa( ulDNSServerAddress, cBuffer );
        FreeRTOS_printf( ( "DNS Server Address: %s\r\n\r\n\r\n", cBuffer ) );
    }
    else
    {
        FreeRTOS_printf( ( "Application idle hook network down\n" ) );
    }
}

/*-----------------------------------------------------------*/

#if ( ipconfigUSE_LLMNR != 0 ) || ( ipconfigUSE_NBNS != 0 ) || ( ipconfigDHCP_REGISTER_HOSTNAME == 1 )
    const char * __weak__ pcApplicationHostnameHook( void )
    {
        /* Assign the name "FreeRTOS" to this network node.  This function will
         * be called during the DHCP: the machine will be registered with an IP
         * address plus this name. */
        return "FreeRTOSQemuMPS2An385";
    }

#endif

/*-----------------------------------------------------------*/

#if ( ipconfigUSE_LLMNR != 0 ) || ( ipconfigUSE_NBNS != 0 )

    BaseType_t __weak__ xApplicationDNSQueryHook( const char * pcName )
    {
        BaseType_t xReturn;

        /* Determine if a name lookup is for this node.  Two names are given
         * to this node: that returned by pcApplicationHostnameHook() and that set
         * by mainDEVICE_NICK_NAME. */
        if( strcasecmp( pcName, pcApplicationHostnameHook() ) == 0 )
        {
            xReturn = pdPASS;
        }
        else
        {
            xReturn = pdFAIL;
        }

        return xReturn;
    }

#endif /* if ( ipconfigUSE_LLMNR != 0 ) || ( ipconfigUSE_NBNS != 0 ) */

/*-----------------------------------------------------------*/

/*
 * Callback that provides the inputs necessary to generate a randomized TCP
 * Initial Sequence Number per RFC 6528.
 */
uint32_t __weak__ ulApplicationGetNextSequenceNumber( uint32_t ulSourceAddress,
                                                      uint16_t usSourcePort,
                                                      uint32_t ulDestinationAddress,
                                                      uint16_t usDestinationPort )
{
    ( void ) ulSourceAddress;
    ( void ) usSourcePort;
    ( void ) ulDestinationAddress;
    ( void ) usDestinationPort;

    return ( uint32_t ) uxRand();
}

/*-----------------------------------------------------------*/

/*
 * Supply a random number to FreeRTOS+TCP stack.
 */
BaseType_t __weak__ xApplicationGetRandomNumber( uint32_t * pulNumber )
{
    *( pulNumber ) = ( uint32_t ) uxRand();
    return pdTRUE;
}

/*-----------------------------------------------------------*/

static void prvWait_ms( uint32_t ulSleep_ms )
{
    vTaskDelay( pdMS_TO_TICKS( ulSleep_ms ) );
}

/*-----------------------------------------------------------*/

static BaseType_t prvReadMacAddrFromEeprom( uint8_t * pucMACAddress )
{
    BaseType_t xResult = pdFALSE;
    static const struct smsc9220_eth_dev_cfg_t SMSC9220_ETH_DEV_CFG =
    {
        .base = SMSC9220_BASE
    };

    static struct smsc9220_eth_dev_data_t SMSC9220_ETH_DEV_DATA =
    {
        .state = 0
    };

    static const struct smsc9220_eth_dev_t SMSC9220_ETH_DEV =
    {
        &( SMSC9220_ETH_DEV_CFG ),
        &( SMSC9220_ETH_DEV_DATA )
    };

    enum smsc9220_error_t err;

    err = smsc9220_init( &SMSC9220_ETH_DEV, prvWait_ms );

    if( err == SMSC9220_ERROR_NONE )
    {
        /* Read the mac address from the LAN9118 / 9220 */
        err = smsc9220_read_mac_address( &SMSC9220_ETH_DEV, ( char * ) pucMACAddress );
    }

    if( err == SMSC9220_ERROR_NONE )
    {
        xResult = pdTRUE;
    }
    return xResult;
}

/*-----------------------------------------------------------*/

void __weak__ vPlatformInitIpStack( void )
{
    BaseType_t xResult;
    uint8_t ucIPAddress[ 4 ];
    uint8_t ucNetMask[ 4 ] = { 255, 255, 0, 0 };
    uint8_t ucNullAddress[ 4 ] = { 0, 0, 0, 0 };
    uint8_t ucMACAddress[ 6 ];

    UBaseType_t uxRandomNumber = uxRand();

    NVIC_SetPriority( ETHERNET_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY );

    /* Attempt to read MAC address from LAN9220 / LAN9118 eeprom */
    if( prvReadMacAddrFromEeprom( ucMACAddress ) == pdTRUE )
    {
        LogInfo( ( "Using eeprom MAC address: %.02X:%.02X:%.02X:%.02X:%.02X:%.02X",
                   ucMACAddress[ 0 ], ucMACAddress[ 1 ], ucMACAddress[ 2 ],
                   ucMACAddress[ 3 ], ucMACAddress[ 4 ], ucMACAddress[ 5 ] ) );
    }
    else
    {
        /* Generate a random MAC address in the reserved range */
        ucMACAddress[ 0 ] = 0x00;
        ucMACAddress[ 1 ] = 0x11;
        ucMACAddress[ 2 ] = ( uxRandomNumber & 0xFF );
        ucMACAddress[ 3 ] = ( ( uxRandomNumber >> 8 ) & 0xFF );
        ucMACAddress[ 4 ] = ( ( uxRandomNumber >> 16 ) & 0xFF );
        ucMACAddress[ 5 ] = ( ( uxRandomNumber >> 24 ) & 0xFF );
        LogInfo( ( "Using random MAC address: %.02X:%.02X:%.02X:%.02X:%.02X:%.02X",
                   ucMACAddress[ 0 ], ucMACAddress[ 1 ], ucMACAddress[ 2 ],
                   ucMACAddress[ 3 ], ucMACAddress[ 4 ], ucMACAddress[ 5 ] ) );
    }

    /* Assign a link-local address in the 169.254.0.0/16 range */
    ucIPAddress[ 0 ] = 169U;
    ucIPAddress[ 1 ] = 254U;
    ucIPAddress[ 2 ] = ( ( uxRandomNumber >> 16 ) & 0xFF );
    ucIPAddress[ 3 ] = ( ( uxRandomNumber >> 24 ) & 0xFF );

    xResult = FreeRTOS_IPInit( ucIPAddress, ucNetMask, ucNullAddress, ucNullAddress, ucMACAddress );
    configASSERT( xResult == pdTRUE );
}

/*-----------------------------------------------------------*/
