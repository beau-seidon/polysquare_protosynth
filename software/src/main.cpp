#include <Arduino.h>
#include <MIDI.h>
// #include <algorithm>


MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);


#define SERIAL1_BAUD 9600
#define LED_PIN 2					// LED pin on ESP32 WROOM
#define SYNTH_MIDI_CHANNEL 1

#define DUTY_POT 0
#define TUNE_POT 1
#define INTERVAL_POT 2

const int MAX_VOICES = 6;
const int MAX_POTS = 3;

const int SILENCE = 0;
const int INVALID_NOTE = 128;


const int LED_CHANNELS[MAX_VOICES] = {1, 2, 3, 4, 5, 6};
const int VOICE_PINS[MAX_VOICES] = {23, 32, 21, 19, 18, 22};  // {23, 22, 21, 19, 18, 32};

const byte RESOLUTION = 8;


int active_voices;
int note_buffer[MAX_VOICES];

unsigned long int t[MAX_VOICES];

const float DEFAULT_FREQ = 440.0;


const int POT_PINS[MAX_POTS] = {36, 39, 34};
float pot_values[3] = {0.0};

const byte CC_NUMBERS[3] = {20, 21, 22};
byte cc_values[3] = {63};

byte active_pot = 0;

unsigned long int t_pot;
const int pot_read_delay = 10;


int duty = 127;					// [15, 239]
float coarse_tune = 0.0;		// [220, 440]
float fine_tune = 0.0;			// [-1, 1]
float note_interval = 12.0;		// [8, 16]


unsigned long int t_LED;
const int LED_delay = 10;
byte LED_state = 0;


int A = 0;		// time in ms
int D = 20;		// time in ms
byte S = 127;	// note velocity
int R = 100;	// time in ms




// -----------------------------------------------------------------------------
// declarations

void setup_pots();
void read_pots();
void pot_to_duty(byte);
void adjust_fine_tune(float);
void adjust_interval(float);
void initialize_voices();
byte get_note_buffer_position(byte);
void handleNoteOn(byte, byte, byte);
void handleNoteOff(byte, byte, byte);
void show_activity(int);




// -----------------------------------------------------------------------------
// main functions

void setup() {
	pinMode(LED_PIN, OUTPUT);
	setup_pots();
	initialize_voices();
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.begin(MIDI_CHANNEL_OMNI);
	t_pot = millis();
	Serial.begin(SERIAL1_BAUD);
}


void loop() {
	read_pots();
	pot_to_duty(cc_values[DUTY_POT]);
	adjust_fine_tune(pot_values[TUNE_POT]);
	adjust_interval(pot_values[INTERVAL_POT]);
    MIDI.read();
	show_activity(0);
}




// -----------------------------------------------------------------------------
// definitions

void setup_pots() {
	for(int p = 0; p < MAX_POTS; p++) {
		pinMode(POT_PINS[p], INPUT);
	}
}

void initialize_voices() {
	active_voices = 0;
	for(byte c = 0; c < MAX_VOICES; c++) {
		ledcSetup(LED_CHANNELS[c], DEFAULT_FREQ, RESOLUTION);
		ledcAttachPin(VOICE_PINS[c], LED_CHANNELS[c]);
		ledcWrite(LED_CHANNELS[c], SILENCE);
		note_buffer[c] = INVALID_NOTE;
	}
}


void velocity_to_duty(byte vel) {
	int duty_max = 223;
	int duty_min = 15;

	if(vel > 127) {
		duty = duty_max;
	} else if(vel < 0) {
		duty = duty_min;
	} else {
		duty = (((duty_max - duty_min) / (127)) * (vel)) + duty_min;
	}
}


void pot_to_duty(byte cc) {
	int duty_max = 223;
	int duty_min = 15;

	if(cc > 127) {
		duty = duty_max;
	} else if(cc <= 0) {
		duty = duty_min;
	} else {
		duty = (((duty_max - duty_min) / (127)) * (cc)) + duty_min;
	}

	for(byte c = 0; c < MAX_VOICES; c++) {
		if(note_buffer[c] != INVALID_NOTE) {
			ledcWrite(LED_CHANNELS[c], duty);
		}
	}
}


void adjust_fine_tune(float val) {
	float tune_max = 1.0;
	float tune_min = -1.0;

	if(val >= 4095.0) {
		fine_tune = tune_max;
	} else if(val <= 0.0) {
		fine_tune = tune_min;
	} else {
		fine_tune = (((tune_max - tune_min) / (4095.0)) * (val)) + tune_min;
	}	
}


void adjust_interval(float val) {
	float interval_max = 24.0;
	float interval_min = 6.0;

	if(val >= 4095.0) {
		note_interval = interval_max;
	} else if(val <= 0.0) {
		note_interval = interval_min;
	} else {
		note_interval = (((interval_max - interval_min) / (4095.0)) * (val)) + interval_min;
	}	
}

byte get_note_buffer_position(byte pitch) {
	int *note = std::find(std::begin(note_buffer), std::end(note_buffer), pitch);
	if (note != std::end(note_buffer)) {
		return std::distance(note_buffer, note);
	}
	return 255;
}


void handleNoteOn(byte channel, byte pitch, byte velocity) {
	// velocity_to_duty(velocity);

	if(active_voices < MAX_VOICES) {
		byte inactive_voice = get_note_buffer_position(INVALID_NOTE);
		if(inactive_voice >= 0 && inactive_voice <= MAX_VOICES) {
			note_buffer[inactive_voice] = pitch;

			float freq = (DEFAULT_FREQ + coarse_tune) * pow(2, ((pitch - (69 - fine_tune)) / note_interval));

			ledcChangeFrequency(LED_CHANNELS[inactive_voice], freq, RESOLUTION);
			ledcWrite(LED_CHANNELS[inactive_voice], duty);	

			t[inactive_voice] = millis();
			active_voices++;
			show_activity(1);
		}
	}
}


void handleNoteOff(byte channel, byte pitch, byte velocity) {
	if(active_voices > 0) {
		byte active_voice = get_note_buffer_position(pitch);
		if(active_voice >= 0 && active_voice <= MAX_VOICES) {
			note_buffer[active_voice] = INVALID_NOTE;
			ledcWrite(LED_CHANNELS[active_voice], SILENCE);
			active_voices--;
		}
	}
}


void show_activity(int action) {
	if(action == 1)	{
		digitalWrite(LED_PIN, HIGH);
		LED_state = 1;
		t_LED = millis();
		return;
	}
	if(action == 0 && LED_state == 1) {
		if(millis() > (t_LED + LED_delay)) {
			digitalWrite(LED_PIN, LOW);
			LED_state = 0;
		}
	}
}


void read_pots() {
	if(active_pot >= MAX_POTS) {
		active_pot = 0;
		Serial.println();	
	}

	float prev_pot_val = pot_values[active_pot];

	if(millis() > (t_pot + pot_read_delay)) {
		pot_values[active_pot] = analogRead(POT_PINS[active_pot]);
		pot_values[active_pot] = analogRead(POT_PINS[active_pot]);

		if(abs(prev_pot_val - pot_values[active_pot]) >= 32) {
			cc_values[active_pot] = (byte)(127 * (pot_values[active_pot] / 4095));
			MIDI.sendControlChange(CC_NUMBERS[active_pot], cc_values[active_pot], SYNTH_MIDI_CHANNEL);
		}

		active_pot++;
		t_pot = millis();
	}
}
