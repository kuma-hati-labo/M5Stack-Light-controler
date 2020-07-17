// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <M5Stack.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include <stdio.h>


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        21 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 5 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

#define LIGHT_STATE_ON 1
#define LIGHT_STATE_OFF 0
#define COLOR_VALUE_STEP 25
#define COLOR_VALUE_MAX 10
#define COLOR_VALUE_MINI 1
#define LIGHT_VALUE_START 5

// 初期状態は ON
int lightState = LIGHT_STATE_ON;      // 初期状態は点灯
int redValue = COLOR_VALUE_STEP * LIGHT_VALUE_START;
int greenValue = COLOR_VALUE_STEP * LIGHT_VALUE_START;
int blueValue = COLOR_VALUE_STEP * LIGHT_VALUE_START;

int lightValue = LIGHT_VALUE_START;


void setup() {
  // Initialize the M5Stack object
  M5.begin();
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("Brightness Controler");

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  // 初期状態は点灯
  lightCtrl(lightState, lightValue);
  updateLCD(lightValue);
}

void lightCtrl(int state, int value) {
  if (LIGHT_STATE_ON == state) {
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(COLOR_VALUE_STEP * value, COLOR_VALUE_STEP * value, COLOR_VALUE_STEP * value));
    }
  } else {
    pixels.clear();
  }
  pixels.show();   // Send the updated pixel colors to the hardware. 
}

void updateLCD(int level) {
  const char *formStr = "Brightness level:%2d";
  char buf[strlen(formStr) + 1];
  M5.Lcd.fillRect(204, 20, 24, 20, 0x0821);
  M5.Lcd.setCursor(0, 20);
  sprintf(buf,formStr, level);
  M5.Lcd.print(buf);
}

void loop() {
  if (M5.BtnA.wasPressed()) {
    if (lightValue > COLOR_VALUE_MINI) {
      lightValue -= 1;
      lightCtrl(lightState, lightValue);
      updateLCD(lightValue);
    }
  }
  if (M5.BtnC.wasPressed()) {
    if (lightValue < COLOR_VALUE_MAX) {
      lightValue += 1;
      lightCtrl(lightState, lightValue);
      updateLCD(lightValue);
    }
  }
  if (M5.BtnB.wasPressed()) {
    if (LIGHT_STATE_ON == lightState) {
      lightState = LIGHT_STATE_OFF;
    } else {
      lightState = LIGHT_STATE_ON;
    }
    lightCtrl(lightState, lightValue);
  }
  m5.update();
}
