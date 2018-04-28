// Libraries used
#include <OneWire.h>
#include <Keypad.h>
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <menuScreen.h>

// Object and variables need for avg temperature calculation
OneWire  ds(52);  // on pin 11 (a 4.7K resistor is necessary)
double tempAvg = 0;
double prevTemp = 0;
int count;

// Control pins for the LCD
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// Easy to understand color Codes
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// Initialize screen objects used
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
String textToDisplay[3] = {"Hello", "My name is Abhishek", "How has your day been?"};
menuScreen interface("interface", textToDisplay, 2);

// Initialize objects for numpad
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {22, 24, 26, 28}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {30, 32, 34, 36}; //connect to the column pinouts of the keypad
Keypad numpad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Initalize variables used for displaying time, temp, etc
char backPress;
int inputs[2] = {0, 0};
int desiredTemp;
int biliTime;
String desTemp;
String desTime;
bool validNums = false;
int out;

// Initialize variables and objects for time dependent events (millis)
long previousMillis = 0; // used to check whether the interval has been exceeded
long biliInterval = 60000; // one minute

// Pin numbers and variables for bililight control
#define biliOne 51
#define biliTwo 53
bool lightsChecked = false;

//Other defined functions
void PIcontrol(int curTemp, int desiredTemp);

void setup(void) {
  // Initiate communications with serial
  Serial.begin(9600);
  // Initiate communications with screen
  tft.begin(0x9341); // SDFP5408

  // set bililight pin modes as output
  pinMode(biliOne, OUTPUT);
  pinMode(biliTwo, OUTPUT);
}

void loop(void) {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius;

  // initial temp + time input and reset if the back button is pressed
  while(!validNums) {

    // turns bililights off if they were on
    digitalWrite(biliOne, LOW);
    digitalWrite(biliTwo, LOW);
    lightsChecked = false;
      
    // reset screen
    tft.fillScreen(BLACK);
    tft.setTextColor(WHITE);
    tft.setRotation(45);
  
    // get inputs + print to serial to make sure it works
    interface.getInputs(tft, numpad, 0, 0, inputs);
    desiredTemp = inputs[0];
    biliTime = inputs[1];
    if (biliTime > 0) {
      desTime = timeConverter(biliTime); 
      biliTime += 1;
    }

    else { desTime = timeConverter(biliTime); }
    Serial.println("Input temp = " + (String)desiredTemp);
    Serial.println("Input time = " + (String)biliTime);

    if (desiredTemp <= 37) { 
      // changes validNums so that the loop is not entered again
      validNums = true;
      
      // print the reference temperature (input temp)
      tft.fillRect(0, 0, 320, 240, BLACK);
      desTemp = "Input Temperature:   " + (String)desiredTemp + " C";
      interface.printStaticLine(tft, desTemp, WHITE, 0, 0);
  
      // print bililight initial time
      interface.printStaticLine(tft, "Input Time: ", WHITE, 0, 60);
      interface.printStaticLine(tft, desTime, CYAN, 135, 60);
      tft.setTextColor(WHITE); 
    }

    else {
      // resets screen to all black and prints an error message to the screen
      tft.fillRect(0, 0, 320, 240, BLACK);
      tft.setCursor(0, 0);
      tft.setTextColor(WHITE);
      tft.print("That is not a valid       temperature. Please enter a temperature between 0   and 37 degrees Celsius.   Press A to continue.");

      // error screen is not exited until the 'A' is pressed
      char nextScreen = numpad.getKey();
      while (!(nextScreen == 'A')) {
        nextScreen = numpad.getKey(); 
      }
    }
  }

  // checks if the back button bas been pressed
  backPress = numpad.getKey();
  // if the button has been pressed, we re-enter the while loop above
  if (backPress == 'B') { validNums = false; }
  
  // code for the temperature sensors
  if ( !ds.search(addr)) {
    tft.setCursor (0, 30);
    tft.setTextColor(WHITE);
    tft.print("Temp = ");

    // calculates the avg temp
    tempAvg = tempAvg/count;

    // if the temp has changed, update the temperature on the screen
    if (tempAvg != prevTemp) {
      updateData(tft, 140, 30, tempAvg, prevTemp, " C");
    }


    /*
     * 
     * Here is where the PWM code will go
     */
     PIcontrol(tempAvg, desiredTemp);

    // resets the calculations
    prevTemp = tempAvg;
    count = 0;
    tempAvg = 0;

    ds.reset_search();
    delay(25);
    return;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // interface conversion, use ds.write(0x44,1) with parasite power on at the end
  // delay(400);     // maybe 750ms is enough, maybe not

  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad


  for ( i = 0; i < 9; i++) { data[i] = ds.read(); }
  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } 
  else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  } 
  celsius = (float)raw / 16.0;
  if (!isnan(celsius)) {
    tempAvg += celsius;
    count ++;
  }

  // bililight timer
  unsigned long currentMillis  = millis();
  if (currentMillis - previousMillis > biliInterval && biliTime > 0) {
    previousMillis = currentMillis;
    biliTime -= 1;
    interface.printStaticLine(tft, timeConverter(biliTime), CYAN, 135, 60);
  }

  // turns bililights on and off (can be made more efficient)
  if (biliTime > 0 && !lightsChecked) {
    digitalWrite(biliOne, HIGH);
    digitalWrite(biliTwo, HIGH);
    lightsChecked = true;
  }

  else if (biliTime <=0){
    digitalWrite(biliOne, LOW);
    digitalWrite(biliTwo, LOW);
  }
  
}

void PIcontrol(int curTemp, int desiredTemp){
    int error = desiredTemp - curTemp; 
    out = out + (100*error)/5
    //constrain to 255 
    analogWrite(out, 200);
    delay(50);
}

