#include "voice_functions.h"



void setup_pots()
{
    for (int p = 0; p < MAX_POTS; p++) { pinMode(POT_PINS[p], INPUT); }
}



void initialize_voices()
{
    active_voices = 0;
    for (int c = 0; c < MAX_VOICES; c++) {
        ledcSetup(LED_CHANNELS[c], DEFAULT_FREQ, RESOLUTION);  // ledc_timer_config_t
        ledcAttachPin(VOICE_PINS[c], LED_CHANNELS[c]);         // ledc_channel_config_t
        ledcWrite(LED_CHANNELS[c], SILENCE);
        note_buffer[c] = INVALID_NOTE;
    }
}



void initialize_tuning()
{
    int pot_value;
    for (int p = 0; p < MAX_POTS; p++) {
        analogRead(POT_PINS[p]);  // charge input
        int prev_pot_value;
        pot_value = analogRead(POT_PINS[p]);
        pot_buffer[p] = pot_value;
    }
}



void read_pots()
{
    if (!tuning_enabled) return;

    if (active_pot >= MAX_POTS) active_pot = 0;

    analogRead(POT_PINS[active_pot]);  // charge input

    int prev_pot_val = pot_buffer[active_pot];
    int pot_value = analogRead(POT_PINS[active_pot]);

    if (abs(pot_value - prev_pot_val) >= POT_CHANGE_THRESHOLD) {
        pot_buffer[active_pot] = pot_value;
        switch (active_pot) {
            case COARSE_TUNE_POT:
                adjust_coarse_tune(pot_value, ADJUST_MODE::POT);
                break;
            case FINE_TUNE_POT:
                adjust_fine_tune(pot_value, ADJUST_MODE::POT);
                break;
            case INTERVAL_SCALE_POT:
                adjust_interval_scale(pot_value, ADJUST_MODE::POT);
                break;
        }
        update_active_voices();
    }
    active_pot++;
}



int velocity_to_duty(byte vel)
{
    int duty_min = 1;  // (pow(2, RESOLUTION)) / (RESOLUTION / 2);
    int duty_max = pow(2, RESOLUTION) / 2;

    int dynamic_duty = range_limit(vel, 0, 127, duty_min, duty_max);

    return dynamic_duty;
}



void adjust_duty(byte cc)
{
    int duty_min = 1 ;  // pow(2, RESOLUTION) / RESOLUTION / 2;
    int duty_max = pow(2, RESOLUTION) - duty_min - 1;

    duty = range_limit(cc, 0, 127, duty_min, duty_max);

    for (int c = 0; c < MAX_VOICES; c++) {
        if (note_buffer[c] != INVALID_NOTE) {
            duty_buffer[c] = duty;
            ledcWrite(LED_CHANNELS[c], duty);
        }
    }
}



void adjust_coarse_tune(int val, int adj_mode)
{
    double tune_min = 0.25;
    double tune_max = 4.0;

    switch (adj_mode){
        case (ADJUST_MODE::CC):
            coarse_tune_adjust = range_limit(val, 0, 127, tune_min, tune_max);
            break;
        case (ADJUST_MODE::POT):
            coarse_tune_adjust = range_limit(val, 0, 4095, tune_min, tune_max);
            break;
    }
}



void adjust_fine_tune(int val, int adj_mode)
{
    double tune_min = -5.0;
    double tune_max = 5.0;

    switch (adj_mode){
        case (ADJUST_MODE::CC):
            fine_tune_adjust = range_limit(val, 0, 127, tune_min, tune_max);
            break;
        case (ADJUST_MODE::POT):
            fine_tune_adjust = range_limit(val, 0, 4095, tune_min, tune_max);
            break;
    }
}



void adjust_interval_scale(int val, int adj_mode)
{
    double interval_min = 2.0;
    double interval_max = 0.5;

    switch (adj_mode){
        case (ADJUST_MODE::CC):
            note_interval_adjust = range_limit(val, 0, 127, interval_min, interval_max);
            break;
        case (ADJUST_MODE::POT):
            note_interval_adjust = range_limit(val, 0, 4095, interval_min, interval_max);
            break;
    }
}



void update_active_voices()
{
    for (int c = 0; c < MAX_VOICES; c++) {
        if (note_buffer[c] != INVALID_NOTE)
            ledcChangeFrequency(LED_CHANNELS[c], frequency(note_buffer[c]), RESOLUTION);
    }
}
