// Modifiers
#define LEFT_CTRL 65507
#define LEFT_SHIFT 65505
#define LEFT_ALT 65513
#define RIGHT_CTRL 65508
#define RIGHT_SHIFT 65506
#define RIGHT_ALT 65514

// Misc keys
#define UP_ARROW 65362
#define DOWN_ARROW 65364
#define LEFT_ARROW 65361
#define RIGHT_ARROW 65363
#define BACKSPACE 65288
#define TAB 65289
#define RETURN 65293
#define MENU 65515  // "Keyboard Application" in USB standard ????
#define ESC 65307
#define INSERT 65379
#define DELETE 65535
#define PAGE_UP 65365
#define PAGE_DOWN 65366
#define HOME 65360
#define END 65367
#define CAPS_LOCK 65509
#define PRINT_SCREEN 65377  // Print Screen / SysRq
#define SCROLL_LOCK 65
#define PAUSE 65  // Pause / Break

// Numeric keypad  TODO fill these out
#define NUM_LOCK 0xDB
#define KP_SLASH 0xDC
#define KP_ASTERISK 0xDD
#define KP_MINUS 0xDE
#define KP_PLUS 0xDF
#define KP_ENTER 0xE0
#define KP_1 0xE1
#define KP_2 0xE2
#define KP_3 0xE3
#define KP_4 0xE4
#define KP_5 0xE5
#define KP_6 0xE6
#define KP_7 0xE7
#define KP_8 0xE8
#define KP_9 0xE9
#define KP_0 0xEA
#define KP_DOT 0xEB

// Functn keys
#define F1 65470
#define F2 65471
#define F3 65472
#define F4 65473
#define F5 65474
#define F6 65475
#define F7 65476
#define F8 65477
#define F9 65478
#define F10 65479
#define F11 65480
#define F12 65481


#include "Keyboard.h"
#include <Mouse.h>
#include  <Adafruit_NeoPixel.h>
const uint8_t LED_PIN    = 16; // RP2040-Zero GPIO of onboard WS2812 RGB LED

const uint8_t LED_COUNT  =  1; //60
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
const uint8_t BRIGHTNESS = 51; // Set BRIGHTNESS to 1/5 (255/5 = 51)

void colorWipe(uint32_t color) {
// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
  //
  for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    //
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
  } 
} // colorWipe()


void setup() {
  Serial1.begin(115200);
  Keyboard.begin();
  Mouse.begin();
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  //
  strip.setBrightness(BRIGHTNESS);
  colorWipe(strip.Color(0,0,0));
}
unsigned long last_press = 0;
  int r = 0;

void loop() {
  int g = 255;
  int b = 0;

  if (Serial1.available() > 0) {
    String in = Serial1.readStringUntil('\n');
    if (in[0] == '$' && in[in.length() - 1] == '#') {  //valid msg
      if (in[1] == '(') {
        r = 255;
        keyboard(in[2], in.substring(5, in.length() - 2));
      } else {
        b = 255;
        mouse(in[2], in.substring(5, in.length() - 2));
      }
    }
  }
  if (millis() - last_press >= 3*1000UL) //release all buttuns if none pressed for 3 seconds
  {
    r=0;
   Keyboard.releaseAll();
  }
  colorWipe(strip.Color(r,g,b));
}

void keyboard(char info, String data) {
   last_press = millis();

  int out = data.toInt();
  if (out > 6500) {
    switch (out) {
      case LEFT_CTRL:
        out = KEY_LEFT_CTRL;
        break;
      case LEFT_SHIFT:
        out = KEY_LEFT_SHIFT;
        break;
      case LEFT_ALT:
        out = KEY_LEFT_ALT;
        break;
      case RIGHT_CTRL:
        out = KEY_RIGHT_CTRL;
        break;
      case RIGHT_SHIFT:
        out = KEY_RIGHT_SHIFT;
        break;
      case RIGHT_ALT:
        out = KEY_RIGHT_ALT;
        break;
      case UP_ARROW:
        out = KEY_UP_ARROW;
        break;
      case DOWN_ARROW:
        out = KEY_DOWN_ARROW;
        break;
      case LEFT_ARROW:
        out = KEY_LEFT_ARROW;
        break;
      case RIGHT_ARROW:
        out = KEY_RIGHT_ARROW;
        break;
      case BACKSPACE:
        out = KEY_BACKSPACE;
        break;
      case TAB:
        out = KEY_TAB;
        break;
      case RETURN:
        out = KEY_RETURN;
        break;
      case MENU:
        out = KEY_MENU;
        break;
      case ESC:
        out = KEY_ESC;
        break;
      case INSERT:
        out = KEY_INSERT;
        break;
      case DELETE:
        out = KEY_DELETE;
        break;
      case PAGE_UP:
        out = KEY_PAGE_UP;
        break;
      case PAGE_DOWN:
        out = KEY_PAGE_DOWN;
        break;
      case HOME:
        out = KEY_HOME;
        break;
      case END:
        out = KEY_END;
        break;
      case CAPS_LOCK:
        out = KEY_CAPS_LOCK;
        break;
      case PRINT_SCREEN:
        out = KEY_PRINT_SCREEN;
        break;
      case F1:
        out = KEY_F1;
        break;
      case F2:
        out = KEY_F2;
        break;
      case F3:
        out = KEY_F3;
        break;
      case F4:
        out = KEY_F4;
        break;
      case F5:
        out = KEY_F5;
        break;
      case F6:
        out = KEY_F6;
        break;
      case F7:
        out = KEY_F7;
        break;
      case F8:
        out = KEY_F8;
        break;
      case F9:
        out = KEY_F9;
        break;
      case F10:
        out = KEY_F10;
        break;
      case F11:
        out = KEY_F11;
        break;
      case F12:
        out = KEY_F12;
        break;
    }
  }
  if (info == '1') {  //pressed down
    Keyboard.press(out);
  } else {  //release
    Keyboard.release(out);
  }
}

void mouse(char info, String data) {
  if (info == '0') {  //movement
    data = data.substring(3);
    int idx = data.indexOf(",");
    int x = data.substring(0, idx).toInt();
    int y = data.substring(idx + 1).toInt();

    Mouse.move(x, y, 0);


  } else if (info == '1') {  //left1234
    char press = data[0];
    if (press == '1') {
      Mouse.press(MOUSE_LEFT);
    } else {
      Mouse.release(MOUSE_LEFT);
    }

  } else if (info == '2') {  //wheel
    char press = data[0];
    if (press == '1') {
      Mouse.press(MOUSE_MIDDLE);
    } else {
      Mouse.release(MOUSE_MIDDLE);
    }

  } else if (info == '3') {  //right
    char press = data[0];
    if (press == '1') {
      Mouse.press(MOUSE_RIGHT);
    } else {
      Mouse.release(MOUSE_RIGHT);
    }

  } else if (info == '4') {  //scroll      press = data[0]
    int idx = data.indexOf(",");
    data = data.substring(3);
    int scrollx = data.substring(0, idx).toInt();
    int scrolly = data.substring(idx + 1).toInt();
    Mouse.move(0, 0, scrolly);
  }
}