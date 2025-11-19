#pragma once
#include "Controller.h"

//void onCanRxInterruptEnable();
//void onCanRxInterruptDisable();

class PDController: public Controller {
  private:

  public:
    long targetP = 0;
    float targetD = 0;

    float gainP = 1;
    float gainD = 0.001;

    float currentTorque = 0;


    void setDesiredPosition(long angle) {
      this->targetP = angle;
    }


    int getTorque() override {
      // make sure params are not changed during calculation
      //onCanRxInterruptDisable(); @todo: from old can communication impl

      currentTorque = (gainP * (float)(targetP - posSensor->encoder.getAngleAbsolute().angle)) + gainD*(targetD - posSensor->getVelocityFilteredFloat());
      /// @todo                                                                               ^ should be + here?
      //onCanRxInterruptEnable(); @todo: from old can communication impl
      return (int) currentTorque;
    }
};