#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_event.h"
#include "anthracide_led.h"
#include "anthracide_i2c.h"
#include "sigma_dsp.h"
#include "SigmaDSP_parameters.h"
#include "udp_server.h"
#include "ir_nec_decoder.h"
#include "nextion_display.h"
#include "data_store.h"

static const char *TAG = "ANTHRACIDE";

void initialize_component()
{
    ESP_LOGI(TAG, "Initializes components");
    data_store_initialize();
    led_intialize();
    i2c_initialize();
    sigma_dsp_initialize();
    nextion_display_initialize();
    udp_server_initialize();
}

void start_tasks()
{
    ESP_LOGI(TAG, "Starting tasks");
    data_store_start();
    ir_nec_decoder_start();
    nextion_display_start();
    udp_server_start();
}

void app_main(void)
{
    // Create the default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    initialize_component();
    volume_slew(MOD_MAINVOLUME_ALG0_TARGET_ADDR, MIN_dB, 12);
    start_tasks();
    // nextion_display_send_command("get t0.txt");
}