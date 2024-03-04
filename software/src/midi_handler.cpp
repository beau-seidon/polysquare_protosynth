#include "midi_handler.h"



MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, midi2);


void initialize_midi() {
    midi2.setHandleNoteOn(handleNoteOn);
    midi2.setHandleNoteOff(handleNoteOff);
    midi2.setHandleControlChange(handleControlChange);
    midi2.begin(SYNTH_MIDI_CHANNEL);
}


void read_midi() {
    midi2.read();
}


void handleNoteOn(byte channel, byte pitch, byte velocity) {
    if (active_voices < MAX_VOICES) {
        int inactive_voice = get_note_buffer_position(INVALID_NOTE);

        if (inactive_voice >= 0 && inactive_voice < MAX_VOICES) {
            note_buffer[inactive_voice] = pitch;
            duty_buffer[inactive_voice] = velocity_to_duty(velocity);

            ledcChangeFrequency(LED_CHANNELS[inactive_voice], frequency(pitch), RESOLUTION);
            ledcWrite(LED_CHANNELS[inactive_voice], duty_buffer[inactive_voice]);   

            active_voices = get_active_voice_count();

            show_activity(true);
        }
    }
}


void handleNoteOff(byte channel, byte pitch, byte velocity) {
    if (active_voices > 0) {
        int active_voice = get_note_buffer_position(pitch);

        if (active_voice >= 0 && active_voice < MAX_VOICES) {
            note_buffer[active_voice] = INVALID_NOTE;
            duty_buffer[active_voice] = SILENCE;

            ledcWrite(LED_CHANNELS[active_voice], SILENCE);

            active_voices = get_active_voice_count();
        }
    }
}


void handleControlChange(byte channel, byte cc_number, byte cc_value) {
    if (cc_number == 1) adjust_duty(cc_value);
    if (cc_number == 123) initialize_voices();
}
