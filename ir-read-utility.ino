// Download the IRremote library from here:
// https://github.com/z3t0/Arduino-IRremote
// and put it in your Arduino libraries folder.
#include <IRremote.h>

// Receive input from IR remote on this pin
int irPin = 11;

// Set up the receiver
IRrecv irrecv(irPin);
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    delay(1000);
  }
}
