#include "menuScreen.h"

#define BLACK   0x0000

int menuScreen::xMax = 240;
int menuScreen::yMax = 320;

menuScreen::menuScreen(String name, String &textToDisplay[], Adafruit_TFTLCD &display) {
    _name = name;
    Lines = textToDisplay;
    screen = display;
}

void menuScreen::printScreen() {
    int16_t y = 40;
    int16_t moveDown = 25;
    screen.setTextSize(2);

    screen.print(_name);
    y+=35;
    screen.setCursor(0, y);

    if sizeof(Lines) >= 8 {
        moveDown = 21;
    }

    for (int i; i < sizeof(Lines); i++) {
        if(y >= (yMax - 20) {
            screen.fillScreen(BLACK);
            y = 0;
        }
        screen.setCursor(0, y);
        screen.print(Lines[i]);
        y+=moveDown;
    }
}