// Include the AccelStepper Library
#include <AccelStepper.h>
#include <IRremote.h>


// Define step constant
#define MotorInterfaceType 4
#define SPEED 40
#define UPCODE 16755030
#define DOWNCODE 16734630
#define KEEPCODE 4294967295

// Define IR Reciever
int RECV_PIN = 18;
IRrecv irrecv(RECV_PIN);
decode_results results;

// Creates an instance
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
AccelStepper myStepper(MotorInterfaceType, 14, 16, 15, 17);

// will store last time code was received
unsigned long previousMillis = 0;
long lastValue = UPCODE;
long currentValue = UPCODE;

void setup() {
  // set the maximum speed, acceleration factor and initial speed
  myStepper.setMaxSpeed(1000.0);
  myStepper.setAcceleration(1000);
  myStepper.setSpeed(1000);

  // Start the receiver
  irrecv.enableIRIn();

  // Monitor initialization
  Serial.begin(9600);
}

void loop() {

  unsigned long currentMillis = millis();

  //this checks to see if a code has been red
  while (irrecv.decode(&results)) {
    //stores current time code was red
    previousMillis = currentMillis;

    Serial.print("Reciever value:");
    Serial.println(results.value);

    //If remote sends a unique code when button is keep pressed, used the last value
    if (results.value == KEEPCODE){
      currentValue = lastValue;
    } else if (results.value == UPCODE || results.value == DOWNCODE){
      lastValue = currentValue = results.value;
    }

    if (currentValue == UPCODE) {
      Serial.println("Forward");
      myStepper.moveTo(myStepper.currentPosition() - SPEED);
    }
    if (currentValue == DOWNCODE) {
      Serial.println("Backward");
      myStepper.moveTo(myStepper.currentPosition() + SPEED);
    }
    //receive the next value
    irrecv.resume();
  }

  //if its been more than 3000ms since last code
  if (currentMillis - previousMillis >= 3000) {
    // Turn off motor
    digitalWrite(14, LOW);
    digitalWrite(15, LOW);
    digitalWrite(16, LOW);
    digitalWrite(17, LOW);
  } else {
    // Run stepper
    myStepper.run();
  }
}
