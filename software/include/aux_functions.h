#pragma once

#include <Arduino.h>
#include "constants.h"
#include "parameters.h"



double frequency(byte pitch);

double range_limit(double val, double min_x, double min_y, double max_x, double max_y);

int get_note_buffer_position(int pitch);
int get_active_voice_count(void);

void show_activity(bool action);

void debug_print_buffers(const char *message, int *note_buffer, int *duty_buffer);
