//--------------------------------------------------------------------------------------------------
/** @file
 * This app provides information on how to write user space  SPI commands for Legato based products
 * It provides a write/read function that sends data to an MCP3004 analog to digital converter
 * then reads the response
**/
//--------------------------------------------------------------------------------------------------

#include "legato.h"
#include "interfaces.h"

unsigned char chanBits;
uint8_t chan = 0; //read from channel 1
uint32_t a2dVal = 0;

uint8_t rxData [3];
uint8_t txData [3];

le_result_t res;

COMPONENT_INIT
{

    //chanBits = 0b10000000 | (chan << 4) ;
    chanBits = (~0) << 7;
    chanBits = chanBits | (chan << 4);

    txData [0] = 0x1; //start bit
    txData [1] = (~0) << 7;
    txData [1] |= (chan << 4);
    txData [2] = 0x0; //dont care


    LE_INFO("===========> SPI application has started");

    le_spi_DeviceHandleRef_t spiHandle;

    LE_INFO("Opening SPI file handle");
    res = le_spi_Open("spidev0.0", &spiHandle);
    LE_FATAL_IF(res != LE_OK, "le_spi_Open failed with result=%s", LE_RESULT_TXT(res));

    LE_INFO("Configuring SPI");
    le_spi_Configure(spiHandle, 0, 8, 1000000, 0);


    LE_INFO("Reading from the MCP3004 specified channe");
    size_t readBufferSize = NUM_ARRAY_MEMBERS(rxData);    
    le_spi_WriteReadHD(spiHandle, txData, NUM_ARRAY_MEMBERS(txData), rxData, &readBufferSize);
    LE_FATAL_IF(res != LE_OK, "le_spi_WriteReadHD failed with result=%s", LE_RESULT_TXT(res));

    a2dVal = rxData[1] & 0x03;
    a2dVal = (a2dVal << 8) | rxData[2];

    LE_INFO("txData[0]= %d", txData[0]);
    LE_INFO("txData[1]= %d", txData[1]);
    LE_INFO("txData[2]= %d", txData[2]);
    
    LE_INFO("rxData[0]= %d", rxData[0]);
    LE_INFO("rxData[1]= %d", rxData[1]);
    LE_INFO("rxData[2]= %d", rxData[2]);
    LE_INFO("A2D = %d", a2dVal);
    

    LE_INFO("CLOSING SPI <======================");
    le_spi_Close(spiHandle);
}
