#include <stdint.h>        /* C99 types */
#include <stdbool.h>       /* bool type */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "esp_log.h"
#include "esp_console.h"
#include "esp_system.h"
#include "argtable3/argtable3.h"
#include "cmd_decl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "loragw_hal.h"
#include "loragw_reg.h"
#include "loragw_aux.h"

#define SPI_SPEED 8000000

/* -------------------------------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

/* -------------------------------------------------------------------------- */
/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

#define DEFAULT_RSSI_OFFSET 0.0
#define DEFAULT_NOTCH_FREQ 129000U

static const char* TAG = "[UTIL_LORAGW_HAL]";

/** Arguments**/
static struct
{
    struct arg_dbl *a;
    struct arg_dbl *b;
    struct arg_dbl *t;
    struct arg_int *r;
    struct arg_int *k;
    struct arg_end *end;
} loragw_hal_args;

static int loragw_hal_test(int argc, char** argv)
{
    struct lgw_conf_board_s boardconf;
    struct lgw_conf_rxrf_s rfconf;
    struct lgw_conf_rxif_s ifconf;

    struct lgw_pkt_rx_s rxpkt[4]; /* array containing up to 4 inbound packets metadata */
    struct lgw_pkt_tx_s txpkt; /* configuration and metadata for an outbound packet */
    struct lgw_pkt_rx_s *p; /* pointer on a RX packet */

    int i, j;
    int nb_pkt;
    uint32_t fa = 0, fb = 0, ft = 0;
    enum lgw_radio_type_e radio_type = LGW_RADIO_TYPE_NONE;
    uint8_t clocksource = loragw_hal_args.k->ival[0]; /* Radio B is source by default */

    uint32_t tx_cnt = 0;
    unsigned long loop_cnt = 0;
    uint8_t status_var = 0;
    double xd = 0.0;
    int xi = 0;

    int nerrors = arg_parse(argc, argv, (void**) &loragw_hal_args);
    if (nerrors != 0)
    {
        if(nerrors == 1)
        {
            if(loragw_hal_args.k->count == 0)
            {
                ESP_LOGI(TAG, "Using default value for option k");
            }
            else
            {
                arg_print_errors(stderr, loragw_hal_args.end, argv[0]);
                return 1;
            }
        }
        else
        {
            arg_print_errors(stderr, loragw_hal_args.end, argv[0]);
            return 1;
        }
        
    }
    fa = (uint32_t)(((loragw_hal_args.a->dval[0])*1e6) + 0.5);
    fb = (uint32_t)(((loragw_hal_args.b->dval[0])*1e6) + 0.5);
    ft = (uint32_t)(((loragw_hal_args.t->dval[0])*1e6) + 0.5);
    switch(loragw_hal_args.r->ival[0])
    {
        case 1255:
            radio_type = LGW_RADIO_TYPE_SX1255;
            break;
        case 1257:
            radio_type = LGW_RADIO_TYPE_SX1257;
            break;
        default:
            return -1;
    }
    clocksource = (uint8_t)loragw_hal_args.k->ival[0];

    /* check input parameters */
    if ((fa == 0) || (fb == 0) || (ft == 0))
    {
        return -1;
    }

    if (radio_type == LGW_RADIO_TYPE_NONE)
    {
        return -1;
    }
    /* beginning of LoRa concentrator-specific code */
    ESP_LOGI(TAG, "Beginning of test for loragw_hal.c");

    ESP_LOGI(TAG, "*** Library version information ***%s", lgw_version_info());

    /* set configuration for board */
    memset(&boardconf, 0, sizeof(boardconf));

    boardconf.lorawan_public = true;
    boardconf.clksrc = clocksource;
    lgw_board_setconf(boardconf);

    /* set configuration for RF chains */
    memset(&rfconf, 0, sizeof(rfconf));

    rfconf.enable = true;
    rfconf.freq_hz = fa;
    rfconf.rssi_offset = DEFAULT_RSSI_OFFSET;
    rfconf.type = radio_type;
    rfconf.tx_enable = true;
    rfconf.tx_notch_freq = DEFAULT_NOTCH_FREQ;
    lgw_rxrf_setconf(0, rfconf); /* radio A, f0 */

    rfconf.enable = true;
    rfconf.freq_hz = fb;
    rfconf.rssi_offset = DEFAULT_RSSI_OFFSET;
    rfconf.type = radio_type;
    rfconf.tx_enable = false;
    lgw_rxrf_setconf(1, rfconf); /* radio B, f1 */

    /* set configuration for LoRa multi-SF channels (bandwidth cannot be set) */
    memset(&ifconf, 0, sizeof(ifconf));

    ifconf.enable = true;
    ifconf.rf_chain = 1;
    ifconf.freq_hz = -400000;
    ifconf.datarate = DR_LORA_MULTI;
    lgw_rxif_setconf(0, ifconf); /* chain 0: LoRa 125kHz, all SF, on f1 - 0.4 MHz */

    ifconf.enable = true;
    ifconf.rf_chain = 1;
    ifconf.freq_hz = -200000;
    ifconf.datarate = DR_LORA_MULTI;
    lgw_rxif_setconf(1, ifconf); /* chain 1: LoRa 125kHz, all SF, on f1 - 0.2 MHz */

    ifconf.enable = true;
    ifconf.rf_chain = 1;
    ifconf.freq_hz = 0;
    ifconf.datarate = DR_LORA_MULTI;
    lgw_rxif_setconf(2, ifconf); /* chain 2: LoRa 125kHz, all SF, on f1 - 0.0 MHz */

    ifconf.enable = true;
    ifconf.rf_chain = 0;
    ifconf.freq_hz = -400000;
    ifconf.datarate = DR_LORA_MULTI;
    lgw_rxif_setconf(3, ifconf); /* chain 3: LoRa 125kHz, all SF, on f0 - 0.4 MHz */

    ifconf.enable = true;
    ifconf.rf_chain = 0;
    ifconf.freq_hz = -200000;
    ifconf.datarate = DR_LORA_MULTI;
    lgw_rxif_setconf(4, ifconf); /* chain 4: LoRa 125kHz, all SF, on f0 - 0.2 MHz */

    ifconf.enable = true;
    ifconf.rf_chain = 0;
    ifconf.freq_hz = 0;
    ifconf.datarate = DR_LORA_MULTI;
    lgw_rxif_setconf(5, ifconf); /* chain 5: LoRa 125kHz, all SF, on f0 + 0.0 MHz */

    ifconf.enable = true;
    ifconf.rf_chain = 0;
    ifconf.freq_hz = 200000;
    ifconf.datarate = DR_LORA_MULTI;
    lgw_rxif_setconf(6, ifconf); /* chain 6: LoRa 125kHz, all SF, on f0 + 0.2 MHz */

    ifconf.enable = true;
    ifconf.rf_chain = 0;
    ifconf.freq_hz = 400000;
    ifconf.datarate = DR_LORA_MULTI;
    lgw_rxif_setconf(7, ifconf); /* chain 7: LoRa 125kHz, all SF, on f0 + 0.4 MHz */

    /* set configuration for LoRa 'stand alone' channel */
    memset(&ifconf, 0, sizeof(ifconf));
    ifconf.enable = true;
    ifconf.rf_chain = 0;
    ifconf.freq_hz = 0;
    ifconf.bandwidth = BW_250KHZ;
    ifconf.datarate = DR_LORA_SF10;
    lgw_rxif_setconf(8, ifconf); /* chain 8: LoRa 250kHz, SF10, on f0 MHz */

    /* set configuration for FSK channel */
    memset(&ifconf, 0, sizeof(ifconf));
    ifconf.enable = true;
    ifconf.rf_chain = 1;
    ifconf.freq_hz = 0;
    ifconf.bandwidth = BW_250KHZ;
    ifconf.datarate = 64000;
    lgw_rxif_setconf(9, ifconf); /* chain 9: FSK 64kbps, on f1 MHz */

    /* set configuration for TX packet */
    memset(&txpkt, 0, sizeof(txpkt));
    txpkt.freq_hz = ft;
    txpkt.tx_mode = IMMEDIATE;
    txpkt.rf_power = 10;
    txpkt.modulation = MOD_LORA;
    txpkt.bandwidth = BW_125KHZ;
    txpkt.datarate = DR_LORA_SF9;
    txpkt.coderate = CR_LORA_4_5;
    strcpy((char *)txpkt.payload, "TX.TEST.LORA.GW.????" );
    txpkt.size = 20;
    txpkt.preamble = 6;
    txpkt.rf_chain = 0;
/*
    memset(&txpkt, 0, sizeof(txpkt));
    txpkt.freq_hz = F_TX;
    txpkt.tx_mode = IMMEDIATE;
    txpkt.rf_power = 10;
    txpkt.modulation = MOD_FSK;
    txpkt.f_dev = 50;
    txpkt.datarate = 64000;
    strcpy((char *)txpkt.payload, "TX.TEST.LORA.GW.????" );
    txpkt.size = 20;
    txpkt.preamble = 4;
    txpkt.rf_chain = 0;
*/

    /* connect, configure and start the LoRa concentrator */
    i = lgw_start(SPI_SPEED);
    if (i == LGW_HAL_SUCCESS) 
    {
        ESP_LOGI(TAG, "*** Concentrator started ***");
    } 
    else 
    {
        ESP_LOGE(TAG, "*** Impossible to start concentrator ***");
        return -1;
    }

    /* once configured, dump content of registers to a file, for reference */
    // FILE * reg_dump = NULL;
    // reg_dump = fopen("reg_dump.log", "w");
    // if (reg_dump != NULL) {
        // lgw_reg_check(reg_dump);
        // fclose(reg_dump);
    // }

    while (1)
    {
        loop_cnt++;

        /* fetch N packets */
        nb_pkt = lgw_receive(ARRAY_SIZE(rxpkt), rxpkt);

        if (nb_pkt == 0)
        {
            wait_ms(300);
        }
        else
        {
            /* display received packets */
            for(i=0; i < nb_pkt; ++i)
            {
                p = &rxpkt[i];
                ESP_LOGI(TAG, "---Rcv pkt #%d >>", i+1);
                if (p->status == STAT_CRC_OK)
                {
                    ESP_LOGI(TAG, " if_chain:%2d", p->if_chain);
                    ESP_LOGI(TAG, " tstamp:%010u", p->count_us);
                    ESP_LOGI(TAG, " size:%3u", p->size);
                    switch (p-> modulation)
                    {
                        case MOD_LORA: ESP_LOGI(TAG, " LoRa"); break;
                        case MOD_FSK: ESP_LOGI(TAG, " FSK"); break;
                        default: ESP_LOGE(TAG, " modulation?");
                    }
                    switch (p->datarate)
                    {
                        case DR_LORA_SF7: ESP_LOGI(TAG, " SF7"); break;
                        case DR_LORA_SF8: ESP_LOGI(TAG, " SF8"); break;
                        case DR_LORA_SF9: ESP_LOGI(TAG, " SF9"); break;
                        case DR_LORA_SF10: ESP_LOGI(TAG, " SF10"); break;
                        case DR_LORA_SF11: ESP_LOGI(TAG, " SF11"); break;
                        case DR_LORA_SF12: ESP_LOGI(TAG, " SF12"); break;
                        default: ESP_LOGE(TAG, " datarate?");
                    }
                    switch (p->coderate)
                    {
                        case CR_LORA_4_5: ESP_LOGI(TAG, " CR1(4/5)"); break;
                        case CR_LORA_4_6: ESP_LOGI(TAG, " CR2(2/3)"); break;
                        case CR_LORA_4_7: ESP_LOGI(TAG, " CR3(4/7)"); break;
                        case CR_LORA_4_8: ESP_LOGI(TAG, " CR4(1/2)"); break;
                        default: ESP_LOGE(TAG, " coderate?");
                    }
                    ESP_LOGI(TAG, "");
                    ESP_LOGI(TAG, " RSSI:%+6.1f SNR:%+5.1f (min:%+5.1f, max:%+5.1f) payload:", p->rssi, p->snr, p->snr_min, p->snr_max);

                    ESP_LOG_BUFFER_HEX(TAG, p->payload, p->size);
                    ESP_LOGI(TAG, " #");
                }
                else if (p->status == STAT_CRC_BAD)
                {
                    ESP_LOGI(TAG, " if_chain:%2d", p->if_chain);
                    ESP_LOGI(TAG, " tstamp:%010u", p->count_us);
                    ESP_LOGI(TAG, " size:%3u", p->size);
                    ESP_LOGE(TAG, " CRC error, damaged packet");
                }
                else if (p->status == STAT_NO_CRC)
                {
                    ESP_LOGI(TAG, " if_chain:%2d", p->if_chain);
                    ESP_LOGI(TAG, " tstamp:%010u", p->count_us);
                    ESP_LOGI(TAG, " size:%3u", p->size);
                    ESP_LOGE(TAG, " no CRC");
                }
                else
                {
                    ESP_LOGI(TAG, " if_chain:%2d", p->if_chain);
                    ESP_LOGI(TAG, " tstamp:%010u", p->count_us);
                    ESP_LOGI(TAG, " size:%3u", p->size);
                    ESP_LOGE(TAG, " invalid status ?!?");
                }
            }
        }

        /* send a packet every X loop */
        if (loop_cnt%16 == 0)
        {
            /* 32b counter in the payload, big endian */
            txpkt.payload[16] = 0xff & (tx_cnt >> 24);
            txpkt.payload[17] = 0xff & (tx_cnt >> 16);
            txpkt.payload[18] = 0xff & (tx_cnt >> 8);
            txpkt.payload[19] = 0xff & tx_cnt;
            i = lgw_send(txpkt); /* non-blocking scheduling of TX packet */
            j = 0;
            ESP_LOGI(TAG, "+++Sending packet #%d, rf path %d, return %dstatus -> ", tx_cnt, txpkt.rf_chain, i);
            do
            {
                ++j;
                wait_ms(100);
                lgw_status(TX_STATUS, &status_var); /* get TX status */
                ESP_LOGI(TAG, "%d:", status_var);
            }
            while ((status_var != TX_FREE) && (j < 100));
            ++tx_cnt;
            ESP_LOGI(TAG, "TX finished");
        }
    }
    lgw_stop();
    ESP_LOGI(TAG, "End of test for loragw_hal.c");
    return 0;
}

void register_loragw_hal()
{
    loragw_hal_args.a = arg_dbl1("a", NULL, "867.5", "Radio A RX frequency in MHz");
    loragw_hal_args.b = arg_dbl1("b", NULL, "868.5", "Radio B RX frequency in MHz");
    loragw_hal_args.t = arg_dbl1("t", NULL, "868.0", "Radio TX frequency in MHz");
    loragw_hal_args.r = arg_int1("r", NULL, "1257", "Radio type (SX1255:1255, SX1257:1257)");
    loragw_hal_args.k = arg_int1("k", NULL, "1", "Concentrator clock source (0: radio_A, 1: radio_B(default))");
    loragw_hal_args.k ->ival[0]=1; /*Set default value*/
    loragw_hal_args.end = arg_end(6);

    const esp_console_cmd_t loragw_hal_cmd=
    {
        .command = "loragw_hal",
        .help = "Run loragw_hal test",
        .hint = NULL,
        .func = &loragw_hal_test,
        .argtable = &loragw_hal_args
    };

    ESP_ERROR_CHECK( esp_console_cmd_register(&loragw_hal_cmd) );
}