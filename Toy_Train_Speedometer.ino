/*
  This project is really a combination of Do-All-DRO project by loansindi (https://github.com/loansindi/Do-All-DRO)
  and sparkfun electronic's toy car speed trap (https://invent.sparkfun.com/cwists/preview/1117-toy-car-speed-trapx)
*/

//pin defenitions
#define dataPin 3 //Pin connected to DS of 74HC595
#define latchPin 5 //Pin connected to ST_CP of 74HC595
#define clockPin 6 //Pin connected to SH_CP of 74HC595
#define PHOTO_PIN_1 A0 //pin connected to the first photoresistor
#define PHOTO_PIN_2 A1 //pin connected to second photoresistor
#define LEDS_PIN 12 //output pin for LEDs

// These bytes are trial-and-error digits for the seven segment displays I have.
#define zero  B11111100
#define one   B01100000
#define two   B11011010
#define three B11110010
#define four  B01100110
#define five  B10110110
#define six   B10111110
#define seven B11100000
#define eight B11111110
#define nine  B11100110

byte digits[] = {zero, one, two, three, four, five, six, seven, eight, nine}; //digits is used to call specific displays

//pins used to select the digit.
#define DISP_1 2
#define DISP_2 4
//#define DISP_3 7
//#define DISP_4 8
byte displays[] = {DISP_1, DISP_2}; //array used to select specific diplay digit

//some setup variables:
const int SPACING = 2; // Space between photoresistors (in inches)
const int CALIBRATION_SAMPLES = 20;// Number of samples to take for calibration
const unsigned long TIMEOUT = 5000000; //Amount of time (microseconds) to wait for next sensor
const unsigned long SHOW_SPEED_TIME = 500000; // Amount of time (microseconds) to show speed before resetting

// Global variables
int threshold_1;
int threshold_2;
int carspeed; //how fast is this thing going?
int dispVal[] = {
  B00000010, B00000010
}; //shows dashes on the display on boot.

void setup() {
  Serial.begin(9600);
  //next 3 lines get the shift register all set up
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  //next three are for the display
  for (byte i; i < 4; i++) {
    pinMode(displays[i], OUTPUT);
  }

  pinMode(LEDS_PIN, OUTPUT); //set up LED pin
  //thus ends pin defs.
  calibrateSensors(); //get a feel for the environment, yanno?
}

void loop() {
  //NB: this code expects pull *DOWN* resistors. connect the resistor to the neg rail
  int photo_light_1;
  int photo_light_2;
  unsigned long time_1;

  // Read light from sensors
  photo_light_1 = analogRead(PHOTO_PIN_1);

  // See if object passed in front of sensor 1
  if ( photo_light_1 <= threshold_1 ) {
    // Save the time at which the object was seen
    time_1 = micros();

    // Now we wait for object to pass in front of sensor 2
    while ( true ) {

      // If sensor 2 sees it, show speed as m/s
      photo_light_2 = analogRead(PHOTO_PIN_2);
      if ( photo_light_2 <= threshold_2 ) {
        showSpeed(time_1);
        break;
      }

      // Else, exit if enough time has passesd
      if ( (micros() - time_1) > TIMEOUT ) {
        showError(1);
        break;
      }
    }
  }
  writeDisplay();
}

void showSpeed(unsigned long t1) {

  unsigned long t2 = micros();
  unsigned long spd;

  // Take the current time and calculate the object's speed
  //this will be in inches per second *10 so we don't have to deal with floats
  spd = ((SPACING * 10000000) / (t2 - t1));
  Serial.print("speed = ");
  Serial.println(spd);
  //now break down the number into individual places
  //this function adapted from Do-All-DRO project by loansindi
  //https://github.com/loansindi/Do-All-DRO
  bool dot = true;
  if (spd > 990) { //if it's going faster than 99 inches per sec
    showError(2); //throw an error, because it shouldn't be going that fast
  }
  else {
    if (spd > 99) { //faster than 9.9 ips
      spd /= 10; //divide by 10 so it fits on the disp
      dot = false; //note that we have to move the dot.
    }

    // we need to build our number
    // by 'knocking off' each place value we can turn our int into discrete digits
    // this works because integer math is kind of silly

    dispVal[0] = digits[spd / 10];// get rid of the trailing 0s
    spd -=  (spd / 10) * 10; //subtract the stuff we've already done
    Serial.println(spd);
    dispVal[1] = digits[spd];// get rid of the trailing 0s

    //next hunk deals with putting the decimal in it's place.
    if (dot) { //if the number is less than 99
      bitSet(dispVal[0], 0); //sets the DP bit of the relevant output byte
    }
  }
}

void showError(byte error) {
  /*Error codes currently in use:
     E1 Timeout
     E2 Speed too High
  */
  Serial.print("Error");
  Serial.println(error);
  dispVal[0] = B10011110; //prints an E in the 1st space
  dispVal[1] = digits[error]; //Prints the error code in the 2nd space
}

void writeDisplay() {
  /*this function will show the byte values in dispVal[] on the display.
    dispVal[] holds binary to be printed to the display, if you put ascii
    text in it you will be confused.
    Also not that LOW is on and HIGH is off concerning the display digits
    because charlieplexing*/
  for (byte i = 0; i < 2; i++) {// loop through the display digits.
    //next 3 lines send the value of the digit to the shift register
    shiftOut(dataPin, clockPin, LSBFIRST, dispVal[i]); //tell the shift register what number to print
    digitalWrite(latchPin, HIGH); //data is served
    digitalWrite(latchPin, LOW); //back to normal

    //next 3 lines show the digit on the display
    digitalWrite(displays[i], LOW); //Now turn on the digit
    delay(10); //wait so the display is on for a tick (or 10).
    digitalWrite(displays[i], HIGH); //Now turn off the digit
  }
}

void calibrateSensors() {

  unsigned int avg_lit_1 = 0;
  unsigned int avg_lit_2 = 0;
  unsigned int avg_unlit_1 = 0;
  unsigned int avg_unlit_2 = 0;
  int i;

  // Turn on LEDs and wait a few ms for them to be fully on
  digitalWrite(LEDS_PIN, HIGH);
  delay(150);

  // Find average light value for sensors with LEDs on
  for ( i = 0; i < CALIBRATION_SAMPLES; i++ ) {
    avg_lit_1 += analogRead(PHOTO_PIN_1);
    avg_lit_2 += analogRead(PHOTO_PIN_2);
  }
  avg_lit_1 /= CALIBRATION_SAMPLES;
  avg_lit_2 /= CALIBRATION_SAMPLES;

  // Turn off LEDs and wait a few ms for them to be fully off
  digitalWrite(LEDS_PIN, LOW);
  delay(150);

  // Find average light value for sensors with LEDs off
  for ( i = 0; i < CALIBRATION_SAMPLES; i++ ) {
    avg_unlit_1 += analogRead(PHOTO_PIN_1);
    avg_unlit_2 += analogRead(PHOTO_PIN_2);
  }
  avg_unlit_1 /= CALIBRATION_SAMPLES;
  avg_unlit_2 /= CALIBRATION_SAMPLES;

  // Calculate thresholds (half way between lit and unlit values)
  threshold_1 = ((avg_lit_1 - avg_unlit_1) / 2) + avg_unlit_1;
  threshold_2 = ((avg_lit_2 - avg_unlit_2) / 2) + avg_unlit_2;
  // Turn on output LEDs and wait for them to be fully on

  Serial.println("calibrating");
  Serial.print(threshold_1);
  Serial.print(" , ");
  Serial.println(threshold_2);
  Serial.println("-------------------");

  digitalWrite(LEDS_PIN, HIGH);
  delay(150);
}
