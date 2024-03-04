#pragma once

#include <Arduino.h>



#define SYNTH_MIDI_CHANNEL 8

#define MAX_VOICES 6
#define INVALID_NOTE -1
#define SILENCE 0
#define DEFAULT_FREQ 440.0
#define RESOLUTION 8

#define MAX_POTS 3
#define COARSE_TUNE_POT 0
#define FINE_TUNE_POT 1
#define INTERVAL_SCALE_POT 2
#define POT_CHANGE_THRESHOLD 16.0

#define LED_DELAY 10

#define SERIAL1_BAUD 9600



const int VOICE_PINS[MAX_VOICES] = { 27, 33, 25, 14, 12, 13 };
const int LED_CHANNELS[MAX_VOICES] = { 1, 2, 3, 4, 5, 6 };

const int POT_PINS[MAX_POTS] = { 36, 34, 39 };
