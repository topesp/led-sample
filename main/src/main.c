
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"

#include "led.h"
#include "wifi.h"

#define TAG "led_sample"

void app_main(void)
{
    // wifi used
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();

    // init led
    led_init();

    // wifi connection
    wifi_con();

    // connnect mqtt server
    

    while (1) {
        ESP_LOGI(TAG, "LED is %s", led_current_state());
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}