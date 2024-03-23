#include "parameters.h"



/* notes */

int note_buffer[MAX_VOICES] = {INVALID_NOTE};
int active_voices;


/* midi cc/vel duty adjust */

int duty_buffer[MAX_VOICES] = {SILENCE};
int duty = pow(2, RESOLUTION) / 2;              // [15.0, 223.0]


/* pot tuning/scale adjust */

float pot_buffer[MAX_POTS];
int active_pot = 0;

double coarse_tune   = 1.0;                     // [0.5, 2.0]
double fine_tune     = 0.0;                     // [-1.0, 1.0]
double note_interval = 1.0;                     // [0.5, 2.0]


/* activity indicator */

bool LED_state = 0;     
unsigned long int t_LED;        


/* MIDI Channel Switches */
bool MIDI_switch_states[4];

int synth_midi_channel = 8;


/* ADSR envelope (not implemented yet) */

// int A  = 0;                                  // time in ms
// int D  = 20;                                 // time in ms
// byte S = 127;                                // note velocity
// int R  = 100;                                // time in ms
// unsigned long int t[MAX_VOICES];
