#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/i2c.h"
#include "esp_log.h"
#include "esp_err.h"

#include "Adafruit_BME280.h"
#include "Adafruit_SSD1306.h"

#include "zigbee_ctrl.h"

static const char* TAG = "TAU_LUEFT";

#define SDA_GPIO      10
#define SCL_GPIO      11
#define I2C_FREQ_HZ   400000

#define OLED_I2C_ADDR 0x3C
#define BME_IN_ADDR   0x76
#define BME_OUT_ADDR  0x77

#define T_MIN         10.0f
#define DEW_MIN       5.0f
#define DELTA_MIN     2.0f
#define INTERVAL_MS   5000

static Adafruit_BME280 bmeIn, bmeOut;
static Adafruit_SSD1306 oled(64, 32, &Wire);

static uint16_t outlet_short_addr = 0; // wird beim Pairing gesetzt

// Taupunkt nach Magnus-Formel
static float calc_dewpoint(float T, float RH) {
    const float b = 17.62f, c = 243.12f;
    float γ = (b * T / (c + T)) + logf(RH / 100.0f);
    return (c * γ) / (b - γ);
}

// Initialisiere I2C
static esp_err_t i2c_master_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA_GPIO,
        .scl_io_num = SCL_GPIO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREQ_HZ
    };
    return i2c_param_config(I2C_NUM_0, &conf) ||
           i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
}

// Anzeige updaten
static void update_display(float Tin, float Hin, float DewIn,
                            float Tout, float Hout, float DewOut, bool on) {
    oled.clearDisplay();
    oled.setCursor(0,0);
    oled.printf("In T:%.1fC RH:%.0f%%\n", Tin, Hin);
    oled.printf("In Dew:%.1fC\n", DewIn);
    oled.printf("Au T:%.1fC RH:%.0f%%\n", Tout, Hout);
    oled.printf("Au Dew:%.1fC\n", DewOut);
    oled.printf("Lueftung: %s", on ? "ON" : "OFF");
    oled.display();
}

// finale Anwendung
void app_main(void) {
    ESP_LOGI(TAG, "Init I2C...");
    ESP_ERROR_CHECK(i2c_master_init());

    ESP_LOGI(TAG, "Init BME280...");
    if (!bmeIn.begin(BME_IN_ADDR)) ESP_LOGE(TAG, "BME IN init failed");
    if (!bmeOut.begin(BME_OUT_ADDR)) ESP_LOGE(TAG, "BME OUT init failed");

    ESP_LOGI(TAG, "Init OLED...");
    if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) ESP_LOGE(TAG, "OLED init failed");

    zigbee_ctrl_init();
    zigbee_ctrl_open_network(180);
    ESP_LOGI(TAG, "Warten auf_PAIRING...");

    vTaskDelay(pdMS_TO_TICKS(5000));
    // outlet_short_addr muss in callback gesetzt werden!

    while (true) {
        float Tin = bmeIn.readTemperature();
        float Hin = bmeIn.readHumidity();
        float Tout = bmeOut.readTemperature();
        float Hout = bmeOut.readHumidity();

        float DewIn = calc_dewpoint(Tin, Hin);
        float DewOut = calc_dewpoint(Tout, Hout);

        bool on = (Tin > T_MIN && DewIn > DEW_MIN && (DewIn - DewOut) > DELTA_MIN);

        if (outlet_short_addr != 0) {
            zigbee_ctrl_send_onoff_cmd(outlet_short_addr, on);
        }
        update_display(Tin, Hin, DewIn, Tout, Hout, DewOut, on);

        ESP_LOGI(TAG, "T_in=%.1f RH=%.1f Dew=%.1f T_out=%.1f RH=%.1f Dew=%.1f Lueftung=%s",
                 Tin, Hin, DewIn, Tout, Hout, DewOut, on ? "ON" : "OFF");

        vTaskDelay(pdMS_TO_TICKS(INTERVAL_MS));
    }
}
