#pragma once

#include <Arduino.h>
#include <MIDI.h>
#include "constants.h"
#include "parameters.h"
#include "aux_functions.h"
#include "voice_functions.h"



void initialize_midi(void);

void read_midi(void);

void handleNoteOn(byte channel, byte pitch, byte velocity);
void handleNoteOff(byte channel, byte pitch, byte velocity);
void handleControlChange(byte channel, byte cc_number, byte cc_value);
