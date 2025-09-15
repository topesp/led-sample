idf_component_register(SRCS "src/led.c" "src/main.c" "src/wifi.c" "src/mq.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES esp_wifi nvs_flash button mqtt
                    REQUIRES)