//--------------------------------------------------------------------------------------------------
/** @file
 * This app provides information on how to write user space  SPI commands for Legato based products
 * It provides a write/read function that sends data to an MCP3004 analog to digital converter
 * then reads the response
**/
//--------------------------------------------------------------------------------------------------

#include "legato.h"
#include "interfaces.h"

le_spi_DeviceHandleRef_t spiHandle;

unsigned char chanBits;
uint8_t chan = 0; //read from channel 1
uint32_t a2dVal = 0;
float voltage = 0.0;

uint8_t rxData [3];
uint8_t txData [3];

le_result_t res;

void pollMCP
(
	le_timer_Ref_t spiTimerRef
)
{
    chanBits = (~0) << 7;
    chanBits = chanBits | (chan << 4);

    txData [0] = 0x1; //start bit
    txData [1] = (~0) << 7;
    txData [1] |= (chan << 4);
    txData [2] = 0x0; //dont care

    LE_INFO("Reading MCP at channel %d: ", chan);
    size_t readBufferSize = NUM_ARRAY_MEMBERS(rxData);
    res = le_spi_WriteReadFD(spiHandle, txData, NUM_ARRAY_MEMBERS(txData), rxData, &readBufferSize);
    LE_FATAL_IF(res != LE_OK, "le_spi_WriteReadHD failed with result=%s", LE_RESULT_TXT(res));

    a2dVal = rxData[1] & 0x03;
    a2dVal = (a2dVal << 8) | rxData[2];
    voltage = a2dVal * 0.0036 - 0.6825;

    LE_INFO("txData[0]= %d", txData[0]);
    LE_INFO("txData[1]= %d", txData[1]);
    LE_INFO("txData[2]= %d", txData[2]);
    
    LE_INFO("rxData[0]= %d", rxData[0]);
    LE_INFO("rxData[1]= %d", rxData[1]);
    LE_INFO("rxData[2]= %d", rxData[2]);
    LE_INFO("A2D = %d", a2dVal);
    LE_INFO("VOLTAGE = %f", voltage);
 
}

COMPONENT_INIT
{

    LE_INFO("===========> SPI application has started");

    LE_INFO("Opening SPI file handle");
    res = le_spi_Open("spidev0.0", &spiHandle);
    LE_FATAL_IF(res != LE_OK, "le_spi_Open failed with result=%s", LE_RESULT_TXT(res));

    LE_INFO("Configuring SPI");
    le_spi_Configure(spiHandle, 0, 8, 960000, 0);


    le_timer_Ref_t spiTimerRef = le_timer_Create("SPI Timer");
    le_timer_SetMsInterval(spiTimerRef, 1000);
    le_timer_SetRepeat(spiTimerRef, 0);

    le_timer_SetHandler(spiTimerRef,pollMCP);
    le_timer_Start(spiTimerRef);
    

    //LE_INFO("CLOSING SPI <======================");
    //le_spi_Close(spiHandle);
}
