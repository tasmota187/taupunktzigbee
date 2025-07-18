# main/CMakeLists.txt
idf_component_register(SRCS "main.c"
                      INCLUDE_DIRS "include"
                      REQUIRES bme280 ssd1306 esp_zigbee)
