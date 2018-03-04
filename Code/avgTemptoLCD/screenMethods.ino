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

void updateData(Adafruit_TFTLCD &screen, int xPos, int yPos, float data, float prevData, String dataType) {
  screen.setTextColor(BLACK);
  screen.setCursor (xPos, yPos);
  screen.print(prevData);
  screen.setCursor(xPos, yPos);
  screen.setTextColor(CYAN);
  
  String toPrint = (String)data + dataType;
  screen.print(toPrint);
  screen.setTextColor(WHITE);
}
