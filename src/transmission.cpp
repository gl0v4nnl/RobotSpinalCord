#include "transmission.h"

trnsm::trnsm(whl usrLeftWheel, whl usrRightWheel) {
    leftWheel = usrLeftWheel;
    rightWheel = usrRightWheel;
}

whl trnsm::GetLeftWheel() const {
    return leftWheel;
}

whl trnsm::GetRightWheel() const {
    return rightWheel;
}

void trnsm::SetLeftWheel(whl usrWheel) {
    leftWheel = usrWheel;
}

void trnsm::SetRightWheel(whl usrWheel) {
    rightWheel = usrWheel;
}
