//#include <FastLED.h>
#include "FastLEDPatterns.h"

CRGB led_array[LED_NUM_0
#ifdef LED_NUM_1
+ LED_NUM_1
#endif
#ifdef LED_NUM_2
+ LED_NUM_2
#endif
#ifdef LED_NUM_3
+ LED_NUM_3
#endif
];

FastLEDPatterns ledPatterns[LED_PATTERN_NUM] = {
  FastLEDPatterns(LED_NUM_0, &led_array[0])  // &led_array[0] -> Pointer to address of start index for this pattern
#ifdef LED_NUM_1
  , FastLEDPatterns(LED_NUM_1, &led_array[LED_NUM_0])
#endif
};

int patternsNum = sizeof(ledPatterns) / sizeof(ledPatterns[0]);

void fastLedSetup() {
#ifdef ROOM_KUECHE 
  /* Pattern:   Top, Button
   * Channels:  1  
   * Order:     Top -> Button
   */
  FastLED.addLeds<LED_TYPE, LED_DATA_PIN, LED_COLOR_ORDER>(led_array, LED_NUM_0 + LED_NUM_1);
#endif

#ifdef ROOM_TEST
  FastLED.addLeds<LED_TYPE, LED_DATA_PIN_0, LED_COLOR_ORDER>(led_array, 0, LED_NUM_1);
  FastLED.addLeds<LED_TYPE, LED_DATA_PIN_1, LED_COLOR_ORDER>(led_array, LED_NUM_1, LED_NUM_2);
#endif
  FastLED.setCorrection( TypicalSMD5050 );
  FastLED.setBrightness(150);
}

void fastLedUpdate() {
  // 1. Update all Patterns
  for (int i = 0; i < LED_PATTERN_NUM; i++) {
    ledPatterns[i].Update();
  }

  // 2. If at least one stripe was updated -> send data out & clear all "updated"-flags
  for (int i = 0; i < LED_PATTERN_NUM; i++) {
    if (ledPatterns[i].updated) {
      FastLED.show(); // this refreshs all stripes!
      // that's why we have to clear all "updated"-flags
      for (int i = 0; i < LED_PATTERN_NUM; i++) {
        ledPatterns[i].updated = false;
      }
    }
  }
}

bool allLedsOff() {
  for (int i = 0; i < LED_PATTERN_NUM; i++) {
    if (ledPatterns[i].getLightOn())
      return false;
  }
  return true;
}

const ColorTemperature colorTemp[20] = {
  UncorrectedTemperature,

  Candle          /* 1900 K, 255, 147, 41 */,
  Tungsten40W     /* 2600 K, 255, 197, 143 */,
  Tungsten100W    /* 2850 K, 255, 214, 170 */,
  Halogen         /* 3200 K, 255, 241, 224 */,
  CarbonArc       /* 5200 K, 255, 250, 244 */,
  HighNoonSun     /* 5400 K, 255, 255, 251 */,
  DirectSunlight  /* 6000 K, 255, 255, 255 */,
  OvercastSky     /* 7000 K, 201, 226, 255 */,
  ClearBlueSky    /* 20000 K, 64, 156, 255 */,

  WarmFluorescent           /* 0 K, 255, 244, 229 */,
  StandardFluorescent       /* 0 K, 244, 255, 250 */,
  CoolWhiteFluorescent      /* 0 K, 212, 235, 255 */,
  FullSpectrumFluorescent   /* 0 K, 255, 244, 242 */,
  GrowLightFluorescent      /* 0 K, 255, 239, 247 */,
  BlackLightFluorescent     /* 0 K, 167, 0, 255 */,
  MercuryVapor              /* 0 K, 216, 247, 255 */,
  SodiumVapor               /* 0 K, 255, 209, 178 */,
  MetalHalide               /* 0 K, 242, 252, 255 */,
  HighPressureSodium        /* 0 K, 255, 183, 76 */
};
uint8_t current_color_temp;

bool setColorTemperature(unsigned long value) {
  if ( !(value <= 20) )
    return false;
  current_color_temp = value;
  FastLED.setTemperature(colorTemp[value]);
  return true;
}

