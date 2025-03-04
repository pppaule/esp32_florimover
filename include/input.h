#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>
#include <ESP32Encoder.h>
#include <Bounce2.h>


//***********encoder variables


class TheButton{
private:
    const int pin;
    Bounce dbnc_button;
public:
    TheButton(int _pin);
    void init();
    bool getSwitch();
    bool switchChanged();
    bool switchFallingEdge();
    bool switchRisingEdge();
  
    void updateSwitch();
};

class encoderInput{

private:
    const int dt_pin;
    const int clk_pin;
    const int switch_pin;
    int encoderValue;
    int encoderSwitch;
    int encoderDirection;
    long oldPosition;
    long newPosition;
    int direction;
    ESP32Encoder encoder;
    Bounce dbnc_button;

public:
    encoderInput(int _dt, int _clk, int _switch);
    void init();
    long getValue();
    int getDirection();
    int getValOnebyOne();
    void updateSwitch();   // wir wollen hier raus, das ist unwürdig.
    bool getSwitch(); // wir wollen hier raus, das ist unwürdig.
    bool switchChanged(); // wir wollen hier raus, das ist unwürdig.
    bool switchFallingEdge(); // wir wollen hier raus, das ist unwürdig.
    bool switchRisingEdge(); // wir wollen hier raus, das ist unwürdig.
      bool pressed();   // wir wollen hier raus, das ist unwürdig.
    bool depressed();  // wir wollen hier raus, das ist unwürdig. 
    bool held_longer_then(int duration);
};

class Joystick{

private:
    const int x_pin;
    const int y_pin;
    int x_value;
    int y_value;   
    int stepSize;
    int stepSizeX;
    int stepSizeY;

public:
    Joystick(int _x, int _y);
    void init(int);
    void setStepSize(int);
    int getX();
    int getY();
    int getRelativeX(bool);
    int getRelativeY(bool);

};



#endif
