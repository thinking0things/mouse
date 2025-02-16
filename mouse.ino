#include "BleMouse.h"    // Include the Bluetooth Mouse library

// Create a BluetoothMouse object
BleMouse bleMouse;

// Define the pins for the joystick and the switch
const int vxPin = 32;     // Joystick x-axis (horizontal) -> GPIO 32 (analog)
const int vyPin = 33;     // Joystick y-axis (vertical) -> GPIO 33 (analog)
const int swPin = 25;     // Joystick button switch -> GPIO 25 (digital)

// Sensitivity for cursor movement
int sensitivity = 20;  // Adjust this value to control cursor speed

// Define the new center values and dead zone for joystick movement
int centerValue = 2000;  // Central value based on your joystick's calibration
int deadZone = 200;      // Wider dead zone to prevent small movements

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Begin Bluetooth Mouse
  bleMouse.begin();

  Serial.println("Bluetooth Mouse is ready to pair");

  // Configure analog pins with pull-down resistors if needed
  pinMode(vxPin, INPUT_PULLDOWN);
  pinMode(vyPin, INPUT_PULLDOWN);

  // Configure the joystick button pin
  pinMode(swPin, INPUT_PULLUP);  // Joystick button uses pull-up resistor
}

void loop() {
  if (bleMouse.isConnected()) {
    // Read joystick values
    int vxValue = analogRead(vxPin);
    int vyValue = analogRead(vyPin);
    int swValue = digitalRead(swPin);  // Read joystick button state

    // Print joystick values for debugging
    Serial.print("VX: ");
    Serial.print(vxValue);
    Serial.print(" | VY: ");
    Serial.println(vyValue);

    // Center the readings around the new center value (3000)
    int xDeviation = vxValue - centerValue;
    int yDeviation = vyValue - centerValue;

    int deltaX = 0;
    int deltaY = 0;

    // Map the deviations to cursor movement if outside the widened dead zone
    if (abs(xDeviation) > deadZone) {
      deltaX = map(xDeviation, -2048, 2047, -sensitivity, sensitivity);
    }

    if (abs(yDeviation) > deadZone) {
      deltaY = map(yDeviation, -2048, 2047, -sensitivity, sensitivity);
    }

    // Move the cursor based on joystick input
    bleMouse.move(deltaX, -deltaY);  // Invert deltaY for natural movement

    // Handle joystick button press for mouse click
    if (swValue == LOW) {  // Button is pressed (active low)
      if (!bleMouse.isPressed(MOUSE_LEFT)) {
        bleMouse.press(MOUSE_LEFT);  // Press left mouse button
      }
    } else {
      if (bleMouse.isPressed(MOUSE_LEFT)) {
        bleMouse.release(MOUSE_LEFT);  // Release left mouse button
      }
    }

    delay(20);  // Smoother movement
  } else {
    Serial.println("Mouse not connected");
    delay(1000);  // Check every second if the Bluetooth mouse is connected
  }
}



