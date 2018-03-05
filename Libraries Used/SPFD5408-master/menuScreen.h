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

struct dataLine {
    String dataType;
    float data;
    String endType;

    void setData(float _data) {
        data = _data;
    }

    // void printDataLine(Adafruit_TFTLCD &screen, float newData, int x, int y, int xOffset, bool firstTime) {
    //     if (firstTime) {
    //         screen.setCursor(x, y);
    //         screen.print(dataType);
    //     }
    //     if (newData != data) {
    //         screen.setCursor((x + xOffset), y);
    //         String toPrint = newData + endType;
    //         screen.setTextColor(BLACK)
    //         screen.print(toPrint);
    //     }

    // }

};

class menuScreen {
    public:
        menuScreen(String name, String text[], int textSize);
        // menuScreen(String name, dataLine *data[], Adafruit_TFTLCD &display);
        void printStaticScreen(Adafruit_TFTLCD& screen);
        void printStaticInput(Adafruit_TFTLCD& screen, int x, int y);
        int getTwoDigVal(Adafruit_TFTLCD& screen, Keypad& numpad, int x, int y);
        int getTemp(Adafruit_TFTLCD& screen, Keypad& numpad, int x, int y);
        long getHourAndMin(Adafruit_TFTLCD& screen, Keypad& numpad, int x, int y);
        long getBiliTime(Adafruit_TFTLCD& screen, Keypad& numpad, int x, int y);

    private:
        String _name;
        String *Lines;
        int _textSize;
        int xWidth, yHeight;
        // // dataLine _data[];
        // bool _isData;
        static int16_t xMax, yMax;
        static long cursorDuration;
};

#endif