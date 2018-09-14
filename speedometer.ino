/*
  This project is really a combination of Do-All-DRO project by loansindi (https://github.com/loansindi/Do-All-DRO)
  and sparkfun electronic's toy car speed trap (https://invent.sparkfun.com/cwists/preview/1117-toy-car-speed-trapx)
*/

//pin defenitions
#define latchPin 5 //Pin connected to ST_CP of 74HC595
#define clockPin 6 //Pin connected to SH_CP of 74HC595
#define dataPin 3 //Pin connected to DS of 74HC595
#define PHOTO_PIN_1 = A0; //pin connected to the first photoresistor
#define PHOTO_PIN_2 = A1; //pin connected to second photoresistor
#define LEDS_PIN = 7; //output pin for LEDs

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
byte displays[] = {DISP_1, DISP_2, DISP_3, DISP_4}; //array used to select specific diplay digit

//some setup variables:
const int SPACING = 67; // Space between photoresistors (in mm)
const int CALIBRATION_SAMPLES = 20;// Number of samples to take for calibration
const unsigned long TIMEOUT = 500000; //Amount of time (microseconds) to wait for next sensor
const unsigned int SHOW_SPEED_TIME = 500; // Amount of time (milliseconds) to show speed before resetting

// Global variables
int threshold_1;
int threshold_2;
int carspeed; //how fast is this thing going?
int dispVal[] = {
  0, 0, 0, 0
};

void setup() {
  //next 3 lines get the shift register all set up
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  //these three are for the display
  for (byte i; i < 4; i++) {
    pinMode(displays[i], OUTPUT);
  }
}

void loop() {
  grabplace(carspeed); //break down the number into individual places
  //next 3 lines: now that we have the places, send out the data
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
    dispVal[i] = newRpm / pow(10, i);// get rid of the trailing 0t
    newRpm -=  dispVal[i] * pow(10, i); //subtract the stuff we've already done
  }
}

void writeDisplay(byte value, byte pin) {
  //this function adapted from Do-All-DRO project by loansindi
  //https://github.com/loansindi/Do-All-DRO

  //next lines turn off the pin that was just on
  //there is no 0th pin, so if we've wrapped around to the 1st pin we turn off the 4th
  if (pin == DISP_1) { //if we started
    digitalWrite(DISP_4, HIGH); //turn off the 4th
  } else {
    digitalWrite(pin - 1, HIGH); //act normal everybody
  }

  //next 3 lines print the value
  shiftOut(dataPin, clockPin, MSBFIRST, value); //tell the shift register what number to print
  digitalWrite(latchPin, HIGH); //data is served
  digitalWrite(latchPin, LOW); //back to normal

  digitalWrite(pin, LOW); //Now turn on the next display
  delay(10); //wait so the display is on for a sec.
}
