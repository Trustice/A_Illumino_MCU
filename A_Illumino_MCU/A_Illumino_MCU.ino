#define A_ILLUMINO_VERSION  "0.1"
const char compile_date[] = __DATE__ " " __TIME__;
//#define WIFI_DISABLED

void setup() {
  Serial.begin(115200);
  Serial.print("\n[ILLUMINO] Version: ");
  Serial.println(A_ILLUMINO_VERSION);
  Serial.print("Compile Date: ");
  Serial.println(compile_date);
  Serial.println("#######################");

#ifndef WIFI_DISABLED
  connectWiFi();
#endif

  pirSetup();

  fastLedSetup();
  
  dhtSetup();

  mprSetup();

}

void loop() {
  serialEvent();

#ifndef WIFI_DISABLED
  listenWebServer();
#endif
  fastLedUpdate();

  dhtUpdate();

  updatePir();
  
}
