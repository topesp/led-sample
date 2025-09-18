#include "mq.h"
#include "led.h"

#include "esp_log.h"
#include "mqtt_client.h"

#define TAG "mq_c"

static void mqtt_common_event_handler(void *event_handler_arg,
                               esp_event_base_t event_base,
                               int32_t event_id,
                               void *event_data)
{

    char *led_control_topic = "device/led/control";
    ESP_LOGI(TAG, "MqttEventHandler: event_base %s, event_id %ld", event_base, event_id);

    esp_mqtt_event_id_t mqtt_event_id  = (esp_mqtt_event_id_t) event_id;

    switch (mqtt_event_id)
    {
    case MQTT_EVENT_CONNECTED:
        // Subscribe to topics after connection
        esp_mqtt_client_handle_t client = (esp_mqtt_client_handle_t)event_handler_arg;
        esp_mqtt_client_subscribe(client, led_control_topic, 0);
        ESP_LOGI(TAG, "Subscribed to %s topic", led_control_topic);
        break;    
    case MQTT_EVENT_DISCONNECTED:
        // disconnected
        break;
    case MQTT_EVENT_DATA:
        // Handle received data
        esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
        
        ESP_LOGI(TAG, "Received message: topic=%.*s data=%.*s", 
                 event->topic_len, event->topic,
                 event->data_len, event->data);
        
        // Handle LED control messages using direct comparison with length
        if (event->topic_len == strlen(led_control_topic) && 
            memcmp(event->topic, led_control_topic, event->topic_len) == 0) {
            
            if (event->data_len == strlen("on") && 
                memcmp(event->data, "on", event->data_len) == 0) {
                led_turn_on();
                ESP_LOGI(TAG, "LED turned ON");
            } 
            else if (event->data_len == strlen("off") && 
                     memcmp(event->data, "off", event->data_len) == 0) {
                led_turn_off();
                ESP_LOGI(TAG, "LED turned OFF");
            }
        }
        break;
    default:
        break;
    }
}

static esp_mqtt_client_handle_t client = NULL;

// Task to periodically publish LED status
static void led_status_task(void *pvParameters)
{
    while (1) {
        if (client) {
            // Get current LED status and publish
            const char *status = led_is_on() ? "on" : "off";
            esp_mqtt_client_publish(client, "device/led/status", status, strlen(status), 1, 0);
            ESP_LOGI(TAG, "Published LED status: %s", status);
        }
        vTaskDelay(pdMS_TO_TICKS(5000)); // Report every 5 seconds
    }
}

void mq_con()
{
    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtt://1.15.13.162",
    };
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_common_event_handler, client);
    esp_mqtt_client_start(client);

    // Create task for LED status reporting
    xTaskCreate(led_status_task, "led_status_task", 2048, NULL, 5, NULL);
}