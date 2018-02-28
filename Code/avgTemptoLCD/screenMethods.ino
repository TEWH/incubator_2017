#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>

// Easy to understand color Codes
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void updateData(TouchScreen &screen, int xPos, int yPos, auto data, auto prevData, string dataType) {
  screen.setTextColor(BLACK);
  screen.setCursor (x, y);
  screen.print(prevData);
  screen.setCursor(x, y);
  screen.setTextColor(CYAN);
  
  String toPrint = data + dataType;
  screen.print(data);
}