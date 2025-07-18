#pragma once
#include "esp_zigbee_core.h"

#define ZB_COORD_EP          1
#define ZB_OUTLET_ENDPOINT   1
#define ZB_CLUSTER_ID        ESP_ZB_ZCL_CLUSTER_ID_ON_OFF

void zb_init(void);
void zb_open_network(uint32_t duration_s);
void zb_send_onoff_cmd(uint16_t addr, bool on);
