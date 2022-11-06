#include <stdio.h>
#include "esp_log.h"
#include "driver/rmt.h"
#include "ir_nec_decoder.h"
#include "ir_tools.h"

static const char *TAG = "IR DECODER";
ESP_EVENT_DEFINE_BASE(IR_EVENTS);

static void ir_nec_decoder_task(void *arg)
{
    uint32_t addr = 0;
    uint32_t cmd = 0;
    size_t length = 0;
    bool repeat = false;
    RingbufHandle_t rb = NULL;
    rmt_item32_t *items = NULL;
    ir_code_t ir_code;

    rmt_config_t rmt_rx_config = RMT_DEFAULT_CONFIG_RX(CONFIG_RMT_RX_GPIO, CONFIG_RMT_RX_CHANNEL);
    rmt_config(&rmt_rx_config);
    rmt_driver_install(CONFIG_RMT_RX_CHANNEL, 1000, 0);
    ir_parser_config_t ir_parser_config = IR_PARSER_DEFAULT_CONFIG((ir_dev_t)CONFIG_RMT_RX_CHANNEL);
    ir_parser_config.flags |= IR_TOOLS_FLAGS_PROTO_EXT; // Using extended IR protocols (both NEC and RC5 have extended version)
    ir_parser_t *ir_parser = NULL;
    ir_parser = ir_parser_rmt_new_nec(&ir_parser_config);

    // get RMT RX ringbuffer
    rmt_get_ringbuf_handle(CONFIG_RMT_RX_CHANNEL, &rb);
    assert(rb != NULL);
    // Start receive
    rmt_rx_start(CONFIG_RMT_RX_CHANNEL, true);
    while (1)
    {
        items = (rmt_item32_t *)xRingbufferReceive(rb, &length, portMAX_DELAY);
        if (items)
        {
            length /= 4; // one RMT = 4 Bytes
            if (ir_parser->input(ir_parser, items, length) == ESP_OK)
            {
                if (ir_parser->get_scan_code(ir_parser, &addr, &cmd, &repeat) == ESP_OK)
                {
                    ir_code.addr = addr;
                    ir_code.cmd = cmd;
                    ir_code.repeat = repeat;
                    ESP_ERROR_CHECK(esp_event_post(IR_EVENTS, IR_EVENT_RECEIVED, &ir_code, sizeof(ir_code), portMAX_DELAY));
                    // ESP_LOGI(TAG, "Scan Code %s --- addr: 0x%04x cmd: 0x%04x", repeat ? "(repeat)" : "", addr, cmd);
                }
            }
            // after parsing the data, return spaces to ringbuffer.
            vRingbufferReturnItem(rb, (void *)items);
        }
    }
    ir_parser->del(ir_parser);
    rmt_driver_uninstall(CONFIG_RMT_RX_CHANNEL);
    vTaskDelete(NULL);
}

void ir_nec_decoder_initialize(void)
{
}

void ir_nec_decoder_start(void)
{
    xTaskCreate(ir_nec_decoder_task, "ir_nec_decoder_task", 4096, NULL, 10, NULL);
}
