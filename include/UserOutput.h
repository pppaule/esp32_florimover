// UserOutput.h
#ifndef USEROUTPUT_H
#define USEROUTPUT_H

#include <U8g2lib.h>

enum State
{
    STATE_ONE,
    STATE_TWO,
    STATE_THREE,
    NUM_STATES
};

class UserOutput
{
public:
    struct menuItem_x
    {
        const char *name;
        void (*action)();
    };

    //     struct Coordinate
    // {
    //   bool assigned = false;
    //   int x = 0;
    //   int y = 0;
    // };

    static const int numberOfPlaybackActions = 4; // Anzahl der Aktionen
    static menuItem_x playbackActions[numberOfPlaybackActions];

    UserOutput(U8G2 &display);

    void drawBackground();
    void drawMovingPoint(int x, int y);
    void drawSlider(int x, int y, int width, int value);
    void updateDisplay();
    void clearDisplay();
    void drawMonitor(int x, int y, int width, int height, int value);
    void draw_ElipticalButton(int x, int y, int width, int height, bool filled, const char *text);
    void draw_encoderButton(int x, int y, int radius, bool filled, int enc_position);
    void draw_encoderButton_with_label(int x, int y, int radius, bool filled, int enc_position, bool highlight, const char *text);

    void drawSmiley(int x, int y, int radius); // a generic smiley
    void drawCircleWithLine(int x, int y, int radius, float angleDegrees);
    // void displayState();
    // void nextState();
    // void previousState();

    void draw_label(int x, int y, int w, int h, bool filled, const char *text);
    void draw_arrow(int x, int y, int r, int direction);

    // void draw_dot_with_number(int x, int y, int radius, bool filled, int number);
    void draw_dot_with_number(int x, int y, int radius, bool filled, bool selected, int number);

    // void draw_black_dot(int x, int y, int width, int height, int radius, int dot_x, int dot_y);
    void drawFader(int x, int y, int width, int height, int value_8bit);
    void draw_6_faders();

    /* a "Window" is a representation of the movinglights x/y - pan (540 deg. and tilt 270 deg.). this is saved as a 16 bit
    integer. output is done via two dmx channels (hibite = grobwerte, lowbite = feinwerte). for graphic representation we only
    using the high byte is more then enough.)
    */

    // this is to be obsolete functions
    void initPlaybackWindow();
    void drawPlaybackWindow(int dmx_x, int dmx_y, int part, int step, bool valve); // draws the animation and diag-values.
    void draw_playbackMenu();
    void init_4pts_menu();
    void draw_4pts_menu();
    void draw_4pts_bar(int x, int y, int w, int h, int r, const char *text);

    void drawContinueWindow();
    void drawAgainWindow();
    void drawShowisoverWindow();
    void drawPauseWindow();
    void drawBeginningPause();
    void drawStatusBar(int part);
    void setMinMaxValues(int[4]);

    // general functions for menu setup. a menu consists of a main window, a status bar and
    // a side box (optional). there is an init functionm to manifest the dimensions and a draw
    // function to update the menu in the loop.

    void init_main_window(int x, int y, int w, int h, int r);
    void draw_main_window();
    void draw_txt_in_window(const char *row1, const char *row2, const char *row3);
    void textloop_in_status_bar_2_items(int interval, const char *text1, const char *text2);

    void init_status_bar(int x, int y, int w, int h, int r);
    void draw_status_bar(bool filled, const char *text);
    void draw_alert_window(const char *text);
    
    void init_joystickBox(int x, int y, int w, int h, int b, int r2);
    void draw_joystickBox(int joy_x, int joy_y, bool pressed);

    void init_side_box(int x, int y, int w, int h);
    void draw_side_box();

    int main_window_x = 0; // left margin
    int main_window_y = 0; // top margin
    int main_window_w = 0; // width
    int main_window_h = 0; // height
    int main_window_r = 0; // radius

    int status_bar_x = 0; // left margin
    int status_bar_y = 0; // top margin
    int status_bar_w = 0; // width
    int status_bar_h = 0; // height
    int status_bar_r = 0; // radius

    int side_box_x = 0; // left margin
    int side_box_y = 0; // top margin
    int side_box_w = 0; // width
    int side_box_h = 0; // height

    // alert window values
    int alert_window_x = 0; // left margin
    int alert_window_y = 0; // top margin
    int alert_window_w = 0; // width
    int alert_window_h = 0; // height
    int alert_window_r = 0; // radius

    int joy_r_x = 0;
    int joy_r_y = 0;
    int joy_r_w = 0;
    int joy_r_h = 0;
    int joy_r_b = 0;
    int joy_r_r2 = 0;

    // playback smiley in playback window variables
    int xmin;
    int xmax;
    int ymin;
    int ymax;

    int currentItem;
    int x;
    int y;
    int w;
    int h;
    unsigned long playbackMenuTimer;
    int playBackMenu_interval = 2000;
    int playBackMenu_currentItem;

private:
    U8G2 &u8g2;
    State currentState = STATE_ONE;
};

void play_animation();
void change_cue();
void exit_menu();
void stop_action();
void exit_playback();

void goto_parking_position_and_secure_lamp();

#endif
