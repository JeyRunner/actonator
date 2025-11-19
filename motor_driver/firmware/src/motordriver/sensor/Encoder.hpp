//#include "usbd_cdc_if.h"

#include <cstdarg>
#include <cmath>
#include "motordriver/util/constants.h"
#include "modulation/SpaceVectorModulation.h"
#include "can/packages/Status.h"


/**
 * Angle in encoder format.
 * Where 360° ^= ENCODER_MAX_VALUE.
 */
struct EncoderAngle {
    explicit EncoderAngle(long angle) :angle(angle)
    {}
    long angle{};
};



/**
 * Access the AS5600 encoder via i2c.
 */
class Encoder {

    static constexpr bool ENCODER_LOG = true;

    const uint16_t addr_raw = 0x36;
    const uint16_t addr = (addr_raw << 1);

    const uint8_t REG_ADDRESS_STATUS = 0x0B;
    const uint8_t REG_ADDRESS_ANGLE_FILTERED_L = 0x0F;
    const uint8_t REG_ADDRESS_ANGLE_FILTERED_H = 0x0E;


    /// last angle from previous update() call
    long prev_time_ms = 0;
    float angle_prev_2PI = 0;
    long angle_prev = 0;
    long angle_abs_prev = 0;
    long angle_velocity = 0;


    int full_rotations = 0;
    float angle_2PI = 0;


  public:
    enum class ENCODER_DIRECTION {
        POSITIVE, // same direction as the motor
        NEGATIVE  // inverse direction as the motor
    };


    static constexpr int ENCODER_RESOLUTION_IN_BITS = 12;
    /// maximum encoder value which corresponds to 360°
    static constexpr int ENCODER_MAX_VALUE = 1 << ENCODER_RESOLUTION_IN_BITS; // = 2^ENCODER_RESOLUTION_IN_BITS


    /// Divide encoder angle by this value to convert it to the modulation angle format.
    static constexpr int CONVERT_DIVIDE_ENCODER_ANGLE = ENCODER_MAX_VALUE / MODULATION_DATA_FULLROTATION_VALUE;


    /// set this i2c handler before calling init.
    I2C_HandleTypeDef *i2c = nullptr;
    //Encoder(I2C_HandleTypeDef *i2c): i2c(i2c) {
    //}

    ENCODER_STATUS status = ENCODER_STATUS::UNKNOWN;


    /**
     * Check if encoder is connected and if magnet is in correct distance.
     * @return true if ok
     */
    bool init() {
      printf("ENCODER_MAX_VALUE: %d\n", ENCODER_MAX_VALUE);

      //log("check encoder i2 status ...");
      auto ready = HAL_I2C_IsDeviceReady(i2c, addr, 1000, 1000);
      printf("encoder i2 is ready: %d \n", (ready == HAL_StatusTypeDef::HAL_OK));


      // write config to encoder
      writeConfig();


      //printf("will read encoder status ... \n");
      status = readStatus();
      printf("read encoder status [DONE]: %d \n", status);
      return status == ENCODER_STATUS::OK_MAGNET_DETECTED;
    }


    void writeConfig() {
      const uint8_t config_pm = 0b00;         // power mode: normal
      const uint8_t config_hysteresis = 0b00; // hysteresis: off
      const uint8_t config_outs = 0b00;       // output stage: analog

      const uint8_t config_pwmf = 0b00; // pwm freq: 115Hz (not relevant because output is analog)
      const uint8_t config_sf = 0b00;   // slow filter: 16x - max filtering
      const uint8_t config_fth = 0b000; // fast filter threshold: slow filter only
      const bool    config_wd = false;  // watchdog: off


      const uint8_t config_low =
          (config_wd << 5) |
              (config_fth << 2) |
              (config_sf << 0);
      const uint8_t config_high =
          (config_pwmf << 6) |
              (config_outs << 4) |
              (config_hysteresis << 2) |
              (config_pm << 0);

      // print config
      char config_high_str[30];
      char config_low_str[30];
      itoa(config_high, config_high_str, 2);
      itoa(config_low, config_low_str, 2);
      //printf("will write encoder config:   low: %s,  high: %s \n", config_low_str, config_high_str);

      // write
      bool ok = true;
      ok &= writeRegOneByte(0x07, config_low);  // low
      ok &= writeRegOneByte(0x08, config_high); // high
      printf("encoder write config %s \n", (ok ? "was successful" : "failed"));
    }



  private:
    /**
     * Read raw encoder value.
     * Max value which corresponds to 360° is ENCODER_MAX_VALUE.
     * @return encoder angle value.
     */
    int readAngle_encoderValue() {
      uint16_t data = 0;
      // raw values
      // auto data_low = readRegOneByte(0x0D);
      // auto data_high = readRegOneByte(0x0C);

      /// read filtered values

      // this does not work: value can change between the two separate readouts
      /*
      auto data_low = readRegOneByte(REG_ADDRESS_ANGLE_FILTERED_L);
      auto data_high = readRegOneByte(REG_ADDRESS_ANGLE_FILTERED_H);
      */
      // read two bytes at once
      uint8_t data_wrong_oder[2] = {
          0, // data high
          0  // data low
      };
      uint8_t &data_high = data_wrong_oder[0];
      uint8_t &data_low = data_wrong_oder[1];
      readReg2Byte(REG_ADDRESS_ANGLE_FILTERED_H, data_wrong_oder);

      // printf("angle raw: %d.%d \n", data_low, data_high);

      data = data_low;
      data |= ((data_high & 0b00001111) << 8);

      /*
      printf("%d, %d, %d \n", data_low, data_high, data);
      char buffer [33];
      itoa(data_high,buffer,2);
      char buffer_d [33];
      itoa(data,buffer_d,2);
      printf ("binary: %s\t - %s\n",buffer, buffer_d);
      */

      return data;
    }



  public:
    /**
     * Read angle where 360 degree corresponds to 2 PI.
     */
    float readAngle2PI() {
      return ((float) readAngle_encoderValue() / (float)ENCODER_MAX_VALUE) * PI_2;
    }


    /**
     * Read angle where 360 degree corresponds to MODULATION_DATA_FULLROTATION_VALUE.
     *-/
    int readAngleModulationFormat() {
      return readAngle_encoderValue() / CONVERT_DIVIDE_ENCODER_ANGLE;
    }
     */




    ENCODER_STATUS readStatus() {
      auto status = readRegOneByte(REG_ADDRESS_STATUS);
      if (status & 0b0000'1000) {
        return ENCODER_STATUS::MAGNET_TOO_STRONG;
      }
      else if (status & 0b0001'0000) {
        return ENCODER_STATUS::MAGNET_TOO_WEAK;
      }
      else if (status & 0b0010'0000) {
        return ENCODER_STATUS::OK_MAGNET_DETECTED;
      }
      else {
        return ENCODER_STATUS::UNKNOWN;
      }
    }


    char* encoderStatusToStr(ENCODER_STATUS encoderStatus) {
      switch (encoderStatus) {
        case ENCODER_STATUS::OK_MAGNET_DETECTED:
          return "OK_MAGNET_DETECTED";
        case ENCODER_STATUS::MAGNET_TOO_STRONG:
          return "MAGNET_TOO_STRONG";
        case ENCODER_STATUS::MAGNET_TOO_WEAK:
          return "MAGNET_TOO_WEAK";
        case ENCODER_STATUS::UNKNOWN:
          return "UNKNOWN";
      }
      return "";
    }


    /**
     * Update measured angle, track full rotations.
     * Updates full_rotations and angle_2PI.
     * @deprecated
     * @return absolute angle (2 PI = 360°) with also considers full rotations
     *-/
    float update2PI() {
      float angle = readAngle2PI();
      float d_angle = angle - angle_prev_2PI;
      // check if performed full rotation
      if (abs(d_angle) > 0.7*PI_2) {
        // angle goes from high to low value -> completed rotation in positive direction
        if (d_angle < 0) {
          full_rotations += 1;
        }
        else {
          full_rotations -= 1;
        }
      }
      angle_prev_2PI = angle;
      return getAngleAbsolute();
    }
     */



    /**
     * Update measured angle, track full rotations.
     * Updates full_rotations and angle_prev.
     * @return absolute angle (360° ^= ENCODER_MAX_VALUE) with also considers full rotations.
     */
    EncoderAngle update() {
      long time_ms = HAL_GetTick();

      long angle = readAngle_encoderValue();

      // get status
      // this takes some time -> only do when getting value 0
      if (/*angle <= 0*/ true) {
        status = readStatus();
        if (status != ENCODER_STATUS::OK_MAGNET_DETECTED) {
          printf("encoder error: %s \n", encoderStatusToStr(status));

          // aboard
          angle_prev = angle;
          return EncoderAngle(0);
        }
      }


      // glitch in readout -> fixed
      /*
      if (angle > ENCODER_MAX_VALUE || angle < 0) {
        printf("encoder readout glitch, angle: %lu\n", angle);
        // use old angle
        return getAngleAbsolute();
      }
      */

      //printf("chartUpdate: %lu, %ld \n", HAL_GetTick(), angle);


      long long d_angle = angle - angle_prev;
      //printf("---- angle: %lu,  dAngle: %lu (prev: %lu)\n", angle, d_angle, angle_prev);


      // check if performed full rotation
      if (abs(d_angle) > (long long)(0.7*ENCODER_MAX_VALUE)) {
        // angle goes from high to low value -> completed rotation in positive direction
        if (d_angle < 0) {
          full_rotations += 1;
        }
        else {
          full_rotations -= 1;
        }
        //printf("full rotation (%d), angle: %lu,  dAngle: %lld (prev: %lu)\n", full_rotations, angle, d_angle, angle_prev);
      }
      // glitch in readout -> fixed
      /*
      else if (abs(d_angle) > (long long)(0.1*ENCODER_MAX_VALUE)) {
        printf("encoder glitch, angle: %lu,  dAngle: %lld, abs: %lld (prev: %lu)\n", angle, d_angle, abs(d_angle), angle_prev);
        // use old angle
        //return getAngleAbsolute();
      }
      */
      angle_prev = angle;


      EncoderAngle angleAbs = getAngleAbsolute();

      // calc velocity
      long dtime = time_ms - prev_time_ms;
      //printf("dtime: %ld \n", dtime);

      prev_time_ms = time_ms;
      return angleAbs;
    }


    /**
     * Get absolute angle with also considers full rotations.
     */
    float getAngleAbsolute2PI() {
      return ((float)full_rotations)*PI_2 + angle_prev_2PI;
    }



    /**
     * Get absolute angle with also considers full rotations.
     * @return angle where 360° ^= ENCODER_MAX_VALUE
     */
    EncoderAngle getAngleAbsolute() const {
      return EncoderAngle(full_rotations*ENCODER_MAX_VALUE + angle_prev);
    }







  private:
    uint8_t readRegOneByte(uint8_t reg) {
      uint8_t data = 0;
      HAL_I2C_Mem_Read(i2c, addr, reg, I2C_MEMADD_SIZE_8BIT, &data, sizeof(data), 1000);
      return data;
    }

    void readReg2Byte(uint8_t reg, uint8_t *data_ptr) {
      uint16_t data = 0;
      HAL_I2C_Mem_Read(i2c, addr, reg, I2C_MEMADD_SIZE_8BIT, data_ptr, 2, 1000);
    }

    bool writeRegOneByte(uint8_t reg, uint8_t data) {
      HAL_StatusTypeDef status = HAL_I2C_Mem_Write(i2c, addr, reg, 1, &data, sizeof(data), 1000);
      switch (status) {
        case HAL_OK:
          printf("write HAL_OK\n");
          break;
        case HAL_ERROR:
          printf("write HAL_ERROR\n");
          break;
        case HAL_BUSY:
          printf("write HAL_BUSY\n");
          break;
        case HAL_TIMEOUT:
          printf("write HAL_TIMEOUT\n");
          break;
      }
      return status == HAL_StatusTypeDef::HAL_OK;
    }



    /*
    inline void log(const char *format, ...) {
      if constexpr(ENCODER_LOG) {
        va_list args;
        va_start(args, format);
        printf(format, args);
        printf("\n");
        va_end(args);
      }
    }
     */
};