#include <OneWire.h>

OneWire  ds(52);  // on pin 11 (a 4.7K resistor is necessary)
double tempAvg = 0;
double prevTemp = 0;
int count;

long previousMillis = 0;
long interval = 100;
bool _white = false;

// Include files so that we are able to interface with the touch screen
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>

// Control pins for the LCD
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// Pin selection for touch
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

// Easy to understand color Codes
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// Touchscreen Pressure Values
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

// Setup the LCD
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup(void) {
  Serial.begin(9600);
  //Start cross communication with the LCD

  // Initiate communcations
  tft.begin(0x9341); // SDFP5408
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setRotation(45);
  tft.setTextSize(3); 

}

void loop(void) {

  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius;
  unsigned long currentMillis = millis();

  if ( !ds.search(addr)) {
    tft.setCursor (0, 0);
    tft.print("Temp = ");
'    if (currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;
      if (_white) {
        tft.fillRect(140, 0, 15, 30, BLACK);
        _white = false;
      }
      else{
        tft.fillRect(140, 0, 15, 30, WHITE);
        _white = true;
      }
    }
    tempAvg = tempAvg/count;

    if (tempAvg != prevTemp) {
      updateData(tft, 140, 0, tempAvg, prevTemp, " C");
    }

    prevTemp = tempAvg;
    count = 0;
    tempAvg = 0;

    ds.reset_search();
    delay(25);
    return;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, use ds.write(0x44,1) with parasite power on at the end
  delay(400);     // maybe 750ms is enough, maybe not

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
}
