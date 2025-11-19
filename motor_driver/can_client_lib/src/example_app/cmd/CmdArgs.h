#pragma once

#include<iostream>
using namespace std;


/*
 * CmdArgs class
 */
class CmdArgs
{
  public:
    string canInterface = "vcan0";
    unsigned int clientId = 1;
    bool runMotorInitSequence = false;
    bool forceMotorEnable = false;

    bool outputCurrentMotorValues = false;
    bool usePdValuesFromuC = false;
    float motor_max_throttle = 0.150;

    float vel_filter_smoothingTf = 20'000;
    float gainP = 1;
    float gainD = 0;

    float pdSinWavelength = 1;
    float pdSinWaveAmplitude = 1000;

    unsigned int loopDelayMs = 10;

    static CmdArgs parseArgs(int argc, const char **argv);
};
