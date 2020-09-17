/*
 * Device_UART.c
 *
 *  Created on: 17 de set de 2020
 *      Author: gustavo
 */

#include <stdint.h>
#include <stdbool.h>


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "device.h"

// Declares a queue structure for the UART
QueueHandle_t qUART[3];
#if UART_STRING == UART_QUEUE
QueueHandle_t qUART_Tx[3];
#endif

// Declares a semaphore structure for the UART
SemaphoreHandle_t sUART[3];

// Declares a mutex structure for the UART
SemaphoreHandle_t mutexTx[3];


void Init_UART0(void *parameters)
{
    uart_config_t *uart_conf = (uart_config_t *)parameters;
 
}



void Init_UART1(void *parameters)
{

}

void Init_UART2(void *parameters)
{

}

//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
void UARTIntHandler(void){
 
}



size_t UART_Write(OS_Device_Control_t *dev, char *string, size_t size ){
    char data;
    size_t nbytes = 0;
   
    return nbytes;
}

size_t UART_Read(OS_Device_Control_t *dev, char *string, size_t size ){
    size_t nbytes = 0;
  
failed_rx:
    return nbytes;
}

size_t UART_Set(OS_Device_Control_t *dev, uint32_t request, uint32_t value){
    uart_config_t *uart_conf = (uart_config_t *)dev->device->DriverData;

    switch(request){
        case UART_BAUDRATE:
            break;
        case UART_PARITY:
            break;
        case UART_STOP_BITS:
            break;
        case UART_QUEUE_SIZE:
            break;
        case UART_TIMEOUT:
            uart_conf->timeout = value;
            break;
        default:
            break;
    }

    return 0;
}

size_t UART_Get(OS_Device_Control_t *dev, uint32_t request){
    uint32_t ret;
    uart_config_t *uart_conf = (uart_config_t *)dev->device->DriverData;
    switch(request){
        case UART_BAUDRATE:
            ret = uart_conf->baudrate;
            break;
        case UART_PARITY:
            ret = uart_conf->parity;
            break;
        case UART_STOP_BITS:
            ret = uart_conf->stop_bits;
            break;
        case UART_QUEUE_SIZE:
            ret = uart_conf->queue_size;
            break;
        case UART_TIMEOUT:
            ret = uart_conf->timeout;
            break;
        default:
            ret = 0;
            break;
    }
    return ret;
}

const device_api_t UART_api ={
        .read = (Device_Control_read_t)UART_Read,
        .write = (Device_Control_write_t)UART_Write,
        .set = (Device_Control_set_t)UART_Set,
        .get = (Device_Control_get_t)UART_Get
};

void OSOpenUART(OS_Device_Control_t *dev, void *parameters){
    switch(dev->device_number){
        case 0:
            Init_UART0(parameters);
            dev->device->base_address = UART0_BASE;
            break;
#if 0
        case 1:
            Init_UART1(parameters);
            dev->device->base_address = UART1_BASE;
            break;
        case 2:
            Init_UART2(parameters);
            dev->device->base_address = UART2_BASE;
            break;
#endif
        default:
            break;
    }
    dev->api = &UART_api;
}
