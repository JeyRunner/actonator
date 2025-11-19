#pragma once

#include "motordriver/can/packages/CanPackages.h"
#include "control/SystemStateController.h"
#include "util/PermanentStorage.h"

void canSetup();
bool canSendQueueNotFull();
bool canSendQueueEmpty();
bool canSendData(uint32_t destAddress, uint8_t *data, uint8_t dataLength);
void onCanTxReadyInterruptEnable();
void onCanTxReadyInterruptDisable();


class CanCommunication {
  private:
    static Motor *motor;
    static PDController *pdController;
    static PosSensor *posSensor;
    static SystemStateController *systemStateController;
    static MotorFocControl *focControl;

    static uint32_t masterCanId;
    static bool setupDone;

  public:
    /// package types which wait in this queue for being send
    ///  each index: [CAN_PACKAGE_TYPE, CAN_PACKAGE_TYPE____FIELDS]
    ///  when entry is true: this type waits for being send
    static bool packageSendQueue[CAN_PACKAGE_TYPE::CAN_PACKAGE_TYPE_LENGTH][CAN_PACKAGE_TYPE__FIELDS__MAX_NUM];

  public:
    /**
     * Call this to init members, call before any other function!
     */
    static void setMembers(Motor *motor, PDController *pdController, PosSensor *posSensor, SystemStateController *systemStateController, MotorFocControl *motorFocControl) {
      CanCommunication::motor = motor;
      CanCommunication::pdController = pdController;
      CanCommunication::posSensor = posSensor;
      CanCommunication::systemStateController = systemStateController;
      CanCommunication::focControl = motorFocControl;
      masterCanId = 0;
      setupDone = false;

      // test
      //packageSendQueue[CAN_PACKAGE_TYPE::MOTOR_STATE][CAN_PACKAGE_TYPE__MOTOR_STATE__FIELDS::POSITION] = true;
    }

    static void initCanHardware() {
      canSetup();
      // test
      //canSendOrInQueue(CAN_PACKAGE_TYPE::MOTOR_STATE, CAN_PACKAGE_TYPE__MOTOR_STATE__FIELDS::POSITION);
      //canSendOrInQueue(CAN_PACKAGE_TYPE::COMMUNICATION_ERROR, CAN_PACKAGE_TYPE__COMMUNICATION_ERROR::CAN_REQUESTS_TO_FAST);
    }



    static void onCanPackagedReceived(uint32_t addressSource, uint8_t *data, uint8_t dataLength) {
      CanPackage canPackage;
      bool ok = canPackage.setFromBytes(data, dataLength);
      if (!ok) {
        return;
      }

      switch (canPackage.canPackageType) {
        case CAN_PACKAGE_TYPE::SETUP:
          masterCanId = addressSource;
          setupDone = true;
          canSendOrInQueue(CAN_PACKAGE_TYPE::SETUP_RESPONSE);
          break;

        case CAN_PACKAGE_TYPE::REBOOT:
          NVIC_SystemReset();
          break;

        case CAN_PACKAGE_TYPE::SET_SETTINGS: {
          switch ((CAN_PACKAGE_TYPE__SET_SETTINGS__FIELDS) canPackage.fieldType) {
            case VELOCITY_FILTER_TF:
              //posSensor->angleAbsoluteVelocity.Tf = canPackage.dataAsUInt16();
              posSensor->angleAbsoluteVelocityFloat.Tf = canPackage.dataAsFloat();
              break;
            case MOTOR_MAX_TORQUE:
              break;
          }
          break;
        }


        case CAN_PACKAGE_TYPE::INIT_MOTOR_SEQUENCE:
          systemStateController->systemControlState = SYSTEM_CONTROL_STATE::INIT_MOTOR_SEQUENCE;
          break;

        case CAN_PACKAGE_TYPE::ENABLE_MOTOR:
          motor->enableMotor(true);
          break;
        case CAN_PACKAGE_TYPE::DISABLE_MOTOR:
          motor->enableMotor(false);
          break;

        case CAN_PACKAGE_TYPE::SET_PD_VALUES:
          //return;
          switch ((CAN_PACKAGE_TYPE__SET_PD_VALUES__FIELDS) canPackage.fieldType) {
            case DESIRED_POS:
              pdController->targetP = canPackage.dataAsLong();
              break;
            case GAIN_P:
              pdController->gainP = canPackage.dataAsFloat();
              break;
            case DESIRED_VEL:
              pdController->targetD = canPackage.dataAsFloat();
              break;
            case GAIN_D:
              pdController->gainD = canPackage.dataAsFloat();
              break;
          }
          breakpoint();
          break;

        case CAN_PACKAGE_TYPE::REQUEST_MOTOR_STATE: {
          /*
          for (int i = 0; i < 1'000; ++i) { // simulate delay
            __asm("NOP");
          }
           */
          // ignore when current motor state did not finish sending
          if (packageSendQueue[CAN_PACKAGE_TYPE::MOTOR_STATE][CAN_PACKAGE_TYPE__MOTOR_STATE__FIELDS::POSITION] ||
              packageSendQueue[CAN_PACKAGE_TYPE::MOTOR_STATE][CAN_PACKAGE_TYPE__MOTOR_STATE__FIELDS::VELOCITY] ||
              packageSendQueue[CAN_PACKAGE_TYPE::MOTOR_STATE][CAN_PACKAGE_TYPE__MOTOR_STATE__FIELDS::TORQUE]) {
            //breakpoint();
            canSendOrInQueue(COMMUNICATION_ERROR, CAN_PACKAGE_TYPE__COMMUNICATION_ERROR::CAN_REQUESTS_TO_FAST);
            break;
          }
          if (canPackage.dataCarried[0]
              & (char) CAN_PACKAGE_TYPE__MOTOR_STATE_REQUEST__REQUESTED_FIELDS::POSITION_AND_VELOCITY) {
            canSendOrInQueue(CAN_PACKAGE_TYPE::MOTOR_STATE, CAN_PACKAGE_TYPE__MOTOR_STATE__FIELDS::POSITION);
            canSendOrInQueue(CAN_PACKAGE_TYPE::MOTOR_STATE, CAN_PACKAGE_TYPE__MOTOR_STATE__FIELDS::VELOCITY);
          }
          if (canPackage.dataCarried[0]
              & (char) CAN_PACKAGE_TYPE__MOTOR_STATE_REQUEST__REQUESTED_FIELDS::TORQUE) {
            canSendOrInQueue(CAN_PACKAGE_TYPE::MOTOR_STATE, CAN_PACKAGE_TYPE__MOTOR_STATE__FIELDS::TORQUE);
          }

          break;
        }

        case CAN_PACKAGE_TYPE::REQUEST_SYSTEM_STATE:
          canSendOrInQueue(CAN_PACKAGE_TYPE::SYSTEM_STATE, 0);
          break;

        default:
          break;
      }
    }

    /// interrupt when can is ready to send
    static void onCanTxReady() {
      onCanTxReadyInterruptDisable();

      for (int type = 0; type < CAN_PACKAGE_TYPE::CAN_PACKAGE_TYPE_LENGTH; ++type) {
        for (int field = 0; field < CanPackage::numFieldTypes(static_cast<CAN_PACKAGE_TYPE>(type)); ++field) {
          if (!canSendQueueNotFull()) {
            onCanTxReadyInterruptEnable();
            return;
          }
          if (packageSendQueue[type][field]) {
            canSendPackageTypeWhenTxIsFree(static_cast<CAN_PACKAGE_TYPE>(type), field);
            packageSendQueue[type][field] = false;
          }
        }
      }
      onCanTxReadyInterruptEnable();
    }


    static void sendInitMotorSequenceDone() {
      canSendOrInQueue(CAN_PACKAGE_TYPE::INIT_MOTOR_SEQUENCE_DONE, 0);
      breakpoint();
    }


    /// interrupt when the can rx queue has an overflow, to many incoming packages
    static void onCanRxFifoOverflow() {
      canSendOrInQueue(CAN_PACKAGE_TYPE::COMMUNICATION_ERROR, CAN_PACKAGE_TYPE__COMMUNICATION_ERROR::CAN_RECEIVE_OVERFLOW);
    }


  //private:
    /**
     * When the can tx is free, this sends the package type
     */
    static void canSendPackageTypeWhenTxIsFree(CAN_PACKAGE_TYPE packageTypeToSend, uint8_t fieldType) {
      CanPackage canPackage;
      canPackage.dataCarriedLength = CanPackage::requiredPackageDataCarriedLength(packageTypeToSend);
      uint8_t dataAll[canPackage.dataCarriedLength+1];
      canPackage.fieldType = fieldType;
      canPackage.dataAll = dataAll;
      canPackage.dataCarried = &dataAll[1];
      canPackage.canPackageType = packageTypeToSend;
      //breakpoint();

      switch (packageTypeToSend) {
        case CAN_PACKAGE_TYPE::SETUP_RESPONSE:
          *(uint16_t*)canPackage.dataCarried = (uint16_t) Encoder::ENCODER_MAX_VALUE;
          //canPackage.dataCarried[0] = 0x24; // test
          break;

        case CAN_PACKAGE_TYPE::MOTOR_STATE: {
          switch (static_cast<CAN_PACKAGE_TYPE__MOTOR_STATE__FIELDS>(fieldType)) {
            case CAN_PACKAGE_TYPE__MOTOR_STATE__FIELDS::POSITION:
              canPackage.dataAsLong() = posSensor->encoder.getAngleAbsolute().angle;
              break;
            case CAN_PACKAGE_TYPE__MOTOR_STATE__FIELDS::VELOCITY:
              canPackage.dataAsFloat() = posSensor->getVelocityFilteredFloat();
              break;
            case TORQUE:
              canPackage.dataAsFloat() = (float)focControl->currentMotorForce; //pdController->currentTorque;
              break;
          }

          break;
        }


        case CAN_PACKAGE_TYPE::SYSTEM_STATE: {
          SystemStateData systemState;
          systemState.motorEnabled = motor->motorEnabled;
          systemState.noError = posSensor->encoder.status == ENCODER_STATUS::OK_MAGNET_DETECTED;
          systemState.encoderStatus = posSensor->encoder.status;
          systemState.systemControlState = systemStateController->systemControlState;
          systemState.writeToData(canPackage.dataCarried);
          break;
        }

        case CAN_PACKAGE_TYPE::COMMUNICATION_ERROR:
          break;

        case CAN_PACKAGE_TYPE::INIT_MOTOR_SEQUENCE_DONE:
          canPackage.dataAsInt32() = motor->motorConfig.encoderElectricalOffset;
          break;

        default:
          break;
      }
      // write package header
      canPackage.writeHeaderToDataAll();

      canSendData(masterCanId,  dataAll, canPackage.dataCarriedLength+1);
    }


  public:
    /**
     * When can tx is free, send this directly.
     * Otherwise add to queue.
     */
    static void canSendOrInQueue(CAN_PACKAGE_TYPE packageTypeToSend, uint8_t fieldType = 0) {
      // disable tx ready interrupt
      onCanTxReadyInterruptDisable();

      if (canSendQueueEmpty()) {
        canSendPackageTypeWhenTxIsFree(packageTypeToSend, fieldType);
      }
      else {
        // schedule to be sent
        packageSendQueue[packageTypeToSend][fieldType] = true;
      }
      onCanTxReadyInterruptEnable();
    }
};


static void CanCommunication_sendInitMotorSequenceDone() {
  CanCommunication::sendInitMotorSequenceDone();
}



Motor *CanCommunication::motor = nullptr;
PDController *CanCommunication::pdController = nullptr;
PosSensor *CanCommunication::posSensor = nullptr;
SystemStateController *CanCommunication::systemStateController = nullptr;
MotorFocControl *CanCommunication::focControl = nullptr;

uint32_t CanCommunication::masterCanId = 0;
bool CanCommunication::setupDone = false;
bool CanCommunication::packageSendQueue[CAN_PACKAGE_TYPE::CAN_PACKAGE_TYPE_LENGTH][CAN_PACKAGE_TYPE__FIELDS__MAX_NUM] = {};