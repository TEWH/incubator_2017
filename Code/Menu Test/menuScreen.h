/*
    VERSION 1 (TWO SCREENS - INPUT SCREEN AND DISPLAY SCREEN)
    menuScreen.h - Library for menu screen for TEWH Incubator interface
    Created by Abhishek Ramani, Febuary 27, 2018
*/
#ifndef menuScreen_h
#define menuScreen_h

#include "Arduino.h"
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
#include <Keypad.h>

#define BLACK   0x0000
#define WHITE   0xFFFF

namespace Menu {

typedef struct dataLine {
    String dataType;
    float data;
    String endType;

    void setData(float _data) {
        data = _data;
    }

    void printDataLine(Adafruit_TFTLCD &screen, float newData, int x, int y, int xOffset, bool firstTime) {
        if (firstTime) {
            screen.setCursor(x, y);
            screen.print(dataType);
        }
        if (new Data != data) {
            screen.setCursor((x + xOffset), y);
            string toPrint = newData + endType;
            screen.setTextColor(BLACK)
            screen.print(toPrint);
        }

    }

}

class menuScreen {
    public:
        menuScreen(String name, String *textToDisplay[], Adafruit_TFTLCD &display);
        menuScreen(String name, dataLine *data[], Adafruit_TFTLCD &display);
        
        void printScreen(dataLine newData, bool firstTime);
        int getVal(Keypad &numpad, int x, int y);
        /*  TODO 
            make function for asking user input
                - setTemp
                - setBiliLightTIme
                - set whether on or off
            update textToDisplay
        */

    private:
        String _name;
        String Lines[];
        dataLine _data[];
        Adafruit_TFTLCD &screen;
        bool _isData;
        static int16_t xMax, yMax;
};

}
#endif