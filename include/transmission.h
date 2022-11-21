#pragma once

#include <stdint.h>
#include "wheel.h"

class trnsm {
    whl leftWheel;
    whl rightWheel;
public:
    trnsm(): leftWheel(), rightWheel() {};
    trnsm(whl usrLeftWheel, whl usrRightWheel);

    whl GetLeftWheel() const;
    whl GetRightWheel() const;
    void SetLeftWheel(whl usrWheel);
    void SetRightWheel(whl usrWheel);
};