/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
  (C)2013 Semtech-Cycleo

Description:
    Minimum test program for the loragw_spi 'library'

License: Revised BSD License, see LICENSE.TXT file include in the project
Maintainer: Sylvain Miermont
Modified for ESP32 by: Alois Mbutura.
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

#include "driver/spi_master.h"

#include "loragw_reg.h"


static const char* TAG = "test_loragw_reg";

#define SPI_SPEED 10000000

/* -------------------------------------------------------------------------- */
/* --- MAIN FUNCTION -------------------------------------------------------- */

#define BURST_TEST_LENGTH    8192

TEST_CASE("loragw_reg", "[loragw_reg]")
{
    int32_t read_value, test_value;
    uint16_t lfsr;
    uint8_t burst_buffout[BURST_TEST_LENGTH];
    uint8_t burst_buffin[BURST_TEST_LENGTH];
    int i;

    ESP_LOGD(TAG,"Beginning of test for loragw_reg.c\n");

    lgw_connect(false, 129E3, SPI_SPEED);
    /* 2 SPI transactions:
    -> 0x80 0x00        <- 0x00 0x00        forcing page 0
    -> 0x01 0x00        <- 0x00 0x64        checking version
    */

    /* --- READ TEST --- */

    lgw_reg_w(LGW_SOFT_RESET, 1);
    lgw_reg_check();

    /* --- READ/WRITE COHERENCY TEST --- */

    /* 8b unsigned */
    test_value = 197; /* 11000101b */
    lgw_reg_w(LGW_IMPLICIT_PAYLOAD_LENGHT, test_value);
    lgw_reg_r(LGW_IMPLICIT_PAYLOAD_LENGHT, &read_value);
    ESP_LOGD(TAG,"IMPLICIT_PAYLOAD_LENGHT = %d (should be %d)\n", read_value, test_value);

    /* 8b signed */
    /* NO SUCH REG AVAILABLE */
    // /* RADIO_SELECT is normally unsigned, modify it manually in loragw_reg.c */
    // test_value = -59; /* 11000101b */
    // lgw_reg_w(LGW_RADIO_SELECT, test_value);
    // lgw_reg_r(LGW_RADIO_SELECT, &read_value);
    // ESP_LOGD(TAG,"RADIO_SELECT = %d (should be %d)\n", read_value, test_value);

    /* less than 8b, with offset, unsigned */
    test_value = 11; /* 1011b */
    lgw_reg_w(LGW_FRAME_SYNCH_PEAK2_POS, test_value);
    lgw_reg_r(LGW_FRAME_SYNCH_PEAK2_POS, &read_value);
    ESP_LOGD(TAG,"FRAME_SYNCH_PEAK2_POS = %d (should be %d)\n", read_value, test_value);

    /* less than 8b, with offset, signed */
    /* NO SUCH REG AVAILABLE */
    // /* MBWSSF_FRAME_SYNCH_PEAK2_POS is normally unsigned, modify it manually in loragw_reg.c */
    // test_value = -5; /* 1011b */
    // lgw_reg_w(LGW_MBWSSF_FRAME_SYNCH_PEAK2_POS, test_value);
    // lgw_reg_r(LGW_MBWSSF_FRAME_SYNCH_PEAK2_POS, &read_value);
    // ESP_LOGD(TAG,"MBWSSF_FRAME_SYNCH_PEAK2_POS = %d (should be %d)\n", read_value, test_value);

    /* 16b unsigned */
    test_value = 49253; /* 11000000 01100101b */
    lgw_reg_w(LGW_PREAMBLE_SYMB1_NB, test_value);
    lgw_reg_r(LGW_PREAMBLE_SYMB1_NB, &read_value);
    ESP_LOGD(TAG,"PREAMBLE_SYMB1_NB = %d (should be %d)\n", read_value, test_value);

    /* 16b signed */
    /* NO SUCH REG AVAILABLE */
    // /* CAPTURE_PERIOD is normally unsigned, modify it manually in loragw_reg.c */
    // test_value = -16283; /* 11000000 01100101b */
    // lgw_reg_w(LGW_CAPTURE_PERIOD, test_value);
    // lgw_reg_r(LGW_CAPTURE_PERIOD, &read_value);
    // ESP_LOGD(TAG,"CAPTURE_PERIOD = %d (should be %d)\n", read_value, test_value);

    /* between 8b and 16b, unsigned */
    test_value = 3173; /* 1100 01100101b */
    lgw_reg_w(LGW_ADJUST_MODEM_START_OFFSET_SF12_RDX4, test_value);
    lgw_reg_r(LGW_ADJUST_MODEM_START_OFFSET_SF12_RDX4, &read_value);
    ESP_LOGD(TAG,"ADJUST_MODEM_START_OFFSET_SF12_RDX4 = %d (should be %d)\n", read_value, test_value);

    /* between 8b and 16b, signed */
    test_value = -1947; /* 11000 01100101b */
    lgw_reg_w(LGW_IF_FREQ_1, test_value);
    lgw_reg_r(LGW_IF_FREQ_1, &read_value);
    ESP_LOGD(TAG,"IF_FREQ_1 = %d (should be %d)\n", read_value, test_value);

    /* --- BURST WRITE AND READ TEST --- */

    /* initialize data for SPI test */
    lfsr = 0xFFFF;
    for(i=0; i<BURST_TEST_LENGTH; ++i)
    {
        burst_buffout[i] = (uint8_t)(lfsr ^ (lfsr >> 4));
        /* ESP_LOGD(TAG,"%05d # 0x%04x 0x%02x\n", i, lfsr, burst_buffout[i]); */
        lfsr = (lfsr & 1) ? ((lfsr >> 1) ^ 0x8679) : (lfsr >> 1);
    }

    lgw_reg_wb(LGW_TX_DATA_BUF_DATA, burst_buffout, 256);
    lgw_reg_rb(LGW_RX_DATA_BUF_DATA, burst_buffin, 256);

    /* impossible to check in software,
    RX_DATA_BUF_DATA is read-only,
    TX_DATA_BUF_DATA is write only,
    use a logic analyser */

    /* --- END OF TEST --- */

    lgw_disconnect();
    /* no SPI transaction */

    ESP_LOGD(TAG,"End of test for loragw_reg.c\n");
}

/* --- EOF ------------------------------------------------------------------ */