/************************
   Serial communication
 ************************/
// check for serial input
// automatically called after every loop() on Arduinos --> not on ESP8266

bool new_state;
unsigned long value;
char *response;

void serialEvent() {
  //delay(10);
  char inByte;

  while (Serial.available() > 0) {
    inByte = Serial.read();
    //    Serial.println((byte)inByte);

    if (isalpha(inByte)) {
      // set the new state, if we recognize it
      setState(inByte);
      new_state = true;
    }
    else if (isdigit(inByte)) {   // calculate incoming value
      value *= 10;
      value += inByte - '0';
    }
    else if (new_state) {     // process state and value
      Serial.print("[Serial] Response: ");
      if (inByte == '_')      // get info
        Serial.println(getInfo(value));
      else   // set something
        Serial.println(handlePreviousState(value));
      value = 0;
      new_state = false;
    }
  }
}



