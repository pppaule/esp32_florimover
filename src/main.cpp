

 #define OUTPUT_DMX
// #define OUTPUT_ARTNET
// #define OUTPUT_BLEMIDI
// #define OUTPUT_MIDI
// #define OUTPUT_OSC
// #define OUTPUT_SERIAL

#include <Arduino.h>
#include <U8g2lib.h>
#include "UserOutput.h"
#include "input.h"
#include <Wire.h>

#define CLK 4                // CLK ENCODER
#define DT 5                 // DT ENCODER
#define encSwitchPin 15      // SWITCH ENCODER
#define YELLOW_BUTTON_PIN 23 // YELLOW BUTTON
#define JOY_X_PIN 32
#define JOY_Y_PIN 35

// goes to fixture struct main

const int dmx_start_address = 1;
const int dmx_pan_offset =  1;
const int dmx_tilt_offset = 3;

encoderInput encoder(CLK, DT, encSwitchPin);
Joystick joystick(JOY_X_PIN, JOY_Y_PIN);
TheButton yellowButton(YELLOW_BUTTON_PIN);

#ifdef OUTPUT_DMX
#include <esp_dmx.h>

int transmitPin = 16;
int receivePin = 17;
int enablePin = 18;

dmx_port_t dmxPort = 1;
byte data[DMX_PACKET_SIZE];
unsigned long lastUpdate = millis();
#endif

//**********Joystick defines and variables ***********

// bool  INVx;
// bool  INVy;

//************ encoder defines and variables

bool inverted;
bool DMX_inverted;
bool joystick_inversion;
/*
  U8g2lib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.

*/
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 22, 21);

UserOutput ui_park(u8g2);

struct Coordinate
{
  bool assigned = false;
  int x = 0;
  int y = 0;
};
// target setup
//  targets are xy coordinates of cornerpoints in a clockwise fashion
//  parking is the safe position to return lamp to.

extern bool joystick_inversion; // joystick flip flag
Coordinate parking_position;

void florimoover();

// this has to be run in loop and during while(1)


void dmx_loop()
{
  static unsigned long now = millis();
  if (now - lastUpdate >= 25) {
    /* Increment every byte in our packet. Notice we don't increment the zeroeth
      byte, since that is our DMX start code. Then we must write our changes to
      the DMX packet. */
    for (int i = 1; i < DMX_PACKET_SIZE; i++) {
      data[i]++;
    }
    dmx_write(dmxPort, data, DMX_PACKET_SIZE);
    //dmx_send(dmxPort, DMX_PACKET_SIZE);
    /* Log our changes to the Serial Monitor. */
    Serial.printf("Sending DMX 0x%02X\n", data[1]);
    lastUpdate = now;
  }

  
  /* Now we can transmit the DMX packet! */
  dmx_send_num(dmxPort, DMX_PACKET_SIZE);

  /* We can do some other work here if we want. */

  /* If we have no more work to do, we will wait until we are done sending our
    DMX packet. */
  dmx_wait_sent(dmxPort, DMX_TIMEOUT_TICK);
  Serial.println("Hallo");
}

void user_loop()
{
  yellowButton.updateSwitch();
  encoder.updateSwitch();
  #ifdef OUTPUT_DMX
      dmx_loop();
    
  // #else
  //   artnet_loop();
  #endif
}


void setup(void)
{
  Serial.begin(2400);
  u8g2.begin();
  encoder.init();

#ifdef OUTPUT_DMX
  dmx_config_t config = DMX_CONFIG_DEFAULT;
  dmx_personality_t personalities[] = {};
  int personality_count = 0;
  dmx_driver_install(dmxPort, &config, personalities, personality_count);
  dmx_set_pin(dmxPort, transmitPin, receivePin, enablePin);
#endif

}

void loop(void)
{
  u8g2.clearBuffer();                 // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable fontemory

  florimoover();

  u8g2.sendBuffer(); // transfer internal memory to the display
  delay(1000);
}

bool firstRun = true;

void florimoover()
{
  encoder.init();
  int textInterval = 2000;
  unsigned long textTimer = millis() + textInterval;
  ui_park.init_main_window(10, 5, 108, 46, 3);
  ui_park.init_joystickBox(85, 14, 42, 42, 6, 12);
  ui_park.init_status_bar(13, 49, 85, 15, 3);
  ui_park.clearDisplay();
  ui_park.draw_txt_in_window("please move", "to parking", "position");

  // parking_position.x = menuCntrlPoint_ParkingX.getCurrentValue();
  // parking_position.y = menuCntrlPoint_ParkingY.getCurrentValue();

  // parking position is the last saved position of the fixture

  parking_position.x = 127;
  parking_position.y = 127;

  int joystickValue_x = 0;
  int joystickValue_y = 0;
  int joystickspeed = 100;

  // has to be saved to eeprom

  // joystick.init(menuMotionSetupJoystickSpeed.getCurrentValue());
  joystick.init(joystickspeed);

  ui_park.init_main_window(10, 5, 108, 46, 3);
  ui_park.init_joystickBox(85, 14, 42, 42, 6, 12);
  ui_park.init_status_bar(13, 49, 85, 15, 3);

  while (1)
  {
    static unsigned long holdTimer = 0;
    static bool pressed_flag = false;

    // ui code starts here
    user_loop();

    static int joystickSpeed = joystickspeed;

    char buffer[20];
    // int enc_val = encoder.getValOnebyOne();
    int enc_val = encoder.getValue();

    if (enc_val > 0)
    {
      // joystickSpeed += 100;
      // if (joystickSpeed > 2000)
      //   joystickSpeed = 2000;
      // joystick.setStepSize(joystickSpeed);
      // sprintf(buffer, "%d", joystickSpeed);
      // ui_park.draw_alert_window(buffer);
      // delay(100);
    }
    else if (enc_val < 0)
    {
      // joystickSpeed -= 100;
      // if (joystickSpeed < 50)
      //   joystickSpeed = 50;
      // joystick.setStepSize(joystickSpeed);
      // sprintf(buffer, "%d", joystickSpeed);
      // ui_park.draw_alert_window(buffer);
      // delay(100);
    }

    if (encoder.pressed())
    {
      holdTimer = millis();
      pressed_flag = true;
    }

    //   // saving routine

    if (pressed_flag && encoder.held_longer_then(1500))
    {
      //  go_to_parking_position_and_secure_lamp();
      // return ; //0;
    }

    if (encoder.depressed())
    {
      if (!firstRun)
      {
        // menuCntrlPoint_ParkingX.setCurrentValue(parking_position.x);
        // menuCntrlPoint_ParkingY.setCurrentValue(parking_position.y);

        // EEPROM.commit();
        //  menuMgr.save(0xfade);

        // ui_park.draw_alert_window("saved");
        // ui_park.updateDisplay();
      }

      pressed_flag = false;
      firstRun = false;
    }
    static unsigned long joystickTimer = millis();

    if (millis() - joystickTimer > 50)
    {
      joystickValue_x = joystick.getX();
      joystickValue_y = map(joystick.getY(), 4096, 0, 0, 4096); // inversed for optics
      parking_position.x += joystick.getRelativeX(joystick_inversion);
      parking_position.x = constrain(parking_position.x, 0, 65535);
      parking_position.y += joystick.getRelativeY(joystick_inversion);
      parking_position.y = constrain(parking_position.y, 0, 65535);

      constrain(parking_position.x, 0, 65536);
      constrain(parking_position.y, 0, 65536);
    }

    //   // joystick conversion code
    //   int dmx_x_ch = dmx_start_address + dmx_pan_offset;
    //   int dmx_y_ch = dmx_start_address + dmx_tilt_offset;

    //   // dmx_value[dmx_x_ch] = (byte)(parking_position.x >> 8) & 0xFF; // highbyte
    //   // dmx_value[dmx_x_ch + 1] = (byte)parking_position.x & 0xFF;    // lowbyte
    //   // dmx_value[dmx_y_ch] = (byte)(parking_position.y >> 8) & 0xFF; // highbyte
    //   // dmx_value[dmx_y_ch + 1] = (byte)parking_position.y & 0xFF;    // lowbyte

    char row1[20];
    char row2[20];
    char row3[20];
    // int pan_value =(map(joystick.getX(), 0, 4095, 100, -100));
    // int tilt_value = (map(joystick.getY(), 0, 4095, -100, 100));
    int pan_value = map(joystick.getRelativeX(joystick_inversion), -100, 100, 127, -127);
    int tilt_value = map(joystick.getRelativeY(joystick_inversion), -50, 50, 127, -127);

    if (pan_value < -127)
      pan_value = -127;
    if (pan_value > 127)
      pan_value = 127;
    if (tilt_value < -127)
      tilt_value = -127;
    if (tilt_value > 127)
      tilt_value = 127;

    constrain(tilt_value, -127, 127);
    sprintf(row1, "ENCVALUE %5d", enc_val);
    sprintf(row2, "PAN %5d %", pan_value);
    sprintf(row3, "TILT %5d %", tilt_value);
    ui_park.draw_txt_in_window(row1, row2, row3);

    //   ui_park.textloop_in_status_bar_2_items(2000, "hold to exit", "push to save");

    if (pressed_flag)
      ui_park.draw_joystickBox(joystickValue_x, joystickValue_y, true);
    else
      ui_park.draw_joystickBox(joystickValue_x, joystickValue_y, false);

    ui_park.updateDisplay();

    // DMX AND SERIAL OUTPUT

    #ifdef OUTPUT_DMX
    static long lastTime = millis();
    int speed = enc_val * 10;
    data[1] = pan_value;
    data[3] = tilt_value;


    #endif

#ifdef OUTPUT_SERIAL
    static long lastTime = millis();
    int speed = enc_val * 10;
    static int i = 0;
    // Serial.print("Hallo");

    if (millis() - lastTime > speed)
    {
      lastTime = millis();

      // Serial.println(joystickValue_x);
      // Serial.println(joystickValue_y);
      // Serial.println();
      // dmx_loop();
      // serial_output();
    }
#endif
   
  }
}