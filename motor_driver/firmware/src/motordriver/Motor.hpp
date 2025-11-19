#pragma once

#include "config.h"
#include "motordriver/util/util.h"
#include "modulation/SpaceVectorModulation.h"



class Motor {
  public:
    /// set these before init
    TIM_HandleTypeDef *pwmTimer = nullptr;
    GPIO_TypeDef *motorEnablePort = nullptr;
    uint16_t motorEnablePin = 0;

    bool motorEnabled = false;

    uint16_t lastAppliedThrottle = 0;


    enum class MOTOR_DIRECTION {
        POSITIVE = 0, // same direction as the encoder
        INVERSE = 1  // inverse direction as the encoder
    };

    /// set by initEstimateMotorConfig()
    struct MotorConfig {
        /// does MotorConfig contain valid values
        bool isValid = false;

        int encoderElectricalOffset = 0;
        int electricalRotationsPerRevolution = 0;
        MOTOR_DIRECTION motorDirection = MOTOR_DIRECTION::POSITIVE;
    } motorConfig;



    /**
     * Init will start the PWM timers required to produce output signals for the motor phases.
     */
    void init() {
      // enable pwm channels
      HAL_TIM_PWM_Start(pwmTimer, TIM_CHANNEL_1);
      HAL_TIM_PWM_Start(pwmTimer, TIM_CHANNEL_2);
      HAL_TIM_PWM_Start(pwmTimer, TIM_CHANNEL_3);
    }



  public:
    /**
     * Enable and disable the motor output.
     * @param enable when set to false all motor output are connected to GND, because MOSFET drivers are then disabled.
     */
    void enableMotor(bool enable) {
      this->motorEnabled = enable;
      HAL_GPIO_WritePin(motorEnablePort, motorEnablePin,
                        (this->motorEnabled ? GPIO_PIN_SET : GPIO_PIN_RESET)
                        );
    }





    /**
     * Apply motor a field via the space vector PWM method.
     * The field is given by an angle and amplitude (throttle)
     * @param angleElectrical the fields electrical angle (will be normalized within ths call)
     * @param throttle the field strength [0 ... 1000]
     */
    void applyPhaseAngleSpaceVector(long angleElectrical, int throttle) {
      lastAppliedThrottle = throttle;
      Vec3 phaseValues = getModulationValues<MODULATION_TYPE::SINUS>(angleElectrical);
      // scale by throttle
      // shift by half max value upwards -> 50% duty circle is virtual Zero
      phaseValues.a = (phaseValues.a * throttle) / 1000 + MODULATION_DATA_MAX_VALUE;
      phaseValues.b = (phaseValues.b * throttle) / 1000 + MODULATION_DATA_MAX_VALUE;
      phaseValues.c = (phaseValues.c * throttle) / 1000 + MODULATION_DATA_MAX_VALUE;

      setOutput(phaseValues.a, phaseValues.b, phaseValues.c);
    }




    static constexpr int ANGLE_MAX = 1000;
    static constexpr int ANGLE_1_3_ROTATION = (int)((float)ANGLE_MAX * (1.0/3));
    static constexpr int ANGLE_1_6_ROTATION = (int)((float)ANGLE_MAX * (1.0/6));

/*
    void applyPhaseAngle2PI(float angle, float throttle = 1.0) {
      float f = angle / PI_2;
      int applInt = (int) (fmod(f, 1.0) * 1000);
      applInt = (applInt < 0) ? (1000 + applInt) : applInt;
      applyPhaseAngle(applInt, throttle);
    }


    void applyPhaseAngle(int angle, float throttle = 1.0) {
      if (angle < 0) {
        printf("motorPhase is negative [skip]: %d \n", angle);
        return;
      }

      int phaseA = 0;
      int phaseB = 0;
      int phaseC = 0;

      int mainSector = (int)((float)angle / ((float)ANGLE_MAX * (1.0/3)));  // 0,1,2
      int subSector = (int)((float)(angle - mainSector*((1.0/3) * ANGLE_MAX)) / ((float)ANGLE_MAX * (1.0/6)));   // 0,1

      int angleToNextMainSector = angle - ((int) mainSector*ANGLE_1_3_ROTATION);
      angleToNextMainSector = (subSector == 0) ? angleToNextMainSector : (ANGLE_1_3_ROTATION - angleToNextMainSector);

      int angleToNextMainSector_asPhaseValue = (int)(((float)angleToNextMainSector / (float)ANGLE_1_6_ROTATION) * (float)PHASE_MAX_VALUE);


      //printf("chartUpdate: %lu, %f, %d, %f \n", HAL_GetTick(), angle/1000.0, mainSector, angleToNextMainSector/1000.0);

      switch (mainSector) {
        case 0: {
          phaseC = 0;
          if (subSector == 0) {
            phaseA = PHASE_MAX_VALUE;
            phaseB = angleToNextMainSector_asPhaseValue;
          }
          else {
            phaseA = angleToNextMainSector_asPhaseValue;
            phaseB = PHASE_MAX_VALUE;
          }
        } break;

        case 1: {
          phaseA = 0;
          if (subSector == 0) {
            phaseB = PHASE_MAX_VALUE;
            phaseC = angleToNextMainSector_asPhaseValue;
          }
          else {
            phaseB = angleToNextMainSector_asPhaseValue;
            phaseC = PHASE_MAX_VALUE;
          }
        } break;

        case 2: {
          phaseB = 0;
          if (subSector == 0) {
            phaseC = PHASE_MAX_VALUE;
            phaseA = angleToNextMainSector_asPhaseValue;
          }
          else {
            phaseA = PHASE_MAX_VALUE;
            phaseC = angleToNextMainSector_asPhaseValue;
          }
        } break;
      }

      phaseA = (int) ((float)phaseA * THROTTLE * throttle);
      phaseB = (int) ((float)phaseB * THROTTLE * throttle);
      phaseC = (int) ((float)phaseC * THROTTLE * throttle);


      // min timer value is 100
      // otherwise no effect on output due to mosfet driver deadtime
      // constexpr int PHASE_MIN_VALUE = 25;
      // phaseA = (phaseA < PHASE_MIN_VALUE && phaseA > 0) ? PHASE_MIN_VALUE : phaseA;
      // phaseB = (phaseB < PHASE_MIN_VALUE && phaseB > 0) ? PHASE_MIN_VALUE : phaseB;
      // phaseC = (phaseC < PHASE_MIN_VALUE && phaseC > 0) ? PHASE_MIN_VALUE : phaseC;

      // printf("chartUpdate: %lu, %d, %d, %d \n", HAL_GetTick(), phaseA, phaseB, phaseC);
      setOutput(phaseA, phaseB, phaseC);
    }
*/

    /**
     *
     * @param angle electrical angle (2PI = 360°)
     * @param throttle motor throttle, value between 0 and 1
     */
    void applyPhaseAngleSinusodial2PI(float angleEL, float throttle) {
      /*
      float voltage_power_supply = 12;
      float Uq = voltage_power_supply * 0.04f * throttle;
      // Sinusoidal PWM modulation
      // Inverse Park + Clarke transformation

      // angle normalization in between 0 and 2pi
      // only necessary if using _sin and _cos - approximation functions
      float angle_el_norm = _normalizeAngle(angleEL);

      // Inverse park transform
      float Ualpha = -_sin(angle_el_norm) * Uq;  // -sin(angle) * Uq;
      float Ubeta =   _cos(angle_el_norm) * Uq;    //  cos(angle) * Uq;

      // Inverse Clarke transform
      float Ua = Ualpha + voltage_power_supply/2;
      float Ub = -0.5 * Ualpha  + SQRT3_2 * Ubeta + voltage_power_supply/2;
      float Uc = -0.5 * Ualpha - SQRT3_2 * Ubeta + voltage_power_supply/2;

      int outA = (int) ((Ua/voltage_power_supply) * 1000.0f);
      int outB = (int) ((Ub/voltage_power_supply) * 1000.0f);
      int outC = (int) ((Uc/voltage_power_supply) * 1000.0f);

      setOutput(outA, outB, outC);
       */
    }


    /**
     * Transform given absolute real rotation angle to the corresponding electrical angle of this motor.
     * @param absAngle the absolute rotation angle (can be larger than 2PI for multiple rotations)
     * @return electrical angle of this motor
     */
    inline float absoluteToElectricalAngle2PI(float absAngle) const {
      return absAngle * (float)motorConfig.electricalRotationsPerRevolution + motorConfig.encoderElectricalOffset;
    }


    /**
     * Transform given absolute real rotation angle to the corresponding electrical angle of this motor.
     * @param absAngle the absolute rotation angle (can be larger than 2PI for multiple rotations)
     * @return electrical angle of this motor
     */
    inline long absoluteToElectricalAngle(long absAngle) const {
      return absAngle * motorConfig.electricalRotationsPerRevolution + motorConfig.encoderElectricalOffset;
    }


    /**
     * Transform given absolute real rotation angle (in encoder format)
     * to the corresponding electrical angle (in modulation format) of this motor.
     * @param absAngle the absolute rotation angle (can be larger than ENCODER_MAX_VALUE for multiple rotations)
     * @return electrical angle of this motor in modulation format (360° ^= MODULATION_DATA_FULLROTATION_VALUE)
     */
    inline long absoluteEncoderAngleToElectricalModulationAngle(EncoderAngle absAngle) const {
      return (absAngle.angle * motorConfig.electricalRotationsPerRevolution) / Encoder::CONVERT_DIVIDE_ENCODER_ANGLE
              + motorConfig.encoderElectricalOffset;
    }



    /**
     * Values between 0 and PHASE_MAX_VALUE
     * @param phaseA
     * @param phaseB
     * @param phaseC
     */
    void setOutput(int phaseA, int phaseB, int phaseC) {
      //printf("%lu, %d, %d, %d \n", HAL_GetTick(), phaseA, phaseB, phaseC);

      __HAL_TIM_SET_COMPARE(pwmTimer, TIM_CHANNEL_1, phaseA);
      __HAL_TIM_SET_COMPARE(pwmTimer, TIM_CHANNEL_2, phaseB);
      __HAL_TIM_SET_COMPARE(pwmTimer, TIM_CHANNEL_3, phaseC);
    }
};