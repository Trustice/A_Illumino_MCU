/*
    Illumino WebServer
    ------------------
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer server(80);

void connectWiFi() {
  // Connect to WiFi network
  Serial.print("[WiFi] Connecting to ");
  Serial.print(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.config(ip, gateway, subnet);//, myDNS, mySUBNET, myDNSServer);

  // Wait for connection and print dots on Serial while not connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" DONE");

  // Print the IP address
  Serial.print("[WiFi] IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_root);
  server.onNotFound(handleRequest);
  server.on("/favicon.ico", handleFavicon);
  // Start the server
  server.begin();
  Serial.println("[WiFi] WebServer started");
}

void handle_root() {
  server.send(200, "text/plain", "Hello from the weather esp8266, read from /temp or /humidity");
  delay(100);
}

void handleFavicon() {
  server.send(404, "text/plain", "");
}

void handleRequest() {
  String request_str = server.uri();
  char request[16];
  server.uri().toCharArray(request, 16);
  Serial.print("[WiFi] Request:  ");
  Serial.println(request);
  
  setState(request[1]);

  char *response_msg;
  unsigned long value = 0;
  for (int i = 2; i < 16; i++) {
    byte inByte = request[i];
    if (isdigit(inByte)) {
      value *= 10;
      value += inByte - '0';
    }
    else if (inByte == '_') {
      response_msg = getInfo(value);
      break;
    }
    else {
      response_msg = handlePreviousState(value);
      value = 0;
      break;
    }
  }
  Serial.print("[WiFi] Response: ");
  Serial.println(response_msg);
  server.send(200, "text/plain", response_msg);
}

void listenWebServer() {
  server.handleClient();
}


bool sendGetRequest(const char url[128]) {
  if (WiFi.status() == WL_CONNECTED) {
    bool return_value;
    HTTPClient http;
    http.begin(url);

    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("*** PASS... code: %d\n", httpCode);
      return_value = true;
    } else {
      Serial.printf("*** FAIL, error: %s\n", http.errorToString(httpCode).c_str());
      return_value = false;
    }

    http.end();
    return return_value;
  }
  else {
    Serial.println("No WiFi!");
    return false;
  }
}
