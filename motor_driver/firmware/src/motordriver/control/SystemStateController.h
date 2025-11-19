#pragma once

#include "can/packages/State.h"
#include "canSot/protocol_generated/MotorControllerCanProtocol_Structs.hpp"
using namespace MotorControllerCanProtocolNamespace;

static void CanCommunication_sendInitMotorSequenceDone();


struct SystemStateController {

  private:
    /*SYSTEM_CONTROL_STATE*/
    MOTOR_OPERATION_MODE systemControlState = MOTOR_OPERATION_MODE::DO_NOTHING;
  public:
    EventFlag systemControlStateChanged;

    Motor &motor;
    MotorParameterEstimator &motorParameterEstimator;
    PosSensor &posSensor;
    MotorFocControl &focControl;
    PDController &pdController;

    // tmp parameters
    int motorParameterEstimator_UseThrottle = 0;


    SystemStateController(Motor &motor, MotorParameterEstimator &motorParameterEstimator, PosSensor &posSensor, MotorFocControl &focControl, PDController &pdController) :
        motor(motor), motorParameterEstimator(motorParameterEstimator), pdController(pdController), posSensor(posSensor), focControl(focControl) {
    }


    void runLoopIteration() {
      // handle errors
      if (posSensor.encoder.status != ENCODER_STATUS::OK_MAGNET_DETECTED) {
        motor.enableMotor(false);
        setSystemControlState(MOTOR_OPERATION_MODE::DO_NOTHING);
      }

      switch (systemControlState) {
        case MOTOR_OPERATION_MODE::DO_NOTHING:
          // manually update encoder
          posSensor.update();
          break;
        case MOTOR_OPERATION_MODE::CONFIGURATION_PROCESS:
          motorParameterEstimator.initEstimateMotorConfig(posSensor.encoder, true, motorParameterEstimator_UseThrottle);
          //CanCommunication_sendInitMotorSequenceDone();  @todo: from old can communication impl
          setSystemControlState(MOTOR_OPERATION_MODE::DO_NOTHING);
          break;
        case MOTOR_OPERATION_MODE::CONTROL_PD:
          focControl.focIteration();
          break;
      }
      //HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
    }


    void setSystemControlState(MOTOR_OPERATION_MODE mode) {
      systemControlState = mode;
      systemControlStateChanged._triggerEvent();
    }

    inline MOTOR_OPERATION_MODE getSystemControlState() {
      return systemControlState;
    }
};