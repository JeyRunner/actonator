#pragma once

#include <stdint.h>
#include <iostream>
#include <magic_enum.hpp>
#include <fmt/core.h>
#include "vector"
#include "string"
#include "functional"
#include "fmt/format.h"
#include "Types.h"

#include <communication/SOTMaster.h>
#include <linux_socketCan/SocketCanInterface.hpp>
#include "protocol_generated/MotorControllerCanProtocol.hpp"

using namespace std;
using namespace fmt;
using namespace MotorControllerCanProtocolNamespace;



/**
 *
 */
class MotorDriverCanMaster {
  private:

  public:
    SocketCanInterface canInterface;
    SOTMaster<MotorControllerCanProtocolNamespace::MotorControllerCanProtocol, SocketCanInterface> canMaster;

    explicit MotorDriverCanMaster(string canInterfaceName)
    : canInterface(canInterfaceName), canMaster(canInterface)
    {}

    /**
     * Start the can interface.
     * @return
     */
    bool startCanInterface() {
      return canInterface.startCanInterface();
    }


private:
    optional<unsigned int> FULL_ROTATION_VALUE = nullopt;

};

