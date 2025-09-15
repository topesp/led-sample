#include "wifi.h"

#include "freertos/FreeRTOS.h"
#include "sdkconfig.h"

#include "esp_wifi.h"
#include "esp_log.h"

#define TAG "wifi_module"

static SemaphoreHandle_t s_semaphore_wifi_got_ip = NULL;
static esp_netif_t* s_netif_sta = NULL;

void handler_on_got_ip(void* arg, esp_event_base_t event_base,
                       int32_t event_id, void* event_data)
{
    ESP_LOGI(TAG, "Got IP event");
    ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
    if (s_semaphore_wifi_got_ip != NULL) {
        xSemaphoreGive(s_semaphore_wifi_got_ip);
    }
    ESP_LOGI(TAG, "Device Ip :" IPSTR, IP2STR(&((event->ip_info).ip)));
}

void handler_on_wifi_disconnect(void* arg, esp_event_base_t event_base,
                            int32_t event_id, void* event_data)
{
    ESP_LOGI(TAG, "Wi-Fi Disconnected, trying to reconnect...");
    esp_wifi_connect();
}

void handler_on_wifi_connect(void* arg, esp_event_base_t event_base,
                            int32_t event_id, void* event_data)
{
    // Handle Wi-Fi connection event if needed
    ESP_LOGI(TAG, "Wi-Fi connected");
}   

void wifi_start(void){
    // Initialize Wi-Fi in station mode
    // This is a placeholder implementation. Replace with actual Wi-Fi initialization code.
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_netif_inherent_config_t esp_netif_config = ESP_NETIF_INHERENT_DEFAULT_WIFI_STA();
    // Warning: the interface desc is used in tests to capture actual connection details (IP, gw, mask)
    esp_netif_config.if_desc = "sta";
    esp_netif_config.route_prio = 128;
    s_netif_sta = esp_netif_create_wifi(WIFI_IF_STA, &esp_netif_config);
    esp_wifi_set_default_wifi_sta_handlers();

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
}


esp_err_t wifi_con(void)
{
    
    wifi_start();
    // Connect to a Wi-Fi network (replace with actual SSID and password)
    wifi_config_t sta_config = {
        .sta = {
            .ssid = CONFIG_ESP_WIFI_SSID,
            .password = CONFIG_ESP_WIFI_PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK
        }
    };
    esp_wifi_set_config(WIFI_IF_STA, &sta_config);

    // Register event handler for Wi-Fi events
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &handler_on_got_ip, NULL);
    esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &handler_on_wifi_disconnect, NULL);
    esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, &handler_on_wifi_connect, NULL);

    s_semaphore_wifi_got_ip = xSemaphoreCreateBinary();
    if (s_semaphore_wifi_got_ip == NULL) {
        ESP_LOGE(TAG, "Failed to create semaphore");
        return ESP_FAIL;
    }

    esp_wifi_connect();

    xSemaphoreTake(s_semaphore_wifi_got_ip, portMAX_DELAY);
    vSemaphoreDelete(s_semaphore_wifi_got_ip);
    s_semaphore_wifi_got_ip = NULL;

    return ESP_OK;
}