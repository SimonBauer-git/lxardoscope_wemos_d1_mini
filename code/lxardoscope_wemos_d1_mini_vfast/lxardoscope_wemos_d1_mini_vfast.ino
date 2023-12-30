#include <ESP8266WiFi.h>

uint16_t adc_addr[1];

void setup() {
  WiFi.mode(WIFI_OFF);
  Serial.begin(2000000);
  ets_intr_lock();  // close interrupt
  noInterrupts();
  scope();
}
void loop() {
}

void scope()  //have an extra function that does the looping as this is faster than void loop
{
  while (true) {
    system_adc_read_fast(adc_addr, 1, 1);
    Serial.write(0x80 | adc_addr[0] >> 4);
    Serial.write((adc_addr[0] << 3) & 0x70);
  }
}
