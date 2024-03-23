#pragma once

#include <Arduino.h>
#include "constants.h"



/* notes */

extern int note_buffer[MAX_VOICES];
extern int active_voices;


/* midi cc/vel duty adjust */

extern int duty_buffer[MAX_VOICES];
extern int duty;


/* pot tuning/scale adjust */

extern float pot_buffer[MAX_POTS];
extern int active_pot;

extern double coarse_tune;
extern double fine_tune;
extern double note_interval;


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
