#ifdef PIN_MPR_IRQ

#include <Wire.h>
#include "Adafruit_MPR121.h"

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

void mprSetup() {
  Serial.print("[MPR] Start ");

  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("### not found, check wiring?");
    //while (1);
  }
  else {
    Serial.println("*** found!");
    pinMode(PIN_MPR_IRQ, INPUT);
    attachInterrupt(digitalPinToInterrupt(PIN_MPR_IRQ), mprUpdate, CHANGE);
  }
}

void mprUpdate() {
  if (digitalRead(PIN_MPR_IRQ) == HIGH)
    return;
  //Serial.print("[MPR] ");

  // Get the currently touched pads
  currtouched = cap.touched();

  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    //if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
    //  Serial.print(i); Serial.println(" touched");
    //}
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print("[MPR] Pad ");
      Serial.print(i);
      Serial.print(" released ");
      uint8_t j;
      switch (i) {
        case 0: {
            for (j = 0; j < LED_PATTERN_NUM; j++) {
              processPattern(j * 100 + 1);
            }
            Serial.println("*** Light On");
            break;
          }
        case 1: {
            for (j = 0; j < LED_PATTERN_NUM; j++) {
              processPattern(j * 100);
            }
            Serial.println("*** Light Off");
            break;
          }
        default:
          Serial.println(" ### unhandled");
          break;
      }
    }
  }

  // reset our state
  lasttouched = currtouched;

  // comment out this line for detailed data from the sensor!
  return;

  // debugging info, what
  Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap.touched(), HEX);
  Serial.print("Filt: ");
  for (uint8_t i = 0; i < 12; i++) {
    Serial.print(cap.filteredData(i)); Serial.print("\t");
  }
  Serial.println();
  Serial.print("Base: ");
  for (uint8_t i = 0; i < 12; i++) {
    Serial.print(cap.baselineData(i)); Serial.print("\t");
  }
  Serial.println();

  // put a delay so it isn't overwhelming
  delay(100);
}

#else
void mprSetup() {}
#endif
