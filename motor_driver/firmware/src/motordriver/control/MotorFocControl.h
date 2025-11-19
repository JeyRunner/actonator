#pragma once


#include "motordriver/control/filter/LowPassFilter.h"
#include "sensor/PosSensor.h"
#include "stm32_custom/StmUtil.h"
#include "util/FixedPoint.h"

#include "control/Controller.h"


/**
 * Do FOC PD control with a initialized motor.
 */
class MotorFocControl
{
  public:
    Motor &motor;
    // Encoder &encoder;
    PosSensor posSensor;
    Controller &controller;

    int maxTorque = 150;
    int currentMotorTorque;


    MotorFocControl(Motor &motor, Encoder &encoder, Controller &controller)
        : motor(motor), posSensor(encoder), controller(controller) {
      controller.posSensor = &this->posSensor;
    }


    /**
     * Call this at each iteration of the programs main loop.
     * This applies the needed motor commands.
     */
    void focIteration() {
      /// update angle and apply filter
      posSensor.update();
      EncoderAngle currentAngle = posSensor.encoder.getAngleAbsolute();

      //printf("90 - %d \n", (int)(MODULATION_DATA_FULLROTATION_VALUE/4));
      int motorForce = controller.getTorque();

      int maxTorqueInternal = 400;//150;//350;
      motorForce = normalize<int>(motorForce, -maxTorque, maxTorque); //
      motorForce = normalize<int>(motorForce, -maxTorqueInternal, maxTorqueInternal); //
      currentMotorTorque = motorForce;
      int motorForceAbs = abs(motorForce);



      log("%ld, %ld , %d\n",
             currentAngle.angle,
             posSensor.angleAbsoluteVelocity.value,
             motorForce);
             //*/


      long electricalAngle = motor.absoluteEncoderAngleToElectricalModulationAngle(currentAngle);


      // set field 90° before or after current electrical angle
      // depending on the required direction for reducing the error
      long applyElectricalAngle =
          electricalAngle + (int) (MODULATION_DATA_FULLROTATION_VALUE / 4) * ((motorForce > 0) ? -1 : 1); // added minus - to fix wrong direction: @todo fix in Motor.initEstimateMotorConfig_encoderOffset(...)
      applyElectricalAngle =
          applyElectricalAngle * ((motor.motorConfig.motorDirection == Motor::MOTOR_DIRECTION::POSITIVE) ? 1 : -1);


      //printf("motorForce %d,  electrical-angle: %ld \n", motorForce, applyElectricalAngle);
      // motor.applyPhaseAngleSinusodial2PI(applyElectricalAngle, motorForce);
      motor.applyPhaseAngleSpaceVector(applyElectricalAngle, motorForceAbs);

      //printf("chartUpdate: %lu, %ld, %ld \n", HAL_GetTick(), currentAngle.angle, pError);
    }


    void focInlineFieldTest() {
      /// update angle and apply filter
      posSensor.update();
      EncoderAngle currentAngle = posSensor.encoder.getAngleAbsolute();
      long electricalAngle = motor.absoluteEncoderAngleToElectricalModulationAngle(currentAngle);

      int applyElectricalAngle =
          electricalAngle * ((motor.motorConfig.motorDirection == Motor::MOTOR_DIRECTION::POSITIVE) ? 1 : -1);

      /*
      float f = applyElectricalAngle / PI_2;
      int applInt = (int) (fmod(f, 1.0) * 1000);
      applInt = (applInt < 0) ? (1000 + applInt) : applInt;
      motor.applyPhaseAngle(applInt);
       */
      motor.applyPhaseAngleSpaceVector(applyElectricalAngle, 80);
      //printf("chartUpdate: %lu, %ld, %ld \n", HAL_GetTick(), currentAngle.angle, (electricalAngle % MODULATION_DATA_FULLROTATION_VALUE));
      //printf("%ld, %ld \n", currentAngle.angle, (electricalAngle % MODULATION_DATA_FULLROTATION_VALUE));
    }
};