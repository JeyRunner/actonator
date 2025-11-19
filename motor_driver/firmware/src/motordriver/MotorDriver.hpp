//#include "usbd_cdc_if.h"
#include <cmath>
#include <cstdio>
#include "motordriver/sensor/Encoder.hpp"
#include "config.h"
#include "motordriver/util/util.h"
#include "Motor.hpp"
#include "MotorParameterEstimation.hpp"
#include "motordriver/control/MotorFocControl.h"
#include "control/PDController.h"
//#include "can/CanCommunication.h"
#include "control/SystemStateController.h"
#include "test/test_filters__.hpp"

#include "canSot/CanProtocolHandler.h"



Encoder encoder;
Motor motor;

void setup() {
  HAL_Delay(1000);
  /*
  log("-----------------------------------\n"
      "-- WALKING ROBOT MOTOR DRIVER -----\n"
      "-----------------------------------\n");
      */
  //log("start setup ...");

  encoder.i2c = i2c_2;
  encoder.init();

  motor.pwmTimer = timer_htim1;
  motor.motorEnablePort = MOTOR2_ENABLE_GPIO_Port;
  motor.motorEnablePin = MOTOR2_ENABLE_Pin;
  motor.init();

  log("setup [DONE]");
}




/// the handle to the used can, set in main.cpp
extern CAN_HandleTypeDef hcan;


[[noreturn]]
void __attribute__ ((noinline)) program()  {
  //test_filters();
  //return;

  //printf("start motor driver ... \n");




  // -------------------------------------
  // Test Feed forward mode
  /*
  motor.enableMotor(true);
  int a = 0;
  while (true) {
    a += 1;
    //HAL_Delay(1);
    for (int i = 0; i < 60; ++i) {
      HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
    }
    motor.applyPhaseAngleSpaceVector(a, 100);
    HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
  }
   */



  // enable motor 1
  //HAL_GPIO_WritePin(MOTOR2_ENABLE_GPIO_Port, MOTOR2_ENABLE_Pin, GPIO_PIN_SET);
  //HAL_GPIO_WritePin(MOTOR2_ENABLE_GPIO_Port, MOTOR2_ENABLE_Pin, GPIO_PIN_RESET);



  //HAL_Delay(1000);
  //HAL_Delay(1000);

  //motor.motorConfig.electricalRotationsPerRevolution = 7;
  //motor.motorConfig.encoderElectricalOffset = -110;
  //motor.motorConfig.motorDirection = Motor::MOTOR_DIRECTION::NEGATIVE;
  //motor.initEstimateMotorConfig(encoder);
  //return;

  //motor.enableMotor(true);
  //motor.initEstimateMotorConfig_encoderOffset(encoder, 120);

  MotorParameterEstimator motorParameterEstimator(motor);

  PDController pdController;
  MotorFocControl focControl(motor, encoder, pdController);
  SystemStateController controller(motor, motorParameterEstimator, focControl.posSensor, focControl, pdController);

  // can
  CanProtocolHandler canProtocolHandler(controller);//(controller);
  canProtocolHandler.init();

  //CanCommunication::setMembers(&motor, &pdController, &focControl.posSensor, &controller, &focControl);
  //CanCommunication::initCanHardware();
  motor.enableMotor(false);

  // run loop
  float i=0;
  while(true) {
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PinState::GPIO_PIN_SET);
    canProtocolHandler.loopCanCommunication();
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PinState::GPIO_PIN_RESET);

    controller.runLoopIteration();
    //controller.systemControlState = SYSTEM_CONTROL_STATE::PD_CONTROLLER;
    //CanCommunication::canSendOrInQueue(CAN_PACKAGE_TYPE::MOTOR_STATE, CAN_PACKAGE_TYPE__MOTOR_STATE__FIELDS::POSITION);

    // test
    /*
    if (i > MODULATION_DATA_FULLROTATION_VALUE) {
      i = 0;
    }
    i+=0.05;
    pdController.setDesiredPosition(getModulationValues<MODULATION_TYPE::SINUS>((int)i).a * 1);
     */
  }

  /*
  motor.enableMotor(true);
  while (true) {
    focControl.focInlineFieldTest();
  }
  //*/

  //HAL_Delay(2000);
  //motor.enableMotor(true);

  int c = 0;
  while(true) {
    focControl.focIteration();
    //posSensor.update();
    //continue;

    HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
    c++;

    if (c == 8000) {
      pdController.setDesiredPosition(0);
    } else if (c == 2*8000) {
      pdController.setDesiredPosition(10200);
      c = 0;
    }
  }
}
