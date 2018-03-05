#include "menuScreen.h"

#define BLACK   0x0000
#define WHITE   0xFFFF
#define GREEN   0x07E0

int menuScreen::xMax = 320;
int menuScreen::yMax = 240;
long menuScreen::cursorDuration = 500;

menuScreen::menuScreen(String name, String text[], int textSize) {
    _name = name;
    Lines = text;
    _textSize = textSize;
    xWidth = 5 * textSize;
    yHeight = 9 * textSize;
}

// menuScreen::menuScreen(String name, dataLine *data[], Adafruit_TFTLCD &display) {
//     _name = name;
//     _data = data;
//     screen = display;
//     _isData = true;
// }

void menuScreen::printStaticScreen(Adafruit_TFTLCD &screen) {
    int16_t x = 0;
    int16_t y = 0;
    int16_t moveDown = 30;
    screen.setTextSize(_textSize);
    screen.setTextColor(WHITE);
    screen.setCursor(x, y);
    screen.print(_name);
    y+=yHeight;

    for (int i = 0; i < sizeof(Lines); i++) {
        screen.setCursor(x, y);
        screen.print(Lines[i]);
        y += yHeight;
    }
}

void menuScreen::printStaticInput(Adafruit_TFTLCD& screen, int x, int y) {
    screen.setTextSize(_textSize);
    screen.setTextColor(WHITE);
    screen.setCursor(x, y);
    screen.fillRect(0, y, xMax, yHeight, BLACK);
    screen.print("Input Temp (degC):   C");
    screen.fillRect(0, y+yHeight, xMax, yHeight, BLACK);
    screen.setCursor(x, y+yHeight);
    screen.print("Bililight Time: 00hrs00min");
}

int menuScreen::getTwoDigVal(Adafruit_TFTLCD& screen, Keypad& numpad, int x, int y) {
    int count = 0;
    long previousMillis = 0;
    bool _white = 0;
    int val = 0;
    int num;
    while (count < 3) {
        unsigned long currentMillis = millis();
        char keyEntered = numpad.getKey();
        if (currentMillis - previousMillis > cursorDuration) {
            previousMillis = currentMillis;
            if (_white) {
                if (count == 0) { screen.fillRect(x, y, xWidth, yHeight, BLACK); }
                else if (count == 1) { screen.fillRect(x + xWidth+1, y, xWidth, yHeight, BLACK); }
                else if (count == 2) { screen.fillRect(x + 2*xWidth+2, y, xWidth, yHeight, BLACK); }
                _white = false;
            }
            else if (!_white) {
                if (count == 0) { screen.fillRect(x, y, xWidth, yHeight, WHITE); }
                else if (count == 1) { screen.fillRect(x + xWidth+1, y, xWidth, yHeight, WHITE); }
                else if (count == 2) { screen.fillRect(x + 2*xWidth+2, y, xWidth, yHeight, WHITE); }
                _white = true;
            }
            else {}
        }
        
        else if (keyEntered && count == 0) {
            screen.fillRect(x, y, xWidth, yHeight, BLACK);
            num = keyEntered - '0';
            if (num > -1 && num < 10) {
                screen.setCursor(x, y);
                screen.print(keyEntered); 
                val += 10 * num;
                count++;
            }
        }

        else if (keyEntered && count == 1) {
            screen.fillRect(x + xWidth+1, y, xWidth, yHeight, BLACK);
            num = keyEntered - '0';
            if (num > -1 && num < 10) {
                screen.setCursor(x + xWidth+1, y);
                screen.print(keyEntered);
                val += num;
                count++;
            }
            else if (keyEntered == 'B') {
                screen.setCursor(x, y);
                val = 0;
                count--;
            }
        }

        else if (keyEntered && count == 2) {
            if (keyEntered == 'A') {
                count++;
            }
            else if(keyEntered == 'B') {
                screen.fillRect(x + 2*xWidth+2, y, xWidth, yHeight, BLACK);
                val -= (val % 10);
                count--;
            }
        }
    }
    screen.fillRect(x + 2*xWidth+2, y, xWidth, yHeight, BLACK);
    return val;
}

int menuScreen::getTemp(Adafruit_TFTLCD& screen, Keypad &numpad, int x, int y) {
    screen.setTextSize(_textSize);
    screen.setTextColor(WHITE);
    screen.setCursor(x, y);
    screen.fillRect(0, y, xMax, yHeight, BLACK);
    screen.print("Input Temp (degC):   C");
    screen.drawFastHLine(0, yMax - 60, xMax, WHITE);
    screen.setCursor(0, yMax - 60);
    screen.print("Input temperature in      degrees Celsius.");
    screen.setTextColor(GREEN);
    int temp = this->getTwoDigVal(screen, numpad, x + 22 * xWidth, y);
    return temp;
}

long menuScreen::getHourAndMin(Adafruit_TFTLCD& screen, Keypad& numpad, int x, int y) {
    int count = 0;
    long previousMillis = 0;
    bool _white = 0;
    int val = 0;
    int num;
    screen.setCursor(x, y);
    screen.print("  hr,   min");
    char keyEntered = numpad.getKey();
    while(true) {

    }
    // while (count < 5) {
    //     unsigned long currentMillis = millis();
    //     char keyEntered = numpad.getKey();
    //     if (currentMillis - previousMillis > cursorDuration) {
    //         previousMillis = currentMillis;
    //         if (_white) {
    //             if (count == 0) { screen.fillRect(x, y, xWidth, yHeight, BLACK); }
    //             else if (count == 1) { screen.fillRect(x + xWidth+1, y, xWidth, yHeight, BLACK); }
    //             else if (count == 2) { screen.fillRect(x + 2*xWidth+2, y, xWidth, yHeight, BLACK); }
    //             _white = false;
    //         }
    //         else if (!_white) {
    //             if (count == 0) { screen.fillRect(x, y, xWidth, yHeight, WHITE); }
    //             else if (count == 1) { screen.fillRect(x + xWidth+1, y, xWidth, yHeight, WHITE); }
    //             else if (count == 2) { screen.fillRect(x + 2*xWidth+2, y, xWidth, yHeight, WHITE); }
    //             _white = true;
    //         }
    //         else {}
    //     }
        
    //     else if (keyEntered && count == 0) {
    //         screen.fillRect(x, y, xWidth, yHeight, BLACK);
    //         num = keyEntered - '0';
    //         if (num > -1 && num < 10) {
    //             screen.setCursor(x, y);
    //             screen.print(keyEntered); 
    //             val += 10 * num;
    //             count++;
    //         }
    //     }

    //     else if (keyEntered && count == 1) {
    //         screen.fillRect(x + xWidth+1, y, xWidth, yHeight, BLACK);
    //         num = keyEntered - '0';
    //         if (num > -1 && num < 10) {
    //             screen.setCursor(x + xWidth+1, y);
    //             screen.print(keyEntered);
    //             val += num;
    //             count++;
    //         }
    //         else if (keyEntered == 'B') {
    //             screen.setCursor(x, y);
    //             val = 0;
    //             count--;
    //         }
    //     }

    //     else if (keyEntered && count == 2) {
    //         if (keyEntered == 'A') {
    //             count++;
    //         }
    //         else if(keyEntered == 'B') {
    //             screen.fillRect(x + 2*xWidth+2, y, xWidth, yHeight, BLACK);
    //             val -= (val % 10);
    //             count--;
    //         }
    //     }
    // }
    // screen.fillRect(x + 2*xWidth+2, y, xWidth, yHeight, BLACK);
    return val;
}

long menuScreen::getBiliTime(Adafruit_TFTLCD& screen, Keypad& numpad, int x, int y) {
    screen.setTextSize(_textSize);
    screen.setTextColor(WHITE);
    screen.setCursor(x, y);
    // screen.fillRect(0, y, xMax, yHeight, BLACK);
    screen.print("Bililight Time: 00hrs00min");
    screen.drawFastHLine(0, yMax - 60, xMax, WHITE);
    screen.setCursor(x, yMax - 60);
    screen.print("Input time for bililights in hours and minutes.");
    screen.setTextColor(GREEN);
    // long time = this->getBiliTime(screen, numpad, x + 20*xWidth, y);
    // return time;
    return 0;
}