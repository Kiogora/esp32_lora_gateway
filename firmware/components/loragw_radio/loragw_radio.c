/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
  (C)2013 Semtech-Cycleo

Description:
    Functions used to handle LoRa concentrator radios.

License: Revised BSD License, see LICENSE.TXT file include in the project
Maintainer: Michael Coracin
*/

/* -------------------------------------------------------------------------- */
/* --- DEPENDANCIES --------------------------------------------------------- */

#include <stdint.h>     /* C99 types */
#include <stdbool.h>    /* bool type */
#include <stdio.h>      /* printf fprintf */

#include "loragw_sx125x.h"
#include "loragw_sx1272_fsk.h"
#include "loragw_sx1272_lora.h"
#include "loragw_sx1276_fsk.h"
#include "loragw_sx1276_lora.h"
#include "loragw_spi.h"
#include "loragw_aux.h"
#include "loragw_reg.h"
#include "loragw_hal.h"
#include "loragw_radio.h"
#include "loragw_fpga.h"
#include "esp_log.h"

/* -------------------------------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */

static char* TAG = "[LORAGW_RADIO]";

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define CHECK_NULL(a) if(a == NULL){ESP_LOGE(TAG, "%s:%d: ERROR: NULL POINTER AS ARGUMENT",__FUNCTION__, __LINE__);return LGW_REG_ERROR;}

/* -------------------------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */

/**
@struct lgw_radio_FSK_bandwidth_s
@brief Associate a bandwidth in kHz with its corresponding register values
*/
struct lgw_sx127x_FSK_bandwidth_s
{
    uint32_t    RxBwKHz;
    uint8_t     RxBwMant;
    uint8_t     RxBwExp;
};

/**
@struct lgw_radio_type_version_s
@brief Associate a radio type with its corresponding expected version value
        read in the radio version register.
*/
struct lgw_radio_type_version_s
{
    enum lgw_radio_type_e type;
    uint8_t reg_version;
};

/* -------------------------------------------------------------------------- */
/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

#define PLL_LOCK_MAX_ATTEMPTS 5

const struct lgw_sx127x_FSK_bandwidth_s sx127x_FskBandwidths[] =
{
    { 2600  , 2, 7 },   /* LGW_SX127X_RXBW_2K6_HZ */
    { 3100  , 1, 7 },   /* LGW_SX127X_RXBW_3K1_HZ */
    { 3900  , 0, 7 },   /* ... */
    { 5200  , 2, 6 },
    { 6300  , 1, 6 },
    { 7800  , 0, 6 },
    { 10400 , 2, 5 },
    { 12500 , 1, 5 },
    { 15600 , 0, 5 },
    { 20800 , 2, 4 },
    { 25000 , 1, 4 },   /* ... */
    { 31300 , 0, 4 },
    { 41700 , 2, 3 },
    { 50000 , 1, 3 },
    { 62500 , 0, 3 },
    { 83333 , 2, 2 },
    { 100000, 1, 2 },
    { 125000, 0, 2 },
    { 166700, 2, 1 },
    { 200000, 1, 1 },   /* ... */
    { 250000, 0, 1 }    /* LGW_SX127X_RXBW_250K_HZ */
};

/* -------------------------------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */

extern spi_device_handle_t lgw_spi_target; /*! generic pointer to the SPI device */

/* -------------------------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */

void sx125x_write(uint8_t channel, uint8_t addr, uint8_t data);
uint8_t sx125x_read(uint8_t channel, uint8_t addr);

int setup_sx1272_FSK(uint32_t frequency, enum lgw_sx127x_rxbw_e rxbw_khz, int8_t rssi_offset);
int setup_sx1276_FSK(uint32_t frequency, enum lgw_sx127x_rxbw_e rxbw_khz, int8_t rssi_offset);

int reset_sx127x(enum lgw_radio_type_e radio_type);

/* -------------------------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS DEFINITION ----------------------------------------- */

void sx125x_write(uint8_t channel, uint8_t addr, uint8_t data)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __FUNCTION__);
    int reg_add, reg_dat, reg_cs;

    /* checking input parameters */
    if (channel >= LGW_RF_CHAIN_NB)
    {
        ESP_LOGE(TAG, "INVALID RF_CHAIN");
        return;
    }
    if (addr >= 0x7F)
    {
        ESP_LOGE(TAG, "ADDRESS OUT OF RANGE");
        return;
    }

    /* selecting the target radio */
    switch (channel)
    {
        case 0:
            reg_add = LGW_SPI_RADIO_A__ADDR;
            reg_dat = LGW_SPI_RADIO_A__DATA;
            reg_cs  = LGW_SPI_RADIO_A__CS;
            break;

        case 1:
            reg_add = LGW_SPI_RADIO_B__ADDR;
            reg_dat = LGW_SPI_RADIO_B__DATA;
            reg_cs  = LGW_SPI_RADIO_B__CS;
            break;

        default:
            ESP_LOGE(TAG, "UNEXPECTED VALUE %d IN SWITCH STATEMENT", channel);
            return;
    }

    /* SPI master data write procedure */
    lgw_reg_w(reg_cs, 0);
    lgw_reg_w(reg_add, 0x80 | addr); /* MSB at 1 for write operation */
    lgw_reg_w(reg_dat, data);
    lgw_reg_w(reg_cs, 1);
    lgw_reg_w(reg_cs, 0);

    return;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

uint8_t sx125x_read(uint8_t channel, uint8_t addr)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __FUNCTION__);
    int reg_add, reg_dat, reg_cs, reg_rb;
    int32_t read_value;

    /* checking input parameters */
    if (channel >= LGW_RF_CHAIN_NB)
    {
        ESP_LOGE(TAG, "INVALID RF_CHAIN");
        return 0;
    }
    if (addr >= 0x7F)
    {
        ESP_LOGE(TAG, "ADDRESS OUT OF RANGE");
        return 0;
    }

    /* selecting the target radio */
    switch (channel)
    {
        case 0:
            reg_add = LGW_SPI_RADIO_A__ADDR;
            reg_dat = LGW_SPI_RADIO_A__DATA;
            reg_cs  = LGW_SPI_RADIO_A__CS;
            reg_rb  = LGW_SPI_RADIO_A__DATA_READBACK;
            break;

        case 1:
            reg_add = LGW_SPI_RADIO_B__ADDR;
            reg_dat = LGW_SPI_RADIO_B__DATA;
            reg_cs  = LGW_SPI_RADIO_B__CS;
            reg_rb  = LGW_SPI_RADIO_B__DATA_READBACK;
            break;

        default:
            ESP_LOGE(TAG, "UNEXPECTED VALUE %d IN SWITCH STATEMENT", channel);
            return 0;
    }

    /* SPI master data read procedure */
    lgw_reg_w(reg_cs, 0);
    lgw_reg_w(reg_add, addr); /* MSB at 0 for read operation */
    lgw_reg_w(reg_dat, 0);
    lgw_reg_w(reg_cs, 1);
    lgw_reg_w(reg_cs, 0);
    lgw_reg_r(reg_rb, &read_value);

    return (uint8_t)read_value;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

int setup_sx1272_FSK(uint32_t frequency, enum lgw_sx127x_rxbw_e rxbw_khz, int8_t rssi_offset)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __FUNCTION__);
    uint64_t freq_reg;
    uint8_t ModulationShaping = 0;
    uint8_t PllHop = 1;
    uint8_t LnaGain = 1;
    uint8_t LnaBoost = 3;
    uint8_t AdcBwAuto = 0;
    uint8_t AdcBw = 7;
    uint8_t AdcLowPwr = 0;
    uint8_t AdcTrim = 6;
    uint8_t AdcTest = 0;
    uint8_t RxBwExp = sx127x_FskBandwidths[rxbw_khz].RxBwExp;
    uint8_t RxBwMant = sx127x_FskBandwidths[rxbw_khz].RxBwMant;
    uint8_t RssiSmoothing = 5;
    uint8_t RssiOffsetReg;
    uint8_t reg_val;
    int x;

    /* Set in FSK mode */
    x = lgw_sx127x_reg_w(SX1272_REG_OPMODE, 0);
    wait_ms(100);
    x |= lgw_sx127x_reg_w(SX1272_REG_OPMODE, 0 | (ModulationShaping << 3)); /* Sleep mode, no FSK shaping */
    wait_ms(100);
    x |= lgw_sx127x_reg_w(SX1272_REG_OPMODE, 1 | (ModulationShaping << 3)); /* Standby mode, no FSK shaping */
    wait_ms(100);

    /* Set RF carrier frequency */
    x |= lgw_sx127x_reg_w(SX1272_REG_PLLHOP, PllHop << 7);
    freq_reg = ((uint64_t)frequency << 19) / (uint64_t)32000000;
    x |= lgw_sx127x_reg_w(SX1272_REG_FRFMSB, (freq_reg >> 16) & 0xFF);
    x |= lgw_sx127x_reg_w(SX1272_REG_FRFMID, (freq_reg >> 8) & 0xFF);
    x |= lgw_sx127x_reg_w(SX1272_REG_FRFLSB, (freq_reg >> 0) & 0xFF);

    /* Config */
    x |= lgw_sx127x_reg_w(SX1272_REG_LNA, LnaBoost | (LnaGain << 5)); /* Improved sensitivity, highest gain */
    x |= lgw_sx127x_reg_w(0x68, AdcBw | (AdcBwAuto << 3));
    x |= lgw_sx127x_reg_w(0x69, AdcTest | (AdcTrim << 4) | (AdcLowPwr << 7));

    /* set BR and FDEV for 200 kHz bandwidth*/
    x |= lgw_sx127x_reg_w(SX1272_REG_BITRATEMSB, 125);
    x |= lgw_sx127x_reg_w(SX1272_REG_BITRATELSB, 0);
    x |= lgw_sx127x_reg_w(SX1272_REG_FDEVMSB, 2);
    x |= lgw_sx127x_reg_w(SX1272_REG_FDEVLSB, 225);

    /* Config continues... */
    x |= lgw_sx127x_reg_w(SX1272_REG_RXCONFIG, 0); /* Disable AGC */
    RssiOffsetReg = (rssi_offset >= 0) ? (uint8_t)rssi_offset : (uint8_t)(~(-rssi_offset)+1); /* 2's complement */
    x |= lgw_sx127x_reg_w(SX1272_REG_RSSICONFIG, RssiSmoothing | (RssiOffsetReg << 3)); /* Set RSSI smoothing to 64 samples, RSSI offset to given value */
    x |= lgw_sx127x_reg_w(SX1272_REG_RXBW, RxBwExp | (RxBwMant << 3));
    x |= lgw_sx127x_reg_w(SX1272_REG_RXDELAY, 2);
    x |= lgw_sx127x_reg_w(SX1272_REG_PLL, 0x10); /* PLL BW set to 75 KHz */
    x |= lgw_sx127x_reg_w(0x47, 1); /* optimize PLL start-up time */

    if (x != LGW_REG_SUCCESS)
    {
        ESP_LOGE(TAG, "FAILED TO CONFIGURE SX127x");
        return x;
    }

    /* set Rx continuous mode */
    x = lgw_sx127x_reg_w(SX1272_REG_OPMODE, 5 | (ModulationShaping << 3)); /* Receiver Mode, no FSK shaping */
    wait_ms(500);
    x |= lgw_sx127x_reg_r(SX1272_REG_IRQFLAGS1, &reg_val);
    /* Check if RxReady and ModeReady */
    if ((TAKE_N_BITS_FROM(reg_val, 6, 1) == 0) || (TAKE_N_BITS_FROM(reg_val, 7, 1) == 0) || (x != LGW_REG_SUCCESS))
    {
        ESP_LOGE(TAG, "SX1272 FAILED TO ENTER RX CONTINUOUS MODE");
        return LGW_REG_ERROR;
    }
    wait_ms(500);

    ESP_LOGV(TAG, "SUCCESSFULLY CONFIGURED SX1272 FOR FSK MODULATION (RXBW=%d)", rxbw_khz);

    return LGW_REG_SUCCESS;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

int setup_sx1276_FSK(uint32_t frequency, enum lgw_sx127x_rxbw_e rxbw_khz, int8_t rssi_offset)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __FUNCTION__);
    uint64_t freq_reg;
    uint8_t ModulationShaping = 0;
    uint8_t PllHop = 1;
    uint8_t LnaGain = 1;
    uint8_t LnaBoost = 3;
    uint8_t AdcBwAuto = 0;
    uint8_t AdcBw = 7;
    uint8_t AdcLowPwr = 0;
    uint8_t AdcTrim = 6;
    uint8_t AdcTest = 0;
    uint8_t RxBwExp = sx127x_FskBandwidths[rxbw_khz].RxBwExp;
    uint8_t RxBwMant = sx127x_FskBandwidths[rxbw_khz].RxBwMant;
    uint8_t RssiSmoothing = 5;
    uint8_t RssiOffsetReg;
    uint8_t reg_val;
    int x;

    /* Set in FSK mode */
    x = lgw_sx127x_reg_w(SX1276_REG_OPMODE, 0);
    wait_ms(100);
    x |= lgw_sx127x_reg_w(SX1276_REG_OPMODE, 0 | (ModulationShaping << 3)); /* Sleep mode, no FSK shaping */
    wait_ms(100);
    x |= lgw_sx127x_reg_w(SX1276_REG_OPMODE, 1 | (ModulationShaping << 3)); /* Standby mode, no FSK shaping */
    wait_ms(100);

    /* Set RF carrier frequency */
    x |= lgw_sx127x_reg_w(SX1276_REG_PLLHOP, PllHop << 7);
    freq_reg = ((uint64_t)frequency << 19) / (uint64_t)32000000;
    x |= lgw_sx127x_reg_w(SX1276_REG_FRFMSB, (freq_reg >> 16) & 0xFF);
    x |= lgw_sx127x_reg_w(SX1276_REG_FRFMID, (freq_reg >> 8) & 0xFF);
    x |= lgw_sx127x_reg_w(SX1276_REG_FRFLSB, (freq_reg >> 0) & 0xFF);

    /* Config */
    x |= lgw_sx127x_reg_w(SX1276_REG_LNA, LnaBoost | (LnaGain << 5)); /* Improved sensitivity, highest gain */
    x |= lgw_sx127x_reg_w(0x57, AdcBw | (AdcBwAuto << 3));
    x |= lgw_sx127x_reg_w(0x58, AdcTest | (AdcTrim << 4) | (AdcLowPwr << 7));

    /* set BR and FDEV for 200 kHz bandwidth*/
    x |= lgw_sx127x_reg_w(SX1276_REG_BITRATEMSB, 125);
    x |= lgw_sx127x_reg_w(SX1276_REG_BITRATELSB, 0);
    x |= lgw_sx127x_reg_w(SX1276_REG_FDEVMSB, 2);
    x |= lgw_sx127x_reg_w(SX1276_REG_FDEVLSB, 225);

    /* Config continues... */
    x |= lgw_sx127x_reg_w(SX1276_REG_RXCONFIG, 0); /* Disable AGC */
    RssiOffsetReg = (rssi_offset >= 0) ? (uint8_t)rssi_offset : (uint8_t)(~(-rssi_offset)+1); /* 2's complement */
    x |= lgw_sx127x_reg_w(SX1276_REG_RSSICONFIG, RssiSmoothing | (RssiOffsetReg << 3)); /* Set RSSI smoothing to 64 samples, RSSI offset 3dB */
    x |= lgw_sx127x_reg_w(SX1276_REG_RXBW, RxBwExp | (RxBwMant << 3));
    x |= lgw_sx127x_reg_w(SX1276_REG_RXDELAY, 2);
    x |= lgw_sx127x_reg_w(SX1276_REG_PLL, 0x10); /* PLL BW set to 75 KHz */
    x |= lgw_sx127x_reg_w(0x43, 1); /* optimize PLL start-up time */

    if (x != LGW_REG_SUCCESS)
    {
        ESP_LOGE(TAG, "FAILED TO CONFIGURE SX127x");
        return x;
    }

    /* set Rx continuous mode */
    x = lgw_sx127x_reg_w(SX1276_REG_OPMODE, 5 | (ModulationShaping << 3)); /* Receiver Mode, no FSK shaping */
    wait_ms(500);
    x |= lgw_sx127x_reg_r(SX1276_REG_IRQFLAGS1, &reg_val);
    /* Check if RxReady and ModeReady */
    if ((TAKE_N_BITS_FROM(reg_val, 6, 1) == 0) || (TAKE_N_BITS_FROM(reg_val, 7, 1) == 0) || (x != LGW_REG_SUCCESS))
    {
        ESP_LOGE(TAG, "SX1276 FAILED TO ENTER RX CONTINUOUS MODE");
        return LGW_REG_ERROR;
    }
    wait_ms(500);

    //ESP_LOGV("SUCCESSFULLY CONFIGURED SX1276 FOR FSK MODULATION (RXBW=%d)", rxbw_khz);

    return LGW_REG_SUCCESS;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

int reset_sx127x(enum lgw_radio_type_e radio_type)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __FUNCTION__);
    int x;

    switch(radio_type)
    {
        case LGW_RADIO_TYPE_SX1276:
            x  = lgw_fpga_reg_w(LGW_FPGA_CTRL_RADIO_RESET, 0);
            x |= lgw_fpga_reg_w(LGW_FPGA_CTRL_RADIO_RESET, 1);
            if (x != LGW_SPI_SUCCESS)
            {
                ESP_LOGE(TAG, "FAILED TO RESET SX127x RADIO");
                return x;
            }
            break;
        case LGW_RADIO_TYPE_SX1272:
            x  = lgw_fpga_reg_w(LGW_FPGA_CTRL_RADIO_RESET, 1);
            x |= lgw_fpga_reg_w(LGW_FPGA_CTRL_RADIO_RESET, 0);
            if (x != LGW_SPI_SUCCESS)
            {
                ESP_LOGE(TAG, "FAILED TO RESET SX127x RADIO");
                return x;
            }
            break;
        default:
            ESP_LOGE(TAG, "FAILED TO RESET SX127X, UNSUPPORTED RADIO TYPE (%d) FOUND", radio_type);
            return LGW_REG_ERROR;
    }

    return LGW_REG_SUCCESS;
}

/* -------------------------------------------------------------------------- */
/* --- PUBLIC FUNCTIONS DEFINITION ------------------------------------------ */

int lgw_setup_sx125x(uint8_t rf_chain, uint8_t rf_clkout, bool rf_enable, uint8_t rf_radio_type, uint32_t freq_hz)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __FUNCTION__);
    uint32_t part_int = 0;
    uint32_t part_frac = 0;
    int cpt_attempts = 0;

    if (rf_chain >= LGW_RF_CHAIN_NB)
    {
        ESP_LOGE(TAG, "INVALID RF_CHAIN");
        return -1;
    }

    /* Get version to identify SX1255/57 silicon revision */
    ESP_LOGV(TAG, "NOTE: SX125x #%d VERSION REGISTER RETURNED 0x%02x", rf_chain, sx125x_read(rf_chain, 0x07));

    /* General radio setup */
    if (rf_clkout == rf_chain)
    {
        sx125x_write(rf_chain, 0x10, SX125x_TX_DAC_CLK_SEL + 2);
        ESP_LOGV(TAG, "NOTE: SX125x #%d CLOCK OUTPUT ENABLED", rf_chain);
    }
    else
    {
        sx125x_write(rf_chain, 0x10, SX125x_TX_DAC_CLK_SEL);
        ESP_LOGV(TAG, "NOTE: SX125x #%d CLOCK OUTPUT DISABLED", rf_chain);
    }

    switch (rf_radio_type)
    {
        case LGW_RADIO_TYPE_SX1255:
            sx125x_write(rf_chain, 0x28, SX125x_XOSC_GM_STARTUP + SX125x_XOSC_DISABLE*16);
            break;
        case LGW_RADIO_TYPE_SX1257:
            sx125x_write(rf_chain, 0x26, SX125x_XOSC_GM_STARTUP + SX125x_XOSC_DISABLE*16);
            break;
        default:
            ESP_LOGE(TAG, "FAILED TO SETUP SX125x, UNSUPPORTED RADIO TYPE (%d) FOUND", rf_radio_type);
            break;
    }

    if (rf_enable == true)
    {
        /* Tx gain and trim */
        sx125x_write(rf_chain, 0x08, SX125x_TX_MIX_GAIN + SX125x_TX_DAC_GAIN*16);
        sx125x_write(rf_chain, 0x0A, SX125x_TX_ANA_BW + SX125x_TX_PLL_BW*32);
        sx125x_write(rf_chain, 0x0B, SX125x_TX_DAC_BW);

        /* Rx gain and trim */
        sx125x_write(rf_chain, 0x0C, SX125x_LNA_ZIN + SX125x_RX_BB_GAIN*2 + SX125x_RX_LNA_GAIN*32);
        sx125x_write(rf_chain, 0x0D, SX125x_RX_BB_BW + SX125x_RX_ADC_TRIM*4 + SX125x_RX_ADC_BW*32);
        sx125x_write(rf_chain, 0x0E, SX125x_ADC_TEMP + SX125x_RX_PLL_BW*2);

        /* set RX PLL frequency */
        switch (rf_radio_type)
        {
            case LGW_RADIO_TYPE_SX1255:
                part_int = freq_hz / (SX125x_32MHz_FRAC << 7); /* integer part, gives the MSB */
                part_frac = ((freq_hz % (SX125x_32MHz_FRAC << 7)) << 9) / SX125x_32MHz_FRAC; /* fractional part, gives middle part and LSB */
                break;
            case LGW_RADIO_TYPE_SX1257:
                part_int = freq_hz / (SX125x_32MHz_FRAC << 8); /* integer part, gives the MSB */
                part_frac = ((freq_hz % (SX125x_32MHz_FRAC << 8)) << 8) / SX125x_32MHz_FRAC; /* fractional part, gives middle part and LSB */
                break;
            default:
                ESP_LOGE(TAG, "FAILED TO SETUP SX125x PLL FREQ, UNSUPPORTED RADIO TYPE (%d) FOUND", rf_radio_type);
                break;
        }

        sx125x_write(rf_chain, 0x01,0xFF & part_int); /* Most Significant Byte */
        sx125x_write(rf_chain, 0x02,0xFF & (part_frac >> 8)); /* middle byte */
        sx125x_write(rf_chain, 0x03,0xFF & part_frac); /* Least Significant Byte */

        /* start and PLL lock */
        do {
            if (cpt_attempts >= PLL_LOCK_MAX_ATTEMPTS)
            {
                ESP_LOGE(TAG, "ERROR: FAIL TO LOCK PLL");
                return -1;
            }
            sx125x_write(rf_chain, 0x00, 1); /* enable Xtal oscillator */
            sx125x_write(rf_chain, 0x00, 3); /* Enable RX (PLL+FE) */
            ++cpt_attempts;
            ESP_LOGV(TAG, "NOTE: SX125x #%d PLL START/LOCK (ATTEMPT %d)", rf_chain, cpt_attempts);
            wait_ms(1);
        } while((sx125x_read(rf_chain, 0x11) & 0x02) == 0);
    }
    else
    {
        ESP_LOGV(TAG, "NOTE: SX125x #%d KEPT IN STANDBY MODE", rf_chain);
    }

    return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

int lgw_sx127x_reg_w(uint8_t address, uint8_t reg_value)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __FUNCTION__);
    return lgw_spi_w(&lgw_spi_target, LGW_SPI_MUX_MODE1, LGW_SPI_MUX_TARGET_SX127X, address, reg_value);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

int lgw_sx127x_reg_r(uint8_t address, uint8_t *reg_value)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __FUNCTION__);
    return lgw_spi_r(&lgw_spi_target, LGW_SPI_MUX_MODE1, LGW_SPI_MUX_TARGET_SX127X, address, reg_value);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

int lgw_setup_sx127x(uint32_t frequency, uint8_t modulation, enum lgw_sx127x_rxbw_e rxbw_khz, int8_t rssi_offset)
{
    ESP_LOGD(TAG, "ENTERED FUNCTION [%s]", __FUNCTION__);
    int x, i;
    uint8_t version;
    enum lgw_radio_type_e radio_type = LGW_RADIO_TYPE_NONE;
    struct lgw_radio_type_version_s supported_radio_type[2]=
    {
        {LGW_RADIO_TYPE_SX1272, 0x22},
        {LGW_RADIO_TYPE_SX1276, 0x12}
    };

    /* Check parameters */
    if (modulation != MOD_FSK)
    {
        ESP_LOGE(TAG, "MODULATION NOT SUPPORTED FOR SX127X (%u)", modulation);
        return LGW_REG_ERROR;
    }
    if (rxbw_khz > LGW_SX127X_RXBW_250K_HZ)
    {
        ESP_LOGE(TAG, "RX BANDWIDTH NOT SUPPORTED FOR SX127X (%u)", rxbw_khz);
        return LGW_REG_ERROR;
    }

    /* Probing radio type */
    for (i = 0; i < (int)(sizeof supported_radio_type); i++)
    {
        /* Reset the radio */
        x = reset_sx127x(supported_radio_type[i].type);
        if (x != LGW_SPI_SUCCESS)
        {
            ESP_LOGE(TAG, "FAILED TO RESET SX127x");
            return x;
        }
        /* Read version register */
        x = lgw_sx127x_reg_r(0x42, &version);
        if (x != LGW_SPI_SUCCESS)
        {
            ESP_LOGE(TAG, "FAILED TO READ SX127X VERSION REGISTER");
            return x;
        }
        /* Check if we got the expected version */
        if (version != supported_radio_type[i].reg_version)
        {
            ESP_LOGV(TAG, "SX127x VERSION REGISTER - READ:0x%02x, EXPECTED:0x%02x", version, supported_radio_type[i].reg_version);
            continue;
        }
        else
        {
            ESP_LOGV(TAG, "sx127x radio has been found (type:%d, version:0x%02x)", supported_radio_type[i].type, version);
            radio_type = supported_radio_type[i].type;
            break;
        }
    }
    if (radio_type == LGW_RADIO_TYPE_NONE)
    {
        ESP_LOGE(TAG, "SX127X RADIO NOT FOUND");
        return LGW_REG_ERROR;
    }

    /* Setup the radio */
    switch (modulation)
    {
        case MOD_FSK:
            if (radio_type == LGW_RADIO_TYPE_SX1272)
            {
                x = setup_sx1272_FSK(frequency, rxbw_khz, rssi_offset);
            }
            else
            {
                x = setup_sx1276_FSK(frequency, rxbw_khz, rssi_offset);
            }
            break;
        default:
            ESP_LOGE(TAG, "UNSUPPORTED MODULATION TYPE");
            /* Should not happen */
            break;
    }
    if (x != LGW_REG_SUCCESS)
    {
        ESP_LOGE(TAG, "FAILED TO SETUP SX127x");
        return x;
    }

    return LGW_REG_SUCCESS;
}

/* --- EOF ------------------------------------------------------------------ */
