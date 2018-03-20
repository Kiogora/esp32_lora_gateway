/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
  (C)2013 Semtech-Cycleo

Description:
    LoRa concentrator : Timer synchronization
        Provides synchronization between unix, concentrator and gps clocks

License: Revised BSD License, see LICENSE.TXT file include in the project
Maintainer: Michael Coracin
*/

/* -------------------------------------------------------------------------- */
/* --- DEPENDANCIES --------------------------------------------------------- */

#include <stdio.h>        /* printf, fprintf, snprintf, fopen, fputs */
#include <stdint.h>        /* C99 types */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "trace.h"
#include "timersync.h"
#include "loragw_hal.h"
#include "loragw_reg.h"
#include "loragw_aux.h"
#include "esp_log.h"

static const char* TAG= "[TIMERSYNC]";

/* -------------------------------------------------------------------------- */
/* --- PRIVATE CONSTANTS & TYPES -------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* --- PRIVATE VARIABLES (GLOBAL) ------------------------------------------- */

static SemaphoreHandle_t mx_timersync = NULL;
static struct timeval offset_unix_concent = {0,0}; /* timer offset between unix host and concentrator */

/* -------------------------------------------------------------------------- */
/* --- PRIVATE SHARED VARIABLES (GLOBAL) ------------------------------------ */
extern SemaphoreHandle_t mx_concent;

/* -------------------------------------------------------------------------- */
/* --- PUBLIC FUNCTIONS DEFINITION ------------------------------------------ */

int get_concentrator_time(struct timeval *concent_time, struct timeval unix_time) {
    struct timeval local_timeval;

    if (concent_time == NULL) {
        MSG("ERROR: %s invalid parameter\n", __FUNCTION__);
        return -1;
    }

    xSemaphoreTake(mx_timersync, portMAX_DELAY); /* protect global variable access */
    timersub(&unix_time, &offset_unix_concent, &local_timeval);
    xSemaphoreGive(mx_timersync);

    /* TODO: handle sx1301 coutner wrap-up !! */
    concent_time->tv_sec = local_timeval.tv_sec;
    concent_time->tv_usec = local_timeval.tv_usec;

    MSG_DEBUG(DEBUG_TIMERSYNC, " --> TIME: unix current time is   %ld,%ld\n", unix_time.tv_sec, unix_time.tv_usec);
    MSG_DEBUG(DEBUG_TIMERSYNC, "           offset is              %ld,%ld\n", offset_unix_concent.tv_sec, offset_unix_concent.tv_usec);
    MSG_DEBUG(DEBUG_TIMERSYNC, "           sx1301 current time is %ld,%ld\n", local_timeval.tv_sec, local_timeval.tv_usec);

    return 0;
}

/* ---------------------------------------------------------------------------------------------- */
/* --- THREAD 6: REGULARLAY MONITOR THE OFFSET BETWEEN UNIX CLOCK AND CONCENTRATOR CLOCK -------- */

void task_timersync(void *pvParameters)
{
    mx_timersync = xSemaphoreCreateMutex();
    struct timeval unix_timeval;
    struct timeval concentrator_timeval;
    uint32_t sx1301_timecount = 0;
    struct timeval offset_previous = {0,0};
    struct timeval offset_drift = {0,0}; /* delta between current and previous offset */

    while (1) {
        /* Regularly disable GPS mode of concentrator's counter, in order to get
            real timer value for synchronizing with host's unix timer */
        ESP_LOGI(TAG, "INFO: Disabling GPS mode for concentrator's counter...");
        xSemaphoreTake(mx_concent, portMAX_DELAY);
        lgw_reg_w(LGW_GPS_EN, 0);
        xSemaphoreGive(mx_concent);

        /* Get current unix time */
        gettimeofday(&unix_timeval, NULL);

        /* Get current concentrator counter value (1MHz) */
        xSemaphoreTake(mx_concent, portMAX_DELAY);
        lgw_get_trigcnt(&sx1301_timecount);
        xSemaphoreGive(mx_concent);
        concentrator_timeval.tv_sec = sx1301_timecount / 1000000UL;
        concentrator_timeval.tv_usec = sx1301_timecount - (concentrator_timeval.tv_sec * 1000000UL);

        /* Compute offset between unix and concentrator timers, with microsecond precision */
        offset_previous.tv_sec = offset_unix_concent.tv_sec;
        offset_previous.tv_usec = offset_unix_concent.tv_usec;

        /* TODO: handle sx1301 coutner wrap-up */
        xSemaphoreTake(mx_timersync, portMAX_DELAY); /* protect global variable access */
        timersub(&unix_timeval, &concentrator_timeval, &offset_unix_concent);
        xSemaphoreGive(mx_timersync);

        timersub(&offset_unix_concent, &offset_previous, &offset_drift);

        ESP_LOGD(TAG, "  sx1301    = %u (µs) - timeval (%ld,%ld)",
            sx1301_timecount,
            concentrator_timeval.tv_sec,
            concentrator_timeval.tv_usec);
        ESP_LOGD(TAG, "  unix_timeval = %ld,%ld\n", unix_timeval.tv_sec, unix_timeval.tv_usec);

        ESP_LOGI(TAG, "INFO: host/sx1301 time offset=(%lds:%ldµs) - drift=%ldµs",
            offset_unix_concent.tv_sec,
            offset_unix_concent.tv_usec,
            offset_drift.tv_sec * 1000000UL + offset_drift.tv_usec);
        ESP_LOGI(TAG, "INFO: Enabling GPS mode for concentrator's counter.\n");
        xSemaphoreTake(mx_concent, portMAX_DELAY); /* TODO: Is it necessary to protect here? */
        lgw_reg_w(LGW_GPS_EN, 1);
        xSemaphoreGive(mx_concent);

        /* delay next sync */
        /* If we consider a crystal oscillator precision of about 20ppm worst case, and a clock
            running at 1MHz, this would mean 1µs drift every 50000µs (10000000/20).
            As here the time precision is not critical, we should be able to cope with at least 1ms drift,
            which should occur after 50s (50000µs * 1000).
            Let's set the thread sleep to 1 minute for now */
        wait_ms(8000);
    }
}
