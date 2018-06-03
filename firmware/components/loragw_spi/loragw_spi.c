/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
  (C)2013 Semtech-Cycleo

Description:
    Host specific functions to address the LoRa spi_target_ptr registers through
    a SPI interface.
    Single-byte read/write and burst read/write.
    Could be used with multiple SPI ports

License: Revised BSD License, see LICENSE.TXT file include in the project
Maintainer: Sylvain Miermont
Modified for ESP32 by: Alois Mbutura.
*/
#include <stdint.h>        /* C99 types */
#include <stdio.h>         /* printf fprintf */
#include <stdlib.h>        /* malloc free */
#include <string.h>        /* memset */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/spi_master.h"

#include "loragw_spi.h"
#include "loragw_hal.h"

static const char* TAG = "[LORAGW_SPI]";

#define READ_ACCESS     0x00
#define WRITE_ACCESS    0x80

#define NUM_QUEUE 7

#define CHECK_NULL(a) if(a == NULL){ESP_LOGE(TAG, "%s:%d: ERROR: NULL POINTER AS ARGUMENT",__FUNCTION__, __LINE__);return LGW_SPI_ERROR;}

/* Convert (u)int8 variable type to (u)int16 */
static uint16_t conv_int_8_16(uint8_t msb, uint8_t lsb)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __func__);
    uint16_t combined = 0x0000;
    combined = msb;
    combined = combined << 8;
    combined |= lsb;
    return combined;
}

/* SPI initialization and configuration */

int lgw_spi_open(spi_device_handle_t* spi_target_ptr, long speed)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __func__);
    esp_err_t ret;
    CHECK_NULL(spi_target_ptr)

    spi_bus_config_t buscfg=
    {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };
    /*
    We hardcode one device onto the SPI bus by design, this will have to be refactored if more are added.
    */
    spi_device_interface_config_t devcfg=
    {
        .clock_speed_hz = speed,                        //Clock out at speed Hz
        .mode = 0,                                      //SPI mode 0(CPOL=0,CPHA=0)
        .spics_io_num = PIN_NUM_CS,                     //CS pin
        .queue_size = NUM_QUEUE,                        //We want to be able to queue NUM_QUEUE transactions at a time
    };
    //Initialize the SPI bus
    //We use assert due to the async nature of the esp32.
    ret = spi_bus_initialize(HSPI_HOST, &buscfg, 1);
    if(ret == ESP_OK)
    {
        ESP_LOGV(TAG, "SPI BUS INITIALISATION SUCCESSFUL");
    }
    else
    {
        ESP_LOGE(TAG, "SPI BUS INITIALISATION FAILURE");
        lgw_spi_close(spi_target_ptr);
        return LGW_SPI_ERROR;
    }
    //Attach the spi_target_ptr to the SPI bus
    ret = spi_bus_add_device(HSPI_HOST, &devcfg, spi_target_ptr);
    if(ret == ESP_OK)
    {
        ESP_LOGV(TAG, "SPI DEVICE ADDITION SUCCESSFUL");
        return LGW_SPI_SUCCESS;
    }
    else
    {
        ESP_LOGE(TAG, "SPI DEVICE ADDITION FAILURE");
        lgw_spi_close(spi_target_ptr);
        return LGW_SPI_ERROR;
    }
}

/* SPI release */

int lgw_spi_close(spi_device_handle_t *spi_target_ptr)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __func__);
    /* check input variables */
    CHECK_NULL(spi_target_ptr);
    esp_err_t ret;
    ret=spi_bus_remove_device(*spi_target_ptr);
    if(ret == ESP_OK)
    {
        ESP_LOGV(TAG, "SPI DEVICE REMOVAL SUCCESS");
    }
    else
    {
        ESP_LOGE(TAG, "SPI DEVICE REMOVAL FAILURE");
        return LGW_SPI_ERROR;
    }
    ret = spi_bus_free(HSPI_HOST);
    if(ret == ESP_OK)
    {
        ESP_LOGV(TAG, "SPI HOST FREE SUCCESS");
        return LGW_SPI_SUCCESS;
    }
    else
    {
        ESP_LOGE(TAG, "FAILED TO FREE SPI HOST");
        return LGW_SPI_ERROR;
    }
}

/* Simple write */

int lgw_spi_w(spi_device_handle_t *spi_target_ptr, uint8_t spi_mux_mode, uint8_t spi_mux_target, uint8_t address, uint8_t data)
{   
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __func__);   
    uint8_t addr_buf[2] = {0};
    uint8_t size = 0;

    spi_transaction_ext_t tx;

    // check input pointers
    CHECK_NULL(spi_target_ptr);

    if ((address & 0x80) != 0)
    {
        ESP_LOGE(TAG,"WARNING: SPI ADDRESS > 127");
    }

    //Zero out the transaction struct first and prep flags
    memset(&tx, 0, sizeof(tx));
    tx.base.flags=SPI_TRANS_VARIABLE_ADDR|SPI_TRANS_USE_TXDATA;

    //Prepare frame to be sent
    //If FPGA, add SPI_MUX_TARGET HEADER(FPGA, EEPROM,SX127x) to simple write cmd
    if (spi_mux_mode == LGW_SPI_MUX_MODE1)
    {
        addr_buf[1] = spi_mux_target;
        addr_buf[0] = WRITE_ACCESS | (address & 0x7F);
        tx.base.addr = conv_int_8_16(addr_buf[1], addr_buf[0]);
        size = 2;
    }
    //If no FPGA, don't add explicit HEADER to simple write cmd
    else
    {
        addr_buf[0] = WRITE_ACCESS | (address & 0x7F);
        tx.base.addr = addr_buf[0];
        size = 1;
    }

    tx.address_bits=size*8;
    tx.base.length=1*8;
    tx.base.tx_data[0] = data;
    
    esp_err_t ret=spi_device_transmit(*spi_target_ptr, (spi_transaction_t*)&tx); //Perform the transmit transaction!
    if(ret == ESP_OK)
    {
        ESP_LOGV(TAG, "DMA SPI QUEUE AND WRITE SUCCESS");
        return LGW_SPI_SUCCESS;
    }
    else
    {
        ESP_LOGE(TAG, "SPI WRITE FAILURE");
        return LGW_SPI_ERROR;
    }
}

/* Simple read */

int lgw_spi_r(spi_device_handle_t* spi_target_ptr, uint8_t spi_mux_mode, uint8_t spi_mux_target, uint8_t address, uint8_t *data)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __func__);

    uint8_t size = 0; 
    uint8_t addr_buf[2] = {0};

    spi_transaction_ext_t tx;

    // check input pointers
    CHECK_NULL(spi_target_ptr);
    CHECK_NULL(data);

    if ((address & 0x80) != 0)
    {
        ESP_LOGE(TAG,"WARNING: SPI address > 127");
    }

    // I/O transaction
    //Zero out the transaction struct first and prep flags
    memset(&tx, 0, sizeof(tx));
    tx.base.flags=SPI_TRANS_VARIABLE_ADDR|SPI_TRANS_USE_TXDATA|SPI_TRANS_USE_RXDATA;

    // prepare frame to be sent
    //We send a 0x00 as the line is full duplex and handles reads and writes concurrently.
    if (spi_mux_mode == LGW_SPI_MUX_MODE1)
    {
        addr_buf[1] = spi_mux_target;
        addr_buf[0] = READ_ACCESS | (address & 0x7F);
        tx.base.addr = conv_int_8_16(addr_buf[1], addr_buf[0]);
        size = 2;
    }
    else //For single byte and error.
    {
        addr_buf[0] = READ_ACCESS | (address & 0x7F);
        tx.base.addr = addr_buf[0];
        size = 1;
    }
    ESP_LOGV(TAG,"SPI address field is 0x%04X or %d dec",address, address);

    tx.address_bits=size*8;
    tx.base.length=1*8;
    //tx data is already zeroed out. Saves a line of code.
    //rx length is also zeroed out. rx length is therefore equal to length.   
    esp_err_t ret=spi_device_transmit(*spi_target_ptr, (spi_transaction_t*)&tx); //Perform the transmit transaction!
    if(ret == ESP_OK)
    {
#if CONFIG_LOG_DEFAULT_LEVEL == 5
        for(int i=0; i<4; i++)
        {
            ESP_LOGD(TAG, "Received data value is [%u], Value: 0x%02X", i, tx.base.rx_data[i]);
        }
#endif
        *data = tx.base.rx_data[0]; //Pointer points to transaction rx array
        ESP_LOGV(TAG, "Note: SPI read success");
        return LGW_SPI_SUCCESS;
    }
    else
    {
        ESP_LOGE(TAG, "ERROR: SPI READ FAILURE");
        return LGW_SPI_ERROR;
    }
  
}

/* Burst (multiple-byte) write */

int lgw_spi_wb(spi_device_handle_t *spi_target_ptr, uint8_t spi_mux_mode, uint8_t spi_mux_target, uint8_t address, uint8_t *data, uint16_t size)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __func__);

    int i=0;
    uint8_t addr_size;
    uint8_t addr_buf[2]={0};
    spi_transaction_ext_t tx;

    int size_to_do=0;
    int chunk_size=0;
    int offset=0;

    // check input pointers
    CHECK_NULL(spi_target_ptr);
    CHECK_NULL(data);

    if ((address & 0x80) != 0)
    {
        ESP_LOGE(TAG, "WARNING: SPI ADDRESS > 127");
    }
    if (size == 0)
    {
        ESP_LOGW(TAG, "WARNING: BURST WRITE OF NULL LENGTH");
        return LGW_SPI_ERROR;
    }

    if (spi_mux_mode == LGW_SPI_MUX_MODE1)
    {
        addr_buf[1] = spi_mux_target;
        addr_buf[0] = WRITE_ACCESS | (address & 0x7F);

        addr_size = 2;
    }
    else //For single bytes and errors.
    {
        addr_buf[0] = WRITE_ACCESS | (address & 0x7F);
        addr_size = 1;
    }

    //Create new buffer to hold the command, address and data to do
    size_to_do = size;
    //TODO: Find the number of transactions to send the data. Then queue the transactions.
    for (i = 0; size_to_do > 0; i++)
    {
        memset(&tx, 0, sizeof(tx));

        tx.base.flags=SPI_TRANS_VARIABLE_ADDR;
        if(addr_size == 2)
        {
            tx.base.addr = conv_int_8_16(addr_buf[1], addr_buf[0]);
        }
        else //For single byte and error
        {
            tx.base.addr = addr_buf[0];
        }

        tx.address_bits=addr_size*8;

        //Check if size to write is greater than sx1301 buffer, truncate to buffer size and modulo buffer size write in chunks.
        chunk_size = (size_to_do < LGW_BURST_CHUNK) ? size_to_do : LGW_BURST_CHUNK;
        offset = i * LGW_BURST_CHUNK;
        tx.base.length = chunk_size*8;
        tx.base.tx_buffer = data+offset;
        esp_err_t ret=spi_device_transmit(*spi_target_ptr, (spi_transaction_t*)&tx);
        if(ret != ESP_OK)
        {
            return LGW_SPI_ERROR;
        }
        ESP_LOGV(TAG,"BURST WRITE:OUT OF TOTAL DATA LENGTH %d, THIS TRANS LENGTH %d BYTES, # TOTAL TRANSFERRED %d", size, chunk_size, (offset+chunk_size));
        size_to_do -= chunk_size; //Subtract the quantity of data already transferred.
    }
    if ((offset+chunk_size) != size)
    {
        ESP_LOGE(TAG, "ERROR: BURST WRITE SIZE MISMATCH");
        return LGW_SPI_ERROR;
    }
    else
    {
        ESP_LOGV(TAG, "Note: SPI burst write success");
        return LGW_SPI_SUCCESS;
    }
}


/* Burst (multiple-byte) read */

int lgw_spi_rb(spi_device_handle_t* spi_target_ptr, uint8_t spi_mux_mode, uint8_t spi_mux_target, uint8_t address, uint8_t *data, uint16_t size)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __func__);

    int i=0;
    uint8_t addr_size=0;
    uint8_t addr_buf[2]={0};
    spi_transaction_ext_t tx;

    int size_to_do=0;
    int chunk_size=0;
    int offset=0;

    // check input pointers
    CHECK_NULL(spi_target_ptr);
    CHECK_NULL(data);

    if ((address & 0x80) != 0)
    {
        ESP_LOGW(TAG, "WARNING: SPI address > 127");
    }
    if (size == 0)
    {
        ESP_LOGD(TAG, "ERROR: BURST OF NULL LENGTH");
        return LGW_SPI_ERROR;
    }

    if (spi_mux_mode == LGW_SPI_MUX_MODE1)
    {
        addr_buf[1] = spi_mux_target;
        addr_buf[0] = READ_ACCESS | (address & 0x7F);

        addr_size = 2;
    }
    else //For single bytes and errors.
    {
        addr_buf[0] = READ_ACCESS | (address & 0x7F);
        addr_size = 1;
    }
    size_to_do = size;
    uint8_t* txbuf=(uint8_t*)malloc(size);
    memset(txbuf, 0, size);
 
    for (i = 0; size_to_do > 0; i++)
    {
        memset(&tx, 0, sizeof(tx));

        tx.base.flags=SPI_TRANS_VARIABLE_ADDR;
        if(addr_size == 2)
        {
            tx.base.addr = conv_int_8_16(addr_buf[1], addr_buf[0]);
        }
        else //For single byte and error
        {
            tx.base.addr = addr_buf[0];
        }

        tx.address_bits=addr_size*8;

        //Check if size to write is greater than sx1301 buffer, truncate to buffer size and modulo buffer size write in chunks.
        chunk_size = (size_to_do < LGW_BURST_CHUNK) ? size_to_do : LGW_BURST_CHUNK;
        offset = i * LGW_BURST_CHUNK;
        tx.base.length = chunk_size*8;
        tx.base.tx_buffer = txbuf+offset;
        tx.base.rx_buffer = data+offset;
        esp_err_t ret=spi_device_transmit(*spi_target_ptr, (spi_transaction_t*)&tx);
        if(ret != ESP_OK)
        {
            return LGW_SPI_ERROR;
        }
        ESP_LOGV(TAG,"BURST READ:OUT OF TOTAL DATA LENGTH %d, THIS TRANS LENGTH %d BYTES, # TOTAL TRANSFERRED %d", size, chunk_size, (offset+chunk_size));
        size_to_do -= chunk_size; //Subtract the quantity of data already transferred.
    }
    free(txbuf);
    if ((offset+chunk_size) != size)
    {
        ESP_LOGE(TAG, "ERROR: SPI BURST READ FAILURE");
        return LGW_SPI_ERROR;
    }
    else
    {
        ESP_LOGV(TAG, "Note: SPI burst read success");
        return LGW_SPI_SUCCESS;
    }
}