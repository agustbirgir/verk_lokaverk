#include <Servo.h> // Sambærilegt og import í python

Servo motor; // bý til tilvik af Servo klasanum
int motor_pinni = 9; // pinninn sem ég nota til að stjórna mótornum

const int TrigPin = 2; 
const int EchoPin = 3; 
float fjarlaegd; // fjarlægð þess sem mælt er

#include "pitches.h"

#define melodyPin 11

// Jingle Bells

int melody[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};

int tempo[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};

void buzz(int targetPin, long frequency, long length) {
  digitalWrite(13, HIGH);
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(13, LOW);

}

void setup() {
  motor.attach(motor_pinni); // segi servo tilvikinu hvaða pinna á að nota

  Serial.begin(9600); 

  pinMode(TrigPin,OUTPUT);
  pinMode(EchoPin,INPUT);

  pinMode(11, OUTPUT); // Buzzer
  pinMode(13, OUTPUT);
}

void loop() {
  // Servo mótorinn getur snúist 90° til vinstri og 90° til hægri, samtals 180°.
  // write fallið tekur á móti gráðunum
  motor.write(10); // mótorinn fer lengst til vinstri
  delay(1000);
  motor.write(60); // mótorinn fer í miðjuna
  delay(1000);

  // sendir út púlsar
  digitalWrite(TrigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin,LOW);

  // mælt hvað púlsarnir voru lengi að berast til baka
  fjarlaegd = pulseIn(EchoPin,HIGH)/58.0; // deilt með 58 til að breyta í cm

  if(fjarlaegd < 0) { // fjarlægð getur ekki verið mínustala
    fjarlaegd = 0;}
    Serial.println(fjarlaegd);
  if (fjarlaegd < 100) {
    Serial.println(" 'Jingle Bells'");
    int size = sizeof(melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / tempo[thisNote];

      buzz(melodyPin, melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration);

    }
  }
}
  

     
