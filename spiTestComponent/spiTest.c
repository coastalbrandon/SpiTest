//--------------------------------------------------------------------------------------------------
/** @file
 * This app provides information on how to write user space  SPI commands for Legato based products
 * It provides three functions that should cover all use cases for SPI usage
 * SpiInit()  - This function initialized all the parameters on the master side based on the slave requirements
 * SpiRead()  - This function allows to read data from slave device
 * SpiWrite() - This function allows to write to slave device
 */
//--------------------------------------------------------------------------------------------------

#include "legato.h"
#include "interfaces.h"


uint8_t read_ID_tx[] =
{
    0xD0
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

    le_result_t res;

    spi_DeviceHandleRef_t spiHandle;
    res = spi_Open("sierra_spi", &spiHandle);
    LE_FATAL_IF(res != LE_OK, "spi_Open failed with result=%d", res);

    LE_DEBUG("Configuring SPI");
    spi_Configure(spiHandle, SPI_SPI_MODE_0, 8, 960000, 0);

    LE_DEBUG("Testing WriteReadHD");
    size_t readBufferSize = NUM_ARRAY_MEMBERS(ID_rx);
    spi_WriteReadHD(spiHandle, read_ID_tx, NUM_ARRAY_MEMBERS(read_ID_tx), ID_rx, &readBufferSize);

    LE_DEBUG("Testing WriteReadFD");
    readBufferSize = NUM_ARRAY_MEMBERS(ID_rx);
    spi_WriteReadFD(spiHandle, read_ID_tx, NUM_ARRAY_MEMBERS(read_ID_tx), ID_rx, &readBufferSize);

    LE_DEBUG("Testing WriteHD");
    spi_WriteHD(spiHandle, write_buffer_tx, NUM_ARRAY_MEMBERS(write_buffer_tx));
    spi_WriteHD(spiHandle, write_buffer2_tx, NUM_ARRAY_MEMBERS(write_buffer2_tx));

    LE_DEBUG("Testing WriteReadHD");
    size_t readRxSize = NUM_ARRAY_MEMBERS(read_rx);
    spi_WriteReadHD(
        spiHandle, read_buffer_tx, NUM_ARRAY_MEMBERS(read_buffer_tx), read_rx, &readRxSize);

    spi_Close(spiHandle);
}
