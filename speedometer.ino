/*
  This project is really a combination of Do-All-DRO project by loansindi (https://github.com/loansindi/Do-All-DRO)
  and sparkfun electronic's toy car speed trap (https://invent.sparkfun.com/cwists/preview/1117-toy-car-speed-trapx)
*/

#define latchPin 5 //Pin connected to ST_CP of 74HC595
#define clockPin 6 //Pin connected to SH_CP of 74HC595
#define dataPin 3 //Pin connected to DS of 74HC595

int carspeed;
int dispVal[] = {
  0, 0, 0, 0
};


// These bytes are trial-and-error digits for the seven segment displays I have.
#define zero B11111100
#define one B01100000
#define two B11011010
#define three B11110010
#define four B01100110
#define five B10110110
#define six B10111110
#define seven B11100000
#define eight B11111110
#define nine B11100110
byte digits[] = {zero, one, two, three, four, five, six, seven, eight, nine}; //digits is used to call specific displays

//pins used to select the digit.
#define DISP_1 2
#define DISP_2 4
#define DISP_3 7
#define DISP_4 8
byte displays[] = {DISP_1, DISP_2, DISP_3, DISP_4};

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  for (byte i; i < 4; i++) {
    pinMode(displays[i], OUTPUT);
  }
}

void loop() {
  grabplace(carspeed);
  for (byte i; i < 4; i++) {
    writeDisplay(dispVal[i], displays[i]);
  }
}

void grabplace(int newRpm) {
  //this function adapted from Do-All-DRO project by loansindi
  //https://github.com/loansindi/Do-All-DRO

  // we need to build our number
  // by 'knocking off' each place value we can turn our int into discrete digits
  // this works because integer math is kind of silly

  for (int i = 3; newRpm >= 0; i--) {
    dispVal[i] = newRpm / pow(10, i);
    newRpm -=  dispVal[i] * pow(10, i);
  }
}

void writeDisplay(byte value, byte pin) {

  //this function adapted from Do-All-DRO project by loansindi
  //https://github.com/loansindi/Do-All-DRO

  //off
  if (pin == DISP_1) {
    digitalWrite(DISP_4, HIGH); // state
  } else {
    digitalWrite(pin - 1, HIGH); // state
  }

  //on
  digitalWrite(pin, LOW); //state

  shiftOut(dataPin, clockPin, MSBFIRST, value);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
}
