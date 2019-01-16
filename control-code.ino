// Download the IRremote library from here:
// https://github.com/z3t0/Arduino-IRremote
// and put it in your Arduino libraries folder.
#include <IRremote.h>

// FanGates control MOSFET transistors associated with
// thee pins.
int fanGates[5] = {3,5,6,9,10};

// Individual states for each Gate (on 1/off 0)
int fanState[5]  = {0}; 

// Array of power levels for each fan
int fanSpeeds[5] = {0};

// Adjust power levels as-needed based on fan speed
// and strength. You may find you need a much lower
// "low" speed and a much higher "high" speed. Values
// range from 1-255.

// Power level for fan on high speed
int highPower = 180;

// Power level for fan on low speed
int lowPower = 50;

// Millisecond delay storage vars for the IR receiver
// and fan speed control timing
unsigned long prevIrMs = 0; 
unsigned long prevFanMs[5] = {0}; 

// Duration intervals for IR read and fan motor speeds
int irInterval = 1000;
int fanInterval = 2000; 

// Receive input from IR remote on this pin
int irPin = 11;

// Each fan is controlled invidually by its own button on the remote
// control. There's also one button that turns off all of the fans.
//  You'll need to update these values to match the raw data
// coming from your particular remote control. 
//
// See the build guide and ir-read-utility.ino for instructions on how
// to obtain these values.

// Raw ints from the IR remote
int fanBtns[5] = {16753245,16736925,16769565,16720605,16712445};
int allOffBtn = 14535; // This button turns off all fans

// Set up the receiver
IRrecv irrecv(irPin);
decode_results results;

void setup() {
  // Start the IR receiver 
  irrecv.enableIRIn();

  // Set pin modes and initial state for Fan gates
  for (int i = 0; i < 5; i = i + 1) {
    pinMode(fanGates[i], OUTPUT);
    digitalWrite(fanGates[i], LOW);
  }
}

void loop() {
  // Run control loop on each fan
  for (int i = 0; i < 5; i = i + 1) {
    control_fan(i);
  }

  // Toggle fan state if we receive IR input
  if (irrecv.decode(&results)) {
    toggle_fan_state(results.value);
    unsigned long currentMs = millis();

    // Delay reading more IR input for the duration of
    // the IR interval - basically debouce IR
    if (currentMs - prevIrMs >= irInterval) {
      // Update millisecond storage var and restart
      // reading of IR input
      prevIrMs = currentMs;
      irrecv.resume();
    }
  }
}

// Set fan states based on IR input.
void toggle_fan_state(int fan) {
  if (fan == allOffBtn) {
    for (int i = 0; i < 5; i = i + 1) {
      fanState[i] = 0;
    }
  } else {  
    // Find the fan gate that corresponds to the button.
    int index = 100; // Default out of range value
    for (int i = 0; i < 5; i = i + 1) {
      if (fanBtns[i] == fan) {
        index = i;
      }
    }
      
    // Ignore any uncomfigured buttons (index still out
    // of range)
    if (index < 100) {
      fanState[index] = (fanState[index] == 0)? 1 : 0;  
    }
  }
}

// Control fan gates based on timing and their state as
// set via remote input.
void control_fan(int fan) {
  // If the fan gate has been toggled on, get current
  // millisecond timestamp and set fan motor to either
  // high speed or low speed depending on timing.
  if(fanState[fan] == 1) {
    unsigned long currentMs = millis();
    if (currentMs - prevFanMs[fan] >= fanInterval) {
      prevFanMs[fan] = currentMs;
      if(fanSpeeds[fan] != highPower) {
        fanSpeeds[fan] = highPower;
        analogWrite(fanGates[fan], highPower);
        fanInterval = random(2000,4000);
      } else {
        fanSpeeds[fan] = lowPower;
        analogWrite(fanGates[fan], lowPower);
        fanInterval = random(1000,2000);
      }
    }
  // If the fan gate has been toggled off, set its
  // pin to LOW, toggling the gate.
  } else {
    if(fanSpeeds[fan] > 0) {
      fanSpeeds[fan] = 0;
      digitalWrite(fanGates[fan], LOW);
    }
  }
}