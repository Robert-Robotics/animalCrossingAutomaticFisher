// For Teensy 3.2
// By: Robert
// Description: Catches fish for you in Animal Crossing

// Setup pin values
const int relayPin = 14;
const int ledPin = 13;
const int analogPin = A1;

void setup() {
  // Set pinmodes
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(analogPin, INPUT);

  // Begin serial communication
  Serial.begin(9600);
  // Delay until serial starts
  while (!Serial) {}
  // Write relay pin to high (breaks circuit)
  digitalWrite(relayPin, HIGH);
  digitalWrite(ledPin, LOW);
}

// Initialize time values and fish notifier
unsigned long startMillis;
unsigned long stopMillis;
int fishFlag = 0;
int incomingByte = 0;
int value = 0;

// Create a function to press the button for 50 ms
void pressButton() {
  digitalWrite(relayPin, LOW);
  delay(50);
  digitalWrite(relayPin, HIGH);
  delay(10);
}


void loop() {
  // If you press enter in serial monitor, execute button press function
  // (Cast Line)
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    pressButton();
  }

  // Read voltage divider to see if the motor is going off indicating a fish is on the line
  value = analogRead(analogPin);
  // Value of ~400 is read when motor is active, so go into if statement
  if (value > 380) {
    // Set initial time when fish is detected
    startMillis = millis();
    // Trap in while loop until vibrations stops
    while (value > 380) {
      value = analogRead(analogPin);
      // If you would like to see the values that the microcontroller is picking up, uncomment these
      //      Serial.print("Value: ");
      //      Serial.println(value);
    }
    // Motor has stopped vibrating, so hit "lap" on stopwatch
    stopMillis = millis() - startMillis;
    // Print vibration duration
    Serial.print("Stop Millis: ");
    Serial.println(stopMillis);
    // Set fish flag to 1 to show the motor has gone off
    fishFlag = 1;
  }
  // If the vibration duration is around 190-200ms then the fish has taken the bait
  // These bounds were set fairly arbitrarily just to weed out false positives
  if (stopMillis < 225 && fishFlag && stopMillis > 150) {
    stopMillis = stopMillis - millis();
    // Press button to reel in fish, and reset fish flag variable
    pressButton();
    fishFlag = 0;
    delay(2000);
  }
}
