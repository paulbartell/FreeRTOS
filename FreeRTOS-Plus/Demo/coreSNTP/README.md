# coreSNTP Demo

## On this page:
1. Introduction
2. (Optional) Instructions for setting up a chronyd based NTP server
3. Multithreaded Architecture - SNTP Client Task & Sample App Task
4. Model for Wall-Clock System Clock
5. Clock Correction/Discipline Model
6. Initialization Time in Demo

## Introduction
This demo shows how an application can use the coreSNTP library to maintain a real-time clock (or wall-clock time) in the Co-ordinated Universal Time (UTC) zone by setting up an SNTP client to periodically synchronize system time with time servers. The demo also showcases how the coreSNTP library can be used to communicate securely with SNTP/NTP servers using mutual authentication. The example uses symmetric-key based AES-128-CMAC as the authentication algorithm. To run this demo with authentication enabled, the server(s) you use must support authentication with the AES-128-CMAC algorithm, and symmetric key(s) must be generated and pre-shared between the client (the demo) and the server(s) before you run the demo.

## (Optional) Instructions for setting up a chronyd based NTP server
To run the coreSNTP demo with AES-128-CMAC based authentication, an NTP server that supports authentication with the algorithm is required. An NTP server can be setup with the chronyd package with support for AES-128-CMACM authentication. The chronyd server can be configured to use a set of upstream servers to periodically synchronize its time so that it can accurately respond to time requests made to it by the coreSNTP demo client.

Follow the [Instructions for setting up an NTP server](https://www.freertos.org/sntp/setting-up-an-ntp-server.html) (with authentication enabled) with chronyd in an AWS EC2 Linux instance with the Amazon Time Sync service as the upstream server.

## Multithreaded Architecture - SNTP Client Task & Sample App Task
The demo architecture contains two tasks:

### SNTP Client Task
This task is a dedicated SNTP client that synchronizes time periodically with time server(s) using the coreSNTP library. It corrects the system clock based on the received server time and the calculated system clock drift from server time to maintain a wall-clock/real-time clock in the UTC time zone. The logic for the SNTP client task can be seen in the SNTPClientTask.c file.

### Sample Application Task
This task is a sample application task that depends on real-time information that it queries from the system. This basic application task periodically prints the real-time/wall-clock time in UTC that is has queried from the system. The code for this sample application task that queries system time periodically can be found in the SampleAppTask.c file.
All the uses of the coreSNTP library are encapsulated in the SNTP Client Task to showcase how an SNTP client task agent/utility can be set up in a FreeRTOS system to service time requests from multiple application tasks.

## Model for Wall-Clock System Clock
The demo shows how devices that do not have a Real-Time Clock hardware module can maintain a real-time clock (or wall-time clock) in the UTC time zone in RAM. The demo uses the following mathematical model that is flexible for being updated from periodic SNTP time synchronizations:
```
System Time = Base Time +
              Time Elapsed since last SNTP time synchronization +
              Slew Adjustment

where

BaseTime = Time set at boot or the last synchronized time
Slew Rate = Number of milliseconds of clock correction per system time second
No. of ticks since last SNTP sync = Current FreeRTOS Tick Count -
                                    Tick count at last SNTP synchronization

Time Elapsed since last SNTP time synchronization =
                           No. of ticks since last SNTP synchronization
                                                  x
                           Number of milliseconds per FreeRTOS tick

Slew Adjustment = Slew Rate x Time Elapsed since last SNTP sync
```

In the above mathematical model, any frequency discrepancies in the system's clock oscillator cause clock drift that skews the execution timing of FreeRTOS ticks in relation to actual/internet time. A Slew Adjustment offset is applied to the calculation of the system clock to correct for the total clock drift. In this way, the system time matches the real-world time. The Slew Adjustment depends on the Slew Rate, which is the adjustment that should be periodically applied to the system time to counter the clock drift of the system clock. This rate is calculated once from the clock-offset value provided by the coreSNTP library when it receives the time from a time server.

## Clock Correction/Discipline Model
The demo uses a combination of **step** and **slew** methodologies to correct the system clock, which affect the **Base Time** and **Slew Adjustment** parameters, respectively, of the mathematical model of the system clock mentioned in the previous section.

1. The **Step Correction** updates the **Base Time** parameter of the system clock when it receives the time from a server. This correction is used on **EVERY** successful time synchronization cycle with a time server to immediately correct the system clock to match server time.

2. The **Slew Correction** approach is used to compensate for system clock drift that occurs during the interval between successful time synchronizations with with time server(s). Clock drift occurs in the interval because the system time is based on the HW system clock oscillator that introduces the drift due to its frequency deviation. The **Slew Rate** that is used for slew correction is calculated ONLY once, on the occasion of the second successful time synchronization with a time server. This is because the demo initializes the system time as (the first second of) the democonfigSYSTEM_START_YEAR, and so the actual system clock drift over a period of time can be correctly calculated only **AFTER** the demo system time has been synchronized once with the real-world time obtained from a time server. Given the interval between the first and second time synchronizations with a server, the actual system clock drift can be calculated and the **Slew Rate** used for clock adjustment can be correctly established.

Here is a summary of how the clock-discipline methodology of the demo affects the system:
```
On every time response recevied from time server,

  Base Time = Time from Server

On the 2nd successful time synchronization with time server,

  Slew Rate = Clock-Offset (calculated by coreSNTP library) / Poll Period
```

## Initialization Time in the Demo
The demo initializes the system time with the first second of the year configured with the democonfigSYSTEM_START_YEAR macro. For example, if the macro is configured with the year 2021, the demo initializes system time as the 1st of January 2021 0h:0m:0s. This is done so that the system time is not initialized as zero, but with a time that is close enough to the real-world time that it can meet the application's needs to perform operations like server certificate validation to establish a TLS connection before it is able to synchronize its time with a time server to obtain accurate real-world time. This is useful for those device platforms that do not have a battery backed Real-Time Clock (RTC) hardware module and so do not have a hardware mechanism to obtain the real-world time at system startup.

