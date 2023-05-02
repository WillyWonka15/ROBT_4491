//************************************************************************
//
// FILE:   usciSPI.h
//
// TITLE:  source file for SPI module use to communicate with pixy cam
//
// using function from ti library for the SPI module
//
//
//************************************************************************
#include "usciSPI.h"
#include "driverlib.h"
#include "device.h"
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
void usciSPIinit()
{
    //put in reset mode
    SpiaRegs.SPICCR.bit.SPISWRESET = 0;

    //clock polarity, output on rising edge, latch on falling edge, clk iddling low
    SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;

    //16 bit word length
    SpiaRegs.SPICCR.bit.SPICHAR = CHARACTER_LENGTH_8;

    //enable transmit
    SpiaRegs.SPICTL.bit.TALK = TRANSMIT_ENABLE;

    //configure as master
    SpiaRegs.SPICTL.bit.MASTER_SLAVE = SPI_MASTER_MODE;

    //configure baud rate
    SpiaRegs.SPIBRR.bit.SPI_BIT_RATE = BAUD_200K;

    //
    SPI_disableFIFO(SPIA_BASE);


    //initialize GPIO
    usciSPIgpioInit();

    //taken out of reset
    SpiaRegs.SPICCR.bit.SPISWRESET = 1;

}

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
void usciSPIgpioInit()
{
    //SPIA SCLK
    GPIO_setMasterCore(SCIA_GPIO_CLK, GPIO_CORE_CPU1);
    //configure pin to peripheral function
    GPIO_setPinConfig(GPIO_60_SPICLKA);
    //
    GPIO_setQualificationMode(SCIA_GPIO_CLK, GPIO_QUAL_SYNC);

    //SPIA SIMO
    GPIO_setMasterCore(SCIA_GPIO_SIMO, GPIO_CORE_CPU1);
    //configure pin to peripheral function
    GPIO_setPinConfig(GPIO_58_SPISIMOA);
    //
    GPIO_setQualificationMode(SCIA_GPIO_SIMO, GPIO_QUAL_SYNC);

    //SPIA SOMI
    GPIO_setMasterCore(SCIA_GPIO_SOMI, GPIO_CORE_CPU1);
    //configure pin to peripheral function
    GPIO_setPinConfig(GPIO_59_SPISOMIA);
    //
    GPIO_setQualificationMode(SCIA_GPIO_SOMI, GPIO_QUAL_SYNC);

    //SPIA STE
    GPIO_setMasterCore(SCIA_GPIO_STE, GPIO_CORE_CPU1);
    //configure pin to gpio function
    GPIO_setPinConfig(GPIO_124_GPIO124);
    //set pin as o/p
    GPIO_setDirectionMode(SCIA_GPIO_STE, GPIO_DIR_MODE_OUT);
    //disable STE
    DISABLE_STE;
    //
    GPIO_setQualificationMode(SCIA_GPIO_STE, GPIO_QUAL_SYNC);
}

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
void usciSPItxWord(unsigned char txWord)
{
    int16 dummy;

    //poll TXIFG
    WAIT_FOR_SPI_TX;

    //write to TXBUF, shift by 8 because it need to be left justify
    SpiaRegs.SPITXBUF = txWord << 8;

    //poll RXIFG
    WAIT_FOR_SPI_RX;

    //perform a dummy read on RXBUF  to reset INT_FLAG, idk is there a better way to do this ??
    dummy = SpiaRegs.SPIRXBUF;
}

//*************************************************************************
// Function: usciSPIrxWord
// - receive a word through SPI module
// Arguments: int16_t *txWord
//
// return: none
// Author: Will Nguyen
// Date: April 4th, 2023
// Modified: April 4th, 2023
//************************************************************************
void usciSPIrxWord(unsigned char *rxWord)
{
    //poll TXIFG
    WAIT_FOR_SPI_TX;

    //send dummy byte
    SpiaRegs.SPITXBUF = DUMMY;

    //poll RXIFG
    WAIT_FOR_SPI_RX;

    //read RXBUF when its ready
    *rxWord = SpiaRegs.SPIRXBUF;
}

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
void usciSPItxStr(const unsigned char *txStr, int16 buffLen)
{
    int16 i;
    //enable STE (bring this outside)
    ENABLE_STE;

    for (i = 0; i < buffLen; i++)
    {
        usciSPItxWord(txStr[i]);
    }

    //disable STE
    DISABLE_STE;
}

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
void usciSPIrxStr(unsigned char *rxStr, int16 buffLen)
{
    int16_t i;
    unsigned char buffer;
    unsigned char temp;
    //enable STE (bring this outside)
    ENABLE_STE;

    //cleaning
    /*for (i = 0; i < 7; i++)
    {
        usciSPIrxWord(&temp);
    }*/

    //receiving data
    for (i = 0; i < buffLen; i++)
    {
        usciSPIrxWord(&buffer);
        rxStr[i] = buffer;
    }

    //disable STE
    DISABLE_STE;
}
