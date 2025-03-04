

// UserOutput.cpp
#include <UserOutput.h>

UserOutput::UserOutput(U8G2 &display) : u8g2(display), currentState(STATE_ONE) {}

void UserOutput::clearDisplay()
{
    u8g2.clearBuffer();
}

void UserOutput::drawBackground()
{
    u8g2.setDrawColor(1);       // Weiß
    u8g2.drawBox(0, 0, 20, 60); // Hintergrund
    u8g2.sendBuffer();
}

void UserOutput::drawMovingPoint(int x, int y)
{
    u8g2.setDrawColor(0);     // Schwarz
    u8g2.drawBox(x, y, 1, 1); // Ein Pixel großer Punkt
    u8g2.sendBuffer();
}

void UserOutput::drawSlider(int x, int y, int width, int value)
{
    u8g2.setDrawColor(1);                  // Weiß
    u8g2.drawFrame(x, y, width, 10);       // Rahmen des Schiebereglers
    int fillWidth = (value * width) / 100; // Berechnung der Füllung basierend auf dem Wert
    u8g2.drawBox(x, y, fillWidth, 10);     // Füllung des Schiebereglers
    u8g2.sendBuffer();
}

void UserOutput::init_joystickBox(int x, int y, int w, int h, int b, int r2)
{ // b for border
    joy_r_x = x;
    joy_r_y = y;
    joy_r_w = w;
    joy_r_h = h;
    joy_r_b = b;
    joy_r_r2 = r2;
}

void UserOutput::draw_joystickBox(int joy_x, int joy_y, bool pressed)
{
    int x = joy_r_x;
    int y = joy_r_y;
    int w = joy_r_w;
    int h = joy_r_h;
    int b = joy_r_b;
    int r2 = joy_r_r2; //radius of inner circle
    

    u8g2.setDrawColor(1); // Weiß
    u8g2.drawRBox(x, y, w, h, b);
    u8g2.setDrawColor(0); // Schwarz
    u8g2.drawRFrame(x, y, w, h, b);
    u8g2.drawRBox(x+b, y+b, w-b*2, h-b*2, b-1);
   
    int center_x = x + (w / 2);
    int center_y = y + (h / 2);
    u8g2.setDrawColor(1);
    //u8g2.drawfilledEllipse(center_x, center_y, 10, 10);t_x, joystick_rect_y, joystick_rect_w, joystick_rect_h, joystick_rect_b);
    int mapped_x = map(joy_x, 4095, 0, x+b+r2, x + w - b - r2);
    int mapped_y = map(joy_y, 0, 4095, y+b+r2, y + w - b - r2);
    
    u8g2.drawFilledEllipse(mapped_x, mapped_y, r2, r2);
    u8g2.setDrawColor(0);
    if (!pressed)
    u8g2.drawCircle(mapped_x, mapped_y, r2-3);
    else u8g2.drawFilledEllipse(mapped_x, mapped_y, r2-3, r2-3);
}

// void UserOutput::draw_joystickBox(int joy_x, int joy_y){
//     u8g2.setDrawColor(1); // Weiß
//     u8g2.drawRBox(joy_x, joy_y, joystick_rect_w, joystick_rect_h, joystick_rect_b);
//     u8g2.setDrawColor(0); // Schwarz
//     u8g2.drawRBox(joy_x-joystick_rect_b, joy_y-joystick_rect_b, joystick_rect_w-joystick_rect_b*2, joystick_rect_h-joystick_rect_b*2, joystick_rect_b-joystick_rect_b); 
//     int center_x = joy_x + joystick_rect_w / 2;
//     int center_y = joy_y + joystick_rect_h / 2;
//     u8g2.setDrawColor(1); // Weiß
//     u8g2.drawFilledEllipse(center_x, center_y, 10, 10);
// }

void UserOutput::drawMonitor(int x, int y, int width, int height, int value)
{
    u8g2.setDrawColor(1);                  // Weiß
    u8g2.drawRBox(x, y, width, height, 4); // Rahmen des Monitors
                                           // drawSmiley(x + (width / 2), y + (height) / 2, height / 2 - 5);
}

void UserOutput::draw_ElipticalButton(int x, int y, int width, int height, bool filled, const char *text)
{
    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.setDrawColor(1); // Weiß
    int textWidth = u8g2.getStrWidth(text);

    // font format FONT_SMALL u8g2_font_helvR08_tf
    // font format FONT_BIG u8g2_font_helvR10_tf

    // Zeichnen der Ellipse
    if (filled)
    {
        u8g2.drawFilledEllipse(x, y, width / 2, height / 2, U8G2_DRAW_ALL);
        u8g2.setDrawColor(0); // black
        u8g2.drawEllipse(x, y, width / 2 + 1, height / 2 + 1, U8G2_DRAW_ALL);
    }
    else
    {
        u8g2.setDrawColor(0); // border is white
        u8g2.drawFilledEllipse(x, y, width / 2, height / 2, U8G2_DRAW_ALL);
        u8g2.setDrawColor(1); // black
        u8g2.drawEllipse(x, y, width / 2, height / 2, U8G2_DRAW_ALL);
    }
    // Zeichnen des Textes zentriert in der Ellipse
    if (filled)
    {
        u8g2.setDrawColor(0);
    }
    else
    {
        u8g2.setDrawColor(1);
    }
    u8g2.drawStr(x - textWidth / 2, y + 3, text);
    // u8g2.sendBuffer(); // Aktualisieren des Displays
}

void UserOutput::updateDisplay()
{
    u8g2.sendBuffer();
}

void UserOutput::drawSmiley(int x, int y, int radius)
{
    // Zeichnen des Gesichts
    u8g2.setDrawColor(0); // Weiß
    u8g2.drawCircle(x, y, radius, U8G2_DRAW_ALL);

    // Zeichnen der Augen
    int eyeRadius = radius / 5;
    int eyeOffsetX = radius / 2;
    int eyeOffsetY = radius / 3;
    u8g2.drawCircle(x - eyeOffsetX, y - eyeOffsetY, eyeRadius, U8G2_DRAW_ALL); // Linkes Auge
    u8g2.drawCircle(x + eyeOffsetX, y - eyeOffsetY, eyeRadius, U8G2_DRAW_ALL); // Rechtes Auge

    // Zeichnen des Mundes als Bogen
    int mouthWidth = radius;      // Mundbreite entsprechend dem Radius anpassen
    int mouthHeight = radius / 2; // Mundhöhe entsprechend dem Radius anpassen
    int mouthStartAngle = 10;     // Startwinkel des Mundbogens
    int mouthEndAngle = 170;      // Endwinkel des Mundbogens
    u8g2.drawEllipse(x, y + eyeOffsetY, mouthWidth / 2, mouthHeight / 2, U8G2_DRAW_LOWER_RIGHT | U8G2_DRAW_LOWER_LEFT);
}

void UserOutput::draw_encoderButton(int x, int y, int radius, bool filled, int enc_position)
{
    if (filled)
    {
        u8g2.drawFilledEllipse(x, y, radius / 2, radius / 2, U8G2_DRAW_ALL);
        u8g2.setDrawColor(0); // black
        u8g2.drawEllipse(x, y, radius / 2 - 2, radius / 2 - 2, U8G2_DRAW_ALL);
        // u8g2.setDrawColor(0); // black

        // u8g2.drawEllipse(x, y, radius / 2+1, radius / 2+1, U8G2_DRAW_ALL);
        // u8g2.setDrawColor(1); // black

        // u8g2.drawEllipse(x, y, radius / 2+2, radius / 2+2, U8G2_DRAW_ALL);
    }
    else
    {
        u8g2.setFont(u8g2_font_helvR08_tf);
        u8g2.setDrawColor(0); // border is white
        u8g2.drawFilledEllipse(x, y, radius / 2, radius / 2, U8G2_DRAW_ALL);
        u8g2.setDrawColor(1); // black
        u8g2.drawEllipse(x, y, radius / 2, radius / 2, U8G2_DRAW_ALL);
    }
}

void UserOutput::draw_encoderButton_with_label(int x, int y, int radius, bool filled, int enc_position, bool highlight, const char *text)
{
    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.setDrawColor(1); // Weiß
    int textWidth = u8g2.getStrWidth(text);

    // font format FONT_SMALL u8g2_font_helvR08_tf
    // font format FONT_BIG u8g2_font_helvR10_tf

    if (filled)
    {
        u8g2.drawFilledEllipse(x, y, radius / 2, radius / 2, U8G2_DRAW_ALL);
        u8g2.setDrawColor(0); // black
        u8g2.drawEllipse(x, y, radius / 2 - 2, radius / 2 - 2, U8G2_DRAW_ALL);
    }
    else
    {
        u8g2.setDrawColor(0); // border is white
        u8g2.drawFilledEllipse(x, y, radius / 2, radius / 2, U8G2_DRAW_ALL);
        u8g2.setDrawColor(1); // black
        u8g2.drawEllipse(x, y, radius / 2, radius / 2, U8G2_DRAW_ALL);
    }

    if (!highlight)
        draw_label(x + 12, y - 3, 52, 12, false, text);
    else
        draw_label(x + 12, y - 3, 52, 12, true, text);
}
void UserOutput::drawCircleWithLine(int x, int y, int radius, float encoderPosition)
{
    // Umrechnung von Grad in Bogenmaß für trigonometrische Funktionen
    // encoderposition [0..24]
    float angleDegrees = encoderPosition * (360.0 / 24.0);
    float angleRadians = angleDegrees * (PI / 180.0);

    // Berechnen des Endpunkts der Linie
    int lineEndX = x + radius * cos(angleRadians);
    int lineEndY = y + radius * sin(angleRadians);
    int lineStartX = x - radius * cos(angleRadians);
    int lineStartY = y - radius * sin(angleRadians);

    // Zeichnen des Kreises
    u8g2.setDrawColor(1); // Weiß
    u8g2.drawCircle(x, y, radius + 4, U8G2_DRAW_ALL);
    u8g2.setDrawColor(0);
    u8g2.drawLine(lineEndX, lineEndY, x, y);
    u8g2.setDrawColor(1);
    u8g2.drawFilledEllipse(x, y, radius - (radius / 2), radius - (radius / 3), U8G2_DRAW_ALL);
}

void UserOutput::draw_label(int x, int y, int w, int h, bool filled, const char *text)
{
    if (!filled)
    {
        u8g2.setDrawColor(1);
        u8g2.drawRBox(x, y, w, h, 4);
        u8g2.setDrawColor(0);
        u8g2.drawRBox(x + 1, y + 1, w - 2, h - 2, 2);

        u8g2.setFont(u8g2_font_helvR08_tf);
        u8g2.setDrawColor(1); // Weiß
        int textWidth = u8g2.getStrWidth(text);
        int textHeight = u8g2.getAscent();
        u8g2.setDrawColor(1);
        u8g2.drawStr(x + 5, y + textHeight + 5, text);
    }
    else
    {
        u8g2.setDrawColor(1);
        u8g2.drawRBox(x, y, w, h, 4);
        u8g2.setDrawColor(0);
        u8g2.drawRFrame(x, y, w, h, 4);
        u8g2.setFont(u8g2_font_helvR08_tf);
        int textWidth = u8g2.getStrWidth(text);
        int textHeight = u8g2.getAscent();
        u8g2.drawStr(x + 5, y + textHeight + 5, text);
    }
}

void UserOutput::draw_playbackMenu()
{
    // drawMonitor(5, 2, 73, 30, 0);

    // u8g2.drawFilledEllipse(15, 55, 18, 18, U8G2_DRAW_ALL);
    // draw_encoderButton(15, 55, 18, true, 0);

    // draw_label(31, 52, 50, 12, "hhalo");
    // updateDisplay();
}

void UserOutput::draw_arrow(int x, int y, int r, int angle)
{
    u8g2.setDrawColor(1);
    u8g2.drawFilledEllipse(x, y, r, r);

    float arrowLength = r * 0.2; // Länge der Pfeillinien
    int cx = x;
    int cy = y;

    // Berechnung der Koordinaten der Spitze des Pfeils auf der Kreislinie
    float arrow1x = cx + ((r - 7) * cos(angle * PI / 180)); // X-Koordinate der Spitze
    float arrow1y = cy + ((r - 7) * sin(angle * PI / 180)); // Y-Koordinate der Spitze

    // Berechnung der Koordinaten der Pfeilschenkel

    u8g2.setDrawColor(0);
    u8g2.drawFilledEllipse(arrow1x, arrow1y, 4, 4);
    // Zeichnen der Linien
    //   u8g2.drawLine(arrow1x, arrow1y, arrow2x,arrow2y);
    //   u8g2.drawLine(arrow1x, arrow1y, arrow3x, arrow3y);

    updateDisplay();
    // if (direction == 0)
    // {
    //     u8g2.drawTriangle(x, y, x + w, y + h / 2, x, y + h);
    // }
    // else
    // {
    //     u8g2.drawTriangle(x + w, y, x, y + h / 2, x + w, y + h);
    // }
}

UserOutput::menuItem_x UserOutput::playbackActions[UserOutput::numberOfPlaybackActions] = {
    {"PLAY", play_animation},
    {"CHANGE", change_cue},
    {"STOP", stop_action},
    {"EXIT", exit_playback}

};

void UserOutput::setMinMaxValues(int bounds[4])
{
    xmin = bounds[0];
    ymin = bounds[1];
    xmax = bounds[2];
    ymax = bounds[3];
}

void UserOutput::initPlaybackWindow()
{

    //       int window_width = 90;
    //   int window_height = 40;
    //   int window_x = (u8g2.getWidth() - window_width) / 2;
    //   int window_y = 5;

    x = 2;
    y = 2;
    w = 90;
    h = 40;

    playbackMenuTimer = millis();
    playBackMenu_interval = 2000;
    int playBackMenu_currentItem;
    u8g2.clearDisplay();
    u8g2.clearBuffer();
    u8g2.setDrawColor(1); // Weiß
    u8g2.drawRBox(x, y, w, h, 4);
    u8g2.sendBuffer();
}

void UserOutput::drawPlaybackWindow(int dmx_x, int dmx_y, int part, int step, bool valve)
{
    // prints diagnostic data and draws x/y values as line on a white window.

    int box_x = 3;
    int box_y = y + h + 4;
    int box_w = 120;
    int box_h = 15;
    int ofs_x = 5;
    int ofs_y = 12;
    int sidebox_x = 95;
    int sidebox_y = y;
    int sidebox_w = 30;
    int sidebox_h = h;

    // eeds to go intio an extra function or class to handle a buffer
    //  of the already drawn x/y values.
    //  also this needs a zoom function . (redefine window by max and min of the
    //  smiley array)) min and max von den eckpunkten

    // this paints x/y values on a white window made in initPlaybackWindow()
    int display_x = map(dmx_x, xmin, xmax, x, x + h) + w / 4;
    int display_y = map(dmx_y, ymax, ymin, y, y + h);  //vertauscht DIRTY FIX 

    if (valve)
        u8g2.setDrawColor(0); // Schwarz
    else
        u8g2.setDrawColor(1); // Weiß
    // u8g2.drawPixel(12,12);
    u8g2.drawBox(display_x, display_y, 1, 1);
   

    char buffer[15];

    u8g2.setDrawColor(1); // Weiß
    u8g2.drawRFrame(box_x, box_y, box_w, box_h, 4);
    //  u8g2.setFont(u8g2_font_helvB08_tf);
    u8g2.setFont(u8g2_font_NokiaSmallPlain_tf);

    // draws a sidebox to display x and y values
    u8g2.setDrawColor(1); // Weiß

    u8g2.drawRFrame(sidebox_x, sidebox_y, sidebox_w, sidebox_h, 4);
    u8g2.setDrawColor(0);
    sprintf(buffer, "x:%3d", dmx_x);
    u8g2.drawBox(sidebox_x + 3, sidebox_y + 3, u8g2.getStrWidth(buffer) + 1, 10);
    u8g2.setDrawColor(1);
    u8g2.drawStr(sidebox_x + 3, sidebox_y + 13, buffer);
    u8g2.setDrawColor(0);
    sprintf(buffer, "y:%3d", dmx_y);
    u8g2.drawBox(sidebox_x + 3, sidebox_y + 13, u8g2.getStrWidth(buffer) + 1, 10);
    u8g2.setDrawColor(1);
    u8g2.drawStr(sidebox_x + 3, sidebox_y + 23, buffer);
    u8g2.setDrawColor(0);
    sprintf(buffer, "%d", step);
    u8g2.drawBox(sidebox_x + 3, sidebox_y + 26, u8g2.getStrWidth(buffer) + 1, 10);
    u8g2.setDrawColor(1);
    u8g2.drawStr(sidebox_x + 3, sidebox_y + 35, buffer);

    drawStatusBar(part);

    u8g2.sendBuffer();
}

void UserOutput::init_4pts_menu()
{
    int margin_left = 5;
    int margin_top = 5;
    int window_width = 118; // 540 degrees of movement mapped to oled size
    int window_height = 40; // 270 degrees of movement mapped to oled size
    int window_radius = 3;
    int dot_r = 8; // radius of the dot

    u8g2.clearBuffer();
    u8g2.setDrawColor(1); // Weiß
    // u8g2.drawStr(0, 10, "4 Points");
    u8g2.drawRBox(margin_left, margin_top, window_width, window_height, window_radius);
    u8g2.setDrawColor(0); // Weiß
    // draw_dot_with_number(30, 20, dot_r, false, 1);
    // draw_dot_with_number(90, 20, dot_r, false, 2);
    // draw_dot_with_number(30, 45, dot_r, false, 4);
    // draw_dot_with_number(100, 45, dot_r, false, 3);

    int bar_margin_top = margin_top + window_height + 4;
    int bar_height = 15;
    char buffer[15];
    u8g2.setFont(u8g2_font_NokiaSmallPlain_te);
    sprintf(buffer, "hallo hier");
    // draw_4pts_bar(margin_left, bar_margin_top, window_width, bar_height, window_radius, buffer);
    u8g2.sendBuffer();
}

void UserOutput::draw_4pts_bar(int x, int y, int w, int h, int r, const char *text)
{
    u8g2.setDrawColor(1); // Weiß
    u8g2.drawRFrame(x, y, w, h, r);
    u8g2.drawStr(x + 5, y + 12, text);
}

void UserOutput::drawStatusBar(int part)
{
    char buffer[20];
    x = 2;
    y = 2;
    w = 90;
    h = 40;

    int box_x = 3;
    int box_y = y + h + 4;
    int box_w = 120;
    int box_h = 15;
    int ofs_x = 5;
    int ofs_y = 12;
    static unsigned long timer = millis();
    int interval = 5000;

    enum bar_state
    {
        NOWPLAYING,
        PRESSBUTTONTOPAUSE
    };
    static bar_state bar_state = NOWPLAYING;

    if (bar_state == NOWPLAYING)
    {

        // status box below
        // char buffer[15];
        sprintf(buffer, "Now Playing: ");
        u8g2.setDrawColor(0);
        u8g2.drawBox(box_x + ofs_x, box_y + ofs_y - 10, box_w - 8, 10);
        u8g2.setDrawColor(1);
        u8g2.drawStr(box_x + ofs_x, box_y + ofs_y, buffer);

        char buffer2[15];
        if (part == 0)
        {
            sprintf(buffer2, "Circle");
        }
        else if (part == 1)
        {
            sprintf(buffer2, "Mouth");
        }
        else if (part == 2)
        {
            sprintf(buffer2, "Eyes");
        }
        else if (part == 3)
        {
            sprintf(buffer2, "All");
        }
        else if (part == 4)
        {
            sprintf(buffer2, "SHOW");
        }
        else
        {
            sprintf(buffer2, "unknown");
        }
        u8g2.setDrawColor(0);
        // u8g2.drawBox(64, box_y + ofs_y - 10,0 , 10);
        u8g2.setDrawColor(1);
        u8g2.drawStr(64, box_y + ofs_y, buffer2);
        u8g2.sendBuffer();

        if (millis() - timer > interval)
        {
            bar_state = PRESSBUTTONTOPAUSE;
            timer = millis();
        }
    }
    // break;

    if (bar_state == PRESSBUTTONTOPAUSE)
    {
        char buffer[15];
        sprintf(buffer, "Press Button to PAUSE.");
        u8g2.setDrawColor(0);
        u8g2.drawBox(box_x + ofs_x, box_y + ofs_y - 10, box_w - 8, 10);
        u8g2.setDrawColor(1);
        u8g2.drawStr(box_x + ofs_x, box_y + ofs_y, buffer);
        if (millis() - timer > interval)
        {
            bar_state = NOWPLAYING;
            timer = millis();
        }
        // break;
    }
}

void UserOutput::draw_alert_window( const char *text)
{

    int x = 30;
    int y = 20;
    int w = 50;
    int h = 40;

    u8g2.setDrawColor(1);
    u8g2.drawRBox(x, y, w, h, 4);
    u8g2.setDrawColor(0);
    u8g2.drawRFrame(x + 1, y + 1, w - 2, h - 2, 4);
    u8g2.drawStr(x + 8, y + 14, text);
    u8g2.sendBuffer();
}
void UserOutput::drawContinueWindow()
{
    int x = 15;
    int y = 15;
    int w = 70;
    int h = 40;

    u8g2.setDrawColor(0);
    u8g2.drawRBox(x, y, w, h, 5);
    u8g2.setDrawColor(1);
    u8g2.drawRFrame(x + 1, y + 1, w - 2, h - 2, 4);
    u8g2.drawStr(x + 8, y + 14, "press button");
    u8g2.drawStr(x + 8, y + 24, "to continue.");
    u8g2.sendBuffer();
}
void UserOutput::drawAgainWindow()
{
    int x = 15;
    int y = 15;
    int w = 70;
    int h = 40;

    u8g2.setDrawColor(0);
    u8g2.drawRBox(x, y, w, h, 5);
    u8g2.setDrawColor(1);
    u8g2.drawRFrame(x + 1, y + 1, w - 2, h - 2, 4);
    u8g2.drawStr(x + 8, y + 14, "press button");
    u8g2.drawStr(x + 8, y + 24, "to run");
    u8g2.drawStr(x + 8, y + 34, "again");
    u8g2.sendBuffer();

}

void UserOutput::drawShowisoverWindow()
{
    int x = 15;
    int y = 15;
    int w = 70;
    int h = 40;
    u8g2.setDrawColor(0);
    u8g2.drawRBox(x, y, w, h, 5);
    u8g2.setDrawColor(1);
    u8g2.drawRFrame(x + 1, y + 1, w - 2, h - 2, 4);
    u8g2.drawStr(x + 8, y + 14, "the show");
    u8g2.drawStr(x + 8, y + 24, "is over.");
    u8g2.sendBuffer();
}

void UserOutput::drawBeginningPause()
{

    int x = 15;
    int y = 15;
    int w = 90;
    int h = 40;
    u8g2.setFont(u8g2_font_NokiaSmallPlain_tf);
    u8g2.setDrawColor(0);
    u8g2.drawRBox(x, y, w, h, 5);
    u8g2.setDrawColor(1);
    u8g2.drawRFrame(x + 1, y + 1, w - 2, h - 2, 4);

    u8g2.drawStr(x + 8, y + 14, "PLAYBACK !");
    u8g2.drawStr(x + 8, y + 24, "press yellow button ");
    u8g2.drawStr(x + 8, y + 34, "to start!!");

    u8g2.sendBuffer();
}
void UserOutput::drawPauseWindow()
{
    int x = 15;
    int y = 15;
    int w = 90;
    int h = 40;
    u8g2.setDrawColor(0);
    u8g2.drawRBox(x, y, w, h, 5);
    u8g2.setDrawColor(1);
    u8g2.drawRFrame(x + 1, y + 1, w - 2, h - 2, 4);

    u8g2.drawStr(x + 8, y + 14, "PAUSED");
    u8g2.drawStr(x + 8, y + 24, "press to resume");
    u8g2.drawStr(x + 8, y + 34, "hold to exit");

    u8g2.sendBuffer();
}

void UserOutput::draw_dot_with_number(int x, int y, int radius, bool filled, bool selected, int number)
// this drwas a cursoresque dot with a number in it
{
    if (filled)
    {
        if (selected)
        {
            u8g2.setDrawColor(1);
            u8g2.drawFilledEllipse(x, y, radius + 2, radius + 2);
            u8g2.setDrawColor(0);
            u8g2.drawCircle(x, y, radius + 2);
        }
        u8g2.setDrawColor(0);
        u8g2.drawFilledEllipse(x, y, radius, radius);
        u8g2.setDrawColor(1);
        u8g2.setFontDirection(0);
        char buf[2];
        sprintf(buf, "%d", number);
        u8g2.drawStr(x - radius / 2 + 2, y + radius / 2 + 2, buf);
    }
    else
    {
        if (selected)
        {
            u8g2.setDrawColor(1);
            u8g2.drawFilledEllipse(x, y, radius + 2, radius + 2);
            u8g2.setDrawColor(0);
            u8g2.drawCircle(x, y, radius + 2);
        }

        u8g2.setDrawColor(1);
        u8g2.drawFilledEllipse(x, y, radius, radius);
        u8g2.setDrawColor(0);
        u8g2.drawEllipse(x, y, radius, radius);
        u8g2.setDrawColor(0);
        u8g2.setFontDirection(0);
        char buf[2];
        sprintf(buf, "%d", number);
        u8g2.drawStr(x - radius / 2 + 2, y + radius / 2 + 2, buf);
    }
}

void UserOutput::drawFader(int x, int y, int width, int height, int value_8bit)
{

    // draw a rounded fader at the given position
    // x,y: upper left corner

    for (int i = 0; i < height; i++)
    {
        u8g2.setDrawColor(1);
        u8g2.drawFilledEllipse(x + (width / 2), y + (width / 2) + i, width / 2, width / 2);
    }
    int corrected_height = map(value_8bit, 0, 255, 0, height);
    for (int i = 0; i < height; i++)
    {
        int draw_value = map(value_8bit, 0, 255, 0, height);
        if (i < draw_value)
        {
            u8g2.setDrawColor(0);
            u8g2.drawFilledEllipse(x + (width / 2), (width / 2) + corrected_height - (width / 4) - i, width / 4, width / 4);
        }
    }
}

void UserOutput::draw_6_faders()
{
    // draw a row of 6 faders and play with its values! example
    for (int y = 0; y < 6; y++)
    {
        for (int i = 0; i < 255; i += 25)
        {
            drawFader(0 + (y * 128 / 6), 0, 10, 32, i);
            u8g2.sendBuffer();
            delay(0);
        }
        for (int i = 255; i > 0; i -= 25)
        {
            drawFader(0 + (y * 128 / 6), 0, 10, 32, i);
            u8g2.sendBuffer();
            delay(10);
        }
    }
}

// new functions for general use
void UserOutput::init_main_window(int x, int y, int w, int h, int r)
{
    main_window_x = x;
    main_window_y = y;
    main_window_w = w;
    main_window_h = h;
    main_window_r = r;
}

void UserOutput::draw_main_window()
{
    u8g2.setDrawColor(1); // Weiß
    u8g2.drawRBox(main_window_x, main_window_y, main_window_w, main_window_h, main_window_r);
}

void UserOutput::draw_txt_in_window(const char *row1, const char *row2, const char *row3)
{
    u8g2.setDrawColor(1); // Weiß
    u8g2.drawRBox(main_window_x, main_window_y, main_window_w, main_window_h, main_window_r);
    int margin_left = 5;
    int margin_top = 15;
    int row_height = 13;
    int row_1_x = main_window_x + margin_left;
    int row_1_y = main_window_y + margin_top;
    int row_2_x = main_window_x + margin_left;
    int row_2_y = main_window_y + margin_top + row_height;
    int row_3_x = main_window_x + margin_left;
    int row_3_y = main_window_y + margin_top + 2 * row_height;
    u8g2.setDrawColor(0); // xor
    u8g2.setFont(u8g2_font_NokiaSmallPlain_tf);
    for (int i = 0; i < 3; i++)
    {
        if (i == 0)
            u8g2.drawStr(row_1_x, row_1_y, row1);
        if (i == 1)
            u8g2.drawStr(row_2_x, row_2_y, row2);
        if (i == 2)
            u8g2.drawStr(row_3_x, row_3_y, row3);
    }
}

void UserOutput::init_status_bar(int x, int y, int w, int h, int r)
{
    status_bar_x = x;
    status_bar_y = y;
    status_bar_w = w;
    status_bar_h = h;
    status_bar_r = r;
}

void UserOutput::textloop_in_status_bar_2_items(int interval, const char *text1, const char *text2)
{

    // defect code is nich gut hat ne while
    static unsigned long textTimer = millis();
    static bool state = false;


if (millis() - textTimer > interval){
    textTimer=millis();
    state = !state;
}

if (state) draw_status_bar(true, text1);
else draw_status_bar(false, text2);

}
void UserOutput::init_side_box(int x, int y, int w, int h)
{
    side_box_x = x;
    side_box_y = y;
    side_box_w = w;
    side_box_h = h;
}
void UserOutput::draw_side_box()
{
}

void UserOutput::draw_status_bar(bool filled, const char *text)
{
    draw_label(status_bar_x, status_bar_y, status_bar_w, status_bar_h, filled, text);
}

void play_animation() {}
void change_cue() {}
void stop_action() {}
void exit_playback() {}

