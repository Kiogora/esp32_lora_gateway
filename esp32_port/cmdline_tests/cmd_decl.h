/* Console example — declarations of command registration functions.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#pragma once

//Register util pkt logger test
void register_util_pkt_logger();

//Register LoRagw_cal functions
void register_loragw_cal();

//Register LoRagw_hal functions
void register_loragw_hal();

//Register spi stress functions
void register_spi_stress();

//Register LoRagw_lbt functions
void register_loragw_lbt();

//Register LoRagw_tx continuous functions
void register_tx_continuous();

//Register parson Json parser test
void register_parson_test();
