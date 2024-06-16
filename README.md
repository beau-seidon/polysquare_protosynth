# polysquare_protosynth

MIDI-controlled 8-voice polyphonic square wave synthesizer based on the ESP-32 (NodeMCU-32S Dev Kit).

![brain](<other resources/images/polysquare_protosynth_brain.jpg>)


### Changes:
- added dip-switch MIDI channel selection
- added dip-switch toggle to enable/disable tuning pots
- added dip selectable duty cycle control via MIDI CC/mod wheel, or velocity
- added keyboard-invert dip option

### To-Do:
- update wiring diagram to show dip switches and shift register
- create BOM and upload or link to datasheets
- add dip-switch toggle to swap between normal 8-voice square mode or monophonic dual-channel DAC mode for aribitrary 8-bit wave generation
- add duty adsr?
