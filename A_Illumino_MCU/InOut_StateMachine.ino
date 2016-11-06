/*
   Illumino State Machine
   ----------------------
*/
#include <Arduino.h>

// the possible states of the state-machine
typedef enum { GOT_P, GOT_I, GOT_D, GOT_C, GOT_A, GOT_S, GOT_T, GOT_H, GOT_M, EMPTY } states;
states state = EMPTY; // current state-machine state

void setState(const byte inByte) {
  switch (inByte) {
    case 'P':
      state = GOT_P;
      break;
    case 'I':
      state = GOT_I;
      break;
    case 'D':
      state = GOT_D;
      break;
    case 'C':
      state = GOT_C;
      break;
    case 'A':
      state = GOT_A;
      break;
    case 'S':
      state = GOT_S;
      break;
    case 'T':
      state = GOT_T;
      break;
    case 'H':
      state = GOT_H;
      break;
    case 'M':
      state = GOT_M;
      break;
    default:
      state = EMPTY;
      break;
  }
}

char* handlePreviousState(unsigned long value) {
  static char r[16]; // return string as char array

  switch (state) {
    case GOT_P:
      /* FastLED Pattern */
      strcpy(r, "P");
      if (!processPattern(value)) {
        strcat(r, "_ERR:");
      }
      break;
    case GOT_I:
      /* FastLED Update-Interval */
      processInterval(value);
      strcpy(r, "I");
      break;
    case GOT_D: /* FastLED toggle Direction */
      processDirection(value);
      strcpy(r, "D");
      break;
    case GOT_C: /* FastLED set Color */
      strcpy(r, "C");
      if (!processColor(value))
        strcat(r, "_ERR:");
      break;
    case GOT_A: /* FastLED set ColorTemperature */
      strcpy(r, "A");
      if (!setColorTemperature(value))
        strcat(r, "_ERR:");
      break;
   case GOT_S:
      //      processSteps(value);
      strcpy(r, "S");
      break;

#ifdef PIN_PIR
    case GOT_M: /* PIR turn On/Off */
      strcpy(r, "M");
      if (value == 0)
        setPirOn(false);
      else if (value == 1)
        setPirOn(true);
      else
        strcat(r, "_ERR:");
      break;
#else
    case GOT_M:
      strcpy(r, "M_ERR: no PIR attached");
      break;
#endif

    default:
      strcpy(r, "ERR_StateMachine");
      return r;
      break;
  }  // end of switch

  char value_str[12];
  sprintf(value_str, "%lu!", value);
  strcat(r, value_str);

  return r;
}

char * getInfo(unsigned long value) {
  static char r[16]; // return string

  switch (state) {
    case GOT_P: // return current Pattern number
      sprintf(r, "P%d", (value * 100) + ledPatterns[value].getPatternCode());
      break;
    case GOT_I: // send current Interval in ms
      sprintf(r, "I%lu", (value * 10000) + ledPatterns[value].getInterval());
      break;
    case GOT_C: // send current color, value required to define color1 or color2!
      sprintf(r, "C%lu%s", value, ledPatterns[value / 10].getColor(value % 10));
      break;
    case GOT_A: // send current color, value required to define color1 or color2!
      sprintf(r, "A%lu", current_color_temp);
      break;
      //    case GOT_S:
      //      sprintf(r, "S%d", Stripe.TotalSteps);
      //      break;

#ifdef PIN_DHT
    case GOT_T:
      char value_str[8];
      dtostrf(dhtRead(TEMPERATURE), 3, 2, value_str);
      sprintf(r, "T%s", value_str);
      break;
    case GOT_H:
      value_str[8];
      dtostrf(dhtRead(HUMIDITY), 3, 2, value_str);
      sprintf(r, "H%s", value_str);
      break;
#else
    case GOT_T:
      strcpy(r, "T_ERR: no DHT attached");
      break;
    case GOT_H:
      strcpy(r, "H_ERR: no DHT attached");
      break;
#endif

#ifdef PIN_PIR
    case GOT_M: /* PIR turn On/Off */
      if (pir_on)
        strcpy(r, "M1");
      else
        strcpy(r, "M0");
      break;
#else
    case GOT_M:
      strcpy(r, "M_ERR: no PIR attached");
      break;
#endif

    default:
      strcpy(r, "N/A");
      break;
  }
  strcat(r, "!");
  return r;
}



