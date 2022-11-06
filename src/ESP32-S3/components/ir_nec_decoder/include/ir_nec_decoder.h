#pragma once

#include <stdio.h>
#include "esp_event.h"

// Declare an event base
ESP_EVENT_DECLARE_BASE(IR_EVENTS);

enum
{
    IR_EVENT_RECEIVED,
    IR_EVENT_REPEAT
};

typedef struct
{
    uint32_t addr;
    uint32_t cmd;
    bool repeat;    
} ir_code_t;

void ir_nec_decoder_initialize(void);
void ir_nec_decoder_start(void);
