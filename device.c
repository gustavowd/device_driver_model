/*
 * device.c
 *
 *  Created on: 28 de jul de 2016
 *      Author: gustavo
 */

#include "device.h"
#include "string.h"



static OS_Device_t OSDevices[MAX_INSTALLED_DEVICES];
static OS_Device_Control_t OSDevControl[MAX_INSTALLED_DEVICES];

const char *dev_types[]={"UART", "SPI", "I2C", "GPIO"};

OS_Device_Control_t *OSDevOpen(char *name, void *option){
    int8_t    index = 0;
    int8_t    idx = 0;
	char *dname = name;
	Device_Types_t type = END_TYPE;
	int8_t		   device_number = 0;

	// Zerar todos os devices na primeira vez que chamar o BRTOS_Open

	if (name == NULL){
		return (OS_Device_Control_t *)NULL;
	}

	/* Search for the device in the list of devices already installed. */
	for( index = 0; index < MAX_INSTALLED_DEVICES; index++ )
	{
		if (OSDevices[index].name != NULL){
			if( strcmp((const char *)name, (const char *)OSDevices[index].name) == 0 )
			{
				/* Device is already installed */
				return (OS_Device_Control_t *)&OSDevControl[index];
			}
		}
	}

	/* Search for a free device block. */
	for( index = 0; index < MAX_INSTALLED_DEVICES; index++ )
	{
		if(OSDevices[index].name == NULL)
		{
			/* Free device block found! */
			break;
		}
	}

	if(index < MAX_INSTALLED_DEVICES)
	{
		/* Search for the device number in ASCII format. */
		while(*name)
		{
			if((*name >= '0' ) && (*name <= '9'))
			{
				break;
			}
			name++;
		}

		/* Convert the number from its ASCII representation. */
		device_number = (uint8_t)(*name - '0');

		// todo: Verificar, pois pode haver drivers sem final numerico
		if (device_number < 0){
			// Verificar se é um GPIOx
			if( strncmp((const char *)dname, (const char *)"GPIO", (strlen((const char *)dname) - 1)) != 0 ){
				// Se o final for uma letra, mas n�o for um GPIO, retorna NULL
				return (OS_Device_Control_t *)NULL;
			}else{
				device_number = (int8_t)dname[4];
			}
		}

		for( idx = 0; idx < END_TYPE; idx++ )
		{
			// todo: Verificar, pois pode haver drivers sem final numerico
			if( strncmp((const char *)dname, (const char *)dev_types[idx], (strlen((const char *)dname) - 1)) == 0 ){
				type = idx;
				break;
			}
		}

		OSDevices[index].device_type = type;
		OSDevices[index].name = dname;
		OSDevices[index].DriverData = option;
		OSDevControl[index].device = &OSDevices[index];
		OSDevControl[index].device_number = (uint8_t)device_number;


		// Preencher os ponteiros de função para cada tipo de driver
		switch(type){
            #if (INCLUDE_UART_TYPE == 1)
			case UART_TYPE:
			    if (option != NULL){
			        OSOpenUART(&OSDevControl[index], option);
			    }else{
	                OSDevices[index].name = NULL;
	                return (OS_Device_Control_t *)NULL;
			    }
				break;
            #endif
            #if (INCLUDE_SPI_TYPE == 1)
			case SPI_TYPE:
				OSOpenSPI(&OSDevControl[index], option);
				break;
            #endif
            #if (INCLUDE_I2C_TYPE == 1)
			case I2C_TYPE:
				OSOpenI2C(&OSDevControl[index], option);
				break;
            #endif
            #if (INCLUDE_GPIO_TYPE == 1)
			case GPIO_TYPE:
				OSOpenGPIO(&OSDevControl[index], option);
				break;
            #endif
			default:
			    OSDevices[index].name = NULL;
			    return (OS_Device_Control_t *)NULL;
				break;
		}

		return (OS_Device_Control_t *)&OSDevControl[index];
	}else{
		return (OS_Device_Control_t *)NULL;
	}
}


size_t OSDevWrite(OS_Device_Control_t *dev, const void *string, const size_t bytes){
	return dev->api->write(dev,string,bytes);
}


size_t OSDevRead(OS_Device_Control_t *dev, void *string, const size_t bytes){
	return dev->api->read(dev,string,bytes);
}

size_t OSDevSet(OS_Device_Control_t *dev, uint32_t request, uint32_t value){
	return dev->api->set(dev,request,value);
}

size_t OSDevGet(OS_Device_Control_t *dev, uint32_t request){
	return dev->api->get(dev,request);
}
