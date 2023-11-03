#include "user_interface.h"
#include <ESP8266WiFi.h>
uint8_t cbuf[2] = {0x80, 0x00};
uint16_t val[2];

#define num_samples 1
uint16_t adc_addr[num_samples]; // point to the address of ADC continuously fast sampling output
uint16_t adc_num = num_samples; // sampling number of ADC continuously fast sampling, range [1, 65535]
uint8_t adc_clk_div = 1;        // ADC working clock = 80M/adc_clk_div
void setup()
{
  WiFi.mode(WIFI_OFF);
  Serial.begin(2000000);
  ets_intr_lock(); // close interrupt
  noInterrupts();
}
void loop()
{

  system_adc_read_fast(adc_addr, adc_num, adc_clk_div);
  cbuf[0] = 0x80 | adc_addr[0] >> 3;
  cbuf[1] = (adc_addr[0] << 4) & 0x70;
  Serial.write(cbuf, 2);
}
