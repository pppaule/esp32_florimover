#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h> 
#include <SPI.h>
#include <Wire.h> 
#include <Adafruit_MCP23X08.h>
#include <Adafruit_MCP23X17.h>


// Display initialization
#define SCREEN_WIDTH 135
#define SCREEN_HEIGHT 240

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// --- Define Colors ---
// Use 16-bit color codes (RGB565)
#define COLOR_BLACK     0x0000 // Black
#define COLOR_WHITE     0xFFFF // White
#define COLOR_BLUE_DARK 0x001F // Dark Blue (for pressed state)
#define COLOR_BLUE_LIGHT 0x03E0 // Light Blue (for normal state, example)

// Port IO extender
Adafruit_MCP23X17 mcp;

const int NUM_BUTTONS = 8;
// Array to store Button objects
class Button; // Forward declaration of the Button class
Button* buttons[NUM_BUTTONS];

// --- Button Class ---
// This class encapsulates all properties and behaviors of a button.
class Button {
public:
    int16_t x, y, w, h;     // Position (x, y) and size (width, height) of the button on the display
    String label;           // The text displayed on the button
    bool isPressed;         // Current state of the button (true = pressed, false = not pressed)
    uint8_t mcpPin;         // The pin on the MCP23017 assigned to this button (0-7 for PortA)
    uint16_t normalColor;   // Background color in normal state
    uint16_t pressedColor;  // Background color in pressed state

    // Constructor for the Button class
    Button(int16_t xPos, int16_t yPos, int16_t width, int16_t height, String text, uint8_t pin, uint16_t normColor, uint16_t pressColor) {
        x = xPos;
        y = yPos;
        w = width;
        h = height;
        label = text;
        mcpPin = pin;
        normalColor = normColor;
        pressedColor = pressColor;
        isPressed = false; // Initial state is "not pressed"
    }


// Enum to define the possible states for an LED
enum LedState {
    LED_STATE_NON_PRESSED,  // 10% duty cycle
    LED_STATE_PRESSED,      // 60% duty cycle
    LED_STATE_FIXED,        // 60% duty cycle, constant on
    LED_STATE_BLINKING      // 60% duty cycle, 0.5 sec on/off interval
};

// Led Class: Manages the state and behavior of a single LED
class Led {
public:
    uint8_t pin;             // The microcontroller pin connected to the LED
    LedState state;          // Current state of the LED
    unsigned long lastBlinkTime; // Stores the last time the LED state changed for blinking
    bool blinkOn;            // True if LED should be on during blinking cycle
    const unsigned long BLINK_INTERVAL_MS = 500; // Blinking interval in milliseconds

    // Constructor: Initializes the LED pin and sets initial state
    Led(uint8_t ledPin) : pin(ledPin), state(LED_STATE_NON_PRESSED), lastBlinkTime(0), blinkOn(true) {
        pinMode(pin, OUTPUT); // Configure the pin as an output
        analogWrite(pin, 0);  // Ensure LED is off initially
    }

    // Sets the new state for the LED
    void setState(LedState newState) {
        if (state != newState) { // Only update if the state has changed
            state = newState;
            // Reset blinking state if changing to a non-blinking state or new blinking state
            if (state != LED_STATE_BLINKING) {
                analogWrite(pin, 0); // Turn off before setting new state if not blinking
            }
            lastBlinkTime = millis(); // Reset timer for blinking
            blinkOn = true; // Start blinking cycle with LED on
        }
    }

    // Updates the LED's brightness or blinking state based on its current state
    void update() {
        int dutyCycle = 0;
        switch (state) {
            case LED_STATE_NON_PRESSED:
                dutyCycle = (int)(255 * 0.10); // 10% duty cycle
                analogWrite(pin, dutyCycle);
                break;
            case LED_STATE_PRESSED:
                dutyCycle = (int)(255 * 0.60); // 60% duty cycle
                analogWrite(pin, dutyCycle);
                break;
            case LED_STATE_FIXED:
                dutyCycle = (int)(255 * 0.60); // 60% duty cycle, constant on
                analogWrite(pin, dutyCycle);
                break;
            case LED_STATE_BLINKING:
                // Check if it's time to toggle the LED for blinking
                if (millis() - lastBlinkTime >= BLINK_INTERVAL_MS) {
                    blinkOn = !blinkOn; // Toggle blink state
                    lastBlinkTime = millis(); // Reset blink timer
                }
                dutyCycle = blinkOn ? (int)(255 * 0.60) : 0; // 60% on, 0% off
                analogWrite(pin, dutyCycle);
                break;
        }
    }
};

Led* leds[NUM_BUTTONS]; // Array to store Led objects, one for each button



    // Method to draw the button on the display
    void draw() {
        // Fill background based on state
        if (isPressed) {
            tft.fillRect(x, y, w, h, pressedColor);
        } else {
            tft.fillRect(x, y, w, h, normalColor);
        }

        // Draw border around the button
        tft.drawRect(x, y, w, h, COLOR_WHITE); // White border

        // Text settings
        tft.setTextSize(2); // Standard text size, can be adjusted
        tft.setTextColor(COLOR_WHITE); // White text color

        // Draw text centered within the button
        int16_t x1, y1;
        uint16_t wText, hText;
        // The getTextBounds function determines the size of the text
        tft.getTextBounds(label, 0, 0, &x1, &y1, &wText, &hText);

        // Calculate cursor position for centered text
        tft.setCursor(x + (w - wText) / 2, y + (h - hText) / 2);
        tft.print(label);
    }

    // Method to update the button state and redraw if changed
    void updateState(bool newPressedState) {
        // Only redraw if the state has actually changed
        if (isPressed != newPressedState) {
            isPressed = newPressedState;
            draw(); // Redraw the button to reflect the new state
            // Optional: Here you could call a callback function
            // when the button is pressed or released.
            // Example: if (isPressed) { executeButtonAction(); }
        }
    }
};

// --- Setup Function ---
void setup() {
  Serial.begin(115200); // Serial communication for debugging
  Serial.print(F("Hello Flori Mover"));

  // turn on backlite
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);

  // turn on the TFT / I2C power supply
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);

    tft.init(SCREEN_WIDTH, SCREEN_HEIGHT);
    tft.setRotation(0); 
    tft.fillScreen(COLOR_BLACK); 

    if (!mcp.begin_I2C()) {
        Serial.println("Error initializing MCP23017! Check wiring and address.");
        while (1); // Infinite loop on error
    }
    Serial.println("MCP23017 successfully initialized.");

 
    for (int i = 0; i < 8; i++) {
        mcp.pinMode(i, INPUT_PULLUP);
    }

    // --- Define Button Layout ---
    // We will arrange 4 arrow buttons in a cross shape in the center,
    // and the remaining 4 utility buttons at the top and bottom.

    int buttonMargin = 5; // Margin between buttons and to the edge
    int arrowButtonSize = 55; // Size for the square arrow buttons

    // Calculate center coordinates for arrow pad
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;


    // Button for UP (MCP Pin 6)
    buttons[6] = new Button(centerX - arrowButtonSize / 2, centerY - 1.5 * arrowButtonSize - buttonMargin,
                            arrowButtonSize, arrowButtonSize, "UP", 7, COLOR_BLACK, COLOR_BLUE_DARK);

    // Button for DOWN (MCP Pin 7)
    buttons[7] = new Button(centerX - arrowButtonSize / 2, centerY + buttonMargin + arrowButtonSize/2,
                            arrowButtonSize, arrowButtonSize, "DOWN", 4, COLOR_BLACK, COLOR_BLUE_DARK);

    // Button for LEFT (MCP Pin 5)
    buttons[5] = new Button(centerX - arrowButtonSize - buttonMargin, centerY - arrowButtonSize / 2,
                            arrowButtonSize, arrowButtonSize, "LEFT", 5, COLOR_BLACK, COLOR_BLUE_DARK);

    // Button for RIGHT (MCP Pin 4)
    buttons[4] = new Button(centerX + buttonMargin, centerY - arrowButtonSize / 2,
                            arrowButtonSize, arrowButtonSize, "RIGHT", 6, COLOR_BLACK, COLOR_BLUE_DARK);

    // --- Utility Buttons (MCP Pins 0, 1, 2, 3) ---
    // These buttons will be placed at the top and bottom.
    int topButtonWidth = (SCREEN_WIDTH - 4 * buttonMargin) / 2;
    int topButtonHeight = 30; // Fixed height for top buttons

    // Top row utility buttons
    buttons[0] = new Button(centerX - topButtonWidth - 2 * buttonMargin, buttonMargin, topButtonWidth, topButtonHeight, "FOC-", 1, COLOR_BLACK, COLOR_BLUE_DARK);
    buttons[1] = new Button(centerX + buttonMargin / 2, buttonMargin, topButtonWidth, topButtonHeight, "FOC+", 0, COLOR_BLACK, COLOR_BLUE_DARK);
 
    // Bottom utility button
    int bottomButtonHeight = 30;
     buttons[2] = new Button(centerX - topButtonWidth - 2 * buttonMargin, SCREEN_HEIGHT - bottomButtonHeight - buttonMargin,
                            topButtonWidth, bottomButtonHeight, "FLPX", 2, COLOR_BLACK, COLOR_BLUE_DARK);

    buttons[3] = new Button(centerX + buttonMargin / 2 , SCREEN_HEIGHT - bottomButtonHeight - buttonMargin,
                            topButtonWidth, bottomButtonHeight, "FLPY", 3, COLOR_BLACK, COLOR_BLUE_DARK);

    // Draw all buttons initially on the display
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttons[i]->draw();
    }
    // --- Initialize LEDs ---
    uint8_t ledPins[NUM_BUTTONS] = {12, 13, A0, A1, A2, A3, A4, A5};

    // Initialize Led objects, associating them with their respective pins
    for (int i = 0; i < NUM_BUTTONS; i++) {
        leds[i] = new Led(ledPins[i]);
    }
}


    // Example of how to manually set LED states (e.g., for fixed or blinking modes)
    // This is for demonstration. You would integrate this into your application logic.
    // For instance, pressing another button could cycle through these modes for a specific LED.
    // leds[0]->setState(LED_STATE_FIXED);    // Set LED 0 to fixed 60% brightness
    // leds[1]->setState(LED_STATE_BLINKING); // Set LED 1 to blinking
  

void loop() {
    // Loop through all defined buttons
    for (int i = 0; i < NUM_BUTTONS; i++) {
        // Read the current state of the MCP pin.
        // Since we use INPUT_PULLUP, the pin is LOW when the button is pressed.
        // Therefore, we invert the value to get 'true' for pressed.
        bool currentButtonState = !mcp.digitalRead(buttons[i]->mcpPin);

        // Update the button state and redraw if changed
        buttons[i]->updateState(currentButtonState);
    }

    // A small delay to help debounce the buttons
    // and avoid unnecessary CPU load.
    // For more robust debouncing, a timer-based solution is recommended.
    delay(50);
    // Update all LEDs based on their current state (handles blinking, PWM, etc.)
    for (int i = 0; i < NUM_BUTTONS; i++) {
        if (leds[i] != nullptr) { // Ensure the LED object exists
            leds[i]->update();
        }
    }
}
