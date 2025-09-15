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
        // connected
        led_turn_on();
        break;    
    case MQTT_EVENT_DISCONNECTED:
        // disconnected
        break;
    case MQTT_EVENT_SUBSCRIBED:
        
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