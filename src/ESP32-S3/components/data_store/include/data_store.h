#pragma once

typedef struct
{
  int16_t Volume;
  int16_t Volume_prev;
  int16_t Equalizer_45Hz;
  int16_t Equalizer_45Hz_prev;
  int16_t Equalizer_64Hz;
  int16_t Equalizer_64Hz_prev;
  int16_t Equalizer_125Hz;
  int16_t Equalizer_125Hz_prev;
  int16_t Equalizer_250Hz;
  int16_t Equalizer_250Hz_prev;
  int16_t Equalizer_500Hz;
  int16_t Equalizer_500Hz_prev;
  int16_t Equalizer_1KHz;
  int16_t Equalizer_1KHz_prev;
  int16_t Equalizer_2KHz;
  int16_t Equalizer_2KHz_prev;
  int16_t Equalizer_4KHz;
  int16_t Equalizer_4KHz_prev;
  int16_t Equalizer_8KHz;
  int16_t Equalizer_8KHz_prev;
  int16_t Equalizer_16KHz;
  int16_t Equalizer_16KHz_prev;
} sigma_dsp_data_t;

sigma_dsp_data_t sigma_dsp_data;

void data_store_start(void);
void data_store_initialize(void);
