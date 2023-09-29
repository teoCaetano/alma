/*void setupWifiEvents() {
  WiFi.onEvent(WiFiStationGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
}
void initWifi() {
    WiFi.persistent(false);
    //WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    Serial.print ("Modo Client [SSID: JOSAM] ");
    delay (300);
    WiFi.begin("test", "test");
    delay (1000);
    WiFi.setAutoConnect(true);
    Serial.println (" OK");
}

void WiFiStationGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.print("Station connected, IP: ");
  Serial.println(WiFi.localIP());
 
}*/
