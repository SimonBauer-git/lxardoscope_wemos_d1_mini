// alternative approach, instead of continously sending data out, capture a number of samples and then send that in bulk
#include <ESP8266WiFi.h>

uint16_t adc_addr[500];

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
    system_adc_read_fast(adc_addr, 500, 1); //adc_addr is where the Samples are stored, the next thing is the number of readings
    int i = 0;
    while (i<500){               //send the data out after taking 500 readings
    Serial.write(0x80 | adc_addr[i] >> 4);
    Serial.write((adc_addr[i] << 3) & 0x70);
    i++;}
  }
}
