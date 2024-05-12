#pragma once

#include <Arduino.h>
#include "constants.h"
#include "parameters.h"
#include "aux_functions.h"



void setup_pots(void);
void initialize_voices(void);
void initialize_tuning(void);

void read_pots(void);

int velocity_to_duty(byte vel);
void adjust_duty(byte cc);

void adjust_coarse_tune(double val);
void adjust_fine_tune(double val);
void adjust_interval_scale(double val);

void update_active_voices(void);
