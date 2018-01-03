/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
  (C)2013 Semtech-Cycleo

Description:
    Minimum test program for the loragw_spi 'library'
    Use logic analyser to check the results.

License: Revised BSD License, see LICENSE.TXT file include in the project
Maintainer: Sylvain Miermont
*/


/* -------------------------------------------------------------------------- */
/* --- DEPENDANCIES --------------------------------------------------------- */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include "unity.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_partition.h"
#include "esp_log.h"
#include <string.h>


#include "loragw_spi.h"

#define SPI_SPEED 8000000

/* -------------------------------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

/* -------------------------------------------------------------------------- */
/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

#define BURST_TEST_SIZE 2500 /* >> LGW_BURST_CHUNK */
#define TIMING_REPEAT   1    /* repeat transactions multiple times for timing characterisation */

/* -------------------------------------------------------------------------- */
/* --- MAIN FUNCTION -------------------------------------------------------- */

TEST_CASE("loragw_spi", "[loragw_spi]")
{
    int i;

    spi_device_handle_t concentrator;
//    uint8_t data = 0;
    uint8_t dataout[BURST_TEST_SIZE];
//    uint8_t datain[BURST_TEST_SIZE];
    uint8_t spi_mux_mode = LGW_SPI_MUX_MODE0;


    for (i = 0; i < BURST_TEST_SIZE; ++i)
    {
        dataout[i] = 0x30 + (i % 10);
//        datain[i] = 0x00; // garbage data, to be overwritten by received data
    }

    printf("Beginning of test for loragw_spi.c\n");
    lgw_spi_open(&concentrator, SPI_SPEED);

    /* normal R/W test */
    for (i = 0; i < TIMING_REPEAT; ++i)
    {
        lgw_spi_w(&concentrator, spi_mux_mode, LGW_SPI_MUX_TARGET_SX1301, 0xAA, 0x96);
    }
    /*
    for (i = 0; i < TIMING_REPEAT; ++i)
    {
        lgw_spi_r(spi_target, spi_mux_mode, LGW_SPI_MUX_TARGET_SX1301, 0x55, &data);
    }
    */
    /* burst R/W test, small bursts << LGW_BURST_CHUNK */

    for (i = 0; i < TIMING_REPEAT; ++i)
    {
        lgw_spi_wb(&concentrator, spi_mux_mode, LGW_SPI_MUX_TARGET_SX1301, 0x55, dataout, 16);
    }
    /*
    for (i = 0; i < TIMING_REPEAT; ++i)
    {
        lgw_spi_rb(spi_target, spi_mux_mode, LGW_SPI_MUX_TARGET_SX1301, 0x55, datain, 16);
    }
    */
    /* burst R/W test, large bursts >> LGW_BURST_CHUNK */

    for (i = 0; i < TIMING_REPEAT; ++i)
    {
        lgw_spi_wb(&concentrator, spi_mux_mode, LGW_SPI_MUX_TARGET_SX1301, 0x5A, dataout, ARRAY_SIZE(dataout));
    }
    /*    
    for (i = 0; i < TIMING_REPEAT; ++i)
    {
        lgw_spi_rb(spi_target, spi_mux_mode, LGW_SPI_MUX_TARGET_SX1301, 0x5A, datain, ARRAY_SIZE(datain));
    }
    */
    /* last read (blocking), just to be sure no to quit before the FTDI buffer is flushed */
    /*
    lgw_spi_r(spi_target, spi_mux_mode, LGW_SPI_MUX_TARGET_SX1301, 0x55, &data);
    printf("data received (simple read): %d\n",data);
    */
    lgw_spi_close(&concentrator);
    printf("End of test for loragw_spi.c\n");
}
