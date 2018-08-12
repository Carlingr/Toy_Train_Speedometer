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
//numbers is used to call specific digits

byte numbers[] = {
  zero, one, two, three, four, five, six, seven, eight, nine
};

//pins used to select the digit.
#define DIG_1 2
#define DIG_2 4
#define DIG_3 7
#define DIG_4 8

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
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

void writeDisplay() {
  
  //this function adapted from Do-All-DRO project by loansindi
  //https://github.com/loansindi/Do-All-DRO
  
  digitalWrite(latchPin, LOW); //get ready to write the data
  grabplace(carspeed);
  shiftOut(dataPin, clockPin, MSBFIRST, numbers[dispVal[0]]);
  shiftOut(dataPin, clockPin, MSBFIRST, numbers[dispVal[1]]);
  shiftOut(dataPin, clockPin, MSBFIRST, numbers[dispVal[2]]);
  shiftOut(dataPin, clockPin, MSBFIRST, numbers[dispVal[3]]);
  digitalWrite(latchPin, HIGH); //tell the shift regesters we're ready
}







