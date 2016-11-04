#ifdef PIN_PIR

bool pir_on = false;
bool pir_attached = false;

void pirSetup() {
  Serial.println("[PIR] Start");
  pinMode(PIN_PIR, INPUT);
}

void updatePir() {
  if (!pir_on) {
    if (pir_attached) {
      detachInterrupt(digitalPinToInterrupt(PIN_PIR));
      pir_attached = false;
      Serial.println("[PIR] Interrupt detached");
    }
    return;
  }
  //  Serial.println("[PIR]");
  if (allLedsOff()) {
    pir_enable = true;
    if (!pir_attached ) {
      attachInterrupt(digitalPinToInterrupt(PIN_PIR), processPirAction, CHANGE);
      pir_attached = true;
      Serial.println("[PIR] Interrupt attached");
      processPirAction();
    }
  } else if (pir_attached && !pir_enable) {
    detachInterrupt(digitalPinToInterrupt(PIN_PIR));
    pir_attached = false;
    Serial.println("[PIR] Interrupt detached");
  }
}

void setPirOn(bool state) {
  pir_on = state;
}

void processPirAction() // triggered by interrupt
{
  if (digitalRead(PIN_PIR) == 1) {
    for (int i = 0; i < patternsNum; i++) {
      ledPatterns[i].pirOnPattern();
      Serial.println("[PIR] Light On");
    }
  } else {
    for (int i = 0; i < patternsNum; i++) {
      ledPatterns[i].ledOff();
      Serial.println("[PIR] Light Off");
    }
  }
}
#else
void pirSetup() {}
void updatePir() {}
#endif

