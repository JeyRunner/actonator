#pragma once

#include "../sensor/PosSensor.h"

class Controller {
  public:
    /// @param posSensor is expected to be updated externally
    PosSensor *posSensor;


    Controller() {
    }

    virtual int getTorque() = 0;
};