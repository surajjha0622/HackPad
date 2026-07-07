// HackPad firmware for Seeed Studio XIAO RP2040

#include <Keyboard.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

// ---------------- pin map ----------------
constexpr uint8_t ROW_PINS[3] = { D0, D1, D2 };
constexpr uint8_t COL_PINS[2] = { D3, D10 };
constexpr uint8_t ENC_A_PIN   = D9;
constexpr uint8_t ENC_B_PIN   = D8;
constexpr uint8_t ENC_SW_PIN  = D7;

// Onboard XIAO RP2040 NeoPixel (not on the HackPad schematic - built into the module)
constexpr uint8_t NEOPIXEL_POWER_PIN = 11;
constexpr uint8_t NEOPIXEL_DATA_PIN  = 12;
Adafruit_NeoPixel statusLED(1, NEOPIXEL_DATA_PIN, NEO_GRB + NEO_KHZ800);

// ---------------- OLED ----------------
constexpr uint8_t SCREEN_W = 128;
constexpr uint8_t SCREEN_H = 32;
constexpr uint8_t OLED_ADDR = 0x3C;   // try 0x3D if init fails
Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, -1);

// ---------------- keymap: edit here to remap ----------------
// One HID keycode per switch. `modifier` is a single optional modifier key
// (KEY_LEFT_CTRL / KEY_LEFT_SHIFT / KEY_LEFT_ALT / KEY_LEFT_GUI), or 0.
struct KeyAction {
  uint8_t modifier;
  uint8_t key;
  const char* label;
};

KeyAction keymap[3][2] = {
  { {0,             'z',        "Undo"},  {0,             'y',       "Redo"}  },
  { {0,             'c',        "Copy"},  {0,             'v',       "Paste"} },
  { {KEY_LEFT_CTRL, 's',        "Save"},  {0,             KEY_RETURN,"Enter"} },
};

// ---------------- debounce state ----------------
constexpr uint16_t DEBOUNCE_MS = 15;
bool keyRaw[3][2]         = {false};
bool keyStable[3][2]      = {false};
uint32_t keyLastChange[3][2] = {0};

// ---------------- encoder state ----------------
uint8_t encPrevState = 0;
bool encSwRaw = false, encSwStable = false;
uint32_t encSwLastChange = 0;

const char* lastAction = "Ready";

void setup() {
  for (uint8_t r = 0; r < 3; r++) pinMode(ROW_PINS[r], INPUT);   // idle = Hi-Z
  for (uint8_t c = 0; c < 2; c++) pinMode(COL_PINS[c], INPUT_PULLUP);

  pinMode(ENC_A_PIN, INPUT_PULLUP);
  pinMode(ENC_B_PIN, INPUT_PULLUP);
  pinMode(ENC_SW_PIN, INPUT_PULLUP);
  encPrevState = (digitalRead(ENC_A_PIN) << 1) | digitalRead(ENC_B_PIN);

  Keyboard.begin();

  pinMode(NEOPIXEL_POWER_PIN, OUTPUT);
  digitalWrite(NEOPIXEL_POWER_PIN, HIGH);   // XIAO RP2040 needs this to power the LED
  statusLED.begin();
  statusLED.setBrightness(40);
  setLED(0, 20, 20);   // idle color

  Wire.begin();
  if (display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    display.setTextColor(SSD1306_WHITE);
    drawScreen();
  }
}

void loop() {
  scanMatrix();
  pollEncoder();
  pollEncoderSwitch();
}

// ---------------- matrix scan ----------------
void scanMatrix() {
  for (uint8_t r = 0; r < 3; r++) {
    pinMode(ROW_PINS[r], OUTPUT);
    digitalWrite(ROW_PINS[r], LOW);
    delayMicroseconds(5);

    for (uint8_t c = 0; c < 2; c++) {
      bool pressed = (digitalRead(COL_PINS[c]) == LOW);
      if (pressed != keyRaw[r][c]) {
        keyRaw[r][c] = pressed;
        keyLastChange[r][c] = millis();
      }
      if (millis() - keyLastChange[r][c] > DEBOUNCE_MS &&
          keyRaw[r][c] != keyStable[r][c]) {
        keyStable[r][c] = keyRaw[r][c];
        handleKey(r, c, keyStable[r][c]);
      }
    }

    pinMode(ROW_PINS[r], INPUT);   // release row before scanning the next
  }
}

void handleKey(uint8_t r, uint8_t c, bool pressed) {
  KeyAction &a = keymap[r][c];
  if (pressed) {
    if (a.modifier) Keyboard.press(a.modifier);
    Keyboard.press(a.key);
    lastAction = a.label;
    setLED(0, 255, 60);
    drawScreen();
  } else {
    Keyboard.release(a.key);
    if (a.modifier) Keyboard.release(a.modifier);
    setLED(0, 20, 20);
  }
}

// ---------------- rotary encoder (quadrature) ----------------
void pollEncoder() {
  uint8_t state = (digitalRead(ENC_A_PIN) << 1) | digitalRead(ENC_B_PIN);
  if (state != encPrevState) {
    static const int8_t table[16] = {
       0, -1,  1,  0,
       1,  0,  0, -1,
      -1,  0,  0,  1,
       0,  1, -1,  0
    };
    int8_t dir = table[(encPrevState << 2) | state];
    encPrevState = state;
    if (dir > 0) {
      Keyboard.write(KEY_UP_ARROW);     // swap for media keys if you prefer
      lastAction = "Scroll +";
      drawScreen();
    } else if (dir < 0) {
      Keyboard.write(KEY_DOWN_ARROW);
      lastAction = "Scroll -";
      drawScreen();
    }
  }
}

void pollEncoderSwitch() {
  bool pressed = (digitalRead(ENC_SW_PIN) == LOW);
  if (pressed != encSwRaw) {
    encSwRaw = pressed;
    encSwLastChange = millis();
  }
  if (millis() - encSwLastChange > DEBOUNCE_MS && pressed != encSwStable) {
    encSwStable = pressed;
    if (pressed) {
      Keyboard.press(KEY_LEFT_GUI);   // default: launcher/search key tap
      lastAction = "Knob press";
      setLED(60, 0, 255);
      drawScreen();
    } else {
      Keyboard.release(KEY_LEFT_GUI);
      setLED(0, 20, 20);
    }
  }
}

// ---------------- display / LED helpers ----------------
void drawScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("HackPad");
  display.setTextSize(2);
  display.setCursor(0, 14);
  display.println(lastAction);
  display.display();
}

void setLED(uint8_t r, uint8_t g, uint8_t b) {
  statusLED.setPixelColor(0, statusLED.Color(r, g, b));
  statusLED.show();
}
