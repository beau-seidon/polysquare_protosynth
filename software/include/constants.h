#pragma once

#include <Arduino.h>



/* dip switch config */

const byte DIP_ADDRESS_MASK   = 0b11110000;  // set MIDI channel
const byte DIP_TUNING_MASK    = 0b00001000;  // enable tuning pots
const byte DIP_DUTY_MASK      = 0b00000100;  // set duty to MIDI CC or Velocity
const byte DIP_KEYINVERT_MASK = 0b00000010;  // keyboard invert
const byte DIP_POLYPHONY_MASK = 0b00000001;  // set mono/poly



/* tuning config */

const double e = 2.7182818284590452353602874713527;
const double pi = 3.1415926535897932384626433832795;
const double sqrt2 = 1.4142135623730950488016887242097;

const double DIVINE_INTERVAL = e * pi * sqrt2;   // lol hippyshit. value is 12.077007956766618.
const double Cs133_FREQ = 273.9617756;           // Cs133 hyperfine transition frequency, shifted down 25 octaves. note is approximately C#4.

const double DEFAULT_FREQ = Cs133_FREQ;          // A4_standard = 440.0, A4_controversial = 432.0
const byte REFERENCE_NOTE = 61;                  // A4 = 69, C4 = 60, C#4 = 61.  #include "note_names.h"
const double TUNING_INTERVAL = DIVINE_INTERVAL;  // "western music" interval is 12.0.



/* voice config */

const int MAX_VOICES = 8;
const int VOICE_PINS[MAX_VOICES]   = { 13, 12, 14, 27, 26, 25, 33, 32 };
const int LED_CHANNELS[MAX_VOICES] = { 1, 2, 8, 4, 5, 6, 7, 3 };  // channel 3 is very loud for some reason, moved to last voice

const int INVALID_NOTE = -1;
const int SILENCE = 0;

enum DUTY_MODE { DUTY_CC, DUTY_VEL };

const byte RESOLUTION = 8;



/* built-in pot config */

const int MAX_POTS = 3;
const int POT_PINS[MAX_POTS] = { 36, 34, 39 };

enum POTS { COARSE_TUNE_POT = 0, FINE_TUNE_POT = 1, INTERVAL_SCALE_POT = 2 };

const int POT_CHANGE_THRESHOLD = 16;  // 4096 / 256

enum ADJUST_MODE { POT = 0, CC = 1 };



/* shift register config */

const int SR_LD_PIN = 15;     // SH/LD pin 1
const int SR_CLKINH_PIN = 0;  // CLK INH pin 15
const int SR_QH_PIN = 4;      // QH pin 9
const int SR_CLK_PIN = 2;     // CLK pin 2



/* activity LED config */

const int LED_DELAY = 10;



/* serial debug config */

const int SERIAL1_BAUD = 9600;
