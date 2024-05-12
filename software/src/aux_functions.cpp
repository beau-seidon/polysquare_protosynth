#include "aux_functions.h"



double frequency(byte pitch)
{
    return (DEFAULT_FREQ * coarse_tune) * pow(2, (((double)pitch - (69.0 + fine_tune)) / (12.0 * note_interval)));
}


// modified version of arduino WMath function map
double range_limit(double x, double in_min, double in_max, double out_min, double out_max)
{
    const double run = in_max - in_min;
    const double rise = out_max - out_min;
    const double delta = x - in_min;
    return (delta * rise) / run + out_min;
}


int get_note_buffer_position(int pitch)
{
    int *note = std::find(std::begin(note_buffer), std::end(note_buffer), pitch);

    if (note != std::end(note_buffer))
        return std::distance(note_buffer, note);

    return -1;
}


int get_active_voice_count()
{
    int av = 0;
    for (int n = 0; n < MAX_VOICES; n++) {
        if (note_buffer[n] > INVALID_NOTE) av++;
    }
    return av;
}


void show_activity(bool action)
{
    if (action) {
        digitalWrite(LED_BUILTIN, HIGH);
        LED_state = true;
        t_LED = millis();
        return;
    }

    if (!action && LED_state) {
        if (millis() > (t_LED + LED_DELAY)) {
            digitalWrite(LED_BUILTIN, LOW);
            LED_state = false;
        }
    }
}


void debug_print_buffers(const char *message, int *note_buffer, int *duty_buffer)
{
    Serial.println(message);

    Serial.print("note_buffer[ ");
    for (int i = 0; i < MAX_VOICES; i++) {
        Serial.print(note_buffer[i]);
        if (i < MAX_VOICES-1) Serial.print(", ");
    }
    Serial.println(" ]");

    Serial.print("duty_buffer[ ");
    for (int i = 0; i < MAX_VOICES; i++) {
        Serial.print(duty_buffer[i]);
        if (i < MAX_VOICES-1) Serial.print(", ");
    }
    Serial.println(" ]");

    Serial.println();
}
