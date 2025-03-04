#include <Input.h>

//joystick inversion 
//-> HIER definieren!!!<-
// HW Protoytpe 1 (hannes board): x = false, y = false 
// kunde: x = false, y = true;
// paule :;
bool  INVx = false;
bool  INVy = true;

// button methods
//------------------------------------------------

TheButton::TheButton(int _pin) : pin(_pin)
{
    dbnc_button = Bounce(); // Instantiate a Bounce object
}

void TheButton::init()
{
    dbnc_button.attach(pin, INPUT_PULLUP);
    dbnc_button.interval(3); // interval in ms
    // dbnc_button.setPressedState(HIGH);  //geht irgendwie nicht, ich versehe die pass_through
    // funktionen mit ausrufezeichen (!!) dirty dirty
}
void TheButton::updateSwitch()
{
    dbnc_button.update();
}

bool TheButton::getSwitch()
{
    return dbnc_button.read();
}

bool TheButton::switchChanged()
{
    Serial.println("switch changed");
    return dbnc_button.changed();
}

// seems to not work properly
bool TheButton::switchFallingEdge()
{
    if (!dbnc_button.fell() & dbnc_button.changed())
    {
        Serial.println("falling edge");
        return true;
    }
    else
    {
        return false;
    }
}

bool TheButton::switchRisingEdge()
{

    if (!dbnc_button.rose() & dbnc_button.changed())
    {
        Serial.println("rising edge");
        return true;
    }
    else
    {
        return false;
    }

    // return (!dbnc_button.rose() & dbnc_button.changed());
}

// encoder methods
//------------------------------------------------

encoderInput::encoderInput(int _dt, int _clk, int _switch) : dt_pin(_dt), clk_pin(_clk), switch_pin(_switch)
{
    oldPosition = 0;
    newPosition = 0;
    direction = 0;
    dbnc_button = Bounce(); // Instantiate a Bounce object
}

void encoderInput::init()
{
    encoder.attachHalfQuad(clk_pin, dt_pin);
    encoder.setCount(0);
    // pinMode(switch_pin, INPUT_PULLUP);
    dbnc_button.attach(switch_pin);
    dbnc_button.interval(3); // interval in ms
    // dbnc_button.setPressedState(HIGH);
}

long encoderInput::getValue()
{

    newPosition = encoder.getCount() / 2;
    if (oldPosition != newPosition)
    {
        if (newPosition - oldPosition > 0)
        {
            direction = 1;
        }
        else if (newPosition - oldPosition < 0)
        {
            direction = -1;
        }
        else
        {
            direction = 0;
        }
        oldPosition = newPosition;
    }
    return newPosition;
}

int encoderInput::getValOnebyOne()
{

   //newPosition = encoder.getCount() / 2;
    if (oldPosition != newPosition)
    {
        if (newPosition - oldPosition > 0)
        {
            direction = 1;
        }
        else if (newPosition - oldPosition < 0)
        {
            direction = -1;
        }
        oldPosition = newPosition;
    }
    else
    {
        direction = 0;
    }
    return direction;
}

int encoderInput::getDirection()
{
    return direction;
}

// debounce methods
void encoderInput::updateSwitch()
{
    dbnc_button.update();
}

bool encoderInput::getSwitch()
{
    return dbnc_button.read();
}

bool encoderInput::switchChanged()
{
    // return dbnc_button.changed();
    if (dbnc_button.changed())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool encoderInput::switchFallingEdge()
{
    // return (dbnc_button.fell() & dbnc_button.changed());
    if (!dbnc_button.fell() & dbnc_button.changed())
        return true;
    return false;
}

bool encoderInput::held_longer_then(int duration){
    if (dbnc_button.currentDuration() > duration)
        return true;
    return false;
}

bool encoderInput::pressed()
{
    return switchRisingEdge();

    // if (!dbnc_button.isPressed())
    //     return true;
    // return false;
}

bool encoderInput::depressed()
{
    return switchFallingEdge();
}

bool encoderInput::switchRisingEdge()
{
    // return (dbnc_button.rose() & dbnc_button.changed());
    if (!dbnc_button.rose() & dbnc_button.changed())
    {
        return true;
    }
    else
    {
        return false;
    }
}

// joystick methods
//------------------------------------------------

Joystick::Joystick(int _x, int _y) : x_pin(_x), y_pin(_y)
{
}
void Joystick::init(int _stepSize)
{
    pinMode(x_pin, INPUT);
    pinMode(y_pin, INPUT);
    stepSize = _stepSize;
    stepSizeX = _stepSize;
    stepSizeY = _stepSize/2;
}

void Joystick::setStepSize(int _stepSize)
{
    stepSize = _stepSize;
    stepSizeX = _stepSize;
    stepSizeY = _stepSize/2;
}

int Joystick::getX()
{
    return analogRead(x_pin);
}
int Joystick::getY()
{
    return analogRead(y_pin);
}

int Joystick::getRelativeX(bool invert)
{
    int x_in = analogRead(x_pin);
    int x = 0;

    // todeszone in der mitte des joysticks
    if (x_in > 2200)
        x = x_in;
    else if (x_in < 1600)
        x += x_in;
    else
        x = 2000;

    if (!invert != !INVx) // XOR

        return map(x, 0, 4000, stepSizeX, -stepSizeX);
    else
        return map(x, 0, 4000, -stepSizeX, stepSizeX);
}
int Joystick::getRelativeY(bool invert)
{
    int y_in = analogRead(y_pin);
    int y = 0;

    // todeszone in der mitte des joysticks
    if (y_in > 2200)
        y = y_in;
    else if (y_in < 1600)
        y += y_in;
    else
        y = 2000;

    if (!invert != !INVy) // XOR    
        return map(y, 0, 4000, stepSizeY, -stepSizeY);
    else
        return map(y, 0, 4000, -stepSizeY, stepSizeY);
}
