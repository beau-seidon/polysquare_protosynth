// written by folkert van heusden <mail@vanheusden.com>

#define LED_BUILTIN 2

HardwareSerial MIDI(1);

typedef struct
{
	uint8_t midi_ch : 4;
	uint8_t note : 7, velocity : 7;
	unsigned long int off_at, started_at;
} phys_channel_t;

#define OMNI 1
#define MONO_POLY 2
uint8_t omp[16]; // one for each midi channel

#define NOTE_OFF_TIME 50
#define PERC_OFF_TIME 25
#define OFF_CHECK_INTERVAL (NOTE_OFF_TIME > PERC_OFF_TIME ? PERC_OFF_TIME : NOTE_OFF_TIME)

#define N_PHYS_CHANNELS 8

phys_channel_t channels[N_PHYS_CHANNELS][2];

static bool ungetc_valid = false;
static uint8_t ungetc_byte = 0x00;

void ungetc(const uint8_t c) {
	if (ungetc_valid) {
		error(0x09);
		return;
	}

	ungetc_byte = c;
	ungetc_valid = true;
}

uint8_t get_byte() {
	if (ungetc_valid) {
		ungetc_valid = false;

		return ungetc_byte;
	}

	digitalWrite(LED_BUILTIN, LOW);
	while(MIDI.available() == 0) {
	}
	digitalWrite(LED_BUILTIN, HIGH);

	return MIDI.read();
}

typedef struct {
	uint8_t midi_command, n_bytes : 2;
} mc_size_t;

const mc_size_t cmds[] = {
	{ 0x80, 2 },
	{ 0x90, 2 },
	{ 0xa0, 2 },
	{ 0xb0, 2 },
	{ 0xc0, 1 },
	{ 0xd0, 1 },
	{ 0xe0, 2 },
	{ 0xf0, 0 },
	{ 0, 0 }
};

bool midi_get(uint8_t *const cmd, uint8_t *const d1, uint8_t *const d2) {
	static uint8_t prev_cmd = 0x00;

	*cmd = get_byte();

	if (*cmd < 0x80) {
		if (!prev_cmd)
			return false;

		ungetc(*cmd);
		*cmd = prev_cmd;
	}

	const uint8_t cmdMasked = (*cmd) & 0xf0;

	*d1 = *d2 = 0x00;

	uint8_t i = 0;
	while(cmds[i].midi_command != cmdMasked && cmds[i].midi_command != 0x00)
		i++;

	if (cmds[i].midi_command == 0x00) {
		error(0x0a);
		return false;
	}

	if (cmds[i].n_bytes >= 1) {
		*d1 = get_byte();

		if (*d1 & 128) {
			ungetc(*d1);
			*d1 = 0x00;
			error(0x0b);
			return false;
		}
	}

	if (cmds[i].n_bytes >= 2) {
		*d2 = get_byte();

		if (*d2 & 128) {
			ungetc(*d2);
			*d2 = 0x00;
			error(0x0c);
			return false;
		}
	}

	if (*cmd < 0xf0)
		prev_cmd = *cmd;

	return true;
}

void setup_pin(const uint8_t pin, const uint8_t ch) {
	Serial.print(ch);
	Serial.print('\t');
	Serial.println(ledcSetup(ch, 22050, 10));

	ledcAttachPin(pin, ch);
}

float midiNoteToFreq(const uint8_t note)
{
	return pow(2.0, (float(note) - 69.0) / 12.0) * 440.0;
}

uint8_t playing = 255;

void noteOffPC(const uint8_t phys_channel) {
	Serial.print(F("noteOffPC: "));
	Serial.println(phys_channel);

	ledcWrite(phys_channel * 2, 0);
}

void noteOnPC(const uint8_t phys_channel, const uint16_t freq, const uint8_t velocity) {
	Serial.print(F("noteOnPC: "));
	Serial.print(phys_channel);
	Serial.print(' ');
	Serial.print(freq);
	Serial.print(' ');
	Serial.println(velocity);

	// ledcWrite(phys_channel * 2, velocity);
	ledcWrite(phys_channel * 2, 255);

	ledcWriteTone(phys_channel * 2, freq);
}

void noteOnPC(const uint8_t phys_channel, const bool is_percussion, const uint8_t note, const uint8_t velocity) {
	uint16_t freq = midiNoteToFreq(note);

	noteOnPC(phys_channel, freq, velocity);
}

void allOffLow() {
	for(uint8_t i=0; i<N_PHYS_CHANNELS; i++)
		noteOffPC(i);
}

void error(const uint8_t e) {
	Serial.print(F(" ["));
	Serial.print(e, HEX);
	Serial.print(F("] "));
}

void initMidi() {
	memset(channels, 0x00, sizeof channels);

        for(uint8_t i=0; i<16; i++)
                omp[i] = MONO_POLY | OMNI;
}

void setup() {
	Serial.begin(115200);

	MIDI.begin(31250, SERIAL_8N1, 16, 19, false);

	pinMode(LED_BUILTIN, OUTPUT);

	setup_pin(23, 0);  // attach IO02 to PWM channel 0
	setup_pin(22, 2);
	setup_pin( 1, 4);
	setup_pin( 3, 6);
	setup_pin(21, 8);
	setup_pin(19, 10);
	setup_pin(18, 12);
	setup_pin( 5, 14);

	initMidi();

//	for(;;) {
//		for(uint16_t k=0; k<256; k += 16) {
			for(uint8_t i=0; i<N_PHYS_CHANNELS; i++) {
				//noteOnPC(i, 440 + i * 131, k);
				noteOnPC(i, 440 + i * 131, 255);
				delay(250);
			}

			allOffLow();

//			delay(250);
//		}
//	}

	Serial.println(F("Go!"));
}

void checkNotesEnd(const bool force) {
	if (force) {
		for(int8_t i=0; i<N_PHYS_CHANNELS; i++) {
			if (channels[i][0].off_at > 0) {
				channels[i][0].velocity = 0;
				noteOffPC(i);
			}
		}
	}
	else {
		const unsigned long int now = millis();

		for(int8_t i=0; i<N_PHYS_CHANNELS; i++) {
			if (channels[i][0].off_at <= now && channels[i][0].off_at > 0) {
				channels[i][0].velocity = 0;
				noteOffPC(i);
			}
		}
	}
}

void stopAnyBeats() {
	for(int8_t i=0; i<N_PHYS_CHANNELS; i++) {
		if (channels[i][0].midi_ch == 9) {
			channels[i][0].velocity = 0;
			noteOffPC(i);
		}
	}
}

bool play(const uint8_t midi_ch, const uint8_t note, uint8_t velocity, const bool off) {
	int8_t left = -1;

	if (midi_ch == 9)
		velocity &= 15;

	///////////// SWITCH OFF POLYPHONIC ///////////////////////
	// if any midi-channel is no longer poly, then first turn off any
	// playing notes on that channel
	if ((omp[midi_ch] & MONO_POLY) == 0) {
		bool to_mono = false;

		for(int8_t i=0; i<N_PHYS_CHANNELS; i++) {
			if (channels[i][0].midi_ch == midi_ch && channels[i][0].velocity > 0) {
				channels[i][0].velocity = 0;
				noteOffPC(i);
				to_mono = true;
			}
		}

		if (to_mono)
			Serial.print(" M ");
	}

	// allow 1 beat concurrently
	if (midi_ch == 9)
		stopAnyBeats();

	checkNotesEnd(false);

	///////////// SWITCH OFF NOTES THAT ENDED ///////////////////////
	// if velocity is 0, then it is note off. end it and free slot
	if (velocity == 0) {
		for(int8_t i=0; i<N_PHYS_CHANNELS; i++) {
			if (channels[i][0].note == note && channels[i][0].midi_ch == midi_ch) {
				channels[i][0].velocity = 0;
				noteOffPC(i);
				left = i;
			}

			if (left != -1)
				break;
		}

		return true;
	}

	const unsigned long int now = millis();

	///////////// MARK NOTES AS WILL-END ///////////////////////
	// if 0x8x, set note as ended, stop it later but slot is available to be replaced
	if (off) {
		unsigned long int off_at = millis() + NOTE_OFF_TIME;

		for(int8_t i=0; i<N_PHYS_CHANNELS; i++) {
			if (channels[i][0].note == note && channels[i][0].midi_ch == midi_ch) {
				channels[i][0].off_at = off_at;
				left = i;
			}

			if (left != -1)
				break;
		}

		return true;
	}

	///////////// FIND NEW SLOT ///////////////////////

	// note already playing with different velocity?
	for(int8_t i=0; i<N_PHYS_CHANNELS; i++) {
		if (channels[i][0].note == note && channels[i][0].midi_ch == midi_ch && left == -1)
			left = i;
		if (left != -1)
			break;
	}

	for(byte attempt=0; attempt<3; attempt++) {
		// see if there's a possibility for the note both on left and right
		if (left == -1) {
			for(int8_t i=0; i<N_PHYS_CHANNELS; i++) {
				if (channels[i][0].velocity == 0) {
					left = i;
					break;
				}
			}
		}

		// all slots full, break a connected left + right
		if (left == -1) {
			for(int8_t i=0; i<N_PHYS_CHANNELS; i++) {
				for(int8_t j=i; j<N_PHYS_CHANNELS; j++) {
					if (channels[i][0].velocity == channels[j][1].velocity &&
					    channels[i][0].note     == channels[j][1].note &&
					    channels[i][0].midi_ch  == channels[j][1].midi_ch) {
							left = i;

						goto break_done;
					}
				}
			}
		}
		break_done:

		if (left != -1)
			break;

		if (attempt == 1)
			checkNotesEnd(true);
		else if (attempt == 2) {
			int8_t cl = -1;
			unsigned long int oldestl = now;
			unsigned long int oldestr = now;

			for(int8_t i=0; i<N_PHYS_CHANNELS; i++) {
				if (channels[i][0].started_at < oldestl) {
					cl = i;
					oldestl = channels[i][0].started_at;
					break;
				}
			}

			if (cl != -1) {
				channels[cl][0].velocity = 0;
				noteOffPC(cl);
			}
		}
	}

	unsigned long int off_at = 0;
	if (midi_ch == 9) // percussion
		off_at = millis() + PERC_OFF_TIME;

	if (left != -1) {
		noteOnPC(left, midi_ch == 9, note, velocity);

		channels[left][0].note = note;
		channels[left][0].velocity = velocity;
		channels[left][0].midi_ch = midi_ch;
		channels[left][0].off_at = off_at;
		channels[left][0].started_at = now;
	}

	if (left == -1)
		error(0x02);

	return left != -1;
}

void allNotesOff() {
	allOffLow();

	initMidi();
}

void loop() {
	uint8_t cmd, d1, d2;
	midi_get(&cmd, &d1, &d2);

	if (!(cmd & 128)) {
		error(0x0d);
		return;
	}

	Serial.println(F(""));
	Serial.print(cmd, HEX);
	Serial.print(' ');
	Serial.print(d1, HEX);
	Serial.print(' ');
	Serial.print(d2, HEX);
	Serial.print(' ');

	const uint8_t cmdMasked = cmd & 0xf0;
	const uint8_t channel = cmd & 0x0f;

	bool err = false;
	if (cmdMasked == 0x80) { // note off
		Serial.print("{OFF} ");
		err = !play(channel, d1, 0, true);
	}
	else if (cmdMasked == 0x90) { // note on
		err = !play(channel, d1, d2 * 2, false);
	}
	else if (cmdMasked == 0xa0) { // polyphonic key pressure (after-touch)
	}
	else if (cmdMasked == 0xb0) { // change mode message
		Serial.print("{CMM ");
		Serial.print(d1);
		Serial.print("}");

		if (d1 >= 123 && d1 <= 127) {
			allNotesOff();

			if (d1 == 124) // omni off
				omp[channel] &= ~OMNI;
			else if (d1 == 125)
				omp[channel] |= OMNI;
			else if (d1 == 126) { // poly off
				uint8_t endCh = channel + (d2 & 0x0f);
				if (endCh > 15)
					endCh = 15;

				for(uint8_t i=channel; i<endCh; i++)
					omp[i] &= ~MONO_POLY;
			}
			else if (d1 == 127) { // poly on
				omp[channel] |= MONO_POLY;
			}
		}
	}
	else if (cmdMasked == 0xc0) { // program change message
	}
	else if (cmdMasked == 0xd0) { // channel pressure (after-touch)
	}
	else if (cmdMasked == 0xe0) { // pitch bend
		uint8_t lsb = d1;
		uint8_t msb = d2;

		float freq_bend = float(((msb << 7) | lsb) - 0x2000) / float(0x2000);
		Serial.print("{PB ");
		Serial.print(freq_bend);
		Serial.print('}');

		for(int8_t i=0; i<N_PHYS_CHANNELS; i++) {
			if (channels[i][0].velocity == 0 || channels[i][0].midi_ch != channel)
				continue;

			float curFreq = midiNoteToFreq(channels[i][0].note);
			float nextFreq = 0.0, useFreq = 0.0;

			if (freq_bend >= 0) {
				nextFreq = midiNoteToFreq(channels[i][0].note + 2);
				useFreq = curFreq + (nextFreq - curFreq) * freq_bend;
			}
			else {
				nextFreq = midiNoteToFreq(channels[i][0].note - 2);
				useFreq = curFreq + (nextFreq - curFreq) * (1.0 - freq_bend);
			}

			Serial.print(useFreq);
			Serial.print(' ');

			noteOnPC(i, uint16_t(useFreq), channels[i][0].velocity);
		}

		Serial.print('|');
	}
	else if (cmd == 0xf8 /* timing clock */ || cmd == 0xfe /* keep alive */) {
	}
	else if (cmdMasked == 0xf0) { // vendor specific
		bool endSeen = false;

		for(uint8_t i=0; i<32; i++) {
			if (get_byte() == 0xf7) {
				endSeen = true;
				break;
			}
		}

		if (!endSeen)
			error(0x0e);
	}
	else if (cmd == 0xff) { // reset all midi devices
		allOffLow();
	}
	else {
		error(0x0f);
	}
}
