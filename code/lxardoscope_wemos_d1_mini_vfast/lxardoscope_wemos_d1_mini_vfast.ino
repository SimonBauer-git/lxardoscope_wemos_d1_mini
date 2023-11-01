#define BUFSIZE 256
#include "user_interface.h"
uint8_t cbuf[3] = {0x80, 0x00, 0x00};
boolean cal1 = false;
uint16_t val[2];
int value;

#define num_samples 1
uint16_t adc_addr[num_samples]; // point to the address of ADC continuously fast sampling output
uint16_t adc_num = num_samples; // sampling number of ADC continuously fast sampling, range [1, 65535]
uint8_t adc_clk_div = 8;        // ADC working clock = 80M/adc_clk_div, range [8, 32], the recommended value is 8

void setup()
{
  Serial.begin(2000000);
}

void loop()
{

  system_adc_read_fast(adc_addr, adc_num, adc_clk_div);

  value = (adc_addr[0]);

  val[0] = (value / 2.046);
  if (cal1 == true)
  {
    val[0] = 0;
  }
  cbuf[0] = 0x80 | val[0] >> 3;
  cbuf[1] = (val[0] << 4) & 0x70;
  Serial.write(cbuf, 2);

  if (Serial.available())
  {
    char inChar = (char)Serial.read();
    if (inChar == '1')
    { // cal channel 1
      // AC1 off
      cal1 = true;
    }

    else if (inChar == '2')
    { // cal channel 1
      // AC1 off
      cal1 = false;
    }
  }
}
