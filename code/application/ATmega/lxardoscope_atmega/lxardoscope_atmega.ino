#define BUFSIZE 256
uint8_t cbuf[3] = {0x80, 0x00, 0x00};
#define CH1 0x1
#define CH2 0x2
#define BOTH 0x3
uint8_t channels = BOTH;

#define ADMUX_INIT B00000000 /* 00 ext reference, 0 right adj, 0filler, 0000 source */

void setup()
{
  Serial.begin(1000000);
  ADCSRA = 0;
  ADCSRB = 0;
  ADMUX = ADMUX_INIT; 
  //ADCSRA = B11101100; /* 1enable, 1start, 1trig, 0int, 1intenable, 100 prescaler 16 */
  //ADCSRA = B11101111;
  ADCSRA = B11001100; /* 1enable, 1start, 0trig, 0int, 1intenable, 101 prescaler 32 */
}

uint16_t val[2];

ISR(ADC_vect)
{
  uint8_t chan = ADMUX & 1;
  val[chan] = ADCL;
  val[chan] |= ADCH << 8;
  switch (channels) {
    case 0:
      ADMUX = ADMUX_INIT;
      ADCSRA |= B01000000;
      Serial.write(cbuf, 2);
      break;
    case CH1:
      ADMUX = ADMUX_INIT;
      ADCSRA |= B01000000;
      cbuf[0] = 0x80 | val[0] >> 3;
      cbuf[1] = (val[0] << 4) & 0x70; 
      Serial.write(cbuf, 2);
      break;
    case CH2:
      ADMUX = ADMUX_INIT | 1;
      ADCSRA |= B01000000;      
      cbuf[0] = 0x80;
      cbuf[1] = val[1] >> 6;
      cbuf[2] = val[1] & 0x7F;
      Serial.write(cbuf, 3);
      break;
    default:
      ADMUX ^= 1;
      ADCSRA |= B01000000;
      if (chan == 1) {
        cbuf[0] = 0x80 | val[0] >> 3;
        cbuf[1] = (val[0] << 4) & 0x70;
        cbuf[1] |= val[1] >> 6;
        cbuf[2] = val[1] & 0x7F;
        Serial.write(cbuf, 3);
      }
  }
}
  
void loop()
{
  while(Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == 'A') {
      channels |= CH1;
    }
    else if (inChar == 'a') {
      channels &= ~CH1;
    }
    else if (inChar == 'B') {
      channels |= CH2;
    }
    else if (inChar == 'b') {
      channels &= ~CH2;
    }
  }
}
