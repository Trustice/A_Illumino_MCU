#ifdef PIN_DHT

#include "DHT.h"

enum { TEMPERATURE, HUMIDITY };

#define DHTTYPE DHT22   // DHT 11 || DHT22  (AM2302), AM2321 || DHT 21 (AM2301)

// Initialize DHT sensor.
DHT dht(PIN_DHT, DHTTYPE);

long dhtLastUpdate = 0;

void dhtSetup() {
  Serial.println("[DHT] Start");
  dht.begin();
  dhtRead(TEMPERATURE);
  dhtRead(HUMIDITY);
}

void dhtUpdate() {
  if (millis() - dhtLastUpdate >= DHT_INTERVAL) {
    dhtRead(TEMPERATURE);
    dhtRead(HUMIDITY);
  }
}

float dhtRead(int m) {
  Serial.print("[DHT] read ");
  float value;
  switch (m) {
    case TEMPERATURE:
      value = dht.readTemperature();
      Serial.print("T: ");
      break;
    case HUMIDITY:
      value = dht.readHumidity();
      Serial.print("H: ");
      break;
    default:
      break;
  }
  Serial.print(value);
  if (isnan(value)) {
    Serial.println(" ###");
    // repeat failed request
    // even if the first request was pass --> reset dhtLastUpdate
    dhtLastUpdate = millis() - DHT_INTERVAL + 1000;
  } else {
    dhtSend(m, value);
    dhtLastUpdate = millis();
  }
  return value;
}

void dhtSend(int m, float value) {
#ifndef WIFI_DISABLED
  const char *uuid = DHT_TEMP_UUID;
  if (!isnan(value)) {
    Serial.print(" *** send to vz ");
    switch (m) {
      case TEMPERATURE:
        uuid = DHT_TEMP_UUID;
        break;
      case HUMIDITY:
        uuid = DHT_HUMI_UUID;
        break;
      default:
        break;
    }

    char value_str [8];
    dtostrf(value, 3, 2, value_str);

    char url[128];
    sprintf(url, "http://%s/middleware.php/data/%s.json?operation=add&value=%s", IP_VOLKSZAEHLER, uuid, value_str);

    if (!sendGetRequest(url)) {
      dhtLastUpdate = millis() - DHT_INTERVAL;
    }
  }
#else
  Serial.println(" ### no wifi");
#endif
}
#else
void dhtSetup() {}
void dhtUpdate() {}
#endif
