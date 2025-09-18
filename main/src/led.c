#include "led.h"

#include "esp_log.h"

#include "iot_button.h"
#include "button_gpio.h"
#include "driver/gpio.h"

#define SWITCH_GPIO GPIO_NUM_21
#define LED_GPIO GPIO_NUM_22
#define TAG "led_c"

static bool led_state = false;

bool led_is_on() {
    return led_state;
}

void handle_switch_button_click(void *arg1, void *arg2) {
    led_state = !led_state;
    gpio_set_level(LED_GPIO, led_state);
    ESP_LOGI(TAG, "BUTTON_SINGLE_CLICK, LED state: %s", led_state ? "ON" : "OFF");
}

void led_init()
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    // Create button device
    button_config_t btn_cfg = {
        .long_press_time = 0,
        .short_press_time = 0,
    };
    button_gpio_config_t btn_gpio_cfg = {
        .gpio_num = SWITCH_GPIO,
        .active_level = 0
    };
    button_handle_t gpio_btn = NULL;
    iot_button_new_gpio_device(&btn_cfg, &btn_gpio_cfg, &gpio_btn);
    if (gpio_btn == NULL) {
        ESP_LOGE(TAG, "Failed to create button device");
        abort();
    }
    iot_button_register_cb(gpio_btn,BUTTON_SINGLE_CLICK, NULL, &handle_switch_button_click, NULL);
}

char *led_current_state()
{
    return led_state ? "on" : "off";
}

int led_turn_on()
{
    led_state = true;
    gpio_set_level(LED_GPIO, led_state);
    ESP_LOGI(TAG, "LED turned ON");
    return 0;
}

int led_turn_off()
{
    led_state = false;
    gpio_set_level(LED_GPIO, led_state);
    ESP_LOGI(TAG, "LED turned OFF");
    return 0;
}