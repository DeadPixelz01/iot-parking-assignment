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
}

void loop() {
  // read the line sensor value - check if anything is over the line
  lineSensorValue = analogRead(lineSensor);

  // ultrasonic sensor triggeing
  digitalWrite(ultrasonicTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasonicTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonicTrig, LOW);

  // use the echo sensor on the ultrasonic to measure the duration of sound
  duration = pulseIn(ultrasonicEcho, HIGH);
  // use the duration to calculate the distance of an object (in cm) from the sensor
  // using the duration of sound
  distance = duration * 0.034 / 2;
  // pass distance (cm) to serial
  Serial.println(distance);

  if (distance > 8) {
    digitalWrite(greenLed, HIGH);
    digitalWrite(redLed, LOW);
    digitalWrite(yellowLed, LOW);
    noTone(buzzer);
    delay(1000);
  } else {
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, HIGH);
    digitalWrite(yellowLed, LOW);
    noTone(buzzer);
    delay(1000);
  }

  if (lineSensorValue <= 600) {
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, LOW);
    while (lineSensorValue <= 600)
    {
      Serial.println(lineSensorValue, DEC);
      tone(buzzer, 600);
      digitalWrite(yellowLed, HIGH);
      delay(500);
      digitalWrite(yellowLed, LOW);
      delay(500);
      lineSensorValue = analogRead(lineSensor);
    }
  }
}
