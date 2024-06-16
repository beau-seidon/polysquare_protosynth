#include "midi_handler.h"



/* UART2 serial midi */
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

/* USB-MIDI via Hairless MIDI Serial Bridge */
// struct CustomBaudRateSettings : public MIDI_NAMESPACE::DefaultSerialSettings { static const long BaudRate = 115200; };
// MIDI_NAMESPACE::SerialMIDI<HardwareSerial, CustomBaudRateSettings> serialMIDI(Serial);
// MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::SerialMIDI<HardwareSerial, CustomBaudRateSettings>> MIDI((MIDI_NAMESPACE::SerialMIDI<HardwareSerial, CustomBaudRateSettings>&)serialMIDI);



void initialize_midi()
{
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleControlChange(handleControlChange);
    MIDI.begin(synth_midi_channel);
}



void read_midi()
{
    MIDI.read();
}



void handleNoteOn(byte channel, byte pitch, byte velocity)
{
    if (active_voices < MAX_VOICES) {
        int inactive_voice = get_note_buffer_position(INVALID_NOTE);

        if (inactive_voice >= 0 && inactive_voice < MAX_VOICES) {
            note_buffer[inactive_voice] = pitch;
            duty_buffer[inactive_voice] = (duty_mode == DUTY_VEL) ? velocity_to_duty(velocity) : duty;

            ledcChangeFrequency(LED_CHANNELS[inactive_voice], frequency(pitch), RESOLUTION);
            ledcWrite(LED_CHANNELS[inactive_voice], duty_buffer[inactive_voice]);

            active_voices++;
            // active_voices = get_active_voice_count();

            show_activity(true);
        }
    }
}



void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    if (active_voices > 0) {
        int active_voice = get_note_buffer_position(pitch);

        if (active_voice >= 0 && active_voice < MAX_VOICES) {
            note_buffer[active_voice] = INVALID_NOTE;
            duty_buffer[active_voice] = SILENCE;

            ledcWrite(LED_CHANNELS[active_voice], SILENCE);

            active_voices--;
            // active_voices = get_active_voice_count();
        }
    }
}



void handleControlChange(byte channel, byte cc_number, byte cc_value)
{
    if (cc_number == 1 || cc_number == 70) {
        adjust_duty(cc_value);
        return;
    }

    if (cc_number == 71) {
        adjust_coarse_tune(cc_value, ADJUST_MODE::CC);
        update_active_voices();
        return;
    }

    if (cc_number == 72) {
        adjust_fine_tune(cc_value, ADJUST_MODE::CC);
        update_active_voices();
        return;
    }

    if (cc_number == 73) {
        adjust_interval_scale(cc_value, ADJUST_MODE::CC);
        update_active_voices();
        return;
    }

    if (cc_number == 123) {
        initialize_voices();
        return;
    }
}
