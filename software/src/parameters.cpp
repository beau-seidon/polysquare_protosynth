#include "parameters.h"



/* initial config */

byte dip_config = 0b00000000;



/* notes */

int note_buffer[MAX_VOICES] = { INVALID_NOTE };
int active_voices;
bool polyphony_mode = true;



/* midi cc/vel duty adjust */

int duty_mode;
int duty_buffer[MAX_VOICES] = { SILENCE };
int duty = pow(2, RESOLUTION) / 2;  // [15, 223]



/* pot tuning/scale adjust */

int pot_buffer[MAX_POTS];
int active_pot = 0;

double coarse_tune_adjust   = 1.0;  // [0.5, 2.0]
double fine_tune_adjust     = 0.0;  // [-1.0, 1.0]
double note_interval_adjust = 1.0;  // [0.5, 2.0]

bool tuning_enabled = true;
bool keyinvert_mode = false;



/* activity indicator */

bool LED_state = 0;
unsigned long int t_LED;



/* MIDI Channel Switches */

bool MIDI_switch_states[4];

int synth_midi_channel = 8;



/* ADSR envelope (not implemented yet) */

// int A  = 0;    // time in ms
// int D  = 20;   // time in ms
// byte S = 127;  // note velocity
// int R  = 100;  // time in ms
// unsigned long int t[MAX_VOICES];
