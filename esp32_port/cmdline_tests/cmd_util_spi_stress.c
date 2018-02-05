/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
  (C)2013 Semtech-Cycleo

Description:
    SPI stress test

License: Revised BSD License, see LICENSE.TXT file include in the project
Maintainer: Sylvain Miermont
*/


/* -------------------------------------------------------------------------- */
/* --- DEPENDANCIES --------------------------------------------------------- */
#include <stdint.h>     /* C99 types */
#include <stdbool.h>    /* bool type */
#include <stdio.h>      /* printf fprintf sprintf fopen fputs */
#include <stdlib.h>     /* rand */
#include <ctype.h>
#include "esp_log.h"
#include "esp_console.h"
#include "esp_system.h"
#include "argtable3/argtable3.h"
#include "cmd_decl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "loragw_reg.h"

/* -------------------------------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */

#define NB_STRESS_MAX 3000

#define ARRAY_SIZE(a)    (sizeof(a) / sizeof((a)[0]))

#define MSG(args...)    fprintf(stderr, args) /* message that is destined to the user */

#define SPI_SPEED 10000000
/* -------------------------------------------------------------------------- */
/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

#define VERS                    103
#define READS_WHEN_ERROR        16 /* number of times a read is repeated if there is a read error */
#define BUFF_SIZE               1024 /* maximum number of bytes that we can write in sx1301 RX data buffer */
#define DEFAULT_TX_NOTCH_FREQ   129E3

static struct
{
    struct arg_int *t;
    struct arg_int *n;
    struct arg_end *end;
}spi_stress_args;
/* -------------------------------------------------------------------------- */
/* --- MAIN FUNCTION -------------------------------------------------------- */

int util_spi_stress(int argc, char **argv)
{
    int i;
    int xi = 0;

    /* application option */
    int test_number = 1;
    int target_cycles=0;
    int cycle_number = 0;
    int repeats_per_cycle = 1000;
    bool error = false;

    /* in/out variables */
    int32_t test_value;
    int32_t read_value;
    int32_t rb1, rb2, rb3; /* interstitial readbacks, to flush buffers if needed */

    /* data buffer */
    int32_t test_addr;
    uint8_t test_buff[BUFF_SIZE];
    uint8_t read_buff[BUFF_SIZE];

    int nerrors = arg_parse(argc, argv, (void**) &spi_stress_args);
    if (nerrors != 0)
    {
        return 1;
    }

    if ((spi_stress_args.t->count != 1) || (spi_stress_args.t->ival[0] < 1) || (spi_stress_args.t->ival[0] > 4))
    {
        MSG("ERROR: invalid test number\n");
        return EXIT_FAILURE;
    }
    else
    {
        test_number = (uint8_t)(spi_stress_args.t->ival[0]);;
    }
    
    if(spi_stress_args.n->count != 1 ||((unsigned int)(spi_stress_args.n->ival[0]) < 1))
    {
        printf("ERROR: invalid number of R/W cycles (MIN 1)\n");
        return 1;
    }
    else
    {
        target_cycles=spi_stress_args.n->ival[0];

        if(target_cycles>NB_STRESS_MAX)
        {
            printf("Setting up infinite R/W test for long operation tests, usually ~24hrs");
        }
        else
        {
            printf("Setting up a test to run for %d R/W cycles", target_cycles);
        }
    }

    MSG("INFO: Starting LoRa concentrator SPI stress-test number %i\n", test_number);

    /* start SPI link */
    i = lgw_connect(false, DEFAULT_TX_NOTCH_FREQ, SPI_SPEED);
    if (i != LGW_REG_SUCCESS)
    {
        MSG("ERROR: lgw_connect() did not return SUCCESS");
        return EXIT_FAILURE;
    }

    if (test_number == 1)
    {
        /* single 8b register R/W stress test */
        while (target_cycles>NB_STRESS_MAX ? 1:(cycle_number<target_cycles))
        {
            printf("Cycle %i > ", cycle_number);
            for (i=0; i<repeats_per_cycle; ++i) 
            {
                test_value = (rand() % 256);
                lgw_reg_w(LGW_IMPLICIT_PAYLOAD_LENGHT, test_value);
                lgw_reg_r(LGW_IMPLICIT_PAYLOAD_LENGHT, &read_value);
                if (read_value != test_value) 
                {
                    error = true;
                    break;
                }
            }
            if (error)
            {
                printf("error during the %ith iteration: write 0x%02X, read 0x%02X\n", i+1, test_value, read_value);
                printf("Repeat read of target register:");
                for (i=0; i<READS_WHEN_ERROR; ++i)
                {
                    lgw_reg_r(LGW_IMPLICIT_PAYLOAD_LENGHT, &read_value);
                    printf(" 0x%02X", read_value);
                }
                printf("\n");
                lgw_disconnect();
                return EXIT_FAILURE;
            }
            else
            {
                printf("did %i R/W on an 8 bits reg with no error\n", repeats_per_cycle);
                ++cycle_number;
            }
        }
    }
    else if (test_number == 2)
    {
        /* single 8b register R/W with interstitial VERSION check stress test */
        while (target_cycles>NB_STRESS_MAX ? 1:(cycle_number<target_cycles))
        {
            printf("Cycle %i > ", cycle_number);
            for (i=0; i<repeats_per_cycle; ++i)
            {
                test_value = (rand() % 256);
                lgw_reg_r(LGW_VERSION, &rb1);
                lgw_reg_w(LGW_IMPLICIT_PAYLOAD_LENGHT, test_value);
                lgw_reg_r(LGW_VERSION, &rb2);
                lgw_reg_r(LGW_IMPLICIT_PAYLOAD_LENGHT, &read_value);
                lgw_reg_r(LGW_VERSION, &rb3);
                if ((rb1 != VERS) || (rb2 != VERS) || (rb3 != VERS) || (read_value != test_value))
                {
                    error = true;
                    break;
                }
            }
            if (error)
            {
                printf("error during the %ith iteration: write %02X, read %02X, version (%i, %i, %i)\n", i+1, test_value, read_value, rb1, rb2, rb3);
                printf("Repeat read of target register:");
                for (i=0; i<READS_WHEN_ERROR; ++i)
                {
                    lgw_reg_r(LGW_IMPLICIT_PAYLOAD_LENGHT, &read_value);
                    printf(" 0x%02X", read_value);
                }
                printf("\n");
                lgw_disconnect();
                return EXIT_FAILURE;
            }
            else
            {
                printf("did %i R/W on an 8 bits reg with no error\n", repeats_per_cycle);
                ++cycle_number;
            }
        }
    }
    else if (test_number == 3)
    {
        /* 32b register R/W stress test */
        while (target_cycles>NB_STRESS_MAX ? 1:(cycle_number<target_cycles))
        {
            printf("Cycle %i > ", cycle_number);
            for (i=0; i<repeats_per_cycle; ++i)
            {
                test_value = (rand() & 0x0000FFFF);
                test_value += (int32_t)(rand() & 0x0000FFFF) << 16;
                lgw_reg_w(LGW_FSK_REF_PATTERN_LSB, test_value);
                lgw_reg_r(LGW_FSK_REF_PATTERN_LSB, &read_value);
                if (read_value != test_value)
                {
                    error = true;
                    break;
                }
            }
            if (error)
            {
                printf("error during the %ith iteration: write 0x%08X, read 0x%08X\n", i+1, test_value, read_value);
                printf("Repeat read of target register:");
                for (i=0; i<READS_WHEN_ERROR; ++i)
                {
                    lgw_reg_r(LGW_FSK_REF_PATTERN_LSB, &read_value);
                    printf(" 0x%08X", read_value);
                }
                printf("\n");
                lgw_disconnect();
                return EXIT_FAILURE;
            }
            else
            {
                printf("did %i R/W on a 32 bits reg with no error\n", repeats_per_cycle);
                ++cycle_number;
            }
        }
    }
    else if (test_number == 4)
    {
        /* databuffer R/W stress test */
        while (target_cycles>NB_STRESS_MAX ? 1:(cycle_number<target_cycles))
        {
            for (i=0; i<BUFF_SIZE; ++i)
            {
                test_buff[i] = rand() & 0xFF;
            }
            printf("Cycle %i > ", cycle_number);
            test_addr = rand() & 0xFFFF;
            lgw_reg_w(LGW_RX_DATA_BUF_ADDR, test_addr); /* write at random offset in memory */
            lgw_reg_wb(LGW_RX_DATA_BUF_DATA, test_buff, BUFF_SIZE);
            lgw_reg_w(LGW_RX_DATA_BUF_ADDR, test_addr); /* go back to start of segment */
            lgw_reg_rb(LGW_RX_DATA_BUF_DATA, read_buff, BUFF_SIZE);
            for (i=0; ((i<BUFF_SIZE) && (test_buff[i] == read_buff[i])); ++i);
            if (i != BUFF_SIZE)
            {
                printf("error during the buffer comparison\n");
                printf("Written values:\n");
                for (i=0; i<BUFF_SIZE; ++i)
                {
                    printf(" %02X ", test_buff[i]);
                    if (i%16 == 15) printf("\n");
                }
                printf("\n");
                printf("Read values:\n");
                for (i=0; i<BUFF_SIZE; ++i)
                {
                    printf(" %02X ", read_buff[i]);
                    if (i%16 == 15) printf("\n");
                }
                printf("\n");
                lgw_reg_w(LGW_RX_DATA_BUF_ADDR, test_addr); /* go back to start of segment */
                lgw_reg_rb(LGW_RX_DATA_BUF_DATA, read_buff, BUFF_SIZE);
                printf("Re-read values:\n");
                for (i=0; i<BUFF_SIZE; ++i)
                {
                    printf(" %02X ", read_buff[i]);
                    if (i%16 == 15) printf("\n");
                }
                printf("\n");
                lgw_disconnect();
                return EXIT_FAILURE;
            }
            else
            {
                printf("did a %i-byte R/W on a data buffer with no error\n", BUFF_SIZE);
                ++cycle_number;
            }
        }
    }
    else
    {
        MSG("ERROR: invalid test number");
        lgw_disconnect();
        return -1;
    }

    /* close SPI link */
    i = lgw_disconnect();
    if (i != LGW_REG_SUCCESS)
    {
        MSG("ERROR: lgw_disconnect() did not return SUCCESS");
        return EXIT_FAILURE;
    }

    MSG("INFO: Exiting LoRa concentrator SPI stress-test program\n");
    return EXIT_SUCCESS;
}

void register_spi_stress()
{
    spi_stress_args.t = arg_int1("t", NULL, "<T>", "<int> specify which test you want to run (1-4)");
    spi_stress_args.n = arg_int1("n", NULL, "<N>", "<int> specify read write cycles to perform, max 3000");
    spi_stress_args.end = arg_end(1);

    const esp_console_cmd_t util_spi_stress_cmd=
    {
        .command = "Util_spi_stress",
        .help = "Run SPI stress tests",
        .hint = NULL,
        .func = &util_spi_stress,
        .argtable = &spi_stress_args
    };

    ESP_ERROR_CHECK( esp_console_cmd_register(&util_spi_stress_cmd) );
}

/* --- EOF ---- */

