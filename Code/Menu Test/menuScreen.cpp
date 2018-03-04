#include "menuScreen.h"

#define BLACK   0x0000
#define WHITE   0xFFFF


namespace Menu {
int menuScreen::xMax = 320;
int menuScreen::yMax = 240;

menuScreen::menuScreen(String name, String &textToDisplay[], Adafruit_TFTLCD &display) {
    _name = name;
    Lines = textToDisplay;
    screen = display;
    _isData = false;

}

menuScreen::menuScreen(String name, dataLine *data[], Adafruit_TFTLCD &display) {
    _name = name;
    _data = data;
    screen = display;
    _isData = true;
}

void menuScreen::printScreen() {
    int16_t y = 20;
    int16_t moveDown = 30;
    screen.setTextSize(3);
    screen.setCursor(0, y);
    screen.print(_name);
    y+=35;

    if(_isData) {
        if sizeof(_data) >= 6 {
            moveDown = 25;
        }

        for (int i; i < sizeof(_data); i++) {
            if(y >= (yMax - 20) {
                screen.fillScreen(BLACK);
                y = 0;
            }
            screen.setCursor(0, y);
            screen.print(_data[i].dataType);
            y += moveDown;
        }

        y = 180;
        screen.drawFastHLine(0, y, 320, WHITE)

    }

    else {
        if sizeof(Lines) >= 6 {
            moveDown = 25;
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
}

int menuScreen::getVal(Keypad &numpad, int x, int y) {
    bool numEntered = false;
    long previousMillis = 0;
    bool _white = false;
    long interval = 100;
    int count = 0;
    int val = 0;
    while (count < 3) {
        while (!numEntered) {
            unsigned long currentMillis = millis();
            int num = keyEntered - '0';
            char keyEntered = numpad.getKey();

            if (currentMillis - previousMillis > interval) {
                previousMillis = currentMillis;
                if (_white) {
                    if (count == 0) { tft.fillRect(x, y, 15, 30, BLACK); }
                    else if (count == 1) { tft.fillRect(x + 15, y, 15, 30, BLACK); }
                    else if (count == 2) { tft.fillRect(x + 130, y, 15, 30, BLACK); }
                }

                else {
                    if (count == 0) { tft.fillRect(x, y, 15, 30, BLACK); }
                    else if (count == 1) { tft.fillRect(x + 15, y, 15, 30, WHITE); }
                    else if (count == 2) { tft.fillRect(x + 30, y, 15, 30, WHITE); }

                }
            }
            
            if (keyEntered && count < 2) {
                if (num >= 0 || num <= 9) {
                    if (count == 0) {
                        screen.setCursor(x, y);
                        val += 10 * num;
                    }

                    else {
                        screen.setCursor(x + 15, y);
                        val += num;
                    }
                    numEntered = true;
                    screen.print(keyEntered);
                    count++;
                }

                else if (keyEntered == 'B' && count == 1) {
                    screen.setCursor(x, y);
                    screen.fillRect(x, y, 15, 30, BLACK);
                    val = 0;
                    count--;             
                }
            }

            else if (keyEntered && count == 2) {
                if (keyEntered == 'B') {
                    screen.setCursor(x +m15, y);
                    screen.fillRect(x + 15, y, 15, 30, BLACK);
                    val = val - (val % 10;
                    count--;
                }

                if (keyEntered == 'A') {
                    break;
                }
            }
        }
    }

    return val;
}
}