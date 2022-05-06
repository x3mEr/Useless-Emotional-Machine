/*
  Modified project of Brandon Clark
  Original:
      https://github.com/braclark/Arduino/blob/master/Useless_Attitude.ino
      https://www.youtube.com/watch?v=nbF8ANHkJv4
      https://www.thingiverse.com/thing:4561617/files
  
  Version 1.0.1
*/

// Board used: Digispark Tiny 85
// Select Board: Digispark (Default - 16.5mhz)
// To use the Digispark Tiny 85 board and SoftRCPulseOut, you must first follow the Arduino IDE setup 
// as explained here: http://digistump.com/wiki/digispark/tutorials/connecting

//Connect servo orange wire to pin 0, red to 5v, and brown to GND
//Also connect a capacitor in between red and brown to be able to move the servo faster
//Without the cap, more than 3 steps per 10 ms may cause a reset

#include <SoftRcPulseOut.h>

#define SERVO_PIN 0

//Connect the switch accross pins 2 and 3
#define SWITCH_PIN 2
#define SWITCH_OUT 3

#define LED_BUILTIN 1

// The range of motion for the servo
#define POS_START 180 // start position - resting position
#define POS_END 45    // end position - hitting the switch

#define N_SWITCHINGS_TO_MAD = 3
#define RAPID_SWITCHINGS_INTERVAL_MS = 4000

// Once it get's mad (more than N_SWITCHINGS_TO_MAD switchings within RAPID_SWITCHINGS_INTERVAL mseconds between each), should the responses be random?
// To set to sequential, start up with the switch off, otherwise the responses will be random

#define N_SEQ 9            // number of sequences in attudude mode

SoftRcPulseOut myservo;
uint8_t pos        = POS_START;
uint8_t target     = POS_START;
uint8_t seq        = 0;
uint8_t rapidCount = 0;
long    lastInput  = 0;

bool    goRandom = false;
bool    fl_attitudeMode = false;

void Seq00(), Seq01(), Seq02(), Seq03(), Seq04(), Seq05(), Seq06(), Seq07(), Seq08(), Seq09();
void (*arr_seq[])() = {Seq00, Seq01, Seq02, Seq03, Seq04, Seq05, Seq06, Seq07, Seq08, Seq09};

void setup() {
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(SWITCH_OUT, OUTPUT);
  digitalWrite(SWITCH_OUT, LOW);

  pinMode(LED_BUILTIN,OUTPUT);			//Built in LED, flashes when ready, on when in attitude mode

  myservo.attach(SERVO_PIN);

  digitalWrite(LED_BUILTIN,HIGH);		// flash the led to show it's ready.
  gotoPercent(50,1,0);					// do a small initialization to show it's ready
  gotoPercent(0,1,0);
  digitalWrite(LED_BUILTIN,LOW);

  if (digitalRead(SWITCH_PIN))			//Reads the switch at startup to set random
    goRandom = false;
  else
    goRandom = true;

  randomSeed(analogRead(5));			//Seed the random function by reading an unconnected analog in pin
}

void loop() {
  if (!digitalRead(SWITCH_PIN)) {
    if (!fl_attitudeMode) {
      if ( (millis()-lastInput) < RAPID_SWITCHINGS_INTERVAL_MS ) {	// if the switch was made within RAPID_SWITCHINGS_INTERVAL_MS mseconds of the last one, increase the number of rapid switchings
        rapidCount++;
        if (rapidCount >= N_SWITCHINGS_TO_MAD) {
          fl_attitudeMode = true;
          digitalWrite(LED_BUILTIN,HIGH);
        }
      }
      else rapidCount = 1;
    }

    if (!fl_attitudeMode)
      seq = 0;
    else {
      if (goRandom == true)
        seq = int(random(1,10));
      else {
        seq++;
        if (seq > N_SEQ)
          seq = 1;
      }
    }

    (*arr_seq[seq])();
    lastInput = millis();
  }
  else
    delay(50);

  if ( fl_attitudeMode && (millis()-lastInput > 10000) ) {
    fl_attitudeMode = false;
    //rapidCount = 0;
    digitalWrite(LED_BUILTIN,LOW);
  }
}

void Seq00(){
  gotoPercent(100,3,0);
  gotoPercent(0,3,0);
}

void Seq01(){
  gotoPercent(100,3,0);
  gotoPercent(0,1,0);
}

void Seq02(){
  gotoPercent(100,1,2000);
  gotoPercent(0,7,0);
}

void Seq03(){
  gotoPercent(100,10,0);
  gotoPercent(75,1,1000);
  gotoPercent(100,10,0);
  gotoPercent(75,10,0);
  gotoPercent(100,10,0);
  gotoPercent(0,5,0);
}

void Seq04(){
  gotoPercent(25,1,500);
  gotoPercent(50,1,500);
  gotoPercent(75,1,2000);
  gotoPercent(100,10,0);
  gotoPercent(0,5,0);
}

void Seq05(){
  gotoPercent(50,5,0);
  gotoPercent(0,1,300);
  gotoPercent(75,5,0);
  gotoPercent(0,1,300);
  gotoPercent(80,5,1000);
  gotoPercent(70,5,0);
  gotoPercent(100,10,0);
  gotoPercent(0,3,0);
}

void Seq06(){
  gotoPercent(100,3,0);
  gotoPercent(80,1,500);
  gotoPercent(100,8,500);
  gotoPercent(80,1,500);
  gotoPercent(100,8,500);
  gotoPercent(80,1,500);
  gotoPercent(50,1,500);
  gotoPercent(25,1,500);
  gotoPercent(0,1,0);
}

void Seq07(){
  gotoPercent(100,2,0);
  for (int i=0; i<10; i++){
    gotoPercent(80,3,0);
    gotoPercent(100,6,0);
    }
  gotoPercent(100,6,2000);
  gotoPercent(50,1,500);
  gotoPercent(0,1,0);
}

void Seq08(){
  for (int i=30; i<=100; i+=10){
    gotoPercent(i,7,0);
    gotoPercent(i-30,4,0);
    }
  gotoPercent(0,5,0);
}

void Seq09(){
  for (int i=0; i<random(10,20); i++){
    gotoPercent(random(0,80),random(1,10),random(0,500));
  }
  gotoPercent(100,random(1,10),random(0,500));
  gotoPercent(0,random(1,10),0);
}

// Required parameters:
//   targetPC   - Target percent (as an integer) 0-100, 0 is starting position, 100 is hitting the switch
//   steps      - Steps per 10 ms update, higher numbers may cause the aurduino to reset without a capacitior
//                3 and under seems to work well without a cap, 11 seems to be the maximum otherwise
//   afterDelay - Adds a delay after completing the move
void gotoPercent(int targetPC, int steps, int afterDelay){
  target = POS_START + ((POS_END - POS_START) * targetPC * .01);
  while(pos != target){
    if (target < pos){
      if (pos-target < steps){
        pos=target;
        } else {
        pos -= steps;
        }
      } else {
      if (target-pos < steps){
        pos=target;
        } else {
        pos += steps;
        }
      }
    myservo.write(pos);
    SoftRcPulseOut::refresh(1);    // generate the servo pulse
    delay(10);
    }
  for (int delayTimer = 0; delayTimer<=afterDelay; delayTimer+=50){ // Refresh the location every 50 ms to keep it alive
    delay(50);
    SoftRcPulseOut::refresh(1);
    }
}
