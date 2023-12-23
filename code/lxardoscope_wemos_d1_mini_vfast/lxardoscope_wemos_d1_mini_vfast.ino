#include <ESP8266WiFi.h>

uint8_t cbuf[2] = {0x80, 0x00};
uint16_t val[2];
uint16_t adc_addr[1];

void setup()
{
  WiFi.mode(WIFI_OFF);
  Serial.begin(2000000);
  ets_intr_lock(); // close interrupt
  noInterrupts();
  scope();
}
void loop()
{

}

void scope()  //have an extra function that does the looping as this is faster than void loop
{while (true)
{  system_adc_read_fast(adc_addr, 1, 1);
  cbuf[0] = 0x80 | adc_addr[0] >> 4;
  cbuf[1] = (adc_addr[0] << 3) & 0x70;
  Serial.write(cbuf, 2);}}
