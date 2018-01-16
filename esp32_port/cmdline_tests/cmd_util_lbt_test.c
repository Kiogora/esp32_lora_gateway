/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
  (C)2013 Semtech-Cycleo

Description:
    Listen Before Talk basic test application

License: Revised BSD License, see LICENSE.TXT file include in the project
Maintainer: Michael Coracinlibloragw/install_ftdi.txt
*/


/* -------------------------------------------------------------------------- */
/* --- DEPENDANCIES --------------------------------------------------------- */
#include <stdint.h>     /* C99 types */
#include <stdbool.h>    /* bool type */
#include <stdio.h>      /* printf fprintf sprintf fopen fputs */
#include "esp_log.h"
#include "esp_console.h"
#include "esp_system.h"
#include "argtable3/argtable3.h"
#include "cmd_decl.h"
#include <stdlib.h>     /* rand */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "loragw_aux.h"
#include "loragw_reg.h"
#include "loragw_hal.h"
#include "loragw_radio.h"
#include "loragw_fpga.h"

/* -------------------------------------------------------------------------- */
/* --- PRIVATE MACROS & CONSTANTS ------------------------------------------- */

#define ARRAY_SIZE(a)   (sizeof(a) / sizeof((a)[0]))
#define MSG(args...)    fprintf(stderr, args) /* message that is destined to the user */

#define SPI_SPEED 8000000

#define DEFAULT_SX127X_RSSI_OFFSET -1

static struct
{
    struct arg_dbl *f;
    struct arg_int *o;
    struct arg_int *r;
    struct arg_int *s;
    struct arg_end *end;
} loragw_lbt_args;

/* -------------------------------------------------------------------------- */
/* --- MAIN FUNCTION -------------------------------------------------------- */

int loragw_lbt_test(int argc, char **argv)
{
    int i;
    int xi = 0;

    /* in/out variables */
    double f1 = 0.0;
    uint32_t f_init = 0; /* in Hz */
    uint32_t f_start = 0; /* in Hz */
    uint16_t loop_cnt = 0;
    int8_t rssi_target_dBm = -80;
    uint16_t scan_time_us = 128;
    uint32_t timestamp;
    uint8_t rssi_value;
    int8_t rssi_offset = DEFAULT_SX127X_RSSI_OFFSET;
    int32_t val, val2;
    int channel;
    uint32_t freq_offset;

    int nerrors = arg_parse(argc, argv, (void**) &loragw_lbt_args);
    if (nerrors != 0)
    {
        return 1;
    }

    f1=loragw_lbt_args.f ->dval[0];
    i=loragw_lbt_args.f ->count;
    if ((i != 1) || (f1 < 30.0) || (f1 > 3000.0))
    {
        MSG("ERROR: Invalid LBT start frequency\n");
        return EXIT_FAILURE;
    }
    else
    {
        f_start = (uint32_t)((f1*1e6) + 0.5);/* .5 Hz offset to get rounding instead of truncating */
    }

    xi=loragw_lbt_args.s ->ival[0];
    i=loragw_lbt_args.s ->count;
    if ((i != 1) || ((xi != 128) && (xi != 5000)))
    {
        MSG("ERROR: scan_time_us must be 128 or 5000 \n");
        return EXIT_FAILURE;
    }
    else
    {
        scan_time_us = xi;
    }
    
    xi=loragw_lbt_args.r ->ival[0];
    i=loragw_lbt_args.r ->count;
    if ((i != 1) || ((xi < -128) && (xi > 0))) {
        MSG("ERROR: rssi_target must be b/w -128 & 0 \n");
        return EXIT_FAILURE;
    }
    else {
        rssi_target_dBm = xi;
    }

    xi=loragw_lbt_args.o ->ival[0];
    i=loragw_lbt_args.o ->count;
    if((i != 1) || (xi < -128) || (xi > 127)) {
        MSG("ERROR: rssi_offset must be b/w -128 & 127\n");
        return EXIT_FAILURE;
    }
    else
    {
        rssi_offset = (int8_t)xi;
    }

    MSG("INFO: Starting LoRa Gateway v1.5 LBT test\n");

    /* Connect to concentrator */
    i = lgw_connect(false, LGW_DEFAULT_NOTCH_FREQ, SPI_SPEED);
    if (i != LGW_REG_SUCCESS)
    {
        MSG("ERROR: lgw_connect() did not return SUCCESS\n");
        return EXIT_FAILURE;
    }


    /* Check if FPGA supports LBT */
    lgw_fpga_reg_r(LGW_FPGA_FEATURE, &val);
    if (TAKE_N_BITS_FROM((uint8_t)val, 2, 1) != true)
    {
        MSG("ERROR: LBT is not supported (0x%x)\n", (uint8_t)val);
        lgw_disconnect();
        return EXIT_FAILURE;
    }

    /* Get FPGA lowest frequency for LBT channels */
    lgw_fpga_reg_r(LGW_FPGA_LBT_INITIAL_FREQ, &val);
    switch (val)
    {
        case 0:
            f_init = 915000000;
            break;
        case 1:
            f_init = 863000000;
            break;
        default:
            MSG("ERROR: LBT start frequency %d is not supported\n", val);
            lgw_disconnect();
            return EXIT_FAILURE;
    }

    /* Initialize 1st LBT channel freq if not given by user */
    if (f_start == 0)
    {
        f_start = f_init;
    }
    else if (f_start < f_init)
    {
        MSG("ERROR: LBT start frequency %u is not supported (f_init=%u)\n", f_start, f_init);
        lgw_disconnect();
        return EXIT_FAILURE;
    }
    MSG("FREQ: %u\n", f_start);

    /* Configure SX127x and read few RSSI points */
    lgw_setup_sx127x(f_init, MOD_FSK, LGW_SX127X_RXBW_100K_HZ, rssi_offset); /* 200KHz LBT channels */
    for (i = 0; i < 100; i++)
    {
        lgw_sx127x_reg_r(0x11, &rssi_value); /* 0x11: RegRssiValue */
        MSG("SX127x RSSI:%i dBm\n", -(rssi_value/2));
        wait_ms(10);
    }

    /* Configure LBT */
    val = -2*(rssi_target_dBm);
    lgw_fpga_reg_w(LGW_FPGA_RSSI_TARGET, val);
    for (i = 0; i < LBT_CHANNEL_FREQ_NB; i++) {
        freq_offset = (f_start - f_init)/100E3 + i*2; /* 200KHz between each channel */
        lgw_fpga_reg_w(LGW_FPGA_LBT_CH0_FREQ_OFFSET+i, (int32_t)freq_offset);
        if (scan_time_us == 5000) { /* configured to 128 by default */
            lgw_fpga_reg_w(LGW_FPGA_LBT_SCAN_TIME_CH0+i, 1);
        }
    }

    lgw_fpga_reg_r(LGW_FPGA_RSSI_TARGET, &val);
    MSG("RSSI_TARGET = %d\n", val);
    if (val != (-2*rssi_target_dBm))
    {
        MSG("ERROR: failed to read back RSSI target register value\n");
        lgw_disconnect();
        return EXIT_FAILURE;
    }
    for (i = 0; i < LBT_CHANNEL_FREQ_NB; i++)
    {
        lgw_fpga_reg_r(LGW_FPGA_LBT_CH0_FREQ_OFFSET+i, &val);
        lgw_fpga_reg_r(LGW_FPGA_LBT_SCAN_TIME_CH0+i, &val2);
        MSG("CH_%i: freq=%u (offset=%i), scan_time=%u (%i)\n", i, (uint32_t)((val*100E3)+f_init), val, (val2==1)?5000:128, val2);
    }
    lgw_fpga_reg_r(LGW_FPGA_VERSION, &val);
    MSG("FPGA VERSION = %d\n", val);

    /* Enable LBT FSM */
    lgw_fpga_reg_w(LGW_FPGA_CTRL_FEATURE_START, 1);

    /* Start test */
    while (1)
    {
        MSG("~~~~\n");
        for (channel = 0; channel < LBT_CHANNEL_FREQ_NB; channel++)
        {
            /* Select LBT channel */
            lgw_fpga_reg_w(LGW_FPGA_LBT_TIMESTAMP_SELECT_CH, channel);

            /* Get last instant when the selected channel was free */
            lgw_fpga_reg_r(LGW_FPGA_LBT_TIMESTAMP_CH, &val);
            timestamp = (uint32_t)(val & 0x0000FFFF) * 256; /* 16bits (1LSB = 256µs) */
            MSG(" TIMESTAMP_CH%u = %u\n", channel, timestamp);
        }

        loop_cnt += 1;
        wait_ms(400);
    }

    /* close SPI link */
    i = lgw_disconnect();
    if (i != LGW_REG_SUCCESS)
    {
        MSG("ERROR: lgw_disconnect() did not return SUCCESS\n");
        return EXIT_FAILURE;
    }

    MSG("INFO: Exiting LoRa Gateway v1.5 LBT test successfully\n");
    return EXIT_SUCCESS;
}

void register_loragw_lbt()
{
    loragw_lbt_args.f = arg_dbl1("f", NULL, "<F>", "<float> frequency in MHz of the first LBT channel");
    loragw_lbt_args.o = arg_int1("o", NULL, "<O>", "<int>   offset in dB to be applied to the SX127x RSSI [-128..127]");
    loragw_lbt_args.r = arg_int1("r", NULL, "<R>", "<int>   target RSSI: signal strength target used to detect if the channel is clear or not [-128..0]");
    loragw_lbt_args.s = arg_int1("s", NULL, "<S>", "<uint>  scan time in µs for all 8 LBT channels [128,5000]");
    loragw_lbt_args.end = arg_end(4);

    const esp_console_cmd_t loragw_lbt_cmd=
    {
        .command = "loragw_lbt",
        .help = "Run loragw_lbt test",
        .hint = NULL,
        .func = &loragw_lbt_test,
        .argtable = &loragw_lbt_args
    };

    ESP_ERROR_CHECK( esp_console_cmd_register(&loragw_lbt_cmd) );
}
/* --- EOF ------------------------------------------------------------------ */

