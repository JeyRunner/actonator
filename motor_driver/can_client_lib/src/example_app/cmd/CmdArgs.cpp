#include "CmdArgs.h"
#include <lyra/lyra.hpp>
using namespace lyra;


CmdArgs CmdArgs::parseArgs(int argc, const char **argv) {
  CmdArgs cmdArgs;

  auto cli = lyra::cli();
  bool showHelp = false;
  cli.add_argument(help(showHelp));
  cli.add_argument(lyra::opt(cmdArgs.canInterface, "can-interface" )["-i"]["--can-interface"]("The linux can interface to use"));
  cli.add_argument(lyra::opt(cmdArgs.clientId, "client-id" )["--client-id"]("The can sot ID of the motordriver to connect to").required(true));
  cli.add_argument(lyra::opt(cmdArgs.runMotorInitSequence)["-m"]["--init-motor"]("Run the Motor Init Sequence first"));
  cli.add_argument(lyra::opt(cmdArgs.forceMotorEnable)["--force-motor-enable"]("Directly enable the motor without any checks, WARNING: DANGEROUS TO USE"));
  cli.add_argument(lyra::opt(cmdArgs.outputCurrentMotorValues)["--output-current-motor-vals"]("Show all changes of the current values: position, velocity, torque"));
  cli.add_argument(lyra::opt(cmdArgs.loopDelayMs, "ms")["--loop-delay-ms"]("Time to wait each iteration of the main communication loop."));
  cli.add_argument(lyra::opt(cmdArgs.usePdValuesFromuC)["--use-pd-vals-from-uc"]("Use the pd values generated on the uC by a sin wave (send pd.pos will be ignored)."));
  cli.add_argument(lyra::opt(cmdArgs.pdSinWavelength, "s")["--pd-sin-lambda"]("Wavelength (s) of the sin wave that is send as pd target value."));
  cli.add_argument(lyra::opt(cmdArgs.pdSinWaveAmplitude, "a")["--pd-sin-amplitude"]("Wave Amplitude of the sin wave that is send as pd target value."));

  cli.add_argument(lyra::opt(cmdArgs.motor_max_throttle, "")["--throttle-limit"]
  ("Set max throttle of the motor (1^=100%), not there is an additional internal throttle limit in the uC."));

  cli.add_argument(lyra::opt(cmdArgs.vel_filter_smoothingTf, "")["--filter-vel-tf"]("Set the TF soothing value of the velocity filter."));
  cli.add_argument(lyra::opt(cmdArgs.gainP, "")["--gain-p"]("Set this gain (proportional) value for the pd controller on the uC."));
  cli.add_argument(lyra::opt(cmdArgs.gainD, "")["--gain-d"]("Set this gain (derivative) value for the pd controller on the uC."));

  // parse args
  auto cli_result = cli.parse({argc, argv});

  // Check that the arguments where valid:
  if (!cli_result){
    std::cerr << "Error in command line: " << cli_result.message() << std::endl;
    exit(1);
  }

  if (showHelp){
    std::cout << endl << cli << std::endl;
    exit(0);
  }

  return cmdArgs;
}

