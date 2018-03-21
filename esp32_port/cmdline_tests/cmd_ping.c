#include <math.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "lwip/inet.h"
#include "lwip/ip4_addr.h"
#include "lwip/dns.h"
#include "lwip/opt.h"
#include "ping.h"
#include "esp_err.h"
#include "esp_ping.h"

#include "esp_console.h"
#include "esp_system.h"
#include "argtable3/argtable3.h"
#include "cmd_decl.h"

static const char* TAG = "[PING]";

static struct 
{
    struct arg_str *ip;
    struct arg_int *count;
    struct arg_int *timeout;
    struct arg_int *delay;
    struct arg_end *end;
} ping_args;

static ip4_addr_t device;
static uint32_t ping_count;   //how many pings per report
static uint32_t ping_timeout; //mS till we consider it timed out
static uint32_t ping_delay;   //mS between pings
static uint32_t waiting_results;
static int total_counts;
static double sum2;
static double mdev;
static double avg;

static char* addr = NULL;

void register_ping();

esp_err_t pingResults(ping_target_id_t msgType, esp_ping_found * pf){
    if (total_counts <= ping_count) {
        ESP_LOGI(TAG, "%d bytes from %s: icmp_seq=%d ttl=%d time=%d ms", pf->bytes, addr, pf->send_count, IP_DEFAULT_TTL, pf->resp_time);
        waiting_results = 0;
        total_counts += 1;
        /*Calculate running square*/
        sum2 += ((pf->resp_time)*(pf->resp_time));
    }
    else {
        /*calculate mean square..base calculation on recv count*/
        sum2 /= pf->recv_count;
         /*Calculate average*/
        avg = (double)pf->total_time/pf->recv_count;
        /*Calculate sample std dev */
        mdev=sqrt(sum2 - (avg * avg));
        ESP_LOGI(TAG, "--- %s ping statistics ---", addr);
        ESP_LOGI(TAG, "%d packets transmitted, %d received, %.1f%% packet loss, time %dms", pf->send_count, pf->recv_count,(float)(((pf->send_count-pf->recv_count)/pf->send_count)*100), pf->total_time);
        ESP_LOGI(TAG, "rtt min/avg/max/mdev = %d/%.1lf/%d/%.1lf", pf->min_time, (float)avg, pf->max_time, mdev); 
    }
    return ESP_OK;
}

int ping_test(int argc, char **argv)
{
    ping_count = 0;  //how many pings per report
    ping_timeout = 0; //mS till we consider it timed out
    ping_delay = 0; //mS between pings
    waiting_results = 0;
    total_counts = 1;
    sum2 = 0.0;
    mdev = 0.0;
    avg = 0.0;

    int nerrors = arg_parse(argc, argv, (void**)&ping_args);

    if (nerrors != 0)
    {
        return ESP_FAIL;
    }

    if ((ping_args.ip->count != 1))
    {
        return ESP_FAIL;
    }
    else
    {
        addr = ping_args.ip->sval[0];
        if(!inet_aton(ping_args.ip->sval[0], &device)){
            return ESP_FAIL;
        }
    }

    if ((ping_args.count->count != 1) || (ping_args.count->ival[0] <= 0))
    {
        return ESP_FAIL;
    }
    else
    {
        ping_count = ping_args.count->ival[0];
    }

    if ((ping_args.timeout->count != 1) || (ping_args.timeout->ival[0] <= 0))
    {
        return ESP_FAIL;
    }
    else
    {
        ping_timeout = ping_args.timeout->ival[0];
    }

    if ((ping_args.timeout->count != 1) || (ping_args.delay->ival[0] <= 0))
    {
        return ESP_FAIL;
    }
    else
    {
        ping_delay = ping_args.delay->ival[0];
    }

    while(total_counts <= ping_count)
    {
        if(!waiting_results)
        {
            esp_ping_set_target(PING_TARGET_IP_ADDRESS_COUNT, &ping_count, sizeof(uint32_t));
            esp_ping_set_target(PING_TARGET_RCV_TIMEO, &ping_timeout, sizeof(uint32_t));
            esp_ping_set_target(PING_TARGET_DELAY_TIME, &ping_delay, sizeof(uint32_t));
            esp_ping_set_target(PING_TARGET_IP_ADDRESS, &device.addr, sizeof(uint32_t));
            esp_ping_set_target(PING_TARGET_RES_FN, &pingResults, sizeof(pingResults));
            ping_init();
            waiting_results = 1;
        }
    }
    ping_deinit();
    return ESP_OK;
}

void register_ping()
{
    ping_args.count = arg_int1("c", NULL, "10", "<int> specify ping count per analysis");
    ping_args.timeout = arg_int1("t", NULL, "1", "<int> specify timeout in seconds as an integer");
    ping_args.delay = arg_int1("d", NULL, "1", "<int> specify delay in seconds as an integer");
    ping_args.ip = arg_str1("n", NULL, "8.8.8.8", "<int> specify ipV4 ip address in dot notation");
    ping_args.end = arg_end(1);

    const esp_console_cmd_t ping_cmd =
    {
        .command = "ping",
        .help = "Run ICMP ping test",
        .hint = NULL,
        .func = &ping_test,
        .argtable = &ping_args
    };

    ESP_ERROR_CHECK( esp_console_cmd_register(&ping_cmd) );
}


