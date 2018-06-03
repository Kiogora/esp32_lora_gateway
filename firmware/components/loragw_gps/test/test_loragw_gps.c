/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
  (C)2013 Semtech-Cycleo
Description:
    Minimum test program for the loragw_gps 'library'
License: Revised BSD License, see LICENSE.TXT file include in the project
Maintainer: Michael Coracin
*/


/* -------------------------------------------------------------------------- */
/* --- DEPENDANCIES --------------------------------------------------------- */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"
#include "string.h"
#include <time.h>       /* struct timespec */
#include <math.h>       /* modf */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* exit */

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

#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_attr.h"

#include "lwip/err.h"
#include "apps/sntp/sntp.h"

#include "loragw_hal.h"
#include "loragw_gps.h"
#include "loragw_aux.h"

#ifndef SPI_SPEED
#define SPI_SPEED 8000000
#endif

#define EXAMPLE_WIFI_SSID "EED Sky"
#define EXAMPLE_WIFI_PASS "20150815"

/* -------------------------------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
struct tref ppm_ref;
/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t wifi_event_group;

/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
const int CONNECTED_BIT = BIT0;

/* -------------------------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS DECLARATION ---------------------------------------- */

static void gps_process_sync(void);
static void gps_process_coords(void);
static void obtain_time(void);
static void initialize_sntp(void);
static void initialise_wifi(void);
static esp_err_t event_handler(void *ctx, system_event_t *event);

/* -------------------------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS DEFINITION ----------------------------------------- */
static void gps_process_sync(void)
{
    /* variables for PPM pulse GPS synchronization */
    uint32_t ppm_tstamp;
    struct timespec ppm_gps;
    struct timespec ppm_utc;

    /* variables for timestamp <-> GPS time conversions */
    uint32_t x, z;
    struct timespec y;

    /* get GPS time for synchronization */
    int i = lgw_gps_get(&ppm_utc, &ppm_gps, NULL, NULL);
    if (i != LGW_GPS_SUCCESS) {
        printf("    No valid reference GPS time available, synchronization impossible.\n");
        return;
    }

    /* get timestamp for synchronization */
    i = lgw_get_trigcnt(&ppm_tstamp);
    if (i != LGW_HAL_SUCCESS) {
        printf("    Failed to read timestamp, synchronization impossible.\n");
        return;
    }

    /* try to update synchronize time reference with the new GPS & timestamp */
    i = lgw_gps_sync(&ppm_ref, ppm_tstamp, ppm_utc, ppm_gps);
    if (i != LGW_GPS_SUCCESS) {
        printf("    Synchronization error.\n");
        return;
    }

    /* display result */
    printf("    * Synchronization successful *\n");
    printf("    UTC reference time: %lld.%09ld\n", (long long)ppm_ref.utc.tv_sec, ppm_ref.utc.tv_nsec);
    printf("    GPS reference time: %lld.%09ld\n", (long long)ppm_ref.gps.tv_sec, ppm_ref.gps.tv_nsec);
    printf("    Internal counter reference value: %u\n", ppm_ref.count_us);
    printf("    Clock error: %.9f\n", ppm_ref.xtal_err);

    x = ppm_tstamp + 500000;
    printf("    * Test of timestamp counter <-> GPS value conversion *\n");
    printf("    Test value: %u\n", x);
    lgw_cnt2gps(ppm_ref, x, &y);
    printf("    Conversion to GPS: %lld.%09ld\n", (long long)y.tv_sec, y.tv_nsec);
    lgw_gps2cnt(ppm_ref, y, &z);
    printf("    Converted back: %u ==> %dµs\n", z, (int32_t)(z-x));
    printf("    * Test of timestamp counter <-> UTC value conversion *\n");
    printf("    Test value: %u\n", x);
    lgw_cnt2utc(ppm_ref, x, &y);
    printf("    Conversion to UTC: %lld.%09ld\n", (long long)y.tv_sec, y.tv_nsec);
    lgw_utc2cnt(ppm_ref, y, &z);
    printf("    Converted back: %u ==> %dµs\n", z, (int32_t)(z-x));
}

static void gps_process_coords(void)
{
    /* position variable */
    struct coord_s coord;
    struct coord_s gpserr;
    int    i = lgw_gps_get(NULL, NULL, &coord, &gpserr);

    /* update gateway coordinates */
    if (i == LGW_GPS_SUCCESS) {
        printf("# GPS coordinates: latitude %.5f, longitude %.5f, altitude %i m\n", coord.lat, coord.lon, coord.alt);
        printf("# GPS err:         latitude %.5f, longitude %.5f, altitude %i m\n", gpserr.lat, gpserr.lon, gpserr.alt);
    }
}

static void obtain_time(void)
{
    ESP_ERROR_CHECK( nvs_flash_init() );
    initialise_wifi();
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT,
                        false, true, portMAX_DELAY);
    initialize_sntp();

    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;
    while(timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count) {
        printf("Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    ESP_ERROR_CHECK( esp_wifi_stop() );
}

static void initialize_sntp(void)
{
    printf("Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
}

static void initialise_wifi(void)
{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_WIFI_SSID,
            .password = EXAMPLE_WIFI_PASS,
        },
    };
    printf("Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        /* This is a workaround as ESP32 WiFi libs don't currently
           auto-reassociate. */
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
}

/* -------------------------------------------------------------------------- */
/* --- MAIN FUNCTION -------------------------------------------------------- */

TEST_CASE("loragw_gps", "[loragw_gps]")
{

    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    // Is time set? If not, tm_year will be (1970 - 1900).
    if (timeinfo.tm_year < (2016 - 1900)) {
        printf("Time is not set yet. Connecting to WiFi and getting time over NTP.");
        obtain_time();
        // update 'now' variable with current time
        time(&now);
    }
    char strftime_buf[64];

    // Set UTC
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    printf("The current date/time in UTC+0:00 is: %s", strftime_buf);

    int i;

    /* concentrator variables */
    struct lgw_conf_board_s boardconf;
    struct lgw_conf_rxrf_s rfconf;

    /* serial variables */
    char serial_buff[128]; /* buffer to receive GPS data */
    size_t wr_idx = 0;     /* pointer to end of chars in buffer */
    int gps_tty_dev; /* file descriptor to the serial port of the GNSS module */

    /* NMEA/UBX variables */
    enum gps_msg latest_msg; /* keep track of latest NMEA/UBX message parsed */

    /* Intro message and library information */
    printf("\nBeginning of test for loragw_gps.c\n");
    printf("*** Library version information ***\n%s\n***\n", lgw_version_info());

    /* Open and configure GPS */
    i = lgw_gps_enable("ubx8", 0, &gps_tty_dev);
    if (i != LGW_GPS_SUCCESS) {
        printf("ERROR: IMPOSSIBLE TO ENABLE GPS\n");
        exit(EXIT_FAILURE);
    }

    /* start concentrator (default conf for IoT SK) */
    /* board config */
    memset(&boardconf, 0, sizeof(boardconf));
    boardconf.lorawan_public = true;
    boardconf.clksrc = 1;
    lgw_board_setconf(boardconf);

    /* RF config */
    memset(&rfconf, 0, sizeof(rfconf));
    rfconf.enable = true;
    rfconf.freq_hz = 868000000;
    rfconf.rssi_offset = 0.0;
    rfconf.tx_notch_freq = 129000U;
    rfconf.type = LGW_RADIO_TYPE_SX1257;
    rfconf.tx_enable = true;
    lgw_rxrf_setconf(0, rfconf);

    lgw_start(SPI_SPEED);

    /* initialize some variables before loop */
    memset(serial_buff, 0, sizeof serial_buff);
    memset(&ppm_ref, 0, sizeof ppm_ref);

    /* loop until user action */
    while (1) {
        size_t rd_idx = 0;
        size_t frame_end_idx = 0;

        /* blocking non-canonical read on serial port-1s timeout*/
        size_t nb_char = uart_read_bytes(gps_tty_dev, (uint8_t*)serial_buff + wr_idx, LGW_GPS_MIN_MSG_SIZE, 200 / portTICK_RATE_MS);
        if (nb_char == 0)
        {
            printf("WARNING: [gps] uart_read_bytes() returned value %d bytes\n", nb_char);
            continue;
        }
        else if (nb_char == -1) {
            printf("ERROR: uart_read_bytes() returned error value %d\n", nb_char);
            break;
        }
        wr_idx += (size_t)nb_char;

        /*******************************************
         * Scan buffer for UBX/NMEA sync chars and *
         * attempt to decode frame if one is found *
         *******************************************/
        while (rd_idx < wr_idx) {
            size_t frame_size = 0;

            /* Scan buffer for UBX sync char */
            if (serial_buff[rd_idx] == LGW_GPS_UBX_SYNC_CHAR) {

                /***********************
                 * Found UBX sync char *
                 ***********************/
                latest_msg = lgw_parse_ubx(&serial_buff[rd_idx], (wr_idx - rd_idx), &frame_size);

                if (frame_size > 0) {
                    if (latest_msg == INCOMPLETE) {
                        /* UBX header found but frame appears to be missing bytes */
                        frame_size = 0;
                    } else if (latest_msg == INVALID) {
                        /* message header received but message appears to be corrupted */
                        printf("WARNING: [gps] could not get a valid message from GPS (no time)\n");
                        frame_size = 0;
                    } else if (latest_msg == UBX_NAV_TIMEGPS) {
                        printf("\n~~ UBX NAV-TIMEGPS sentence, triggering synchronization attempt ~~\n");
                        gps_process_sync();
                    }
                }
            } else if(serial_buff[rd_idx] == LGW_GPS_NMEA_SYNC_CHAR) {
                /************************
                 * Found NMEA sync char *
                 ************************/
                /* scan for NMEA end marker (LF = 0x0a) */
                char* nmea_end_ptr = memchr(&serial_buff[rd_idx],(int)0x0a, (wr_idx - rd_idx));

                if (nmea_end_ptr) {
                    /* found end marker */
                    frame_size = nmea_end_ptr - &serial_buff[rd_idx] + 1;
                    latest_msg = lgw_parse_nmea(&serial_buff[rd_idx], frame_size);

                    if(latest_msg == INVALID || latest_msg == UNKNOWN) {
                        /* checksum failed */
                        frame_size = 0;
                    } else if (latest_msg == NMEA_RMC) { /* Get location from RMC frames */
                        gps_process_coords();
                    }
                }
            }

            if (frame_size > 0) {
                /* At this point message is a checksum verified frame
                   we're processed or ignored. Remove frame from buffer */
                rd_idx += frame_size;
                frame_end_idx = rd_idx;
            } else {
                rd_idx++;
            }
        } /* ...for(rd_idx = 0... */

        if (frame_end_idx) {
          /* Frames have been processed. Remove bytes to end of last processed frame */
          memcpy(serial_buff,&serial_buff[frame_end_idx],wr_idx - frame_end_idx);
          wr_idx -= frame_end_idx;
        } /* ...for(rd_idx = 0... */

        /* Prevent buffer overflow */
        if ((sizeof(serial_buff) - wr_idx) < LGW_GPS_MIN_MSG_SIZE) {
            memcpy(serial_buff,&serial_buff[LGW_GPS_MIN_MSG_SIZE],wr_idx - LGW_GPS_MIN_MSG_SIZE);
            wr_idx -= LGW_GPS_MIN_MSG_SIZE;
        }
    }
    //Unreachable dead code
    printf("\nEnd of test for loragw_gps.c\n");
    exit(EXIT_SUCCESS);
}



/* --- EOF ------------------------------------------------------------------ */