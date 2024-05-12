#include "voice_functions.h"



void setup_pots()
{
    for (int p = 0; p < MAX_POTS; p++) {
        pinMode(POT_PINS[p], INPUT);
    }
}


void initialize_voices()
{
    active_voices = 0;

    for (int c = 0; c < MAX_VOICES; c++) {
        ledcSetup(LED_CHANNELS[c], DEFAULT_FREQ, RESOLUTION);
        ledcAttachPin(VOICE_PINS[c], LED_CHANNELS[c]);
        ledcWrite(LED_CHANNELS[c], SILENCE);
        note_buffer[c] = INVALID_NOTE;
    }
}


void initialize_tuning()
{
    double pot_value;
    for (int p = 0; p < MAX_POTS; p++) {
        pot_value = analogRead(POT_PINS[p]);
        double prev_pot_value;
        pot_value = analogRead(POT_PINS[p]);
        pot_buffer[p] = pot_value;
    }
}


void read_pots()
{
    if (!tuning_enabled) return;

    if (active_pot >= MAX_POTS) active_pot = 0;

    double pot_value;
    pot_value = analogRead(POT_PINS[active_pot]);

    double prev_pot_val = pot_buffer[active_pot];
    pot_value = analogRead(POT_PINS[active_pot]);  // second analog read for accuracy

    if (abs(pot_value - prev_pot_val) >= POT_CHANGE_THRESHOLD) {
        pot_buffer[active_pot] = pot_value;

        switch (active_pot) {
            case COARSE_TUNE_POT:
                adjust_coarse_tune(pot_value);
                break;
            case FINE_TUNE_POT:
                adjust_fine_tune(pot_value);
                break;
            case INTERVAL_SCALE_POT:
                adjust_interval_scale(pot_value);
                break;
        }
        update_active_voices();
    }
    active_pot++;
}


int velocity_to_duty(byte vel)
{
    double duty_min = 0.0;  // (pow(2, RESOLUTION)) / (RESOLUTION / 2);
    double duty_max = pow(2.0, RESOLUTION) / 2.0;

    int dynamic_duty = (int)range_limit((double)vel, 0.0, 127.0, duty_min-1.0, duty_max-1.0);

    return dynamic_duty;
}


void adjust_duty(byte cc)
{
    double duty_min = 0.0;  // pow(2, RESOLUTION) / RESOLUTION / 2;
    double duty_max = pow(2.0, RESOLUTION) - duty_min;

    duty = (int)range_limit((double)cc, 0.0, 127.0, duty_min-1.0, duty_max-1.0);

    for (int c = 0; c < MAX_VOICES; c++) {
        if (note_buffer[c] != INVALID_NOTE) {
            duty_buffer[c] = duty;
            ledcWrite(LED_CHANNELS[c], duty);
        }
    }
}


void adjust_coarse_tune(double val)
{
    double tune_min = 4.0;
    double tune_max = 0.25;

    coarse_tune = range_limit(val, 0.0, 4095.0, tune_min, tune_max);
}


void adjust_fine_tune(double val)
{
    double tune_min = -5.0;
    double tune_max = 5.0;

    fine_tune = range_limit(val, 0.0, 4095.0, tune_min, tune_max);
}


void adjust_interval_scale(double val)
{
    double interval_min = 0.5;
    double interval_max = 2.0;

    note_interval = range_limit(val, 0.0, 4095.0, interval_min, interval_max);
}


void update_active_voices()
{
    for (int c = 0; c < MAX_VOICES; c++) {
        if (note_buffer[c] != INVALID_NOTE) {
            ledcChangeFrequency(LED_CHANNELS[c], frequency(note_buffer[c]), RESOLUTION);
        }
    }
}
