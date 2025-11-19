#include <MotorDriverCanMaster.hpp>
#include <unistd.h>
#include <chrono>
#include <cstdlib>
#include <csignal>
#include "cmd/CmdArgs.h"
#include "StreamWebsocket.h"


ConnectedClient<MotorControllerCanProtocol, SocketCanInterface> *clientGlob = nullptr;


void onAppExit(int i) {
  cout << ">> exiting app -> will disable motor" << endl;
  // wait till last send can command
  usleep(5*1e3);

  //driverCanClient.sendEnableMotor(false);
  if (clientGlob) {
    clientGlob->protocol.remoteCalls.ChangeMotorOperationMode.sendCall({MotorControllerCanProtocolNamespace::REQUEST_MOTOR_OPERATION_MODE::DO_NOTING});
    clientGlob->protocol.objectTree.motor.enabled.write(MOTOR_ENABLED::DISABLED).sendValue();
  }
  usleep(500*1e3);
  exit(0);
}


bool motorEnabled = false;



int main(int argc, const char **argv) {
  auto args = CmdArgs::parseArgs(argc, argv);

  // handle exit
  //std::atexit(onAppExit);
  //^C
  signal(SIGINT, onAppExit);
  //abort()
  signal(SIGABRT, onAppExit);
  //sent by "kill" command
  signal(SIGTERM, onAppExit);
  //^Z
  signal(SIGTSTP, onAppExit);

  // stream motor state to e.g. plotjuggler
  StreamWebsocket streamWebsocket;
  streamWebsocket.start();

  MotorDriverCanMaster master(args.canInterface);
  if (!master.startCanInterface()) {
    exit(1);
  }
  master.canMaster.addAndConnectToClient(args.clientId);
  auto &client = master.canMaster.getClient(args.clientId);
  clientGlob = &client;



  float i = 0;

  while (true) {
    auto start = std::chrono::system_clock::now();
    master.canMaster.processCanFrames();

    if (client.gotConnectedEvent.checkAndReset()) {
      cout << ">> client got connected" << endl;

      // set parameters
      client.protocol.objectTree.motor.target_values_pd.gainP.write(args.gainP).sendValue();
      client.protocol.objectTree.motor.target_values_pd.gainD.write(args.gainD).sendValue();
      cout << ">> set gain values: \tp=" << args.gainP << ", \td=" << args.gainD << endl;
      client.protocol.objectTree.settings.velocity_filter.smoothingTf.write(args.vel_filter_smoothingTf).sendValue();
      cout << ">> set velocity filter: \tsmoothingTf=" << args.vel_filter_smoothingTf << endl;
      client.protocol.objectTree.motor.maxThrottle.write(args.motor_max_throttle).sendValue();
      cout << ">> set motor vals: \tmaxThrottle=" << args.motor_max_throttle << endl;


      // run init
      client.protocol.objectTree.settings.motor_parameters.motorParametersValid.sendReadValueReq();
      //cout << ">> client will call StartMotorInitSequence ..." << endl;
      //client.protocol.remoteCalls.StartMotorInitSequence.sendCall(StartMotorInitSequenceArgDataCaller(100));
    }


    // check if connected
    if (client.isConnected()) {
      if (client.onCommunicationErrorRxOverflow.checkAndReset() || client.onCommunicationErrorTxOverflow.checkAndReset()) {
        cout << "got client rx or tx overflow -> will terminate" << endl;
        onAppExit(0);
      }


      // -- init sequence ------------------
      if (client.protocol.objectTree.settings.motor_parameters.motorParametersValid.receivedValueUpdate.checkAndReset()) {
        cout << ">> got motorParametersValid="
             << magic_enum::enum_name(client.protocol.objectTree.settings.motor_parameters.motorParametersValid.read()) << endl;
        if (client.protocol.objectTree.settings.motor_parameters.motorParametersValid.read() == MotorControllerCanProtocolNamespace::BOOL::TRUE) {
          cout << ">> will call ChangeMotorOperationMode(CONTROL_PD) ..." << endl;
          client.protocol.remoteCalls.ChangeMotorOperationMode.sendCall({REQUEST_MOTOR_OPERATION_MODE::CONTROL_PD});
        }
        else {
          cout << ">> the motor parameters are not set, do you want to run the MotorInitSequence? [Y/n]" << endl;
          string input;
          cin >> input;
          if (input == "Y") {
            cout << ">> will call StartMotorInitSequence ..." << endl;
            client.protocol.remoteCalls.StartMotorInitSequence.sendCall(StartMotorInitSequenceArgDataCaller(100));
          }
          else {
            cout << ">> will exit the application ..." << endl;
            onAppExit(0);
            exit(0);
          }
        }
      }

      // when init done
      client.protocol.remoteCalls.StartMotorInitSequence.handleCallReturned([&](auto returnData) {
        if (returnData.isError) {
          cout << ">> MotorInitSequence ERROR: " << magic_enum::enum_name(returnData.returnError) << "" << endl;
          onAppExit(0);
        }
        else {
          cout << ">> MotorInitSequence done: \n\t\t"
               << "motorDirection=" << magic_enum::enum_name(returnData.returnData.motorDirection)
               << ", electricalRotationsPerRevolution=" << (int)returnData.returnData.electricalRotationsPerRevolution
               << ", motorAlignmentOffset=" << returnData.returnData.motorAlignmentOffset
               << endl;
          // change to pd operation
          cout << ">> will call ChangeMotorOperationMode(CONTROL_PD) ..." << endl;
          client.protocol.remoteCalls.ChangeMotorOperationMode.sendCall({REQUEST_MOTOR_OPERATION_MODE::CONTROL_PD});
        }
      });
      // when operation mode changed
      client.protocol.remoteCalls.ChangeMotorOperationMode.handleCallReturned([&] (auto ret) {
        if (ret.isError) {
          cout << ">> could not change MotorOperationMode: " << magic_enum::enum_name(ret.returnError) << endl;
        }
        else {
          cout << ">> changed MotorOperationMode -> will now enable motor" << endl;
          // use pd sin values generated on the uC, pd.pos values send will be ignored.
          if (args.usePdValuesFromuC) {
            client.protocol.objectTree.motor.test_pd_mode.active.write(MotorControllerCanProtocolNamespace::BOOL::TRUE).sendValue();
          }
          else {
            client.protocol.objectTree.motor.test_pd_mode.active.write(MotorControllerCanProtocolNamespace::BOOL::FALSE).sendValue();
          }

          // enable motor
          client.protocol.objectTree.motor.enabled.write(MOTOR_ENABLED::ENABLED).sendValue();
          motorEnabled = true;
        }
      });




      // when motor is set up
      if (motorEnabled) {
        // request values
        ///*
        client.protocol.objectTree.debug.clientRxBufferNumPackages.sendReadValueReq();
        client.protocol.objectTree.debug.clientTxBufferNumPackages.sendReadValueReq();//*/
        client.protocol.objectTree.motor.current_values.position.sendReadValueReq();
        client.protocol.objectTree.motor.current_values.velocity.sendReadValueReq();
        client.protocol.objectTree.motor.current_values.torque.sendReadValueReq();
        client.protocol.objectTree.motor.current_values.velocityNonFiltered.sendReadValueReq();


        // set p target value
        int val = (int) (std::sin(i/args.pdSinWavelength) * args.pdSinWaveAmplitude); // * 1000.0f
        client.protocol.objectTree.motor.target_values_pd.position.write(val).sendValue();
        streamWebsocket.sendValue("target_pos", val);
      }


      // react to value changes
      // debug values
      if (client.protocol.objectTree.debug.clientRxBufferNumPackages.receivedValueUpdate.checkAndReset()) {
        if (args.outputCurrentMotorValues)
          cout << "got debug.clientRxBufferNumPackages: " << client.protocol.objectTree.debug.clientRxBufferNumPackages.read() << endl;
        streamWebsocket.sendValue("clientRxBufferNumPackages", client.protocol.objectTree.debug.clientRxBufferNumPackages.read());
      }
      if (client.protocol.objectTree.debug.clientTxBufferNumPackages.receivedValueUpdate.checkAndReset()) {
        if (args.outputCurrentMotorValues)
          cout << "got debug.clientTxBufferNumPackages: " << client.protocol.objectTree.debug.clientTxBufferNumPackages.read() << endl;
        streamWebsocket.sendValue("clientTxBufferNumPackages", client.protocol.objectTree.debug.clientTxBufferNumPackages.read());
      }

      // motor values
      if (client.protocol.objectTree.motor.current_values.position.receivedValueUpdate.checkAndReset()) {
        if (args.outputCurrentMotorValues)
          cout << "got motor pos: " << client.protocol.objectTree.motor.current_values.position.read() << endl;
        streamWebsocket.sendValue("position", client.protocol.objectTree.motor.current_values.position.read());
      }
      if (client.protocol.objectTree.motor.current_values.velocity.receivedValueUpdate.checkAndReset()) {
        if (args.outputCurrentMotorValues)
          cout << "got motor velocity: " << client.protocol.objectTree.motor.current_values.velocity.read() << endl;
        streamWebsocket.sendValue("velocity", client.protocol.objectTree.motor.current_values.velocity.read());
      }
      if (client.protocol.objectTree.motor.current_values.torque.receivedValueUpdate.checkAndReset()) {
        if (args.outputCurrentMotorValues)
          cout << "got motor torque: " << client.protocol.objectTree.motor.current_values.torque.read() << endl;
        streamWebsocket.sendValue("torque", client.protocol.objectTree.motor.current_values.torque.read());
      }
      if (client.protocol.objectTree.motor.current_values.velocityNonFiltered.receivedValueUpdate.checkAndReset()) {
        if (args.outputCurrentMotorValues)
          cout << "got motor velocityRaw: " << client.protocol.objectTree.motor.current_values.velocityNonFiltered.read() << endl;
        streamWebsocket.sendValue("velocityNonFiltered", client.protocol.objectTree.motor.current_values.velocityNonFiltered.read());
      }
      if (client.protocol.objectTree.motor.operationMode.receivedValueUpdate.checkAndReset()) {
        cout << ">> operationMode changed to: " << magic_enum::enum_name(client.protocol.objectTree.motor.operationMode.read()) << endl;
      }

      //driverCanClient.sendSetPD(val, 0, 2, 0.0);
      /*
      driverCanClient.sendSetPD(val, 0);
      streamWebsocket.sendValue("positionTarget", val);

      i+=0.005;

      // request state
      driverCanClient.sendRequestSystemState();
      driverCanClient.sendRequestMotorState(RequestMotorStateFields{
          .position_and_velocity = true,
          .torque = true
      });
      //cout << "did sendSetRequestMotorStatePackage" << endl;
      //usleep(2'000);

      driverCanClient.processReceivedPackages();

      //cout << "\n------------------- \n\n\n";
       */
    }
    usleep(args.loopDelayMs * 1000);
    //cout << "----------------------" << endl;

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    //cout << "feq: " << (1/(float)elapsed_seconds.count()) << " Hz \t iteration time: " << elapsed_seconds.count() << "s" << endl;
    i += elapsed_seconds.count();
  }
}