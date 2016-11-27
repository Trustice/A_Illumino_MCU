
bool pir_enable = false;

bool processPattern(unsigned long value) {
  // value = xyy
  uint8_t pattern_id = value / 100; // x
  value %= 100;                     // yy

  if (pattern_id == LED_PATTERN_NUM) {
    for (int i = 0; i < LED_PATTERN_NUM; i++) {
      if ( (value >= 0) && (value <= 9) ) {
        ledPatterns[i].staticPattern(value);
      }
      else if ( (value >= 20) && (value <= 29) ) {
        ledPatterns[i].rainbowPattern(value - 20);
      }
      else if ( (value >= 30) && (value <= 33) ) {
        ledPatterns[i].firePattern(value - 30);
      }
      else
        return false;
    }
  }
  else {
    if ( (value >= 0) && (value <= 9) ) {
      ledPatterns[pattern_id].staticPattern(value);
    }
    else if ( (value >= 20) && (value <= 29) ) {
      ledPatterns[pattern_id].rainbowPattern(value - 20);
    }
    else if ( (value >= 30) && (value <= 33) ) {
      ledPatterns[pattern_id].firePattern(value - 30);
    }
    else
      return false;
  }

#ifdef PIN_PIR
  if (value != 0)
    pir_enable = false;
#endif
  return true;
}

void processInterval(unsigned long value)
{
  /*  value = xyyyy
      yyyy = delay in ms (max. 9999) */
  uint8_t pattern_id = value / 10000;           // x
  value %= 10000;                               // yyyy
  ledPatterns[pattern_id].setInterval(value);
}

bool processColor(unsigned long value)
{
  // value = xyrrrgggbbb
  uint8_t pattern_id = value / 10000000000; // x
  value %= 10000000000;                     // yrrrgggbbb
  uint8_t color_id =   value /  1000000000; // y
  value %= 1000000000;                      // rrrgggbbb
  uint8_t red = value / 1000000;            // rrr
  value %= 1000000;                         // gggbbb
  uint8_t green = value / 1000;             // ggg
  uint8_t blue = value % 1000;              // bbb

  return ledPatterns[pattern_id].setColor(color_id, CRGB(red, green, blue));
}

bool processDirection(unsigned long pattern_id)
{
  if (pattern_id < LED_PATTERN_NUM)
    ledPatterns[pattern_id].toggleDirection();
  else
    return false;
  return true;
}

