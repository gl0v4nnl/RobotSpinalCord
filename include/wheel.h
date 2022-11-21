#pragma once

#include <stdint.h>

typedef enum {
  DRCT_DOWN = -1,
  DRCT_X3,
  DRCT_UP
} e_drct;

class whl
{
  e_drct direction;
  uint8_t strength;

public:
  whl() : direction(DRCT_X3), strength(0) {};
  whl(e_drct usrDirection, uint8_t usrStrength) : direction(usrDirection), strength(usrStrength) {};

  e_drct GetDirection(void) const;
  uint8_t GetStrength(void) const;
  void SetDirection(e_drct usrDirection);
  void SetStrength(uint8_t usrStrength);
  void SetWheel(e_drct usrDirection, uint8_t usrStrength);
};