#include <OneWire.h>



// OneWire DS18S20, DS18B20, DS1822 Temperature Example

//

// http://www.pjrc.com/teensy/td_libs_OneWire.html

//

// The DallasTemperature library can do all this work for you!

// http://milesburton.com/Dallas_Temperature_Control_Library



OneWire  ds(11);  // on pin 11 (a 4.7K resistor is necessary)

double tempAvg = 0;
double prevTemp = 0;
int count;
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



// Setup the touch screen

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);



void setup(void) {

  Serial.begin(9600);

    //Start cross communication with the LCD



  // Initiate communcations

  tft.begin(0x9341); // SDFP5408
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
}



void loop(void) {

  byte i;

  byte present = 0;

  byte type_s;

  byte data[12];

  byte addr[8];

  float celsius, fahrenheit;



  if ( !ds.search(addr)) {

    //Serial.println("No more addresses.");
    
    tft.setTextSize(2); 

    tft.setCursor (0, 90);
    
    tft.print("Avg Temp = ");

//    tft.fillRect(130, 90, 70, 30, BLACK);


    tempAvg = tempAvg/count;

    if (tempAvg != prevTemp) {
      tft.setTextColor(BLACK);
      tft.setCursor (130, 90);
      tft.print(prevTemp);
      tft.setCursor(130, 90);
      tft.setTextColor(WHITE);
      tft.print(tempAvg);
    }

    //tft.setCursor (0, 110);

    //tft.print("Celsius");

    prevTemp = tempAvg;
    
    count = 0;

    tempAvg = 0;

    ds.reset_search();

    delay(25);
    
    return;

  }



  Serial.print("ROM =");

  for( i = 0; i < 8; i++) {

    Serial.write(' ');

    Serial.print(addr[i], HEX);

  }



  if (OneWire::crc8(addr, 7) != addr[7]) {

      Serial.println("CRC is not valid!");

      return;

  }

  Serial.println();



  // the first ROM byte indicates which chip

  switch (addr[0]) {

    case 0x10:

      Serial.println("  Chip = DS18S20");  // or old DS1820

      type_s = 1;

      break;

    case 0x28:

      Serial.println("  Chip = DS18B20");

      type_s = 0;

      break;

    case 0x22:

      Serial.println("  Chip = DS1822");

      type_s = 0;

      break;

    default:

      Serial.println("Device is not a DS18x20 family device.");

      return;

  } 



  ds.reset();

  ds.select(addr);

  ds.write(0x44);        // start conversion, use ds.write(0x44,1) with parasite power on at the end



  delay(400);     // maybe 750ms is enough, maybe not

  // we might do a ds.depower() here, but the reset will take care of it.



  present = ds.reset();

  ds.select(addr);    

  ds.write(0xBE);         // Read Scratchpad



  Serial.print("  Data = ");

  Serial.print(present, HEX);

  Serial.print(" ");

  for ( i = 0; i < 9; i++) {           // we need 9 bytes

    data[i] = ds.read();

    Serial.print(data[i], HEX);

    Serial.print(" ");

  }

  Serial.print(" CRC=");

  Serial.print(OneWire::crc8(data, 8), HEX);

  Serial.println();



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

  } else {

    byte cfg = (data[4] & 0x60);

    // at lower res, the low bits are undefined, so let's zero them

    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms

    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms

    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms

    //// default is 12 bit resolution, 750 ms conversion time

  } 



  /*tft.setCursor (0, 0);

  tft.setTextSize (2);

  tft.setTextColor(RED);
  tft.fillScreen(BLACK);
  */

  celsius = (float)raw / 16.0;
  tempAvg += celsius;
  count ++;
  
  /*fahrenheit = celsius * 1.8 + 32.0;

  tft.print("Temperature = ");

  tft.setCursor (0, 70);

  tft.print(celsius);

  tft.setCursor (0, 90);

  tft.print("Celsius, ");

  tft.setCursor (0, 110);

  //tft.print(fahrenheit);

  tft.setCursor (0, 130);

  tft.println("Fahrenheit");
  */
  delay(25);

}
