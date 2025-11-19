#pragma once


// disable all debug messages of can sot so reduce binary size
#define SOT_DEBUG false

#include <communication/SOTClient.h>
#include <stm32_hal/Stm32HalCanInterface.hpp>
#include "protocol_generated/MotorControllerCanProtocol.hpp"
#include "etl/functional.h"

using namespace MotorControllerCanProtocolNamespace;



/// the handle to the used can, set in main.cpp
extern CAN_HandleTypeDef hcan;

class CanProtocolHandler {

    uint8_t ownSOTDeviceId = 1;
    Stm32HalCanInterface canInterface;
    SOTClient<MotorControllerCanProtocol, Stm32HalCanInterface> sotClient;

    SystemStateController &systemStateController;

    //SystemStateController &systemStateController;


public:
    CanProtocolHandler(SystemStateController &systemStateController)
    : canInterface(ownSOTDeviceId, hcan),
      sotClient(canInterface, ownSOTDeviceId), systemStateController(systemStateController)//,
      //systemStateController(controller)
    {}


    void init() {
      canInterface.startCanInterface();
      auto &objectTree = sotClient.getProtocol().objectTree;

      // set initial values
      //sotClient.getProtocol().objectTree._meta.protocolVersion.write(1);

      // default values
      objectTree.settings.velocity_filter.smoothingTf.write(20'000);
      objectTree.motor.target_values_pd.gainP.write(1);
      objectTree.motor.target_values_pd.gainD.write(0.001);
      objectTree.motor.target_values_pd.position.write(0);
      objectTree.motor.target_values_pd.velocity.write(0);
      objectTree.settings.motor_parameters.motorParametersValid.write(MotorControllerCanProtocolNamespace::BOOL::FALSE);

      // progress can packages during blocking execution of MotorInitSequence
      systemStateController.motorParameterEstimator.estimationTickCallback = etl::delegate<void()>::create<CanProtocolHandler, &CanProtocolHandler::loopCanCommunication>(*this);
    }



    void loopCanCommunication() {
      auto &objectTree = sotClient.getProtocol().objectTree;
      auto &calls = sotClient.getProtocol().remoteCalls;

      // set debug values
      objectTree.debug.clientRxBufferNumPackages.write(canInterface.getRxBufferNumPackages());
      objectTree.debug.clientTxBufferNumPackages.write(canInterface.getTxBufferNumPackages());


      // handle all received can frames
      sotClient.processCanFrames();


      // handle remote calls
      if (calls.ChangeMotorOperationMode.remoteCallCalled.checkAndReset()) {
        handleCall_ChangeMotorOperationMode();
      }
      if (calls.StartMotorInitSequence.remoteCallCalled.checkAndReset()) {
        handleCall_StartMotorInitSequence();
      }
      //calls.ChangeMotorOperationMode.handleCallCalled(this->handleCall_ChangeMotorOperationMode);
      if (calls.SetMotorParameters.remoteCallCalled.checkAndReset()) {
        handleCall_SetMotorParameters();
      }

      // send remote calls return
      if (systemStateController.motorParameterEstimator.estimationDone.checkAndReset()) {
        callDoReturn_StartMotorInitSequence();
      }



      // get settings
      systemStateController.posSensor.angleAbsoluteVelocityFloat.Tf = objectTree.settings.velocity_filter.smoothingTf.read();
      systemStateController.focControl.maxTorque = objectTree.motor.maxThrottle.read() * 1000;
      // @todo get settings.motor_parameters

      // get motor target values
      systemStateController.pdController.gainP = objectTree.motor.target_values_pd.gainP.read();
      systemStateController.pdController.gainD = objectTree.motor.target_values_pd.gainD.read();
      if (objectTree.motor.test_pd_mode.active.read() == MotorControllerCanProtocolNamespace::BOOL::FALSE) {
        systemStateController.pdController.targetP = objectTree.motor.target_values_pd.position.read();
        systemStateController.pdController.targetD = objectTree.motor.target_values_pd.velocity.read();
      }
      else {
        genPdVauesForPdTestMode();
      }


      // set motor values
      objectTree.motor.current_values.position.write((float)systemStateController.posSensor.encoder.getAngleAbsolute().angle);
      objectTree.motor.current_values.velocityNonFiltered.write(systemStateController.posSensor.angleAbsoluteVelocityFloatRaw);
      objectTree.motor.current_values.velocity.write(systemStateController.posSensor.getVelocityFilteredFloat());
      objectTree.motor.current_values.torque.write(((float)systemStateController.focControl.currentMotorTorque) / 1000.0f); // lastAppliedThrottle is between 0 and 1000
      if (objectTree.motor.enabled.receivedValueUpdate.checkAndReset()) {
        if (objectTree.motor.enabled.read() == MOTOR_ENABLED::ENABLED && systemStateController.motor.motorConfig.isValid) {
          systemStateController.motor.enableMotor(true);
        }
        else if (objectTree.motor.enabled.read() == MOTOR_ENABLED::DISABLED) {
          systemStateController.motor.enableMotor(false);
        }
      }


      // set status
      if (systemStateController.systemControlStateChanged.checkAndReset()) {
        // update and send value
        objectTree.motor.operationMode.write(systemStateController.getSystemControlState()).sendValue();
      }
      objectTree.motor.enabled.write(systemStateController.motor.motorEnabled ? MOTOR_ENABLED::ENABLED : MOTOR_ENABLED::DISABLED);
    }



    void handleCall_StartMotorInitSequence() {
      auto &objectTree = sotClient.getProtocol().objectTree;
      auto &call = sotClient.getProtocol().remoteCalls.StartMotorInitSequence;

      systemStateController.motorParameterEstimator_UseThrottle = call.argumentsData.throttle;
      systemStateController.setSystemControlState(MOTOR_OPERATION_MODE::CONFIGURATION_PROCESS);
    }

    void callDoReturn_StartMotorInitSequence() {
      auto &objectTree = sotClient.getProtocol().objectTree;
      auto &call = sotClient.getProtocol().remoteCalls.StartMotorInitSequence;

      // @todo: properly convert enum
      objectTree.settings.motor_parameters.motorDirection.write(
              static_cast<MOTOR_CONFIG_DIRECTION>(systemStateController.motor.motorConfig.motorDirection));
      objectTree.settings.motor_parameters.electricalRotationsPerRevolution.write(systemStateController.motor.motorConfig.electricalRotationsPerRevolution);
      objectTree.settings.motor_parameters.encoderElectricalOffset.write(systemStateController.motor.motorConfig.encoderElectricalOffset);
      objectTree.settings.motor_parameters.motorParametersValid.write(convertBOOL(systemStateController.motor.motorConfig.isValid));
      call.sendReturnOk(StartMotorInitSequenceReturnDataCallable{
              (float)systemStateController.motor.motorConfig.encoderElectricalOffset,
              (uint8_t)systemStateController.motor.motorConfig.electricalRotationsPerRevolution,
              static_cast<MOTOR_CONFIG_DIRECTION>(systemStateController.motor.motorConfig.motorDirection)
      });
    }


    void handleCall_SetMotorParameters() {
      auto &objectTree = sotClient.getProtocol().objectTree;
      auto &call = sotClient.getProtocol().remoteCalls.SetMotorParameters;
      call.sendReturnError({}); // @todo implement
    }


    void handleCall_ChangeMotorOperationMode() {
      auto &objectTree = sotClient.getProtocol().objectTree;
      auto &call = sotClient.getProtocol().remoteCalls.ChangeMotorOperationMode;

      auto targetState = call.argumentsData.requestedOperationMode;
      if (targetState == REQUEST_MOTOR_OPERATION_MODE::DO_NOTING) {
        systemStateController.setSystemControlState(MOTOR_OPERATION_MODE::DO_NOTHING);
        call.sendReturnOk({});
        return;
      }
      if (!systemStateController.motor.motorConfig.isValid) {
        call.sendReturnError(REQUEST_MOTOR_OPERATION_MODE_ERROR::MOTOR_NOT_CONFIGURED);
        return;
      }
      else {
        switch (targetState) {
          case REQUEST_MOTOR_OPERATION_MODE::CONTROL_PD:
            systemStateController.setSystemControlState(MOTOR_OPERATION_MODE::CONTROL_PD);
            break;
        }
        call.sendReturnOk({});
      }
    }



    void genPdVauesForPdTestMode() {
      float s = canInterface.getCurrentMicros() / 1e6f;
      float target_p = sin(s*0.5f)*4000.0f;
      systemStateController.pdController.targetP = target_p;
    }




  private:
    BOOL convertBOOL(bool value) {
      if (value) {
        return BOOL::TRUE;
      }
      else {
        return BOOL::FALSE;
      }
    }
};