#pragma once

#include "CheckEndianness.h"
#include "Status.h"
#include "State.h"

enum CAN_PACKAGE_TYPE {
  /**
   * Indicate there is a problem with the can communication.
   * data=[]
   */
  COMMUNICATION_ERROR,


  /// data=[]
  SETUP,

  /// data=[2byte=MODULATION_DATA_FULLROTATION_VALUE]
  ///    MODULATION_DATA_FULLROTATION_VALUE = unsigned integer value which encodes a full rotation
  SETUP_RESPONSE,

  /// reboots the microcontroller
  /// data = []
  REBOOT,

  /** data=[0.bit=start] @todo: currently data just empty, no aboard
   * -> if start == 1: start init sequence
   * -> if start == 0: aboard init sequence
  */
  INIT_MOTOR_SEQUENCE,

  /** data=[4byte]
   *  - byte 0-4: int32 encoderElectricalOffset
  */
  INIT_MOTOR_SEQUENCE_DONE,

  /** @note currently not implemented!
   * Save the current estimated motor params (direction, electricalOffset, electricalRotationsPerRevolution).
   * Therefore the current motor parameters have to be valid (have to be set before with from INIT_MOTOR_SEQUENCE).
   * data=[]
   */
  SAVE_CURRENT_MOTOR_CONFIG,


  ENABLE_MOTOR,     /// data=[]
  DISABLE_MOTOR,    /// data=[]


  /**
   * if fieldType == CAN_PACKAGE_TYPE__SET_SETTINGS__FIELDS::VELOCITY_FILTER_TF  -> data=2bytes as 4bytes float
   * if fieldType == CAN_PACKAGE_TYPE__SET_SETTINGS__FIELDS::MOTOR_MAX_TORQUE    -> data=2bytes as unsigned int16  (value between 1 and 1000)
   */
  SET_SETTINGS,


  /**
   * if fieldType == CAN_PACKAGE_TYPE__SET_PD_VALUES__FIELDS::DESIRED_POS   -> data=4bytes as long
   * if fieldType == CAN_PACKAGE_TYPE__SET_PD_VALUES__FIELDS::GAIN_P        -> data=4bytes as float
   * if fieldType == CAN_PACKAGE_TYPE__SET_PD_VALUES__FIELDS::DESIRED_VEL   -> data=4bytes as float
   * if fieldType == CAN_PACKAGE_TYPE__SET_PD_VALUES__FIELDS::GAIN_D        -> data=4bytes as float
   */
  SET_PD_VALUES,

  /// data=1byte requested state fields (OR combination of CAN_PACKAGE_TYPE__MOTOR_STATE_REQUEST__REQUESTED_FIELDS)
  /// will send the requested motor states, e.g. current pos and velocity
  REQUEST_MOTOR_STATE,

  /**
   * if fieldType == CAN_PACKAGE_TYPE__MOTOR_STATE__FIELDS::POSITION   -> data = 4bytes as long
   * if fieldType == CAN_PACKAGE_TYPE__MOTOR_STATE__FIELDS::VELOCITY   -> data = 4bytes as float
   * if fieldType == CAN_PACKAGE_TYPE__MOTOR_STATE__FIELDS::TORQUE     -> data = 4bytes as float
   */
  MOTOR_STATE,


  /// data=[]
  REQUEST_SYSTEM_STATE,

  /**
   * data = [SystemStateData]
   *   ok = is error when ok is not 1
   */
  SYSTEM_STATE,



  CAN_PACKAGE_TYPE_LENGTH,
};

// subfields
constexpr uint8_t CAN_PACKAGE_TYPE__FIELDS__MAX_NUM = 8;

enum CAN_PACKAGE_TYPE__SET_SETTINGS__FIELDS {
    VELOCITY_FILTER_TF,
    MOTOR_MAX_TORQUE,
};

enum CAN_PACKAGE_TYPE__SET_PD_VALUES__FIELDS {
    DESIRED_POS,
    GAIN_P,
    DESIRED_VEL,
    GAIN_D
};


enum class CAN_PACKAGE_TYPE__MOTOR_STATE_REQUEST__REQUESTED_FIELDS {
    POSITION_AND_VELOCITY = 0b0000'0001,
    TORQUE   = 0b0000'0100
};

enum CAN_PACKAGE_TYPE__MOTOR_STATE__FIELDS {
    POSITION,
    VELOCITY,
    TORQUE
};

enum CAN_PACKAGE_TYPE__SYSTEM_STATE_ERROR {
    ENCODER_ERROR,
    ENCODER_MAGNET_WEAK
};

enum CAN_PACKAGE_TYPE__COMMUNICATION_ERROR {
    /// microcontroller is not fast enough to respond to all incoming can requests.
    CAN_REQUESTS_TO_FAST,

    /// microcontroller is not fast enough to progress all incoming can requests.
    CAN_RECEIVE_OVERFLOW,
};


struct SystemStateData {
    bool noError = true;
    bool motorEnabled = true;
    ENCODER_STATUS encoderStatus = ENCODER_STATUS::UNKNOWN;
    SYSTEM_CONTROL_STATE systemControlState = SYSTEM_CONTROL_STATE::DO_NOTHING;

    static SystemStateData fromData(uint8_t *data) {
      return SystemStateData{
        .noError = (data[0] & 0b1000'0000) > 0,
        .motorEnabled = (data[0] & 0b0100'0000) > 0,
        .encoderStatus = (ENCODER_STATUS)data[1],
        .systemControlState = (SYSTEM_CONTROL_STATE)data[2],
      };
    }

    void writeToData(uint8_t *data) const {
      data[0] = (((uint8_t) noError) << 7) | (((uint8_t)motorEnabled) << 6);
      data[1] = (uint8_t)encoderStatus;
      data[2] = (uint8_t)systemControlState;
    }
};



/// the data is encoded as little endian
///  -> to also e.g. when int is in dataCarried it is also encoded as little endian
struct CanPackage {
    CAN_PACKAGE_TYPE canPackageType; // 4 bit

    /// the available fields depend on the canPackageType
    /// max 2^3=8 different field types
    uint8_t fieldType = 0; // 3 bit

    /// only relevant for packageTypes that can have different fieldTypes.
    ///  -> only when apply is true all previously send values (and this one) will be applied.
    /// e.g. useful for SET_PD_VALUES, to apply all different send PD values at once.
    bool apply = true; // 1 bit

    uint8_t *dataAll;
    uint8_t *dataCarried;
    uint8_t  dataCarriedLength;


    /// write and read contained data as long
    /// @todo: handle endianness
    bool &dataAsBool() {
      return *((bool *) dataCarried);
    }

    /// write and read contained data as unsigned int
    /// @todo: handle endianness
    uint16_t &dataAsUInt16() {
      return *((uint16_t *) dataCarried);
    }

    /// write and read contained data as long
    /// @todo: handle endianness
    int32_t &dataAsLong() {
      return *((int32_t *) dataCarried);
    }

    /// write and read contained data as float
    /// @todo: handle endianness
    float &dataAsFloat() {
      return *((float *) dataCarried);
    }

    /// write and read contained data as int32
    /// @todo: handle endianness
    int32_t &dataAsInt32() {
      return *((int32_t *) dataCarried);
    }



    /// required dataLength for the different packaged types
    static inline uint8_t requiredPackageDataCarriedLength(CAN_PACKAGE_TYPE type) {
      switch (type) {
        case CAN_PACKAGE_TYPE::SETUP:                 return 0;
        case CAN_PACKAGE_TYPE::SETUP_RESPONSE:        return 2;
        case CAN_PACKAGE_TYPE::SET_SETTINGS:          return 4;
        case CAN_PACKAGE_TYPE::INIT_MOTOR_SEQUENCE:   return 0;
        case CAN_PACKAGE_TYPE::ENABLE_MOTOR:          return 0;
        case CAN_PACKAGE_TYPE::DISABLE_MOTOR:         return 0;
        case CAN_PACKAGE_TYPE::SET_PD_VALUES:         return 4;
        case CAN_PACKAGE_TYPE::REQUEST_MOTOR_STATE:   return 1;
        case CAN_PACKAGE_TYPE::MOTOR_STATE:           return 4;
        case CAN_PACKAGE_TYPE::SYSTEM_STATE:          return 3;
        case CAN_PACKAGE_TYPE::COMMUNICATION_ERROR:   return 0;
        case CAN_PACKAGE_TYPE::REBOOT:                return 0;
        case CAN_PACKAGE_TYPE::INIT_MOTOR_SEQUENCE_DONE:  return 4;
        case CAN_PACKAGE_TYPE::SAVE_CURRENT_MOTOR_CONFIG: return 0;
        case CAN_PACKAGE_TYPE::REQUEST_SYSTEM_STATE:  return 0;
      }
      return 0;
    }

    /// number of different field types for a given package type
    static inline uint8_t numFieldTypes(CAN_PACKAGE_TYPE type) {
      switch (type) {
        case CAN_PACKAGE_TYPE::SETUP:                 return 1;
        case CAN_PACKAGE_TYPE::SETUP_RESPONSE:        return 1;
        case CAN_PACKAGE_TYPE::SET_SETTINGS:          return 2;
        case CAN_PACKAGE_TYPE::INIT_MOTOR_SEQUENCE:   return 1;
        case CAN_PACKAGE_TYPE::ENABLE_MOTOR:          return 1;
        case CAN_PACKAGE_TYPE::DISABLE_MOTOR:         return 1;
        case CAN_PACKAGE_TYPE::SET_PD_VALUES:         return 4;
        case CAN_PACKAGE_TYPE::REQUEST_MOTOR_STATE:   return 1;
        case CAN_PACKAGE_TYPE::MOTOR_STATE:           return 3;
        case CAN_PACKAGE_TYPE::SYSTEM_STATE:          return 1;
        case CAN_PACKAGE_TYPE::COMMUNICATION_ERROR:   return 2;
      }
      return 0;
    }


    /**
     * Set CanPackage values from bytes
     * @return true if successful
     */
    bool setFromBytes(uint8_t *bytes, uint8_t bytesLength) {
      this->canPackageType = static_cast<CAN_PACKAGE_TYPE>((bytes[0] & 0b11110000) >> 4);
      this->fieldType = (bytes[0] & 0b00001110) >> 1;
      this->apply = bytes[0] & 0x0000'0001;
      this->dataAll = bytes;
      this->dataCarried = &bytes[1];
      this->dataCarriedLength = bytesLength - 1;

      // check required data length
      // @todo: for now size has just to be large enough, not exactly the same, due to can2upd allways transmitting 8 bytes
      //return requiredPackageDataCarriedLength(this->canPackageType) == this->dataCarriedLength;
      return requiredPackageDataCarriedLength(this->canPackageType) <= this->dataCarriedLength;
      return true;
    }

    /**
     * write first byte of dataAll according to canPackageType and fieldType.
     */
    void writeHeaderToDataAll() {
      this->dataAll[0] =
          ((static_cast<int>(canPackageType) << 4 ) & 0b11110000) |
          ((fieldType << 1) & 0b00001110) |
          (apply  & 0x0000'0001);
    }
};