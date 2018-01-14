/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
  (C)2013 Semtech-Cycleo

Description:
    Functions used to handle the Listen Before Talk feature

License: Revised BSD License, see LICENSE.TXT file include in the project
Maintainer: Michael Coracin
*/

/* -------------------------------------------------------------------------- */
/* --- DEPENDANCIES --------------------------------------------------------- */

#include <stdbool.h>    /* bool type */
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

#include "loragw_radio.h"
#include "loragw_aux.h"
#include "loragw_lbt.h"
#include "loragw_fpga.h"

static const char* TAG = "loragw_lbt";
/* -------------------------------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define CHECK_NULL(a) if(a == NULL){ESP_LOGE(TAG, "%s:%d: ERROR: NULL POINTER AS ARGUMENT",__FUNCTION__, __LINE__);return LGW_REG_ERROR;}

#define LBT_TIMESTAMP_MASK  0x007FF000 /* 11-bits timestamp */

/* -------------------------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* --- INTERNAL SHARED VARIABLES -------------------------------------------- */

extern spi_device_handle_t lgw_spi_target; /*! generic pointer to the SPI device */
extern uint8_t lgw_spi_mux_mode; /*! current SPI mux mode used */
extern uint16_t lgw_i_tx_start_delay_us;

/* -------------------------------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */

static bool lbt_enable;
static uint8_t lbt_nb_active_channel;
static int8_t lbt_rssi_target_dBm;
static int8_t lbt_rssi_offset_dB;
static uint32_t lbt_start_freq;
static struct lgw_conf_lbt_chan_s lbt_channel_cfg[LBT_CHANNEL_FREQ_NB];

/* -------------------------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */

bool is_equal_freq(uint32_t a, uint32_t b);

/* -------------------------------------------------------------------------- */
/* --- PUBLIC FUNCTIONS DEFINITION ------------------------------------------ */

int lbt_setconf(struct lgw_conf_lbt_s * conf)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __func__);
    int i;

    /* Check input parameters */
    if (conf == NULL)
    {
        return LGW_LBT_ERROR;
    }
    if ((conf->nb_channel < 1) || (conf->nb_channel > LBT_CHANNEL_FREQ_NB))
    {
        ESP_LOGD(TAG, "ERROR: Number of defined LBT channels is out of range (%u)\n", conf->nb_channel);
        return LGW_LBT_ERROR;
    }

    /* Initialize LBT channels configuration */
    memset(lbt_channel_cfg, 0, sizeof lbt_channel_cfg);

    /* Set internal LBT config according to parameters */
    lbt_enable = conf->enable;
    lbt_nb_active_channel = conf->nb_channel;
    lbt_rssi_target_dBm = conf->rssi_target;
    lbt_rssi_offset_dB = conf->rssi_offset;

    for (i=0; i<lbt_nb_active_channel; i++)
    {
        lbt_channel_cfg[i].freq_hz = conf->channels[i].freq_hz;
        lbt_channel_cfg[i].scan_time_us = conf->channels[i].scan_time_us;
    }

    return LGW_LBT_SUCCESS;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

int lbt_setup(void)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __func__);
    int x, i;
    int32_t val;
    uint32_t freq_offset;

    /* Check if LBT feature is supported by FPGA */
    x = lgw_fpga_reg_r(LGW_FPGA_FEATURE, &val);
    if (x != LGW_REG_SUCCESS)
    {
        ESP_LOGD(TAG, "ERROR: Failed to read FPGA Features register\n");
        return LGW_LBT_ERROR;
    }
    if (TAKE_N_BITS_FROM((uint8_t)val, 2, 1) != 1)
    {
        ESP_LOGD(TAG, "ERROR: No support for LBT in FPGA\n");
        return LGW_LBT_ERROR;
    }

    /* Get FPGA lowest frequency for LBT channels */
    x = lgw_fpga_reg_r(LGW_FPGA_LBT_INITIAL_FREQ, &val);
    if (x != LGW_REG_SUCCESS)
    {
        ESP_LOGD(TAG, "ERROR: Failed to read LBT initial frequency from FPGA\n");
        return LGW_LBT_ERROR;
    }
    switch(val)
    {
        case 0:
            lbt_start_freq = 915000000;
            break;
        case 1:
            lbt_start_freq = 863000000;
            break;
        default:
            ESP_LOGD(TAG,  "ERROR: LBT start frequency %d is not supported\n", val);
            return LGW_LBT_ERROR;
    }

    /* Configure SX127x for FSK */
    x = lgw_setup_sx127x(lbt_start_freq, MOD_FSK, LGW_SX127X_RXBW_100K_HZ, lbt_rssi_offset_dB); /* 200KHz LBT channels */
    if (x != LGW_REG_SUCCESS)
    {
        ESP_LOGD(TAG, "ERROR: Failed to configure SX127x for LBT\n");
        return LGW_LBT_ERROR;
    }

    /* Configure FPGA for LBT */
    val = -2*lbt_rssi_target_dBm; /* Convert RSSI target in dBm to FPGA register format */
    x = lgw_fpga_reg_w(LGW_FPGA_RSSI_TARGET, val);
    if (x != LGW_REG_SUCCESS)
    {
        ESP_LOGD(TAG, "ERROR: Failed to configure FPGA for LBT\n");
        return LGW_LBT_ERROR;
    }
    /* Set default values for non-active LBT channels */
    for (i=lbt_nb_active_channel; i<LBT_CHANNEL_FREQ_NB; i++)
    {
        lbt_channel_cfg[i].freq_hz = lbt_start_freq;
        lbt_channel_cfg[i].scan_time_us = 128; /* fastest scan for non-active channels */
    }
    /* Configure FPGA for both active and non-active LBT channels */
    for (i=0; i<LBT_CHANNEL_FREQ_NB; i++)
    {
        /* Check input parameters */
        if (lbt_channel_cfg[i].freq_hz < lbt_start_freq)
        {
            ESP_LOGD(TAG,  "ERROR: LBT channel frequency is out of range (%u)\n", lbt_channel_cfg[i].freq_hz);
            return LGW_LBT_ERROR;
        }
        if ((lbt_channel_cfg[i].scan_time_us != 128) && (lbt_channel_cfg[i].scan_time_us != 5000))
        {
            ESP_LOGD(TAG,  "ERROR: LBT channel scan time is not supported (%u)\n", lbt_channel_cfg[i].scan_time_us);
            return LGW_LBT_ERROR;
        }
        /* Configure */
        freq_offset = (lbt_channel_cfg[i].freq_hz - lbt_start_freq) / 100E3; /* 100kHz unit */
        x = lgw_fpga_reg_w(LGW_FPGA_LBT_CH0_FREQ_OFFSET+i, (int32_t)freq_offset);
        if (x != LGW_REG_SUCCESS)
        {
            ESP_LOGD(TAG,  "ERROR: Failed to configure FPGA for LBT channel %d (freq offset)\n", i);
            return LGW_LBT_ERROR;
        }
        if (lbt_channel_cfg[i].scan_time_us == 5000)
        {   /* configured to 128 by default */
            x = lgw_fpga_reg_w(LGW_FPGA_LBT_SCAN_TIME_CH0+i, 1);
            if (x != LGW_REG_SUCCESS)
            {
                ESP_LOGD(TAG,  "ERROR: Failed to configure FPGA for LBT channel %d (freq offset)\n", i);
                return LGW_LBT_ERROR;
            }
        }
    }

    ESP_LOGD(TAG, "Note: LBT configuration:\n");
    ESP_LOGD(TAG,  "\tlbt_enable: %d\n", lbt_enable );
    ESP_LOGD(TAG,  "\tlbt_nb_active_channel: %d\n", lbt_nb_active_channel );
    ESP_LOGD(TAG,  "\tlbt_start_freq: %d\n", lbt_start_freq);
    ESP_LOGD(TAG,  "\tlbt_rssi_target: %d\n", lbt_rssi_target_dBm );
    for (i=0; i<LBT_CHANNEL_FREQ_NB; i++)
    {
        ESP_LOGD(TAG,  "\tlbt_channel_cfg[%d].freq_hz: %u\n", i, lbt_channel_cfg[i].freq_hz );
        ESP_LOGD(TAG,  "\tlbt_channel_cfg[%d].scan_time_us: %u\n", i, lbt_channel_cfg[i].scan_time_us );
    }

    return LGW_LBT_SUCCESS;

}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

int lbt_start(void)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __func__);
    int x;

    x = lgw_fpga_reg_w(LGW_FPGA_CTRL_FEATURE_START, 1);
    if (x != LGW_REG_SUCCESS)
    {
        ESP_LOGD(TAG, "ERROR: Failed to start LBT FSM\n");
        return LGW_LBT_ERROR;
    }

    return LGW_LBT_SUCCESS;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

int lbt_is_channel_free(struct lgw_pkt_tx_s * pkt_data, uint16_t tx_start_delay, bool * tx_allowed)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __func__);
    int i;
    int32_t val;
    uint32_t tx_start_time = 0;
    uint32_t tx_end_time = 0;
    uint32_t delta_time = 0;
    uint32_t sx1301_time = 0;
    uint32_t lbt_time = 0;
    uint32_t lbt_time1 = 0;
    uint32_t lbt_time2 = 0;
    uint32_t tx_max_time = 0;
    int lbt_channel_decod_1 = -1;
    int lbt_channel_decod_2 = -1;
    uint32_t packet_duration = 0;

    /* Check input parameters */
    if ((pkt_data == NULL) || (tx_allowed == NULL))
    {
        return LGW_LBT_ERROR;
    }

    /* Check if TX is allowed */
    if (lbt_enable == true)
    {
        /* TX allowed for LoRa only */
        if (pkt_data->modulation != MOD_LORA)
        {
            *tx_allowed = false;
            ESP_LOGD(TAG,  "INFO: TX is not allowed for this modulation (%x)\n", pkt_data->modulation);
            return LGW_LBT_SUCCESS;
        }

        /* Get SX1301 time at last PPS */
        lgw_get_trigcnt(&sx1301_time);

        ESP_LOGD(TAG, "################################\n");
        switch(pkt_data->tx_mode)
        {
            case TIMESTAMPED:
                ESP_LOGD(TAG, "tx_mode                    = TIMESTAMPED\n");
                tx_start_time = pkt_data->count_us & LBT_TIMESTAMP_MASK;
                break;
            case ON_GPS:
                ESP_LOGD(TAG, "tx_mode                    = ON_GPS\n");
                tx_start_time = (sx1301_time + (uint32_t)tx_start_delay + 1000000) & LBT_TIMESTAMP_MASK;
                break;
            case IMMEDIATE:
                ESP_LOGD(TAG, "ERROR: tx_mode IMMEDIATE is not supported when LBT is enabled\n");
                /* FALLTHROUGH  */
            default:
                return LGW_LBT_ERROR;
        }

        /* Select LBT Channel corresponding to required TX frequency */
        lbt_channel_decod_1 = -1;
        lbt_channel_decod_2 = -1;
        if (pkt_data->bandwidth == BW_125KHZ)
        {
            for (i=0; i<lbt_nb_active_channel; i++)
            {
                if (is_equal_freq(pkt_data->freq_hz, lbt_channel_cfg[i].freq_hz) == true)
                {
                    ESP_LOGD(TAG,  "LBT: select channel %d (%u Hz)\n", i, lbt_channel_cfg[i].freq_hz);
                    lbt_channel_decod_1 = i;
                    lbt_channel_decod_2 = i;
                    if (lbt_channel_cfg[i].scan_time_us == 5000)
                    {
                        tx_max_time = 4000000; /* 4 seconds */
                    }
                    else
                    {   /* scan_time_us = 128 */
                        tx_max_time = 400000; /* 400 milliseconds */
                    }
                    break;
                }
            }
        }
        else if (pkt_data->bandwidth == BW_250KHZ)
        {
            /* In case of 250KHz, the TX freq has to be in between 2 consecutive channels of 200KHz BW.
                The TX can only be over 2 channels, not more */
            for (i=0; i<(lbt_nb_active_channel-1); i++)
            {
                if ((is_equal_freq(pkt_data->freq_hz, (lbt_channel_cfg[i].freq_hz+lbt_channel_cfg[i+1].freq_hz)/2) == true) && ((lbt_channel_cfg[i+1].freq_hz-lbt_channel_cfg[i].freq_hz)==200E3))
                {
                    ESP_LOGD(TAG, "LBT: select channels %d,%d (%u Hz)\n", i, i+1, (lbt_channel_cfg[i].freq_hz+lbt_channel_cfg[i+1].freq_hz)/2);
                    lbt_channel_decod_1 = i;
                    lbt_channel_decod_2 = i+1;
                    if (lbt_channel_cfg[i].scan_time_us == 5000)
                    {
                        tx_max_time = 4000000; /* 4 seconds */
                    }
                    else 
                    {   /* scan_time_us = 128 */
                        tx_max_time = 200000; /* 200 milliseconds */
                    }
                    break;
                }
            }
        } else {
            /* Nothing to do for now */
        }

        /* Get last time when selected channel was free */
        if ((lbt_channel_decod_1 >= 0) && (lbt_channel_decod_2 >= 0))
        {
            lgw_fpga_reg_w(LGW_FPGA_LBT_TIMESTAMP_SELECT_CH, (int32_t)lbt_channel_decod_1);
            lgw_fpga_reg_r(LGW_FPGA_LBT_TIMESTAMP_CH, &val);
            lbt_time = lbt_time1 = (uint32_t)(val & 0x0000FFFF) * 256; /* 16bits (1LSB = 256µs) */

            if (lbt_channel_decod_1 != lbt_channel_decod_2 )
            {
                lgw_fpga_reg_w(LGW_FPGA_LBT_TIMESTAMP_SELECT_CH, (int32_t)lbt_channel_decod_2);
                lgw_fpga_reg_r(LGW_FPGA_LBT_TIMESTAMP_CH, &val);
                lbt_time2 = (uint32_t)(val & 0x0000FFFF) * 256; /* 16bits (1LSB = 256µs) */

                if (lbt_time2 < lbt_time1)
                {
                    lbt_time = lbt_time2;
                }
            }
        }
        else
        {
            lbt_time = 0;
        }

        packet_duration = lgw_time_on_air(pkt_data) * 1000UL;
        tx_end_time = (tx_start_time + packet_duration) & LBT_TIMESTAMP_MASK;
        if (lbt_time < tx_end_time)
        {
            delta_time = tx_end_time - lbt_time;
        }
        else
        {
            /* It means LBT counter has wrapped */
            printf("LBT: lbt counter has wrapped\n");
            delta_time = (LBT_TIMESTAMP_MASK - lbt_time) + tx_end_time;
        }

        ESP_LOGD(TAG,  "sx1301_time                = %u\n", sx1301_time & LBT_TIMESTAMP_MASK);
        ESP_LOGD(TAG,  "tx_freq                    = %u\n", pkt_data->freq_hz);
        ESP_LOGD(TAG, "------------------------------------------------\n");
        ESP_LOGD(TAG,  "packet_duration            = %u\n", packet_duration);
        ESP_LOGD(TAG,  "tx_start_time              = %u\n", tx_start_time);
        ESP_LOGD(TAG,  "lbt_time1                  = %u\n", lbt_time1);
        ESP_LOGD(TAG,  "lbt_time2                  = %u\n", lbt_time2);
        ESP_LOGD(TAG,  "lbt_time                   = %u\n", lbt_time);
        ESP_LOGD(TAG,  "delta_time                 = %u\n", delta_time);
        ESP_LOGD(TAG, "------------------------------------------------\n");

        /* send data if allowed */
        /* lbt_time: last time when channel was free */
        /* tx_max_time: maximum time allowed to send packet since last free time */
        /* 2048: some margin */
        if ((delta_time < (tx_max_time - 2048)) && (lbt_time != 0))
        {
            *tx_allowed = true;
        }
        else
        {
            ESP_LOGD(TAG, "ERROR: TX request rejected (LBT)\n");
            *tx_allowed = false;
        }
    }
    else
    {
        /* Always allow if LBT is disabled */
        *tx_allowed = true;
    }

    return LGW_LBT_SUCCESS;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

bool lbt_is_enabled(void)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __func__);
    return lbt_enable;
}

/* -------------------------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS DEFINITION ----------------------------------------- */

/* As given frequencies have been converted from float to integer, some aliasing
issues can appear, so we can't simply check for equality, but have to take some
margin */
bool is_equal_freq(uint32_t a, uint32_t b)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __func__);
    int64_t diff;
    int64_t a64 = (int64_t)a;
    int64_t b64 = (int64_t)b;

    /* Calculate the difference */
    diff = llabs(a64 - b64);

    /* Check for acceptable diff range */
    if( diff <= 10000 )
    {
        return true;
    }
    return false;
}

/* --- EOF ------------------------------------------------------------------ */
