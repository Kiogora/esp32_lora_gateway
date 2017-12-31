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
#include <unistd.h>        /* lseek, close */
#include <fcntl.h>        /* open */
#include <string.h>        /* memset */

#include <sys/ioctl.    h>
#include <linux/spi/spidev.h>

#include "loragw_spi.h"
#include "loragw_hal.h"

/* -------------------------------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#if DEBUG_SPI == 1
    #define DEBUG_MSG(str)                fprintf(stderr, str)
    #define DEBUG_PRINTF(fmt, args...)    fprintf(stderr,"%s:%d: "fmt, __FUNCTION__, __LINE__, args)
    #define CHECK_NULL(a)                if(a==NULL){fprintf(stderr,"%s:%d: ERROR: NULL POINTER AS ARGUMENT\n", __FUNCTION__, __LINE__);return LGW_SPI_ERROR;}
#else
    #define DEBUG_MSG(str)
    #define DEBUG_PRINTF(fmt, args...)
    #define CHECK_NULL(a)                if(a==NULL){return LGW_SPI_ERROR;}
#endif

/* -------------------------------------------------------------------------- */
/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

#define READ_ACCESS     0x00
#define WRITE_ACCESS    0x80
// This is handled in an other file to create more hardware freedom
//#define SPI_SPEED       8000000
//#define SPI_DEV_PATH    "/dev/spidev0.0"
//#define SPI_DEV_PATH    "/dev/spidev32766.0"

/* -------------------------------------------------------------------------- */
/* --- PUBLIC FUNCTIONS DEFINITION ------------------------------------------ */

void concentrator_pre_transfer_callback(spi_transaction_t *t) 
{
    /*Record the transaction ID irregardless of transaction type(Read/Write, burst/simple)*/
    static int tx_cnt = 0;
    /*Cast and update the transaction user field for debugging if need be*/
    (int)t->user=tx_cnt;
}

/* SPI initialization and configuration */
void lgw_spi_open(spi_device_handle_t *spi_target_ptr, long speed)
{
    //Safety check for null pointer
    assert(spi_target_ptr != NULL);

    esp_err_t ret;
    spi_device_handle_t concentrator;
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
        .pre_cb=concentrator_pre_transfer_callback,   //Specify pre-transfer callback to handle CS line
    };
    //Initialize the SPI bus
    //We use assert due to the async nature of the esp32.
    ret=spi_bus_initialize(HSPI_HOST, &buscfg, 1);
    assert(ret==ESP_OK);
    //Attach the LCD to the SPI bus
    ret=spi_bus_add_device(HSPI_HOST, &devcfg, &concentrator);
    assert(ret==ESP_OK);

    spi_target_ptr = &concentrator;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* SPI release */
void lgw_spi_close(spi_device_handle_t *spi_target_ptr)
{
    esp_err_t ret;
    ret=spi_bus_remove_device(spi_target_ptr);
    assert(ret==ESP_OK);
    ret=spi_bus_free(HSPI_HOST);
    assert(ret==ESP_OK);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Simple write */
void lgw_spi_w(spi_device_handle_t *spi_target_ptr, uint8_t spi_mux_mode, uint8_t spi_mux_target, uint8_t address, uint8_t data)
{
    /*Set up local handler for the concentrator object passed(*spi_target_ptr)*/
    spi_device_handle_t spi_device;
    
    /*Max data to be sent for simple write is 3 bytes*/    
    uint8_t out_buf[3];
    uint8_t command_size;

    spi_transaction_t k;

    /* check input variables */
    assert(spi_target_ptr != NULL);

    if ((address & 0x80) != 0)
    {
        ESP_LOGD("WARNING: SPI address > 127");
    }

    /*You must check that spi_target is not null beforehand */
    spi_device = *spi_target_ptr;

    /*prepare frame to be sent */
    /*If FPGA detected, add SPI_MUX_TARGET HEADER(FPGA, EEPROM,SX127x) to simple write cmd*/
    if (spi_mux_mode == LGW_SPI_MUX_MODE1)
    {
        out_buf[0] = spi_mux_target;
        out_buf[1] = WRITE_ACCESS | (address & 0x7F);
        out_buf[2] = data;
        command_size = 3;
    }
    /* If no FPGA, don't add explicit HEADER to simple write cmd*/
    else
    {
        out_buf[0] = WRITE_ACCESS | (address & 0x7F);
        out_buf[1] = data;
        command_size = 2;
    }

    /* zero out the transaction struct first*/
    memset(&k, 0, sizeof(k));
    k.length=command_size*8;      //command_size is in bytes, transaction length is in bits.
    k.tx_buffer=(void*)&out_buf;  //Pointer to actual data, safely cast to void pointer; Avoids compile-time warnings.

    ret=spi_device_transmit(spi_device, &k); //Perform the transmit transaction!
    assert(ret==ESP_OK);                     //Should have had no issues.
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Simple read */
void lgw_spi_r(spi_device_handle_t* spi_target_ptr, uint8_t spi_mux_mode, uint8_t spi_mux_target, uint8_t address, uint8_t *data)
{
    spi_device_handle_t spi_device;
    uint8_t out_buf[3];
    uint8_t command_size;
    uint8_t in_buf[ARRAY_SIZE(out_buf)];
    
    spi_transaction_t k; /*Transaction for read cmd, holds and transmits TX and RX data in single tx(Full-duplex)*/

    /* check input variables */
    assert(spi_target_ptr != NULL);
    if ((address & 0x80) != 0)
    {
        ESP_LOGD("WARNING: SPI address > 127\n");
    }
    assert(data != NULL);

    spi_device = *spi_target_ptr; /* must check that spi_target_ptr is not null beforehand */

    /* prepare frame to be sent */
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

    /* I/O transaction */
    /* zero out the transaction struct first*/
    memset(&k, 0, sizeof(k));

    k.length=command_size*8;     //command_size is in bytes, transaction length is in bits.
    k.tx_buffer=(void*)&out_buf; //Pointer to actual data, safely cast to void pointer; Avoids compile-time warnings.
    k.rx_buffer=(void*)&in_buf;  //Pointer to actual data, safely cast to void pointer; Avoids compile-time warnings
    ret=spi_device_transmit(spi_device, &k); //Perform the transmit transaction!
    assert(ret==ESP_OK);
    *data = in_buf[command_size - 1];
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Burst (multiple-byte) write */
void lgw_spi_wb(spi_device_handle_t* spi_target_ptr, uint8_t spi_mux_mode, uint8_t spi_mux_target, uint8_t address, uint8_t *data, uint16_t size)
{
    spi_device_handle_t spi_device;
    uint8_t command[2];
    uint8_t command_size;
    struct spi_ioc_transfer k[2];
    int size_to_do, chunk_size, offset;
    int byte_transfered = 0;
    int i;

    /* check input parameters */
    assert(spi_target_ptr != NULL);
    if ((address & 0x80) != 0)
    {
        ESP_LOGD("WARNING: SPI address > 127\n");
    }
    assert(data != NULL);
    if (size == 0)
    {
        ESP_LOGD("ERROR: BURST OF NULL LENGTH\n");
        return LGW_SPI_ERROR;
    }

    spi_device = *spi_target_ptr; /* must check that spi_target is not null beforehand */

    /* prepare command byte */
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

    /* I/O transaction */
    /* zero out the transaction struct first*/
    memset(&k, 0, sizeof(k));
    k[0].tx_buf = (void*) &command[0]; //***********resume
    k[0].length = command_size*8;
    for (i=0; size_to_do > 0; ++i)
    {
        chunk_size = (size_to_do < LGW_BURST_CHUNK) ? size_to_do : LGW_BURST_CHUNK;
        offset = i * LGW_BURST_CHUNK;
        k[1].tx_buf = (unsigned long)(data + offset);
        k[1].len = chunk_size;
        byte_transfered += (ioctl(spi_device, SPI_IOC_MESSAGE(2), &k) - k[0].len );
        DEBUG_PRINTF("BURST WRITE: to trans %d # chunk %d # transferred %d \n", size_to_do, chunk_size, byte_transfered);
        size_to_do -= chunk_size; /* subtract the quantity of data already transferred */
    }

    /* determine return code */
    if (byte_transfered != size)
    {
        DEBUG_MSG("ERROR: SPI BURST WRITE FAILURE\n");
        return LGW_SPI_ERROR;
    }
    else
    {
        DEBUG_MSG("Note: SPI burst write success\n");
        return LGW_SPI_SUCCESS;
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Burst (multiple-byte) read */
int lgw_spi_rb(void *spi_target, uint8_t spi_mux_mode, uint8_t spi_mux_target, uint8_t address, uint8_t *data, uint16_t size)
{
    int spi_device;
    uint8_t command[2];
    uint8_t command_size;
    struct spi_ioc_transfer k[2];
    int size_to_do, chunk_size, offset;
    int byte_transfered = 0;
    int i;

    /* check input parameters */
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

    spi_device = *(int *)spi_target; /* must check that spi_target is not null beforehand */

    /* prepare command byte */
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

    /* I/O transaction */
    memset(&k, 0, sizeof(k)); /* clear k */
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
        size_to_do -= chunk_size;  /* subtract the quantity of data already transferred */
    }

    /* determine return code */
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

/* --- EOF ------------------------------------------------------------------ */
