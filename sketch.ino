#include <Wire.h>

#define DJLEFT_ADDR 0x0E
#define DJRIGHT_ADDR 0x0D
#define DJ_BUTTONS_PTR 0x12

// For detect
bool djLeftValid = true;
bool djRightValid = true;

uint8_t lastSuccessfulTurntablePacketLeft[3];
uint8_t lastSuccessfulTurntablePacketRight[3];

uint8_t *dj_left = lastSuccessfulTurntablePacketLeft;
uint8_t *dj_right = lastSuccessfulTurntablePacketRight;

uint16_t tt_pos;

bool twi_readFromPointer(uint8_t address, uint8_t pointer, uint8_t length, uint8_t *data)
{
  bool ret = 0;
  Wire.beginTransmission(address);
  Wire.write(pointer);
  Wire.endTransmission();
  Wire.requestFrom(address, length, 1);
  if(Wire.available())
    ret = 1;

  int i = 0;
  while(Wire.available())
  {
    data[i] = Wire.read();
    i++;
  }

  return ret;
}

void setup() {
  Wire.begin();
}

void loop() {

  djLeftValid = twi_readFromPointer(DJLEFT_ADDR, DJ_BUTTONS_PTR, sizeof(lastSuccessfulTurntablePacketLeft), dj_left);
  djRightValid = twi_readFromPointer(DJRIGHT_ADDR, DJ_BUTTONS_PTR, sizeof(lastSuccessfulTurntablePacketRight), dj_right);

  if(djLeftValid)
  {
    if(0 != dj_left[2] )
    {      
      if (tt_pos >= 1024) {
        tt_pos = 1;
      } else if (tt_pos <= 0) {
        tt_pos = 1024 - 1;
      }

      if(dj_left[2] < 200)
        tt_pos += 1;
      else
        tt_pos -= 1;
    }
  }

  if(djRightValid)
  {
    if(0 != dj_right[2] )
    {      
      if (tt_pos >= 1024) {
        tt_pos = 1;
      } else if (tt_pos <= 0) {
        tt_pos = 1024 - 1;
      }

      if(dj_right[2] < 200)
        tt_pos += 1;
      else
        tt_pos -= 1;
    }
  }

  Joystick.X(tt_pos);
}
