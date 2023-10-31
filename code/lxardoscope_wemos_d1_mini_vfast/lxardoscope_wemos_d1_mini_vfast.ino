#define BUFSIZE 256
uint8_t cbuf[3] = {0x80, 0x00, 0x00};
boolean cal1 = false;
uint16_t val[2];

void setup()
{
  Serial.begin(2000000);
}



void loop()
{
  val[0] = (analogRead(A0) / 2.046);
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
