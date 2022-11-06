#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_system.h"
#include "driver/uart.h"
#include "nextion_display.h"
#include "data_store.h"

static const char *TAG = "Nextion Display";

#define BUF_SIZE (1024)

void nextion_display_send_command(const char *cmd)
{
    strcat((char *)cmd, "\xff\xff\xff\0");
    uart_write_bytes(CONFIG_NEXTION_UART_PORT_NUM, cmd, strlen((char *)cmd));
    // ESP_LOGI(TAG, "Recv str: %s", (char *)cmd);
}

void nextion_debug_data(const char *data, int len)
{
    int data_pntr = 0;
    char *hex_string = (char *)malloc(BUF_SIZE);
    char *data_string = (char *)malloc(BUF_SIZE);
    data_string[0] = 0;

    strcat((char *)data_string, "Data length = ");
    sprintf(hex_string, "%d\n", len);
    strcat((char *)data_string, (char *)hex_string);

    for (int pntr = 0; pntr < len; pntr++)
    {

        sprintf(hex_string, "0x%02x", data[pntr]);
        strcat((char *)data_string, (char *)hex_string);

        if (pntr < len - 1)
        {
            strcat((char *)data_string, ", ");
        }

        data_pntr++;
        if (data_pntr > 7)
        {
            strcat((char *)data_string, "\n");
            data_pntr = 0;
        }
    }

    ESP_LOGI(TAG, "%s\n", data_string);
}

void nextion_set_volume(int16_t volume)
{
    char *command_string = (char *)malloc(BUF_SIZE);

    sprintf(command_string, "pg1n0Volume.val=%d", volume);
    nextion_display_send_command(command_string);

    sprintf(command_string, "pg2n0Volume.val=%d", volume);
    nextion_display_send_command(command_string);
}

void nextion_display_task(void)
{
    int received_data_len = -1;
    char *received_data = (char *)malloc(BUF_SIZE);

    while (1)
    {
        // Read data from the UART
        received_data_len = uart_read_bytes(CONFIG_NEXTION_UART_PORT_NUM, received_data, (BUF_SIZE - 1), 20 / portTICK_RATE_MS);

        if (received_data_len > 0)
        {
            received_data[received_data_len] = 0;
            // nextion_debug_data(received_data, received_data_len);
        }
    }
}

void nextion_display_start(void)
{
    xTaskCreate(nextion_display_task, "nextion_display_task", 2048, NULL, 10, NULL);
}

void nextion_display_initialize(void)
{
    ESP_LOGI(TAG, "Initialize");
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = CONFIG_NEXTION_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    int intr_alloc_flags = 0;

    ESP_ERROR_CHECK(uart_driver_install(CONFIG_NEXTION_UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(CONFIG_NEXTION_UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(CONFIG_NEXTION_UART_PORT_NUM, CONFIG_NEXTION_UART_TXD, CONFIG_NEXTION_UART_RXD, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    nextion_display_send_command("page 1");
    nextion_set_volume(sigma_dsp_data.Volume);
}
