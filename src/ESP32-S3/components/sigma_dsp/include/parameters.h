#ifndef PARAMETERS_H
#define PARAMETERS_H

// Math constants
#define pi 3.1415926f

enum filterType
{
  peaking,
  parametric,
  lowShelf,
  highShelf,
  lowpass,
  highpass,
  bandpass,
  bandstop,
  butterworthLowpass,
  butterworthHighpass,
  besselLowpass,
  besselHighpass,
};

enum phase
{
  deg_0 = 0,
  nonInverted = 0,
  deg_180 = 1,
  inverted = 1,
};

enum state
{
  off,
  on,
};

// Compressor typedef
#define COMPRESSORWITHPOSTGAIN
typedef struct
{
  float threshold; // -90 / +6 [dB]
  float ratio;     // 1 - 100
  float attack;    // 1 - 500 [ms]
  float hold;      // 0 - 500 [ms]
  float decayMs;   // 0 - 2000 [ms]
  float postgain;  // -30 / +24 [dB]
} compressor_t;

// Tone control typedef
typedef struct
{
  float boost_Bass_dB;   // [dB]
  float boost_Treble_dB; // [dB]
  float freq_Bass;       // [Hz]
  float freq_Treble;     // [Hz]
  uint8_t phase;         // parameters::phase::deg_0/deg_180
  uint8_t state;         // parameters::state::on/off
} toneCtrl_t;

// 1st order equalizer typedef
typedef struct
{
  float freq;         // Range 20-20000 [Hz]
  float gain;         // Range +/-15 [dB]
  uint8_t filterType; // parameters::filterType::[type]
  uint8_t phase;      // parameters::phase::deg_0/deg_180
  uint8_t state;      // parameters::state::on/off
} firstOrderEQ_t;

// 2nd order equalizer typedef
typedef struct
{
  float Q;            // Parametric, Peaking, range 0-16
  float S;            // Slope (Shelving), range 0-2 (only used with lowShelf and highShelf)
  float bandwidth;    // Bandwidth in octaves, range 0-11 (only used with bandPass and bandStop)
  float boost;        // Range +/-15 [dB]
  float freq;         // Range 20-20000 [Hz]
  float gain;         // Range +/-15 [dB]
  uint8_t filterType; // parameters::filterType::[type]
  uint8_t phase;      // parameters::phase::deg_0/deg_180
  uint8_t state;      // parameters::state::on/off
} secondOrderEQ_t;

#endif
