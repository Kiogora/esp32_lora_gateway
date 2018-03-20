
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "lwip/inet.h"
#include "lwip/ip4_addr.h"
#include "lwip/dns.h"
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
static uint32_t ping_count;  //how many pings per report
static uint32_t ping_timeout; //mS till we consider it timed out
static uint32_t ping_delay; //mS between pings
static uint32_t waiting_results;
static int total_counts;

static char* addr = NULL;

void register_ping();

esp_err_t pingResults(ping_target_id_t msgType, esp_ping_found * pf){
   printf("AvgTime:%.1fmS Sent:%d Rec:%d Err:%d min(mS):%d max(mS):%d ", (float)pf->total_time/pf->recv_count, pf->send_count, pf->recv_count, pf->err_count, pf->min_time, pf->max_time );
   printf("Resp(mS):%d Timeouts:%d Total Time:%d\n",pf->resp_time, pf->timeout_count, pf->total_time);
   waiting_results = 0;
   total_counts +=1;
   return ESP_OK;
}

int ping_test(int argc, char **argv)
{
    ping_count = 0;  //how many pings per report
    ping_timeout = 0; //mS till we consider it timed out
    ping_delay = 0; //mS between pings
    waiting_results = 0;
    total_counts = 0;

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

    while(total_counts<ping_count)
    {
        vTaskDelay(200 / portTICK_PERIOD_MS);
        if(!waiting_results)
        {
            esp_ping_set_target(PING_TARGET_IP_ADDRESS_COUNT, &ping_count, sizeof(uint32_t));
            esp_ping_set_target(PING_TARGET_RCV_TIMEO, &ping_timeout, sizeof(uint32_t));
            esp_ping_set_target(PING_TARGET_DELAY_TIME, &ping_delay, sizeof(uint32_t));
            esp_ping_set_target(PING_TARGET_IP_ADDRESS, &device.addr, sizeof(uint32_t));
            esp_ping_set_target(PING_TARGET_RES_FN, &pingResults, sizeof(pingResults));
            ESP_LOGI(TAG, "Pinging IP address %s",addr);
            ping_init();
            waiting_results = 1;
        }
    }
    ping_deinit();
    return ESP_OK;
}

void register_ping()
{
    ping_args.count = arg_int1("c", NULL, "<C>", "<int> specify ping count per analysis");
    ping_args.timeout = arg_int1("t", NULL, "<T>", "<int> specify timeout in seconds as an integer");
    ping_args.delay = arg_int1("d", NULL, "<D>", "<int> specify delay in seconds as an integer");
    ping_args.ip = arg_str1("n", NULL, "x.x.x.x", "<int> specify ipV4 ip address in dot notation");
    ping_args.end = arg_end(1);

    const esp_console_cmd_t ping_cmd=
    {
        .command = "ping",
        .help = "Run ICMP ping test",
        .hint = NULL,
        .func = &ping_test,
        .argtable = &ping_args
    };

    ESP_ERROR_CHECK( esp_console_cmd_register(&ping_cmd) );
}


