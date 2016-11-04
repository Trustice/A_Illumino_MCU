
bool pir_enable = false;

bool processPattern(unsigned long value) {
  // value = xyyy
  uint8_t pattern_id = value / 100; // x
  value %= 100;                     // yy
  switch (value) {
    case 99:
      ledPatterns[pattern_id].ledOff();
      break;
    case 98:
      if (!ledPatterns[pattern_id].getLightOn())
        ledPatterns[pattern_id].ledOn();
      break;

    /* Static Pattern with Color 0 - 9 */
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      ledPatterns[pattern_id].staticPattern(value);
      break;

    /* Rainbow Patterns */
    case 20:
      ledPatterns[pattern_id].rainbowSyncPattern();
      break;
    case 21:
      ledPatterns[pattern_id].rainbowShortPattern();
      break;
    case 22:
      ledPatterns[pattern_id].rainbowShortPattern();
      break;

    /* Fire Patterns */
    case 30:
    case 31:
    case 32:
    case 33:
      ledPatterns[pattern_id].firePattern(value - 30);
      break;
    default:
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
  if (pattern_id < LED_ARRAY_NUM)
    ledPatterns[pattern_id].toggleDirection();
  else
    return false;
  return true;
}

