//--------------------------------------------------------------------------------------------------
/** @file
 * This app provides information on how to write user space  SPI commands for Legato based products
 * It provides a write/read function that sends data to an MCP3004 analog to digital converter
 * then reads the response
//--------------------------------------------------------------------------------------------------

#include "legato.h"
#include "interfaces.h"

uint8_t spiData[3];
unsigned char chanBits;
int chan = 1; //read from channel 1
int a2dVal = 0;

uint8_t rxData[3];

chanBits = 0b10000000 | (chan << 4);

txData [0] = 1; //start bit
txData [1] = chanBits ;
txData [2] = 0; //dont care


COMPONENT_INIT
{
    LE_INFO("===========> SPI application has started");

    LE_INFO("Opening SPI");
    le_result_t res;
    res = le_spi_Open("spidev0.0", &spiHandle);
    LE_FATAL_IF(res != LE_OK, "le_spi_Open failed with result=%s", LE_RESULT_TXT(res));

    LE_INFO("Configuring SPI");
    le_spi_Configure(spiHandle, 0, 8, 1000000, 0);

    LE_INFO("Testing WriteReadHD, read ID of device");
    size_t readBufferSize = NUM_ARRAY_MEMBERS(txData);    
    le_spi_WriteReadHD(spiHandle, txData, NUM_ARRAY_MEMBERS(txData), rxData,
                   &readBufferSize);
    LE_FATAL_IF(res != LE_OK, "le_spi_WriteReadHD failed with result=%s", LE_RESULT_TXT(res));
    
    a2dVal = (rxData[1]<<4) & 0b1100000000; //merge rxData[1] and rxData[2] to get result
    a2dVal |= (rxData[2] & 0xff);
    
    LE_INFO("A2D Value= %d", a2dVal);

    spi_Close(spiHandle);
}
