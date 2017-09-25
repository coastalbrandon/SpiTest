//--------------------------------------------------------------------------------------------------
/** @file
 * This app provides information on how to write user space  SPI commands for Legato based products
 * It provides various functions that should cover differnt use cases for SPI usage
 * IoT on slot 1 of mangOH Green is enabled on this example
 * spi_Open -  Opens the spi file handle
 * spi_Configure  - This function initialized all the parameters on the master side based on the slave requirements
 * spi_WriteHD()  - This function allows to write  data to slave device in Half Duplex mode
 * spi_WriteRead() - This function allows to write and then read to slave device in Half Duplex mode
 * spi_Close() - Close the spi handle
 * The code below is used on a  DUST IOT module. You can look at page 21 and up of the doc below
 * for exact meaning for the various transmitted values
 * http://cds.linear.com/docs/en/software-and-simulation/Eterna_Serial_Programmer_Guide.pdf
 * Note this example program will be updated once the spi service is available in Legato
 */
//--------------------------------------------------------------------------------------------------

#include "legato.h"
#include "interfaces.h"


uint8_t read_ID_tx[] =
{
    0x9F
};

uint8_t spiData[3];
unsigned char chanBits;
int chan = 1; //read from channel 1

chanBits = 0b10000000 | (chan << 4);

spiData [0] = 1; //start bit
spiData [1] = chanBits ;
spiData [2] = 0; //dont care


COMPONENT_INIT
{
    LE_INFO("===========> SPI application has started");

    LE_INFO("Opening SPI");
    le_result_t res;
    res = le_spi_Open("spidev0.0", &spiHandle);
    LE_FATAL_IF(res != LE_OK, "le_spi_Open failed with result=%s", LE_RESULT_TXT(res));

    LE_INFO("Configuring SPI");
    le_spi_Configure(spiHandle, 0, 8, 960000, 0);

    LE_INFO("Testing WriteReadHD, read ID of device");
    size_t readBufferSize = NUM_ARRAY_MEMBERS(ID_rx);    
    le_spi_WriteReadHD(spiHandle, read_ID_tx, NUM_ARRAY_MEMBERS(read_ID_tx), spiData,
                   &readBufferSize);
    LE_FATAL_IF(res != LE_OK, "le_spi_WriteReadHD failed with result=%s", LE_RESULT_TXT(res));

    spi_Close(spiHandle);
}
