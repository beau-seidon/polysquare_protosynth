#pragma once

#include <Arduino.h>
#include "constants.h"



/* initial config */

extern byte dip_config;



/* notes */

extern int note_buffer[MAX_VOICES];
extern int active_voices;
extern bool polyphony_mode;



/* midi cc/vel duty adjust */

extern int duty_mode;
extern int duty_buffer[MAX_VOICES];
extern int duty;



/* pot tuning/scale adjust */

extern int pot_buffer[MAX_POTS];
extern int active_pot;

extern double coarse_tune_adjust;
extern double fine_tune_adjust;
extern double note_interval_adjust;

extern bool tuning_enabled;
extern bool keyinvert_mode;



/* activity indicator */

extern bool LED_state;
extern unsigned long int t_LED;



/* MIDI Channel Switches */

extern bool MIDI_switch_states[4];

extern int synth_midi_channel;



/* ADSR envelope (not implemented yet) */

// extern int A;
// extern int D;
// extern byte S;
// extern int R;
// extern unsigned long int t[MAX_VOICES];
