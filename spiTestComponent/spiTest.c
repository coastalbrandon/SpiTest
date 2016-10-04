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

uint8_t ID_rx[2];

uint8_t read_buffer_tx[] =
{
    0x03,0x01,0x01,0x02
};

uint8_t write_buffer2_tx[] =
{
    0x44, 0x01, 0x01, 0x02, 0x45, 0x56,0x67,0x44
};



uint8_t write_buffer_tx[] =
{
    0xC7, 0x94, 0x80, 0x9A
};


uint8_t  status_rx[3] = {0x00, };
uint8_t status_tx[1] = {0xD7};
uint8_t  read_rx[8] = {0x00, };
uint8_t  write_rx[4] = {0x00, };



COMPONENT_INIT
{
    LE_INFO("===========> SPI application has started");
    
    LE_FATAL_IF(mangoh_muxCtrl_Iot1Spi1On() != LE_OK, "Couldn't enable SPI on IoT slot 1");

    le_result_t res;

    spi_DeviceHandleRef_t spiHandle;
    res = spi_Open("sierra_spi", &spiHandle);
    LE_FATAL_IF(res != LE_OK, "spi_Open failed with result=%d", res);

    LE_INFO("Configuring SPI");
    spi_Configure(spiHandle, SPI_SPI_MODE_0, 8, 960000, 0);

    LE_INFO("Testing WriteReadHD, read ID of device");
    size_t readBufferSize = NUM_ARRAY_MEMBERS(ID_rx);
    spi_WriteReadHD(spiHandle, read_ID_tx, NUM_ARRAY_MEMBERS(read_ID_tx), ID_rx, &readBufferSize);


    LE_INFO("Testing WriteHD, Bulk erase");
    spi_WriteHD(spiHandle, write_buffer_tx, NUM_ARRAY_MEMBERS(write_buffer_tx));

    LE_INFO("Testing WrieHD, Buffer write");
    spi_WriteHD(spiHandle, write_buffer2_tx, NUM_ARRAY_MEMBERS(write_buffer2_tx));

    LE_INFO("Testing WriteReadHDi, read buffer write value");
    size_t readRxSize = NUM_ARRAY_MEMBERS(read_rx);
    spi_WriteReadHD(
        spiHandle, read_buffer_tx, NUM_ARRAY_MEMBERS(read_buffer_tx), read_rx, &readRxSize);

    spi_Close(spiHandle);
}
