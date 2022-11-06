#pragma once

#include <stdio.h>
#include "esp_event.h"

ESP_EVENT_DECLARE_BASE(UDP_EVENTS);

enum
{
    UDP_EVENT_RECEIVED,
    UDP_EVENT_COMMAND
};

typedef struct
{
    char key[20];
    char value[20];
} apa_code_t; // Anthracide Power Amp App Code

void udp_server_start(void);
void udp_server_initialize(void);
void udp_send_data(char *tx_buffer, int len);
