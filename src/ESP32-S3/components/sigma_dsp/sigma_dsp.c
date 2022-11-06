#include <stdio.h>
#include <math.h>
#include <string.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "sigma_dsp.h"
#include "SigmaDSP_parameters.h"
#include "parameters.h"
#include "ir_nec_decoder.h"
#include "udp_server.h"
#include "nextion_display.h"
#include "data_store.h"

static const char *TAG = "SigmaDSP";
// static const int16_t MIN_dB = -80;
// int16_t Volume = MIN_dB;
// sigma_dsp_data_t sigma_dsp_data;
int16_t Mute = MIN_dB - 1;
// int16_t VolumePrev;
static char str1[20] = "volume=";
static char str2[20];
float FS; // Sample rate in [Hz] The audio sample frequency in [Hz]

secondOrderEQ_t eqBand1;
secondOrderEQ_t eqBand2;
secondOrderEQ_t eqBand3;
secondOrderEQ_t eqBand4;
secondOrderEQ_t eqBand5;
secondOrderEQ_t eqBand6;
secondOrderEQ_t eqBand7;
secondOrderEQ_t eqBand8;
secondOrderEQ_t eqBand9;
secondOrderEQ_t eqBand10;

void floatToFixed(float value, uint8_t *buffer)
{
    // Convert float to 4 byte hex
    int32_t fixedval = (value * ((int32_t)1 << 23));

    // Store the 4 bytes in the passed buffer
    buffer[0] = 0x00; // First must be empty
    buffer[1] = (fixedval >> 24) & 0xFF;
    buffer[2] = (fixedval >> 16) & 0xFF;
    buffer[3] = (fixedval >> 8) & 0xFF;
    buffer[4] = fixedval & 0xFF;
}

void intToFixed(int32_t value, uint8_t *buffer)
{
    // Store the 4 bytes in the passed buffer
    buffer[0] = 0x00; // First must be empty
    buffer[1] = (value >> 24) & 0xFF;
    buffer[2] = (value >> 16) & 0xFF;
    buffer[3] = (value >> 8) & 0xFF;
    buffer[4] = value & 0xFF;
}

esp_err_t write_register(uint8_t i2c_chip_adr, uint16_t control_register_address, uint8_t *data, uint8_t length)
{
    uint8_t MSByte = control_register_address >> 8;
    uint8_t LSByte = (uint8_t)control_register_address & 0xFF;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);

    i2c_master_write_byte(cmd, i2c_chip_adr | I2C_MASTER_WRITE, ACK_CHECK_EN);

    i2c_master_write_byte(cmd, MSByte, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, LSByte, ACK_CHECK_EN);

    //  printf("I2C Write: %#0X\t%0#X", MSByte, LSByte);

    for (uint8_t i = 0; i < length; i++)
    {
        i2c_master_write_byte(cmd, data[i], ACK_CHECK_EN);
        // printf("\t%0#X", data[i]);
    }

    // printf("\n");

    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(CONFIG_I2C_NUM, cmd, 50 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return ret;
}

void sigma_dsp_reset(void)
{
    gpio_set_level(CONFIG_SIGMA_DSP_RESET_PIN, 0);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    gpio_set_level(CONFIG_SIGMA_DSP_RESET_PIN, 1);

    ESP_LOGI(TAG, "DSP hardwaare reset performed");
}

esp_err_t sigma_dsp_ping()
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (CONFIG_SIGMA_DSP_I2C_ADDRESS) | I2C_MASTER_WRITE, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(CONFIG_I2C_NUM, cmd, 50 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return ret;
}

esp_err_t sigma_dsp_reset_pin_init(void)
{
    gpio_config_t reset_pin_conf = {
        .pin_bit_mask = (1ULL << CONFIG_SIGMA_DSP_RESET_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    return gpio_config(&reset_pin_conf);
}

void volume_slew(uint16_t startMemoryAddress, float dB, uint8_t slew)
{
    uint8_t dataArray[5];

    float volume = powf(10, dB / 20);                // 10^(dB / 20)
    int32_t slewrate = 0x400000 / (1 << (slew - 1)); // 0x400000/2^(slew - 1))

    dataArray[0] = startMemoryAddress >> 8;
    dataArray[1] = (uint8_t)startMemoryAddress & 0xFF;
    write_register(CONFIG_SIGMA_DSP_I2C_ADDRESS, SafeloadAddress0, dataArray, 2);

    floatToFixed(volume, dataArray);
    write_register(CONFIG_SIGMA_DSP_I2C_ADDRESS, SafeloadData0, dataArray, 5);

    startMemoryAddress++;
    dataArray[0] = startMemoryAddress >> 8;
    dataArray[1] = (uint8_t)startMemoryAddress & 0xFF;
    write_register(CONFIG_SIGMA_DSP_I2C_ADDRESS, SafeloadAddress1, dataArray, 2);

    intToFixed(slewrate, dataArray);
    write_register(CONFIG_SIGMA_DSP_I2C_ADDRESS, SafeloadData1, dataArray, 5);

    dataArray[0] = 0x00;
    dataArray[1] = 0x3C;
    write_register(CONFIG_SIGMA_DSP_I2C_ADDRESS, CoreRegister, dataArray, 2);
}

/**
 * @brief Controls a second order EQ block
 *
 * @param startMemoryAddress DSP memory address
 * @param equalizer Equalizer parameter struct
 */
void EQsecondOrder(uint16_t startMemoryAddress, secondOrderEQ_t equalizer)
{
    float A, w0, alpha, gainLinear;
    float b0, b1, b2, a0, a1, a2;
    float coefficients[5];

    A = pow(10, (equalizer.boost / 40));         // 10^(boost/40)
    w0 = 2 * pi * equalizer.freq / FS;           // 2*PI*freq/FS
    gainLinear = pow(10, (equalizer.gain / 20)); // 10^(gain/20)

    switch (equalizer.filterType)
    {
        // Parametric
    case parametric:
        // Peaking
    case peaking:
    default:
        alpha = sin(w0) / (2 * equalizer.Q);
        a0 = 1 + alpha / A;
        a1 = -2 * cos(w0);
        a2 = 1 - alpha / A;
        b0 = (1 + alpha * A) * gainLinear;
        b1 = -(2 * cos(w0)) * gainLinear;
        b2 = (1 - alpha * A) * gainLinear;
        break;

        // Low shelf
    case lowShelf:
        alpha = sin(w0) / 2 * sqrt((A + 1 / A) * (1 / equalizer.S - 1) + 2);
        a0 = (A + 1) + (A - 1) * cos(w0) + 2 * sqrt(A) * alpha;
        a1 = -2 * ((A - 1) + (A + 1) * cos(w0));
        a2 = (A + 1) + (A - 1) * cos(w0) - 2 * sqrt(A) * alpha;
        b0 = A * ((A + 1) - (A - 1) * cos(w0) + 2 * sqrt(A) * alpha) * gainLinear;
        b1 = 2 * A * ((A - 1) - (A + 1) * cos(w0)) * gainLinear;
        b2 = A * ((A + 1) - (A - 1) * cos(w0) - 2 * sqrt(A) * alpha) * gainLinear;
        break;

        // High shelf
    case highShelf:
        alpha = sin(w0) / 2 * sqrt((A + 1 / A) * (1 / equalizer.S - 1) + 2);
        a0 = (A + 1) - (A - 1) * cos(w0) + 2 * sqrt(A) * alpha;
        a1 = 2 * ((A - 1) - (A + 1) * cos(w0));
        a2 = (A + 1) - (A - 1) * cos(w0) - 2 * sqrt(A) * alpha;
        b0 = A * ((A + 1) + (A - 1) * cos(w0) + 2 * sqrt(A) * alpha) * gainLinear;
        b1 = -2 * A * ((A - 1) + (A + 1) * cos(w0)) * gainLinear;
        b2 = A * ((A + 1) + (A - 1) * cos(w0) - 2 * sqrt(A) * alpha) * gainLinear;
        break;

        // Lowpass
    case lowpass:
        alpha = sin(w0) / (2 * equalizer.Q);
        a0 = 1 + alpha;
        a1 = -2 * cos(w0);
        a2 = 1 - alpha;
        b0 = (1 - cos(w0)) * (gainLinear / 2);
        b1 = 1 - cos(w0) * gainLinear;
        b2 = (1 - cos(w0)) * (gainLinear / 2);
        break;

        // Highpass
    case highpass:
        alpha = sin(w0) / (2 * equalizer.Q);
        a0 = 1 + alpha;
        a1 = -2 * cos(w0);
        a2 = 1 - alpha;
        b0 = (1 + cos(w0)) * (gainLinear / 2);
        b1 = -(1 + cos(w0)) * gainLinear;
        b2 = (1 + cos(w0)) * (gainLinear / 2);
        break;

        // Bandpass
    case bandpass:
        alpha = sin(w0) * sinh(log(2) / (2 * equalizer.bandwidth * w0 / sin(w0)));
        a0 = 1 + alpha;
        a1 = -2 * cos(w0);
        a2 = 1 - alpha;
        b0 = alpha * gainLinear;
        b1 = 0;
        b2 = -alpha * gainLinear;
        break;

        // Bandstop
    case bandstop:
        alpha = sin(w0) * sinh(log(2) / (2 * equalizer.bandwidth * w0 / sin(w0)));
        a0 = 1 + alpha;
        a1 = -2 * cos(w0);
        a2 = 1 - alpha;
        b0 = 1 * gainLinear;
        b1 = -2 * cos(w0) * gainLinear;
        b2 = 1 * gainLinear;
        break;

        // Butterworth lowpass
    case butterworthLowpass:
        alpha = sin(w0) / 2.0 * 1 / sqrt(2);
        a0 = 1 + alpha;
        a1 = -2 * cos(w0);
        a2 = 1 - alpha;
        b0 = (1 - cos(w0)) * gainLinear / 2;
        b1 = 1 - cos(w0) * gainLinear;
        b2 = (1 - cos(w0)) * gainLinear / 2;
        break;

        // Butterworth highpass
    case butterworthHighpass:
        alpha = sin(w0) / 2.0 * 1 / sqrt(2);
        a0 = 1 + alpha;
        a1 = -2 * cos(w0);
        a2 = 1 - alpha;
        b0 = (1 + cos(w0)) * gainLinear / 2;
        b1 = -(1 + cos(w0)) * gainLinear;
        b2 = (1 + cos(w0)) * gainLinear / 2;
        break;

        // Bessel lowpass
    case besselLowpass:
        alpha = sin(w0) / 2.0 * 1 / sqrt(3);
        a0 = 1 + alpha;
        a1 = -2 * cos(w0);
        a2 = 1 - alpha;
        b0 = (1 - cos(w0)) * gainLinear / 2;
        b1 = 1 - cos(w0) * gainLinear;
        b2 = (1 - cos(w0)) * gainLinear / 2;
        break;

        // Bessel highpass
    case besselHighpass:
        alpha = sin(w0) / 2.0 * 1 / sqrt(3);
        a0 = 1 + alpha;
        a1 = -2 * cos(w0);
        a2 = 1 - alpha;
        b0 = (1 + cos(w0)) * gainLinear / 2;
        b1 = -(1 + cos(w0)) * gainLinear;
        b2 = (1 + cos(w0)) * gainLinear / 2;
        break;
    }

    // For Sigma DSP implementation we need to normalize all the coefficients respect to a0
    // and inverting by sign a1 and a2
    if (a0 != 0.00 && equalizer.state == on)
    {
        if (equalizer.phase == nonInverted) // 0 deg
        {
            coefficients[0] = b0 / a0;
            coefficients[1] = b1 / a0;
            coefficients[2] = b2 / a0;
            coefficients[3] = -1 * a1 / a0;
            coefficients[4] = -1 * a2 / a0;
        }
        else // if(equalizer.phase == parameters::phase::inverted) // 180 deg
        {
            coefficients[0] = -1 * b0 / a0;
            coefficients[1] = -1 * b1 / a0;
            coefficients[2] = -1 * b2 / a0;
            coefficients[3] = -1 * a1 / a0; // This coefficient does not change sign!
            coefficients[4] = -1 * a2 / a0; // This coefficient does not change sign!
        }
    }
    else // if(equalizer.state == parameters::state::off)
    {
        coefficients[0] = 1.00;
        coefficients[1] = 0;
        coefficients[2] = 0;
        coefficients[3] = 0;
        coefficients[4] = 0;
    }

    //   safeload_write(startMemoryAddress, coefficients[0], coefficients[1], coefficients[2], coefficients[3], coefficients[4]);

    uint8_t dataArray[5];

    dataArray[0] = startMemoryAddress >> 8;
    dataArray[1] = (uint8_t)startMemoryAddress & 0xFF;
    write_register(CONFIG_SIGMA_DSP_I2C_ADDRESS, SafeloadAddress0, dataArray, 2);

    floatToFixed(coefficients[0], dataArray);
    // ESP_LOGI(TAG, "dataArray: 0x%x; 0x%x; 0x%x; 0x%x; 0x%x;", dataArray[0], dataArray[1], dataArray[2], dataArray[3], dataArray[4]);
    write_register(CONFIG_SIGMA_DSP_I2C_ADDRESS, SafeloadData0, dataArray, 5);

    startMemoryAddress++;
    dataArray[0] = startMemoryAddress >> 8;
    dataArray[1] = (uint8_t)startMemoryAddress & 0xFF;
    write_register(CONFIG_SIGMA_DSP_I2C_ADDRESS, SafeloadAddress1, dataArray, 2);

    floatToFixed(coefficients[1], dataArray);
    // ESP_LOGI(TAG, "dataArray: 0x%x; 0x%x; 0x%x; 0x%x; 0x%x;", dataArray[0], dataArray[1], dataArray[2], dataArray[3], dataArray[4]);
    write_register(CONFIG_SIGMA_DSP_I2C_ADDRESS, SafeloadData1, dataArray, 5);

    startMemoryAddress++;
    dataArray[0] = startMemoryAddress >> 8;
    dataArray[1] = (uint8_t)startMemoryAddress & 0xFF;
    write_register(CONFIG_SIGMA_DSP_I2C_ADDRESS, SafeloadAddress2, dataArray, 2);

    floatToFixed(coefficients[2], dataArray);
    // ESP_LOGI(TAG, "dataArray: 0x%x; 0x%x; 0x%x; 0x%x; 0x%x;", dataArray[0], dataArray[1], dataArray[2], dataArray[3], dataArray[4]);
    write_register(CONFIG_SIGMA_DSP_I2C_ADDRESS, SafeloadData2, dataArray, 5);

    startMemoryAddress++;
    dataArray[0] = startMemoryAddress >> 8;
    dataArray[1] = (uint8_t)startMemoryAddress & 0xFF;
    write_register(CONFIG_SIGMA_DSP_I2C_ADDRESS, SafeloadAddress3, dataArray, 2);

    floatToFixed(coefficients[3], dataArray);
    // ESP_LOGI(TAG, "dataArray: 0x%x; 0x%x; 0x%x; 0x%x; 0x%x;", dataArray[0], dataArray[1], dataArray[2], dataArray[3], dataArray[4]);
    write_register(CONFIG_SIGMA_DSP_I2C_ADDRESS, SafeloadData3, dataArray, 5);

    startMemoryAddress++;
    dataArray[0] = startMemoryAddress >> 8;
    dataArray[1] = (uint8_t)startMemoryAddress & 0xFF;
    write_register(CONFIG_SIGMA_DSP_I2C_ADDRESS, SafeloadAddress4, dataArray, 2);

    floatToFixed(coefficients[4], dataArray);
    // ESP_LOGI(TAG, "dataArray: 0x%x; 0x%x; 0x%x; 0x%x; 0x%x;", dataArray[0], dataArray[1], dataArray[2], dataArray[3], dataArray[4]);
    write_register(CONFIG_SIGMA_DSP_I2C_ADDRESS, SafeloadData4, dataArray, 5);

    dataArray[0] = 0x00;
    dataArray[1] = 0x3C;
    write_register(CONFIG_SIGMA_DSP_I2C_ADDRESS, CoreRegister, dataArray, 2);
}

static void ir_received_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    ir_code_t ir_code = *((ir_code_t *)event_data);

    if (ir_code.addr == CONFIG_IR_REMOTE_ADDRESS)
    {

        switch (ir_code.cmd)
        {
        case CONFIG_IR_REMOTE_VOLUME_UP:
            if (sigma_dsp_data.Volume < 0)
            {
                sigma_dsp_data.Volume++;
                volume_slew(MOD_MAINVOLUME_ALG0_TARGET_ADDR, sigma_dsp_data.Volume, 12);
                // ESP_LOGI(TAG, "Volume up %d", Volume);

                strcpy(str1, "Volume=");
                sprintf(str2, "%d", sigma_dsp_data.Volume);
                strcat(str1, str2);
                udp_send_data(str1, sizeof(str1));

                nextion_set_volume(sigma_dsp_data.Volume);
            }
            break;

        case CONFIG_IR_REMOTE_VOLUME_DOWN:
            if (sigma_dsp_data.Volume > MIN_dB)
            {
                sigma_dsp_data.Volume--;
                volume_slew(MOD_MAINVOLUME_ALG0_TARGET_ADDR, sigma_dsp_data.Volume, 12);
                // ESP_LOGI(TAG, "Volume DOWN %d", Volume);

                strcpy(str1, "Volume=");
                sprintf(str2, "%d", sigma_dsp_data.Volume);
                strcat(str1, str2);
                udp_send_data(str1, sizeof(str1));

                nextion_set_volume(sigma_dsp_data.Volume);
            }
            break;

        case CONFIG_IR_REMOTE_MUTE:
            if (!ir_code.repeat)
            {
                if (Mute == MIN_dB - 1)
                {
                    Mute = sigma_dsp_data.Volume;
                    sigma_dsp_data.Volume = MIN_dB;
                    volume_slew(MOD_MAINVOLUME_ALG0_TARGET_ADDR, sigma_dsp_data.Volume, 12);
                    // ESP_LOGI(TAG, "Mute %d", Volume);

                    // sprintf(str2, "%d", Mute);
                    // strcpy(str1, str2);
                    udp_send_data("1", 2);

                    nextion_set_volume(sigma_dsp_data.Volume);
                }
                else
                {
                    sigma_dsp_data.Volume = Mute;
                    Mute = MIN_dB - 1;
                    volume_slew(MOD_MAINVOLUME_ALG0_TARGET_ADDR, sigma_dsp_data.Volume, 12);
                    // ESP_LOGI(TAG, "Unmute %d", Volume);

                    udp_send_data("0", 2);

                    nextion_set_volume(sigma_dsp_data.Volume);
                }
            }
            break;

        default:
            ESP_LOGI(TAG, "IR code unsupported");
            break;
        }
    }
}

void udp_send_command(char *command, int value)
{
    char commandString[40] = "";
    char valueString[40] = "";

    strcpy(commandString, command);
    sprintf(valueString, "%d", value);
    strcat(commandString, valueString);
    udp_send_data(commandString, sizeof(commandString));
}

static void udp_received_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    apa_code_t apa_code = *((apa_code_t *)event_data);

    // ESP_LOGI(TAG, "UDP Received Key: %s / Value: %s", apa_code.key, apa_code.value);
    if (strcmp(apa_code.key, "clientstart") == 0)
    {
        udp_send_command("Volume=", sigma_dsp_data.Volume);
        udp_send_command("Equalizer_45Hz=", sigma_dsp_data.Equalizer_45Hz);
        udp_send_command("Equalizer_64Hz=", sigma_dsp_data.Equalizer_64Hz);
        udp_send_command("Equalizer_125Hz=", sigma_dsp_data.Equalizer_125Hz);
        udp_send_command("Equalizer_250Hz=", sigma_dsp_data.Equalizer_250Hz);
        udp_send_command("Equalizer_500Hz=", sigma_dsp_data.Equalizer_500Hz);
        udp_send_command("Equalizer_1KHz=", sigma_dsp_data.Equalizer_1KHz);
        udp_send_command("Equalizer_2KHz=", sigma_dsp_data.Equalizer_2KHz);
        udp_send_command("Equalizer_4KHz=", sigma_dsp_data.Equalizer_4KHz);
        udp_send_command("Equalizer_8KHz=", sigma_dsp_data.Equalizer_8KHz);
        udp_send_command("Equalizer_16KHz=", sigma_dsp_data.Equalizer_16KHz);
        // ESP_LOGI(TAG, "clientstart: %d", sigma_dsp_data.Volume);
    }

    if (strcmp(apa_code.key, "clientclose") == 0)
    {
        ESP_LOGI(TAG, "clientclose: %s", apa_code.value);
    }

    if (strcmp(apa_code.key, "volume") == 0)
    {
        sigma_dsp_data.Volume = atoi(apa_code.value);
        if (sigma_dsp_data.Volume_prev != sigma_dsp_data.Volume)
        {
            sigma_dsp_data.Volume_prev = sigma_dsp_data.Volume;
            volume_slew(MOD_MAINVOLUME_ALG0_TARGET_ADDR, sigma_dsp_data.Volume, 12);

            nextion_set_volume(sigma_dsp_data.Volume);
            // ESP_LOGI(TAG, "volume: %d", sigma_dsp_data.Volume);
        }
    }

    if (strcmp(apa_code.key, "Equalizer_45Hz") == 0)
    {
        sigma_dsp_data.Equalizer_45Hz = atoi(apa_code.value);
        if (sigma_dsp_data.Equalizer_45Hz_prev != sigma_dsp_data.Equalizer_45Hz)
        {
            sigma_dsp_data.Equalizer_45Hz_prev = sigma_dsp_data.Equalizer_45Hz;
            eqBand1.boost = sigma_dsp_data.Equalizer_45Hz;
            EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE0_B0_ADDR, eqBand1);
        }
    }

    if (strcmp(apa_code.key, "Equalizer_64Hz") == 0)
    {
        sigma_dsp_data.Equalizer_64Hz = atoi(apa_code.value);
        if (sigma_dsp_data.Equalizer_64Hz_prev != sigma_dsp_data.Equalizer_64Hz)
        {
            sigma_dsp_data.Equalizer_64Hz_prev = sigma_dsp_data.Equalizer_64Hz;
            eqBand2.boost = sigma_dsp_data.Equalizer_64Hz;
            EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE1_B0_ADDR, eqBand2);
        }
    }

    if (strcmp(apa_code.key, "Equalizer_125Hz") == 0)
    {
        sigma_dsp_data.Equalizer_125Hz = atoi(apa_code.value);
        if (sigma_dsp_data.Equalizer_125Hz_prev != sigma_dsp_data.Equalizer_125Hz)
        {
            sigma_dsp_data.Equalizer_125Hz_prev = sigma_dsp_data.Equalizer_125Hz;
            eqBand3.boost = sigma_dsp_data.Equalizer_125Hz;
            EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE2_B0_ADDR, eqBand3);
        }
    }

    if (strcmp(apa_code.key, "Equalizer_250Hz") == 0)
    {
        sigma_dsp_data.Equalizer_250Hz = atoi(apa_code.value);
        if (sigma_dsp_data.Equalizer_250Hz_prev != sigma_dsp_data.Equalizer_250Hz)
        {
            sigma_dsp_data.Equalizer_250Hz_prev = sigma_dsp_data.Equalizer_250Hz;
            eqBand4.boost = sigma_dsp_data.Equalizer_250Hz;
            EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE3_B0_ADDR, eqBand4);
        }
    }

    if (strcmp(apa_code.key, "Equalizer_500Hz") == 0)
    {
        sigma_dsp_data.Equalizer_500Hz = atoi(apa_code.value);
        if (sigma_dsp_data.Equalizer_500Hz_prev != sigma_dsp_data.Equalizer_500Hz)
        {
            sigma_dsp_data.Equalizer_500Hz_prev = sigma_dsp_data.Equalizer_500Hz;
            eqBand5.boost = sigma_dsp_data.Equalizer_500Hz;
            EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE4_B0_ADDR, eqBand5);
        }
    }

    if (strcmp(apa_code.key, "Equalizer_1KHz") == 0)
    {
        sigma_dsp_data.Equalizer_1KHz = atoi(apa_code.value);
        if (sigma_dsp_data.Equalizer_1KHz_prev != sigma_dsp_data.Equalizer_1KHz)
        {
            sigma_dsp_data.Equalizer_1KHz_prev = sigma_dsp_data.Equalizer_1KHz;
            eqBand6.boost = sigma_dsp_data.Equalizer_1KHz;
            EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE5_B0_ADDR, eqBand6);
        }
    }

    if (strcmp(apa_code.key, "Equalizer_2KHz") == 0)
    {
        sigma_dsp_data.Equalizer_2KHz = atoi(apa_code.value);
        if (sigma_dsp_data.Equalizer_2KHz_prev != sigma_dsp_data.Equalizer_2KHz)
        {
            sigma_dsp_data.Equalizer_2KHz_prev = sigma_dsp_data.Equalizer_2KHz;
            eqBand7.boost = sigma_dsp_data.Equalizer_2KHz;
            EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE6_B0_ADDR, eqBand7);
        }
    }

    if (strcmp(apa_code.key, "Equalizer_4KHz") == 0)
    {
        sigma_dsp_data.Equalizer_4KHz = atoi(apa_code.value);
        if (sigma_dsp_data.Equalizer_4KHz_prev != sigma_dsp_data.Equalizer_4KHz)
        {
            sigma_dsp_data.Equalizer_4KHz_prev = sigma_dsp_data.Equalizer_4KHz;
            eqBand8.boost = sigma_dsp_data.Equalizer_4KHz;
            EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE7_B0_ADDR, eqBand8);
        }
    }

    if (strcmp(apa_code.key, "Equalizer_8KHz") == 0)
    {
        sigma_dsp_data.Equalizer_8KHz = atoi(apa_code.value);
        if (sigma_dsp_data.Equalizer_8KHz_prev != sigma_dsp_data.Equalizer_8KHz)
        {
            sigma_dsp_data.Equalizer_8KHz_prev = sigma_dsp_data.Equalizer_8KHz;
            eqBand9.boost = sigma_dsp_data.Equalizer_8KHz;
            EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE8_B0_ADDR, eqBand9);
        }
    }

    if (strcmp(apa_code.key, "Equalizer_16KHz") == 0)
    {
        sigma_dsp_data.Equalizer_16KHz = atoi(apa_code.value);
        if (sigma_dsp_data.Equalizer_16KHz_prev != sigma_dsp_data.Equalizer_16KHz)
        {
            sigma_dsp_data.Equalizer_16KHz_prev = sigma_dsp_data.Equalizer_16KHz;
            eqBand10.boost = sigma_dsp_data.Equalizer_16KHz;
            EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE9_B0_ADDR, eqBand10);
        }
    }
}

void sigma_dsp_initialize(void)
{
    esp_err_t ret;

    sigma_dsp_data.Volume = -80;
    sigma_dsp_data.Volume_prev = 0;

    ret = sigma_dsp_reset_pin_init();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Reset pin initialization failed");
        return;
    }

    gpio_set_level(CONFIG_SIGMA_DSP_RESET_PIN, 1);
    ESP_LOGI(TAG, "Reset pin initialized");

    sigma_dsp_reset();

    FS = 48000.00f;

    // Initilize EQ band 1
    eqBand1.filterType = lowShelf;
    eqBand1.S = 1;
    eqBand1.Q = 0.7071;
    eqBand1.freq = 32;
    eqBand1.boost = 0;
    eqBand1.state = on;
    EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE0_B0_ADDR, eqBand1);

    // Initilize EQ band 2
    eqBand2.filterType = peaking;
    eqBand2.Q = 1.41;
    eqBand2.freq = 64;
    eqBand2.boost = 0;
    eqBand2.state = on;
    EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE1_B0_ADDR, eqBand2);

    // Initilize EQ band 3
    eqBand3.filterType = peaking;
    eqBand3.Q = 1.41;
    eqBand3.freq = 125;
    eqBand3.boost = 0;
    eqBand3.state = on;
    EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE2_B0_ADDR, eqBand3);

    // Initilize EQ band 4
    eqBand4.filterType = peaking;
    eqBand4.Q = 1.41;
    eqBand4.freq = 250;
    eqBand4.boost = 0;
    eqBand4.state = on;
    EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE3_B0_ADDR, eqBand4);

    // Initilize EQ band 5
    eqBand5.filterType = peaking;
    eqBand5.Q = 1.41;
    eqBand5.freq = 500;
    eqBand5.boost = 0;
    eqBand5.state = on;
    EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE4_B0_ADDR, eqBand5);

    // Initilize EQ band 6
    eqBand6.filterType = peaking;
    eqBand6.Q = 1.41;
    eqBand6.freq = 1000;
    eqBand6.boost = 0;
    eqBand6.state = on;
    EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE5_B0_ADDR, eqBand6);

    // Initilize EQ band 7
    eqBand7.filterType = peaking;
    eqBand7.Q = 1.41;
    eqBand7.freq = 2000;
    eqBand7.boost = 0;
    eqBand7.state = on;
    EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE6_B0_ADDR, eqBand7);

    // Initilize EQ band 8
    eqBand8.filterType = peaking;
    eqBand8.Q = 1.41;
    eqBand8.freq = 4000;
    eqBand8.boost = 0;
    eqBand8.state = on;
    EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE7_B0_ADDR, eqBand8);

    // Initilize EQ band 9
    eqBand9.filterType = peaking;
    eqBand9.Q = 1.41;
    eqBand9.freq = 8000;
    eqBand9.boost = 0;
    eqBand9.state = on;
    EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE8_B0_ADDR, eqBand9);

    // Initilize EQ band 10
    eqBand10.filterType = highShelf;
    eqBand10.S = 1;
    eqBand10.Q = 0.7071;
    eqBand10.freq = 15000;
    eqBand10.boost = 0;
    eqBand10.state = on;
    EQsecondOrder(MOD_MIDEQ1_ALG0_STAGE9_B0_ADDR, eqBand10);

    ESP_ERROR_CHECK(esp_event_handler_instance_register(IR_EVENTS, IR_EVENT_RECEIVED, ir_received_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(UDP_EVENTS, UDP_EVENT_COMMAND, udp_received_handler, NULL, NULL));
}
