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

const int buttonPin = 12;     // the number of the pushbutton pin

int buttonState = 0;         // variable for reading the pushbutton status

void setup() { 
  //Start cross communication with the LCD
  Serial.begin(9600);

  // Initiate communcations
  tft.begin(0x9341); // SDFP5408
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  pinMode(buttonPin, INPUT);
}

int menu = 0; //INSPIRATION!

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
  tft.setCursor (10, 40);
  tft.setTextSize (2);
  tft.println("1 Length |");
  tft.setCursor (10, 80);
  tft.setTextSize (2);
  tft.println("2 Weight |");
  tft.setCursor (10, 120);
  tft.setTextSize (2);
  tft.println("3 Current Temp |");
  tft.setCursor (10, 160);
  tft.setTextSize (2);
  tft.println("4 Timer |"); 
  tft.setCursor (10, 200);
  tft.setTextSize (2);
  tft.println("5 Time |");  
  tft.setCursor (10, 240);
  tft.setTextSize (2);
  tft.println("6 Bili Light |");
  } else {
  tft.fillScreen(BLACK);
  tft.setCursor (10, 40);
  tft.setTextSize (2);
  tft.println("1  Temp: 74F");
  tft.setCursor (10, 80);
  tft.setTextSize (2);
  tft.println("2  Humidity:  20% ");
  tft.setCursor (10, 120);
  tft.setTextSize (2);
  tft.println("3  Time:  15:00");
  tft.setCursor (10, 160);
  tft.setTextSize (2);
  tft.println("4  Battery: 99%");
  }
}
  

