#ifndef FastLEDPatterns_H_
#define FastLEDPatterns_H_

#include <FastLED.h>
// Pattern types supported:
enum  pattern {
  STATIC = 0,
  RAINBOW = 20,
  FIRE = 30,
  NONE = 999,
};
// Pattern directions:
enum direction { FORWARD, REVERSE };

const CRGBPalette16 fire_gPals[4] = {
  HeatColors_p,
  CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White),
  CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White),
  CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White)
};

class FastLEDPatterns
{
  private:
    CRGB *led_array;
    int num_leds;

    pattern activePattern;
    direction activeDirection;
    CRGB colors[10];
    uint8_t current_color_index;

    uint16_t totalSteps;      // total number of steps in the pattern
    uint16_t index;           // current step within the pattern
    uint16_t transition_index;       // index for transition
    uint16_t transition_steps;

    bool light_on;

    unsigned long interval;       // milliseconds between updates
    unsigned long last_update;    // last update of position
    unsigned long interval_save;  // interval after transition

    uint8_t rainbow_cycles;
    uint8_t fire_cooling;
    uint8_t fire_sparking;
    uint8_t fire_gPal_index;

    void increment() {
      //      Serial.print("Index: ");
      //      Serial.print(index);
      //      Serial.print("   LedIndex: ");
      //      Serial.println(led_index);
      switch (activeDirection) {
        case FORWARD:
          if (transition) {
            transition_index++;
            if (transition_index == transition_steps) {
              transition = false;
              setInterval(interval_save);
            }
          }

          index++;
          if (index >= totalSteps)
            index = 0;
          break;

        case REVERSE:
          if (transition) {
            --transition_index;
            if (transition_index == 65535) {
              transition = false;
              setInterval(interval_save);
            }
          }

          --index;
          if (index == 65535)
            index = totalSteps - 1;
          break;
      }
    }

    void setupTransition(unsigned long interval_transition, unsigned long interval_after_transition, uint16_t steps) {
      transition = true;
      interval = interval_transition;
      interval_save = interval_after_transition;
      transition_steps = steps;

      // reset Index
      switch (activeDirection) {
        case FORWARD:
          index = 0;
          transition_index = 0;
          break;
        case REVERSE:
          index = totalSteps - 1;
          transition_index = transition_steps - 1;
          break;
      }
    }

  public:
    bool updated;
    bool transition;

    FastLEDPatterns(int len, CRGB* array_pointer) : index(0), num_leds(len), led_array(array_pointer) {
      colors[0] = CRGB::Black;          // Default Color -> ledOff
      colors[1] = CRGB(250, 170, 30);   // Default Color -> ledOn
      colors[2] = CRGB(200, 170, 50);
      ledOff();
    }

    // return pointer to actual Array, required for FastLED.addLeds<...>(...)
    CRGB* getArray() {
      return led_array;
    }

    void setInterval(unsigned long value) {
      interval = value;
    }

    unsigned long getInterval() {
      return interval;
    }

    bool getLightOn() {
      return light_on;
    }

    int getPatternCode() {
      switch (activePattern) {
        case STATIC:
          return activePattern + current_color_index;
          break;
        case RAINBOW:
          return activePattern + rainbow_cycles;
          break;
        case FIRE:
          return activePattern + fire_gPal_index;
          break;
        default:
          return activePattern;
          break;
      }
    }

    bool setColor(uint8_t color_number, CRGB color) {
      static bool r;
      if ( (color_number == 0) || (color_number == 1) ) {
        Serial.println("[FastLED] color 0/1 protected");
        r = false; // default values for On & Off cannot not be changed
      }
      else {
        colors[color_number] = color;
        r = true;
      }
      return r;
    }

    char* getColor(uint8_t color_number) {
      static char r[16]; // return string
      int red = colors[color_number].red;
      int green = colors[color_number].green;
      int blue = colors[color_number].blue;
      sprintf(r, "%03i%03i%03i", red, green, blue);
      return r;
    }

    void toggleDirection() {
      if (activeDirection == FORWARD)
        activeDirection = REVERSE;
      else
        activeDirection = FORWARD;
      setupTransition(1, interval_save, num_leds);
    }

    void Update() {
      if ((millis() - last_update) > interval) {
        last_update = millis();
        switch (activePattern) {
          case STATIC:
            staticUpdate();
            break;
          case RAINBOW:
            rainbowUpdate();
            break;
          case FIRE:
            fireUpdate();
            break;
          default:
            break;
        }
        updated = true;
      }
    }

    void ledOn() {
      if (!light_on)
        staticPattern(1);
    }

    void ledOff() {
      staticPattern(0);
    }

    void pirOnPattern() {
      //firePattern(0);
      staticPattern(2);
    }

    /*
       Pattern
    */

    void staticPattern(uint8_t new_color_index) {
      activePattern = STATIC;
      current_color_index = new_color_index;
      if (current_color_index == 0) { // turn all leds off
        light_on = false;
        activeDirection = REVERSE;
      }
      else {
        light_on = true;
        activeDirection = FORWARD;
      }
      setupTransition(10, 100, num_leds);
    }

    void staticUpdate() {
      // fill array during transition, no further changes in array required
      if (transition) {
        led_array[transition_index] = colors[current_color_index];
        increment();
      }
    }

    void rainbowPattern(uint16_t cycles) {
      activePattern = RAINBOW;
      activeDirection = FORWARD;
      light_on = true;
      totalSteps = 256;
      rainbow_cycles = cycles;
      setupTransition(10, 100, num_leds);
    }

    void rainbowUpdate() {
      if (transition)
        fill_rainbow(led_array, transition_index, index, rainbow_cycles * 256 / num_leds);
      else
        fill_rainbow(led_array, num_leds, index, rainbow_cycles * 256 / num_leds);
      increment();
    }

    void firePattern(unsigned long gPal_index) {
      activePattern = FIRE;
      light_on = true;
      totalSteps = num_leds;
      setupTransition(17, 17, num_leds);

      fire_cooling = 55;
      fire_sparking = 120;
      fire_gPal_index = gPal_index;
    }

    void fireUpdate() {
      // Add entropy to random number generator; we use a lot of it.
      random16_add_entropy( random16());

      // Array of temperature readings at each simulation cell
      //static byte heat[num_leds];
      byte heat[num_leds];

      // Step 1.  Cool down every cell a little
      for ( int i = 0; i < num_leds; i++) {
        heat[i] = qsub8( heat[i],  random8(0, ((fire_cooling * 10) / num_leds) + 2));
      }

      // Step 2.  Heat from each cell drifts 'up' and diffuses a little
      for ( int k = num_leds - 1; k >= 2; k--) {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
      }

      // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
      if ( random8() < fire_sparking ) {
        int y = random8(7);
        heat[y] = qadd8( heat[y], random8(160, 255) );
      }

      // Step 4.  Map from heat cells to LED colors
      for ( int j = 0; j < num_leds; j++) {
        // Scale the heat value from 0-255 down to 0-240
        // for best results with color palettes.
        byte colorindex = scale8( heat[j], 240);
        CRGB fire_color = ColorFromPalette( fire_gPals[fire_gPal_index], colorindex);

        int pixelnumber;
        //bool gReverseDirection = false;
        if ( activeDirection == FORWARD) {
          pixelnumber = (num_leds - 1) - j;
        } else {
          pixelnumber = j;
        }
        led_array[pixelnumber] = fire_color;

        if ( (transition) && (j == index) ) {
          increment();
          return;
        }
      }
    }
};

#endif
