#include "zigbee_ctrl.h"
#include "esp_zigbee_core.h"
#include "esp_zigbee_light.h"
#include "esp_log.h"

static const char* ZB_TAG = "ZB_CTRL";

void zb_init(void) {
    esp_zb_cfg_t cfg = ESP_ZB_ZC_DEVICE_CONFIG();
    esp_zb_init(&cfg);
    esp_zb_device_register_onoff_cluster(ZB_COORD_EP, true);
    esp_zb_start();
}

void zb_open_network(uint32_t duration_s) {
    esp_zb_apsme_permit_join_req_t permit = {
        .permit_duration = duration_s,
    };
    esp_zb_permit_join(&permit);
}

void zb_send_onoff_cmd(uint16_t addr, bool on) {
    uint8_t cmd_id = on ? ESP_ZB_ZCL_CMD_ON_OFF_ON_ID : ESP_ZB_ZCL_CMD_ON_OFF_OFF_ID;
    esp_zb_zcl_on_off_cmd_req_t req = {
        .dst_addr_u.addr_short = addr,
        .dst_endpoint = ZB_OUTLET_ENDPOINT,
        .src_endpoint = ZB_COORD_EP,
        .cmd_id = cmd_id,
    };
    esp_zb_zcl_on_off_cmd_req(&req);
    ESP_LOGI(ZB_TAG, "Sent %s command to 0x%04X", on ? "ON" : "OFF", addr);
}

// TODO: Event Callback, um `outlet_short_addr` beim Join zu setzen
