/****************************************************************************************************************************************************
// Sticker Shifter
//
// Written by KBeard kris.beard@mantissolutions.com.au 10/10/2019
// Written for Chaos Divine
//
// Code takes 4 input buttons, defined by buttonPin[] array and sends midi notes starting at 1
// There are 5 LEDs, 4 for output and 1 for power.
// there is a one second delay (defined by debounceDelay) in checking for state change of a button (including off state, resulting in a double delay)
//
// designed to be uploaded to an arduino pro micro
*****************************************************************************************************************************************************/

#include "MIDIUSB.h"

//variables (if using noteOn command/function)
//byte midiCh = 0;
//byte note = 1; //starting note
//byte velocity = 127; //velocity to send

//LEDs
const int NLEDs = 5; //No of LEDs including power
const int LEDPin[NLEDs] = {6,7,8,9,5}; //Pins of LEDs in order, with power LED last.

//buttons
const int NButtons = 4;
const int buttonPin[NButtons] = {10,16,14,15}; //Pins of buttons in order
int buttonCState[NButtons] = {0};              //Stores current value of button
int buttonPState[NButtons] = {0};              //Stores previous value of button

//midi cc command
byte midiCh = 0;
byte controlNum = 4; //midi cc control number
const int midiCCValue[NButtons] = {1,2,3,4}; //Pins of LEDs in order, with power LED last.

//debounce
unsigned long lastDebounceTime[NButtons] = {0}; // the last time the output pin was toggled
unsigned long debounceDelay = 1000;                // the debounce delay; increase if the output bounces

void setup() {

  for (int i = 0; i < NButtons; i++){
//    pinMode(buttonPin[i], INPUT);
    pinMode(buttonPin[i], INPUT_PULLUP);
  }
  
  for (int i = 0; i < NLEDs; i++){
    pinMode(LEDPin[i], OUTPUT);
  }

  digitalWrite(LEDPin[NLEDs-1], HIGH);
  
}

void loop() {
  
  buttons(); //Do buttons subprogram loop

}

void buttons() {

  for (int i = 0; i < NButtons; i++){

    buttonCState[i] = digitalRead(buttonPin[i]);

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (buttonPState[i] != buttonCState[i]) {
            lastDebounceTime[i] = millis();

            if (buttonCState[i] == LOW) {
              //noteOn(midiCh, note + i, velocity); // channel, note, velocity
              controlChange(midiCh, controlNum, midiCCValue[i]); // channel, control, value
              MidiUSB.flush();
              
              digitalWrite(LEDPin[i], HIGH);
            }
             
            if (buttonCState[i] == HIGH) {
              digitalWrite(LEDPin[i], LOW);
            }
      }
      buttonPState[i] = buttonCState[i];
    }
  }

}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}
void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}
