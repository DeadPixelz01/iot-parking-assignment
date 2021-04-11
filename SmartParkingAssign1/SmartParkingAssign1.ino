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
  Serial.setTimeout(1);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ultrasonicEcho, INPUT);
  pinMode(ultrasonicTrig, OUTPUT);
}

void loop() {
  // read the line sensor value - check if anything is over the line
  lineSensorValue = analogRead(lineSensor);

  // ultrasonic sensor triggeing
  digitalWrite(ultrasonicTrig, LOW);
  delayMicroseconds(5);
  digitalWrite(ultrasonicTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonicTrig, LOW);

  // use the echo sensor on the ultrasonic to measure the duration of sound
  duration = pulseIn(ultrasonicEcho, HIGH);
  // use the duration to calculate the distance of an object (in cm) from the sensor
  // using the duration of sound
  distance = duration * 0.034 / 2;

  // if the distance is greater than 8 (no car is in the parking bay)
  if (distance > 8) {
    // set the green LED to high and turn off the yellow and red LEDs
    digitalWrite(greenLed, HIGH);
    digitalWrite(redLed, LOW);
    digitalWrite(yellowLed, LOW);
    // disable the buzzer tone
    noTone(buzzer);
    delay(1000);
  // else there must be a car in the parking bay
  } else {
    // set the red LED to high and turn off the green and yellow LEDs
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, HIGH);
    digitalWrite(yellowLed, LOW);
    // disable buzzer tone
    noTone(buzzer);
    // pass distance (cm) to serial
    Serial.print("Taken,");
    Serial.print(distance);
    Serial.println(",Correct");
    // read the string message back from the python program
    Serial.readString();
    delay(1000);
  }

  // check if a car is parked over the line (parked incorrectly)
  if (lineSensorValue <= 600) {
    // turn off the green and red LEDs
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, LOW);
    // while a car is parked over the line sensor
    while (lineSensorValue <= 600)
    {
      // read the string message back from the python program
      Serial.readString();
      // make the buzzer plays a tone
      tone(buzzer, 600);
      // set the yellow LED to flicker on and off
      digitalWrite(yellowLed, HIGH);
      delay(500);
      digitalWrite(yellowLed, LOW);
      delay(500);
      // pass details to serial
      Serial.print("Taken,");
      Serial.print(distance);
      Serial.println(",Incorrect");
      // read the line sensor's values for any changes
      lineSensorValue = analogRead(lineSensor);
    }
  }
}
