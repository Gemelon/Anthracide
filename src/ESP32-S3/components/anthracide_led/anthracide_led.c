#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "anthracide_led.h"

static const char *TAG = "LED";

static uint8_t s_led_state = 0;
static led_strip_t *pStrip_a;
esp_timer_handle_t TIMER;

// Callback that will be executed when the timer period lapses. Posts the timer expiry event
// to the default event loop.
static void timer_callback(void *arg)
{
    if (s_led_state)
    {
        /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
        pStrip_a->set_pixel(pStrip_a, 0, 0, 16, 0);
        /* Refresh the strip to send data */
        pStrip_a->refresh(pStrip_a, 100);
        s_led_state = false;
    }
    else
    {
        /* Set all LED off to clear all pixels */
        pStrip_a->clear(pStrip_a, 50);
        s_led_state = true;
    }
}

void led_intialize(void)
{
    ESP_LOGI(TAG, "Example configured to blink addressable LED!");
    /* LED strip initialization with the GPIO and pixels number*/
    pStrip_a = led_strip_init(CONFIG_LED_RMT_CHANNEL, CONFIG_LED_GPIO, 1);
    /* Set all LED off to clear all pixels */
    pStrip_a->clear(pStrip_a, 50);

    // Create and start the event sources
    esp_timer_create_args_t timer_args = {
        .callback = &timer_callback,
    };

    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &TIMER));
    ESP_ERROR_CHECK(esp_timer_start_periodic(TIMER, CONFIG_BLINK_PERIOD));
}
