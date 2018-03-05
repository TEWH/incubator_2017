// Include files so that we are able to interface with the touch screen
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
#include <Keypad.h>
#include <menuScreen.h>

// Control pins for the LCD
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

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

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

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

//initialize an instance of class NewKeypad
Keypad numpad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

String textToDisplay[3] = {"Hello", "My name is Abhishek", "How has your day been?"};
menuScreen start("Start", textToDisplay, 2);
int num = 0;

long previousMillis = 0;
long interval = 500;
bool _white = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Start cross communication with the LCD

  // Initiate communcations
  tft.begin(0x9341); // SDFP5408
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setRotation(45);
  start.printStaticInput(tft, 0, 0); 
}

void loop() {
  int temp = start.getTemp(tft, numpad, 0, 0);
  Serial.println("Input temp = " + (String)temp);
  tft.fillRect(0, 180, 320, 60, BLACK);
  long biliTime = start.getBiliTime(tft, numpad, 0, 18);
  Serial.println("Input time = " + (String)biliTime);
  delay(1000);
  tft.fillRect(0, 180, 320, 60, BLACK);
}
