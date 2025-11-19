#pragma once

#include "etl/delegate.h"
#include "config.h"
#include "motordriver/util/util.h"
#include "modulation/SpaceVectorModulation.h"
#include "Motor.hpp"

#include "util/EventFlag.h"


class MotorParameterEstimator {
  public:
    Motor &motor;

    /// will be set to true when parameter estimation is completed
    EventFlag estimationDone;


    /// gets called in shot intervalls during blocking execution of initEstimateMotorConfig(...)
    /// use this to e.g. check for events/exec code during this blocking execution;
    etl::optional<etl::delegate<void()>> estimationTickCallback;

    explicit MotorParameterEstimator(Motor &motor) : motor(motor) {}

    /**
     * Estimate motor parameters by slowly rotating the motor for a short time. This will temporarily enable the motor.
     * Call this function to init the motor parameters before any call to applyPhaseAngle(...).
     * @param encoder the initialized position encoder for this motor.
     * @param determineElectricalRotationsPerRevolution if this is true also the electricalRotationsPerRevolution will be measured.
     *                                                  if this is set to false motorConfig.electricalRotationsPerRevolution has to be set before calling this function
     */
    void initEstimateMotorConfig(Encoder &encoder, bool determineElectricalRotationsPerRevolution = true,  int motorThrottle = 120) {
      estimationDone.clear();
      motor.enableMotor(true);

      log("\n");
      log("record motor phase mapping ...");

      // find out positive encoder direction
      log("- find positive direction ...");
      // do 4 [electrical] rotation of the motor
      int checkElectricalInitialWarmupRotationNum = 2;
      int checkElectricalRotationNumber = 5;

      /*
      float elOffset_avg_num = 0;
      float elOffset_avg_sum = 0;
      */

      EncoderAngle angle_start(0);
      long last = 0;

      HAL_Delay(200);
      callTickCallback();
      printf("start \n");
      HAL_Delay(200);

      /// first find out motor direction and number of pole pairs
      for (int rotationIndex = 0;
           rotationIndex < (checkElectricalInitialWarmupRotationNum + checkElectricalRotationNumber); ++rotationIndex) {
        // do one electrical rotation
        for (int angleEl = 0; angleEl < MODULATION_DATA_FULLROTATION_VALUE; angleEl++) {
          motor.applyPhaseAngleSpaceVector(angleEl, motorThrottle);

          HAL_Delay(1);
          HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
          EncoderAngle angle = encoder.update();
          //printf("%ld \n", angle.angle);

          /*
          float elOffset = fmod(angle*7 + elAngleNorm, PI_2);
          //printf("chartUpdate: %f, %f,   %f \n", angle, elAngleNorm, elOffset);
          printf("chartUpdate: %lu,   %f, %f\n", HAL_GetTick(), -angle, fmod(elAngleNorm, PI_2));
          elOffset_avg_num++;
          elOffset_avg_sum += elOffset;
          //printf("offset: %f, %f \n", angle, elAngleNorm);
          */
          callTickCallback();
        }

        long curAngle = encoder.getAngleAbsolute().angle;
        log("completed rotation [%d], encoder now at: %ld  (delta %ld)\n", rotationIndex, curAngle, abs(curAngle - last));
        last = curAngle;


        if (rotationIndex == (checkElectricalInitialWarmupRotationNum-1)) {
          HAL_Delay(200);
          angle_start = encoder.getAngleAbsolute();
          log("\nstart measuring...\n");
        }
      }

      EncoderAngle angle_end = encoder.update();

      // direction
      motor.motorConfig.motorDirection = (angle_end.angle > angle_start.angle)
                                    ? Motor::MOTOR_DIRECTION::POSITIVE : Motor::MOTOR_DIRECTION::INVERSE;

      // electricalRotationsPerRevolution
      long motorMovedAbsEncoder = abs(angle_end.angle - angle_start.angle);
      log("moved avg %f", (motorMovedAbsEncoder / (float) checkElectricalRotationNumber));
      float electricalRotationsPerRevolutionExact =
          Encoder::ENCODER_MAX_VALUE / (motorMovedAbsEncoder / (float) checkElectricalRotationNumber);
      if (determineElectricalRotationsPerRevolution) {
        motor.motorConfig.electricalRotationsPerRevolution = (int) std::round(electricalRotationsPerRevolutionExact);
      }
      //motorConfig.encoderRotationPerElectricalRotation = PI_2 / (float) motorConfig.electricalRotationsPerRevolution;

      initEstimateMotorConfig_encoderOffset(encoder, motorThrottle);
      HAL_Delay(50);
      callTickCallback();

      log("  -> angle_start: %ld,   angle_end: %ld \n", angle_start.angle, angle_end.angle);
      log("  -> encoder %s direction as motor \n", (motor.motorConfig.motorDirection == Motor::MOTOR_DIRECTION::POSITIVE) ? "same" : "inverse");
      log("  -> motor electrical rotations per revolution: %d (exact: %f )" /*, rotAnglePerElect: %f) */ " \n",
          motor.motorConfig.electricalRotationsPerRevolution,
             electricalRotationsPerRevolutionExact /*,
             motorConfig.encoderRotationPerElectricalRotation*/ );


      motor.enableMotor(false);
      motor.motorConfig.isValid = true;
      estimationDone._triggerEvent();
    }


    /**
     * Estimate motor parameters by slowly rotating the motor for a short time. The motor has to be enabled before calling this function.
     * This will only estimate the encoderElectricalOffset.
     * The electricalRotationsPerRevolution and motorDirection have to be correctly set before in motorConfig before calling this function.
     * @param encoder the initialized position encoder for this motor.
     */
    void initEstimateMotorConfig_encoderOffset(Encoder &encoder, int motorThrottle) {
      log("- find out encoderElectricalOffset ...");

      // do N [electrical] rotation of the motor
      int warmupElectricalRotationNumber = 1;
      int checkElectricalRotationNumber = 6;//5;

      long avg_encoderElectricalOffset_sum = 0;
      int avg_encoderElectricalOffset_num = 0;

      /// rotate the motor backwards
      for (int rotationIndex = 0; rotationIndex < checkElectricalRotationNumber + warmupElectricalRotationNumber; ++rotationIndex) {
        // do one electrical rotation
        for (int angleEl = MODULATION_DATA_FULLROTATION_VALUE-1; angleEl >= 0; angleEl--) {
          callTickCallback();
          motor.applyPhaseAngleSpaceVector(angleEl, motorThrottle);

          HAL_Delay(1);
          HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
          EncoderAngle angle = encoder.update();
          //printf("%ld \n", angle.angle);

          // ignore warmup rotations
          if (rotationIndex < warmupElectricalRotationNumber) {
            continue;
          }

          // map current encoder angle to electrical angle
          // -> to calculate electrical offset
          // convert to electrical modulation angle (with motorConfig.encoderElectricalOffset = 0)
          int directionSign = (motor.motorConfig.motorDirection == Motor::MOTOR_DIRECTION::POSITIVE ? 1 : -1);
          // angle.angle *= directionSign;
          int encoderAngleAsElectrical = (angle.angle * motor.motorConfig.electricalRotationsPerRevolution) / Encoder::CONVERT_DIVIDE_ENCODER_ANGLE;
          encoderAngleAsElectrical %= MODULATION_DATA_FULLROTATION_VALUE;
          encoderAngleAsElectrical *= directionSign;
          if (encoderAngleAsElectrical < 0) {
            encoderAngleAsElectrical = MODULATION_DATA_FULLROTATION_VALUE + encoderAngleAsElectrical;
          }

          int encoderElectricalOffsetExact = encoderAngleAsElectrical - angleEl;
          int encoderElectricalOffset = encoderElectricalOffsetExact % MODULATION_DATA_FULLROTATION_VALUE;
          encoderElectricalOffset = (encoderElectricalOffset >= 0) ? encoderElectricalOffset : MODULATION_DATA_FULLROTATION_VALUE+encoderElectricalOffset;

          avg_encoderElectricalOffset_sum += encoderElectricalOffset;
          avg_encoderElectricalOffset_num++;

          log("%d, \t %d", angleEl, encoderAngleAsElectrical);
          log("  -> encoder electrical offset: %d (exact: %d) \n",
                 encoderElectricalOffset,
                 encoderElectricalOffsetExact);
        }

        log("completed rotation [%d]", rotationIndex);
      }

      motor.motorConfig.encoderElectricalOffset = avg_encoderElectricalOffset_sum / avg_encoderElectricalOffset_num;
      log("  -> encoder electrical offset: %d (average)",
             motor.motorConfig.encoderElectricalOffset);
    }


  private:
    void callTickCallback() {
      if (estimationTickCallback) {
        (*estimationTickCallback)();
      }
    }
};