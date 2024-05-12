#pragma once

#include <Arduino.h>



const int MAX_VOICES = 8;
const int INVALID_NOTE = -1;
const int SILENCE = 0;
const double DEFAULT_FREQ = 432.0;
const byte RESOLUTION = 8;

const int VOICE_PINS[MAX_VOICES] = { 13, 12, 14, 27, 26, 25, 33, 32 };
const int LED_CHANNELS[MAX_VOICES] = { 1, 2, 8, 4, 5, 6, 7, 3 };


const int DIP_TUNING_MASK = 0b00001000;

const int MAX_POTS = 3;
const int COARSE_TUNE_POT = 0;
const int FINE_TUNE_POT = 1;
const int INTERVAL_SCALE_POT = 2;
const double POT_CHANGE_THRESHOLD = 16.0;  // small portion of 4095.0

const int POT_PINS[MAX_POTS] = { 36, 34, 39 };


const int DIP_ADDRESS_MASK = 0b11110000;

const int SR_LD_PIN = 15;     // SH/LD pin 1
const int SR_CLKINH_PIN = 0;  // CLK INH pin 15
const int SR_QH_PIN = 4;      // QH pin 9
const int SR_CLK_PIN = 2;     // CLK pin 2


const int LED_DELAY = 10;


const int SERIAL1_BAUD = 9600;
