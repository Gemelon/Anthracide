#pragma once

#include "esp_err.h"

/**
 * @brief ADAU1701 hardware register constants
 *
 */
typedef enum
{
  InterfaceRegister0 = 0x0800,
  InterfaceRegister1 = 0x0801,
  InterfaceRegister2 = 0x0802,
  InterfaceRegister3 = 0x0803,
  InterfaceRegister4 = 0x0804,
  InterfaceRegister5 = 0x0805,
  InterfaceRegister6 = 0x0806,
  InterfaceRegister7 = 0x0807,
  GpioAllRegister = 0x0808,
  Adc0 = 0x0809,
  Adc1 = 0x080A,
  Adc2 = 0x080B,
  Adc3 = 0x080C,
  SafeloadData0 = 0x0810,
  SafeloadData1 = 0x0811,
  SafeloadData2 = 0x0812,
  SafeloadData3 = 0x0813,
  SafeloadData4 = 0x0814,
  SafeloadAddress0 = 0x0815,
  SafeloadAddress1 = 0x0816,
  SafeloadAddress2 = 0x0817,
  SafeloadAddress3 = 0x0818,
  SafeloadAddress4 = 0x0819,
  DataCapture0 = 0x081A,
  DataCpature1 = 0x081B,
  CoreRegister = 0x081C,
  RAMRegister = 0x081D,
  SerialOutRegister1 = 0x081E,
  SerialInputRegister = 0x081F,
  MpCfg0 = 0x0820,
  MpCfg1 = 0x0821,
  AnalogPowerDownRegister = 0x0822,
  AnalogInterfaceRegister0 = 0x0824
} dspRegister;

#define ACK_CHECK_EN 0x1  /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0 /*!< I2C master will not check ack from slave */
#define MIN_dB -80

// typedef struct
// {
//   int16_t Volume;
//   int16_t Volume_prev;
//   int16_t Equalizer_45Hz;
//   int16_t Equalizer_45Hz_prev;
//   int16_t Equalizer_64Hz;
//   int16_t Equalizer_64Hz_prev;
//   int16_t Equalizer_125Hz;
//   int16_t Equalizer_125Hz_prev;
//   int16_t Equalizer_250Hz;
//   int16_t Equalizer_250Hz_prev;
//   int16_t Equalizer_500Hz;
//   int16_t Equalizer_500Hz_prev;
//   int16_t Equalizer_1KHz;
//   int16_t Equalizer_1KHz_prev;
//   int16_t Equalizer_2KHz;
//   int16_t Equalizer_2KHz_prev;
//   int16_t Equalizer_4KHz;
//   int16_t Equalizer_4KHz_prev;
//   int16_t Equalizer_8KHz;
//   int16_t Equalizer_8KHz_prev;
//   int16_t Equalizer_16KHz;
//   int16_t Equalizer_16KHz_prev;
// } sigma_dsp_data_t;

esp_err_t sigma_dsp_ping();
void sigma_dsp_initialize(void);
void volume_slew(uint16_t startMemoryAddress, float dB, uint8_t slew);
