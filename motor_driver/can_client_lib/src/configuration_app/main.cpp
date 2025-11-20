#include <MotorDriverCanMaster.hpp>
#include <unistd.h>


#include <lyra/lyra.hpp>
using namespace lyra;



// args
string args_canInterface = "can0";
unsigned int args_clientId;
optional<float> args_gainP = nullopt;
optional<float> args_gainD = nullopt;
optional<float> args_velocityFilterTF = nullopt;


void parseArgs(int argc, const char **argv) {
  auto cli = lyra::cli();
  bool showHelp = false;
  cli.add_argument(help(showHelp));
  cli.add_argument(lyra::opt(args_canInterface, "can-interface" )["-i"]["--can-interface"]("The can interface to connect to"));
  cli.add_argument(lyra::opt(args_clientId, "client-id" )["--client-id"]("The can sot ID of the motordriver to connect to").required(true));
  cli.add_argument(lyra::opt(args_gainP, "Gain P")["-p"]["--set-gain-p"]("Set the proportional gain of the PD controller").optional());
  cli.add_argument(lyra::opt(args_gainD, "Gain D")["-d"]["--set-gain-d"]("Set the derivative gain of the PD controller").optional());
  cli.add_argument(lyra::opt(args_velocityFilterTF, "TF")["-f"]["--set-velocity-filter-tf"]("Set the time constant value of the velocity filter").optional());

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
}


int main(int argc, const char **argv) {
  parseArgs(argc, argv);

  MotorDriverCanMaster master(args_canInterface);
  if (!master.startCanInterface()) {
    exit(1);
  }
  cout << "connected to " << args_canInterface << "" << " can interface" << endl;
  master.canMaster.addAndConnectToClient(args_clientId);
  auto &client = master.canMaster.getClient(args_clientId);


  cout << "will connect to client " << endl;
  int i = 0;
  while (!client.isConnected()) {
    master.canMaster.processCanFrames();
    usleep(1000);
    i++;
    if (i>= 1000) {
      cout <<"Error: connecting to client timeout" << endl;
      exit(1);
    }
  }
  cout << "connected to client with id " << args_clientId << endl;


  if (args_gainP || args_gainD) {
    cout << "will set pd gains ..." << endl;
    cout << "Error -> Not implemented yet" << endl;
    exit(1);
    //bool ok = driverCanClient.sendSetPD(nullopt, nullopt, args_gainP, args_gainD);
    //cout << "set pd gains -> " << (ok ? "done" : "failed") << endl;
  }
  if (args_velocityFilterTF) {
    cout << "will set velocityFilterTF ..." << endl;
    client.protocol.objectTree.settings.velocity_filter.smoothingTf.write(args_velocityFilterTF.value()).sendValue();
    cout << "set velocityFilterTF -> done " << endl;
  }
}