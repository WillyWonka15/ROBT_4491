//************************************************************************
//
// FILE:   usciSPI.h
//
// TITLE:  header file for SPI module use to communicate with pixy cam
//
// using function from ti library for the SPI module
//
//
//************************************************************************
#ifndef USCISPI_H_
#define USCISPI_H_

#include "F2837xD_device.h"
#include "driverlib.h"
#include "device.h"
//************************************************************************
//SPI CONFIGURATION
#define CHARACTER_LENGTH_8 0x7
#define CHARACTER_LENGTH_16 0xF
#define SPI_MASTER_MODE 1
#define SPI_SLAVE_MODE 0
#define TRANSMIT_ENABLE 1
#define BAUD_5M 0x4
#define BAUD_200K 0x7E
//************************************************************************
//GPIO
#define SCIA_GPIO_CLK 60
#define SCIA_GPIO_SIMO 58
#define SCIA_GPIO_SOMI 59
#define SCIA_GPIO_STE 124
//************************************************************************
//MACROS
#define DISABLE_STE GpioDataRegs.GPDSET.bit.GPIO124 = 1
#define ENABLE_STE GpioDataRegs.GPDCLEAR.bit.GPIO124 = 1
#define WAIT_FOR_SPI_TX while(SpiaRegs.SPISTS.bit.BUFFULL_FLAG)
#define WAIT_FOR_SPI_RX while(!SpiaRegs.SPISTS.bit.INT_FLAG)
#define DUMMY 0x0000
//************************************************************************
//FUNCTION PROTOTYPE
//*************************************************************************
// Function: usciSPIinit
// - initialize the SPI module as full duplex
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: April 2nd, 2023
// Modified: April 2nd, 2023
//************************************************************************
void usciSPIinit();

//*************************************************************************
// Function: usciSPIgpioInit
// - initialize the GPIO using for SPIB
// Arguments:
//
// return: none
// Author: Will Nguyen
// Date: April 3rd, 2023
// Modified: April 3rd, 2023
//************************************************************************
void usciSPIgpioInit();

//*************************************************************************
// Function: usciSPItxWord
// - transmit a word through SPI module
// Arguments: int16_t txWord
//
// return: none
// Author: Will Nguyen
// Date: April 3rd, 2023
// Modified: April 3rd, 2023
//************************************************************************
void usciSPItxWord(unsigned char txWord);

//*************************************************************************
// Function: usciSPIrxWord
// - receive a word through SPI module
// Arguments: int16_t *rxWord
//
// return: none
// Author: Will Nguyen
// Date: April 4th, 2023
// Modified: April 4th, 2023
//************************************************************************
void usciSPIrxWord(unsigned char *rxWord);

//*************************************************************************
// Function: usciSPItxStr
// - transmit an input string from user through SPI
// Arguments: const int16_t , int16_t buffLen
//
// return: none
// Author: Will Nguyen
// Date: April 4th, 2023
// Modified: April 4th, 2023
//************************************************************************
void usciSPItxStr(const unsigned char* txStr, int16 buffLen);

//*************************************************************************
// Function: usciSPItxStr
// - receive a string from slave
// Arguments: int16_t *rxStr , int16_t buffLen
//
// return: none
// Author: Will Nguyen
// Date: April 4th, 2023
// Modified: April 4th, 2023
//************************************************************************
void usciSPIrxStr(unsigned char* rxStr, int16 buffLen);
#endif /* USCISPI_H_ */
