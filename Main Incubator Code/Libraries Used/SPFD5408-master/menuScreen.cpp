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

void menuScreen::printStaticScreen(Adafruit_TFTLCD &screen) {
    int16_t x = 0;
    int16_t y = 0;
    int16_t moveDown = 30;
    screen.setTextSize(_textSize);
    screen.setTextColor(WHITE);
    screen.setCursor(x, y);

    for (int i = 0; i <= sizeof(Lines); i++) {
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
    screen.print("Input Temp (degC):00  C");
    screen.fillRect(0, y+yHeight, xMax, yHeight+10, BLACK);
    screen.setCursor(x, y+yHeight+10);
    screen.print("Bililight Time: 00hrs00min");
}

void menuScreen::printStaticLine(Adafruit_TFTLCD& screen, String content, uint color, int x, int y) {
    screen.setTextSize(_textSize);
    screen.setTextColor(color);
    screen.fillRect(x, y, xMax, yHeight+10, BLACK);
    screen.setCursor(x, y);
    screen.print(content);
}

int menuScreen::getTwoDigVal(Adafruit_TFTLCD& screen, Keypad& numpad, int x, int y, bool secondaryInput) {
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
                else if (count == 1) { screen.fillRect(x + xWidth+2, y, xWidth, yHeight, BLACK); }
                _white = false;
            }
            else if (!_white) {
                if (count == 0) { screen.fillRect(x, y, xWidth, yHeight, WHITE); }
                else if (count == 1) { screen.fillRect(x + xWidth+2, y, xWidth, yHeight, WHITE); }
                _white = true;
            }
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
            else if (keyEntered == 'A') { 
                screen.setCursor(x, y);
                screen.print('0');
                count++; 
            }
            else if (secondaryInput && keyEntered == 'B') {  return -1; }
        }

        else if (keyEntered && count == 1) {
            screen.fillRect(x + xWidth+2, y, xWidth, yHeight, BLACK);
            num = keyEntered - '0';
            if (num > -1 && num < 10) {
                screen.setCursor(x + xWidth+2, y);
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
                val -= (val % 10);
                count--;
            }
        }
    }
    return val;
}

int menuScreen::getTemp(Adafruit_TFTLCD& screen, Keypad &numpad, int x, int y) {
    screen.setTextSize(_textSize);
    screen.setTextColor(WHITE);
    screen.setCursor(x, y);
    screen.fillRect(0, y, xMax, yHeight, BLACK);
    screen.print("Input Temp (degC):00  C");
    screen.drawFastHLine(0, yMax - 60, xMax, WHITE);
    screen.setCursor(0, yMax - 60);
    screen.print("Input 2 digit temp in     degrees Celsius.");
    screen.setTextColor(GREEN);
    int temp = this->getTwoDigVal(screen, numpad, x + 22 * xWidth-4, y, false);
    return temp;
}

int menuScreen::getHourAndMin(Adafruit_TFTLCD& screen, Keypad& numpad, int x, int y) {
    int hrs = this->getTwoDigVal(screen, numpad, x, y, true);

    if (hrs == -1) { return -1; }
    int mins = this->getTwoDigVal(screen, numpad, x+ 5*xWidth+10, y, true);
    if (mins == -1) { return -10;}
    int hm = (hrs * 60) + (mins);
    return hm;
}

int menuScreen::getBiliTime(Adafruit_TFTLCD& screen, Keypad& numpad, int x, int y) {
    screen.setTextSize(_textSize);
    screen.setTextColor(WHITE);
    screen.setCursor(x, y);
    screen.print("Bililight Time: 00hrs00min");
    screen.drawFastHLine(0, yMax - 60, xMax, WHITE);
    screen.setCursor(x, yMax - 60);
    screen.print("Input time for bililights in hours and minutes.");
    screen.setTextColor(GREEN);
    return this->getHourAndMin(screen, numpad, x+19*xWidth+2, y);
}

void menuScreen::getInputs(Adafruit_TFTLCD& screen, Keypad& numpad, int x, int y, int inputs[]) {
    bool timeEntered = false;
    this->printStaticInput(screen, x, y);
    while(!timeEntered) {
        Serial.println("Hey I'm at the beginning of the loop");
        this->printStaticInput(screen, x, y);
        inputs[0] = this->getTemp(screen, numpad, 0, 0);
        screen.fillRect(x, yMax - 60, 320, 240, BLACK);
        bool minEntered = false;
        bool backMin = false;
        while (!minEntered) {
            inputs[1] = this->getBiliTime(screen, numpad, x, (y+yHeight+10));
            if (inputs[1] == -1) { break; }
            else if (inputs[1] != -10) { minEntered = true; }
            screen.fillRect(0, y+yHeight+7, 240, yHeight, BLACK);
        }
        if(inputs[1] != -1) {
            timeEntered = true;
            break;
        }
        screen.fillRect(0, 0, xMax, yMax, BLACK);
    }
}