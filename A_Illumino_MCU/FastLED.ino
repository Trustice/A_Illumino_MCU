//#include <FastLED.h>
#include "FastLEDPatterns.h"

FastLEDPatterns ledPatterns[LED_ARRAY_NUM] = {
#ifdef LED_ARRAY_1_NUM
  FastLEDPatterns(LED_ARRAY_1_NUM)
#endif
#ifdef LED_ARRAY_2_NUM
  , FastLEDPatterns(LED_ARRAY_2_NUM)
#endif
};

int patternsNum = sizeof(ledPatterns) / sizeof(ledPatterns[0]);

void fastLedSetup() {
#ifdef LED_STRIPE_1
  FastLED.addLeds<LED_TYPE_1, LED_DATA_PIN_1, LED_COLOR_ORDER_1>(ledPatterns[LED_ARRAY_INDEX_1].getArray(), LED_NUM_1);
#endif

#ifdef LED_STRIPE_2
  FastLED.addLeds<LED_TYPE_2, LED_DATA_PIN_2, LED_COLOR_ORDER_2>(ledPatterns[LED_ARRAY_INDEX_2].getArray(), LED_NUM_2);
#endif
  FastLED.setBrightness(150);
}

void fastLedUpdate() {
  // 1. Update all Patterns
  for (int i = 0; i < patternsNum; i++) {
    ledPatterns[i].Update();
  }

  // 2. If at least one stripe was updated -> send data out & clear all "updated"-flags
  for (int i = 0; i < patternsNum; i++) {
    if (ledPatterns[i].updated) {
      FastLED.show(); // this refreshs all stripes!
      // that's why we have to clear all "updated"-flags
      for (int i = 0; i < patternsNum; i++) {
        ledPatterns[i].updated = false;
      }
    }
  }
}

bool allLedsOff() {
  for (int i = 0; i < patternsNum; i++) {
    if (ledPatterns[i].getLightOn())
      return false;
  }
  return true;
}


