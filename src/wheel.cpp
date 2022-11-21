#include "wheel.h"

e_drct whl::GetDirection(void) const {
    return direction;
}

uint8_t whl::GetStrength() const {
    return strength;
}

void whl::SetDirection(e_drct usrDirection) {
    if ((int)usrDirection <= (int)DRCT_DOWN)
        direction = DRCT_DOWN;
    else if ((int)usrDirection >= (int)DRCT_UP)
        direction = DRCT_UP;
    else
        direction = DRCT_X3;
}

void whl::SetStrength(uint8_t usrStrength) {
    strength = usrStrength;
}

void whl::SetWheel(e_drct usrDirection, uint8_t usrStrength) {
    SetDirection(usrDirection);
    SetStrength(usrStrength);
}