// LED pins
int redLed = 2;
int greenLed = 3;
int yellowLed = 4;

// buzzer on pin 4
int buzzer = 5;

// line trace on pin 5
int lineSensor = A1;
int lineSensorValue = 0;

// ultrasonic pins
int ultrasonicEcho = 6;
int ultrasonicTrig = 7;
// variables for storing measurements from ultrasonic sensor
long duration;
int distance;

void setup() {
  Serial.begin(9600);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ultrasonicEcho, INPUT);
  pinMode(ultrasonicTrig, OUTPUT);

  // calulcate the distance at start up now (outside the loop)
  distance = calDistance(ultrasonicTrig, ultrasonicEcho);
  lineSensorValue = analogRead(lineSensor);
  
  digitalWrite(greenLed, HIGH);
}

void loop() {

  // rule at the edge - LED lights are controlled at the edge by the Pi now
  if (Serial.available()) {
    // read in a command over serial
    String ledCommand = Serial.readString();
    // if the taken command is read...
    if (ledCommand == "taken") {
      // set the red LED to high and turn off the green and yellow LEDs
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      digitalWrite(yellowLed, LOW);
      // disable buzzer tone
      noTone(buzzer);
    // else if the free command is read...
    } else if (ledCommand == "free") {
      // set the green LED to high and turn off the yellow and red LEDs
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
      digitalWrite(yellowLed, LOW);
      // disable the buzzer tone
      noTone(buzzer);
    }
  }

  // calculate a new distance each iteration of the loop
  int newDistance = calDistance(ultrasonicTrig, ultrasonicEcho);

  // if the new distance is greater than 8 (no car is in the parking bay)
  if (newDistance > 8) {
    // only write a record to serial if there is a change in distance
    // and only if the distance is greater than +/- 1
    if (newDistance != distance) {
      int diff = distance - newDistance;
      if (abs(diff) > 1 | abs(diff) < 1) {
        // pass record to serial
        Serial.print("free,");
        Serial.print(distance);
        Serial.println(",correct");
      }
    }
    // assign the distance var with the value of the new distance
    distance = newDistance;
    delay(500);
  // else there must be a car in the parking bay
  } else {
    // only write a record to serial if there is a change in distance
    // and only if the distance is greater than +/- 1
    if (newDistance != distance) {
      int diff = distance - newDistance;
      if (abs(diff) > 1 | abs(diff) < 1) {
        // pass record to serial
        Serial.print("taken,");
        Serial.print(distance);
        Serial.println(",correct");
      }
    }
    // assign the distance var with the value of the new distance
    distance = newDistance;
    delay(500);
  }

  // read the line sensor value - check if anything is over the line
  int newLineSensorValue = analogRead(lineSensor);
  // check if a car is parked over the line (parked incorrectly)
  if (newLineSensorValue <= 600) {
    // turn off the green and red LEDs
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, LOW);
    // while a car is parked over the line sensor
    while (newLineSensorValue <= 600)
    {
      // make the buzzer plays a tone
      tone(buzzer, 600);
      // set the yellow LED to flicker on and off
      digitalWrite(yellowLed, HIGH);
      delay(500);
      digitalWrite(yellowLed, LOW);
      delay(500);

      if (newLineSensorValue != lineSensorValue) {
        // pass details to serial
        Serial.print("taken,");
        Serial.print(distance);
        Serial.println(",incorrect");
      }

      // read the line sensor's values for any changes
      newLineSensorValue = analogRead(lineSensor);
      lineSensorValue = newLineSensorValue;
    }
    delay(1000);
  }
}

// func for calculating the distance of obj from the ultrasonic sensor
int calDistance(int trig, int echo) {
  // ultrasonic sensor triggeing
  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(echo, LOW);

  // use the echo sensor on the ultrasonic to measure the duration of sound
  long myDuration = pulseIn(ultrasonicEcho, HIGH);
  // use the duration to calculate the distance of an object (in cm) from the sensor
  // using the duration of sound
  int myDistance = myDuration * 0.034 / 2;
  return myDistance;
}
