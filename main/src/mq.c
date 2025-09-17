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
    ESP_LOGI(TAG, "MqttEventHandler: event_base %s, event_id %ld", event_base, event_id);

    esp_mqtt_event_id_t mqtt_event_id  = (esp_mqtt_event_id_t) event_id;

    switch (mqtt_event_id)
    {
    case MQTT_EVENT_CONNECTED:
        // Subscribe to topics after connection
        esp_mqtt_client_handle_t client = (esp_mqtt_client_handle_t)event_handler_arg;
        esp_mqtt_client_subscribe(client, "/led/control", 0);
        ESP_LOGI(TAG, "Subscribed to /led/control topic");
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
        if (event->topic_len == strlen("/led/control") && 
            memcmp(event->topic, "/led/control", event->topic_len) == 0) {
            
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

void mq_con()
{
    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtt://1.15.13.162",
    };
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_common_event_handler, client);
    esp_mqtt_client_start(client);
}