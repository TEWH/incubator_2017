/*
    menuScreen.h - Library for menu screen for TEWH Incubator interface
    Created by Abhishek Ramani, Febuary 27, 2018
*/
#ifndef menuScreen_h
#define menuScreen_h

#include "Arduino.h"
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>

class menuScreen {
    public:
        menuScreen(String name, String *textToDisplay[], Adafruit_TFTLCD &display);
        
        void printScreen();
        void updateText(String );

    private:
        String _name;
        String Lines[];
        Adafruit_TFTLCD screen;
        static int16_t xMax, yMax;
};

#endif