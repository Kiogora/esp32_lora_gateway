/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
  (C)2013 Semtech-Cycleo

Description:
    Host specific functions to address the LoRa concentrator registers through
    a SPI interface.
    Single-byte read/write and burst read/write.
    Does not handle pagination.
    Could be used with multiple SPI ports in parallel (explicit file descriptor)

License: Revised BSD License, see LICENSE.TXT file include in the project
Maintainer: Sylvain Miermont
*/


/* -------------------------------------------------------------------------- */
/* --- DEPENDANCIES --------------------------------------------------------- */

#include <stdint.h>        /* C99 types */
#include <stdio.h>        /* printf fprintf */
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

static const char* TAG = "loragw_spi";

#define READ_ACCESS     0x00
#define WRITE_ACCESS    0x80

#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  19
#define PIN_NUM_CS   22


static void * data_swapcpy(void *dest, const void *src, size_t n)
{ size_t i;
  for(i=0; i<n; i++) 
  {
      ((uint8_t*)dest)[i] = ((uint8_t*)src)[n-i-1];
  }
return (void*)dest; 
}

#if CONFIG_LOG_DEFAULT_LEVEL >= 3
static void pre_transfer_counter(void)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __func__);
    /*Record the transaction ID irregardless of transaction type(Read/Write, burst/simple)*/
    static unsigned long tx_cnt = 0;
    tx_cnt+=1;
    ESP_LOGI(TAG,"SPI Transaction %lu", tx_cnt);
}
#endif

/* SPI initialization and configuration */
void lgw_spi_open(spi_device_handle_t* concentrator, long speed)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __func__);
    esp_err_t ret;
    spi_bus_config_t buscfg=
    {
        .miso_io_num=PIN_NUM_MISO,
        .mosi_io_num=PIN_NUM_MOSI,
        .sclk_io_num=PIN_NUM_CLK,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1
    };
    /*
    We hardcode one device onto the SPI bus by design, this will have to be refactored if more are added.
    */
    spi_device_interface_config_t devcfg=
    {
        .clock_speed_hz=speed,                        //Clock out at speed Hz
        .mode=0,                                      //SPI mode 0(CPOL=0,CPHA=0)
        .spics_io_num=PIN_NUM_CS,                     //CS pin
        .queue_size=7,                                //We want to be able to queue 7 transactions at a time
    };
    //Initialize the SPI bus
    //We use assert due to the async nature of the esp32.
    ret=spi_bus_initialize(HSPI_HOST, &buscfg, 1);
    assert(ret==ESP_OK);
    //Attach the concentrator to the SPI bus
    ret=spi_bus_add_device(HSPI_HOST, &devcfg, concentrator);
    assert(ret==ESP_OK);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* SPI release */
void lgw_spi_close(spi_device_handle_t *concentrator)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __func__);
    /* check input variables */
    assert(concentrator != NULL);
    esp_err_t ret;
    ret=spi_bus_remove_device(*concentrator);
    assert(ret==ESP_OK);
    ret=spi_bus_free(HSPI_HOST);
    assert(ret==ESP_OK);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Simple write */

void lgw_spi_w(spi_device_handle_t *concentrator, uint8_t spi_mux_mode, uint8_t spi_mux_target, uint8_t address, uint8_t data)
{   
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __func__);
    //Max data to be sent for simple write is 3 bytes    
    uint8_t out_buf[3];
    uint8_t command_size;

    spi_transaction_t k;

    //Check input variables
    assert(concentrator != NULL);

    if ((address & 0x80) != 0)
    {
        ESP_LOGD(TAG,"WARNING: SPI address > 127");
    }

    //Prepare frame to be sent
    //If FPGA detected, add SPI_MUX_TARGET HEADER(FPGA, EEPROM,SX127x) to simple write cmd
    if (spi_mux_mode == LGW_SPI_MUX_MODE1)
    {
        out_buf[0] = spi_mux_target;
        out_buf[1] = WRITE_ACCESS | (address & 0x7F);
        out_buf[2] = data;
        command_size = 3;
    }
    //If no FPGA, don't add explicit HEADER to simple write cmd
    else
    {
        out_buf[0] = WRITE_ACCESS | (address & 0x7F);
        out_buf[1] = data;
        command_size = 2;
    }

    //Zero out the transaction struct first
    memset(&k, 0, sizeof(k));
    k.length=command_size*8;     //command_size is in bytes, transaction length is in bits.
    k.flags=SPI_TRANS_USE_TXDATA; //Store data in TX struct as data needs 32bit alignment for DMA performance.
    data_swapcpy(k.tx_data, out_buf, command_size);//Swap copy to avoid little endianness with memcpy

    #if CONFIG_LOG_DEFAULT_LEVEL >= 3
    pre_transfer_counter();
    #endif
    
    esp_err_t ret=spi_device_transmit(*concentrator, &k); //Perform the transmit transaction!
    assert(ret==ESP_OK);                     //Should have had no issues.
}
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Simple read */
/*
void lgw_spi_r(spi_device_handle_t* spi_target_ptr, uint8_t spi_mux_mode, uint8_t spi_mux_target, uint8_t address, uint8_t *data)
{
    spi_device_handle_t spi_device;
    uint8_t out_buf[3];
    uint8_t command_size;   
    
    spi_transaction_t k; //Transaction for read cmd, holds and transmits TX and RX data in single tx(Full-duplex)

    // check input variables
    assert(spi_target_ptr != NULL);
    if ((address & 0x80) != 0)
    {
        ESP_LOGD(TAG,"WARNING: SPI address > 127\n");
    }
    assert(data != NULL);

    spi_device = *spi_target_ptr;

    // prepare frame to be sent
    if (spi_mux_mode == LGW_SPI_MUX_MODE1)
    {
        out_buf[0] = spi_mux_target;
        out_buf[1] = READ_ACCESS | (address & 0x7F);
        out_buf[2] = 0x00;
        command_size = 3;
    }
    else
    {
        out_buf[0] = READ_ACCESS | (address & 0x7F);
        out_buf[1] = 0x00;
        command_size = 2;
    }

    // I/O transaction
    // zero out the transaction struct first
    memset(&k, 0, sizeof(k));

    k.length=command_size*8;     //command_size is in bytes, transaction length is in bits.
    k.flags=SPI_TRANS_USE_TXDATA|SPI_TRANS_USE_RXDATA;
    memcpy(k.tx_data,out_buf, command_size);//Copy array using memcpy.
    esp_err_t ret=spi_device_transmit(spi_device, &k); //Perform the transmit transaction!
    assert(ret==ESP_OK);
    data = (void*)(k.rx_buffer[command_size - 1]); //Pointer points to transaction rx buffer
}
*/

/* Burst (multiple-byte) write */

void lgw_spi_wb(spi_device_handle_t* concentrator, uint8_t spi_mux_mode, uint8_t spi_mux_target, uint8_t address, uint8_t *data, uint16_t size)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __func__);
    uint8_t command[2];
    uint8_t command_size;
    spi_transaction_t k;
    int size_to_do, chunk_size, offset;

    // check input parameters
    assert(concentrator != NULL);
    if ((address & 0x80) != 0)
    {
        ESP_LOGD(TAG, "WARNING: SPI address > 127\n");
    }
    assert(data != NULL);
    if (size == 0)
    {
        ESP_LOGD(TAG, "ERROR: BURST OF NULL LENGTH\n");
        return;
    }

    // prepare command byte
    if (spi_mux_mode == LGW_SPI_MUX_MODE1)
    {
        command[0] = spi_mux_target;
        command[1] = WRITE_ACCESS | (address & 0x7F);
        command_size = 2;
    }
    else
    {
        command[0] = WRITE_ACCESS | (address & 0x7F);
        command_size = 1;
    }
    size_to_do = size;

    //Create new buffer to hold the command, address and data to do
    uint8_t* wb_buffer = (uint8_t*)malloc(command_size+size);
    memcpy((void*)wb_buffer, command, command_size);
    memcpy(&wb_buffer[command_size-1], data, size);
    size_to_do = size+command_size;
    
    // I/O transaction
    // zero out the transaction struct first
    for (int i = 0; size_to_do>0; i++)
    {
        //Check if size to write is greater than sx1301 buffer, if so truncate to buffer size
        chunk_size = (size_to_do < LGW_BURST_CHUNK) ? size_to_do : LGW_BURST_CHUNK;
        offset = i * LGW_BURST_CHUNK;
        memset(&k, 0, sizeof(k));
        k.tx_buffer = (void*)&wb_buffer[offset];
        k.length = (chunk_size*8);
        esp_err_t ret=spi_device_transmit(*concentrator, &k); //Perform the transmit transaction!
        assert(ret==ESP_OK);                                  //Should have had no issues.
        ESP_EARLY_LOGD(TAG,"BURST WRITE:Out of total data length %d, this trans length %d bytes, # total transferred %d", size+command_size, chunk_size, ((size+command_size+chunk_size)-size_to_do));
        size_to_do -= chunk_size; //Subtract the quantity of data already transferred.
    }
    free(wb_buffer);
}


/* Burst (multiple-byte) read */
/*
int lgw_spi_rb(void *spi_target, uint8_t spi_mux_mode, uint8_t spi_mux_target, uint8_t address, uint8_t *data, uint16_t size)
{
    int spi_device;
    uint8_t command[2];
    uint8_t command_size;
    struct spi_ioc_transfer k[2];
    int size_to_do, chunk_size, offset;
    int byte_transfered = 0;
    int i;

    // check input parameters
    CHECK_NULL(spi_target);
    if ((address & 0x80) != 0)
    {
        DEBUG_MSG("WARNING: SPI address > 127\n");
    }
    CHECK_NULL(data);
    if (size == 0)
    {
        DEBUG_MSG("ERROR: BURST OF NULL LENGTH\n");
        return LGW_SPI_ERROR;
    }

    spi_device = *(int *)spi_target;

    // prepare command byte
    if (spi_mux_mode == LGW_SPI_MUX_MODE1)
    {
        command[0] = spi_mux_target;
        command[1] = READ_ACCESS | (address & 0x7F);
        command_size = 2;
    }
    else
    {
        command[0] = READ_ACCESS | (address & 0x7F);
        command_size = 1;
    }
    size_to_do = size;

    // I/O transaction
    memset(&k, 0, sizeof(k));
    k[0].tx_buf = (unsigned long) &command[0];
    k[0].len = command_size;
    k[0].cs_change = 0;
    k[1].cs_change = 0;
    for (i=0; size_to_do > 0; ++i)
    {
        chunk_size = (size_to_do < LGW_BURST_CHUNK) ? size_to_do : LGW_BURST_CHUNK;
        offset = i * LGW_BURST_CHUNK;
        k[1].rx_buf = (unsigned long)(data + offset);
        k[1].len = chunk_size;
        byte_transfered += (ioctl(spi_device, SPI_IOC_MESSAGE(2), &k) - k[0].len );
        DEBUG_PRINTF("BURST READ: to trans %d # chunk %d # transferred %d \n", size_to_do, chunk_size, byte_transfered);
        size_to_do -= chunk_size;  //Subtract the quantity of data already transferred
    }

    //Determine return code
    if (byte_transfered != size)
    {
        DEBUG_MSG("ERROR: SPI BURST READ FAILURE\n");
        return LGW_SPI_ERROR;
    }
    else
    {
        DEBUG_MSG("Note: SPI burst read success\n");
        return LGW_SPI_SUCCESS;
    }
}
*/