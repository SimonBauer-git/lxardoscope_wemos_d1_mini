#include <ESP8266WiFi.h>

uint16_t adc_addr[16384];

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
    system_adc_read_fast(adc_addr, 16384, 1);
    int i =0;
    while (i<16384){ 
    Serial.write(0x80 | adc_addr[i] >> 4);

    Serial.write((adc_addr[i] << 3) & 0x70);
    i++;
  }}
}
