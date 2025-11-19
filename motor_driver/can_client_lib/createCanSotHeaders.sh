CAN_SOT_CLI_EXEC_FOLDER=$@
echo "using CAN_SOT_CLI_EXEC_FOLDER=${CAN_SOT_CLI_EXEC_FOLDER}"

# update the def file
${CAN_SOT_CLI_EXEC_FOLDER}canSotCli genProtocolSpec -f motorControllerCanProtocol.spec.yaml -o motorControllerCanProtocol.def.yaml

# gen header files
${CAN_SOT_CLI_EXEC_FOLDER}canSotCli genCode -f motorControllerCanProtocol.def.yaml -o src/can_master_lib/protocol_generated/ --master
${CAN_SOT_CLI_EXEC_FOLDER}canSotCli genCode -f motorControllerCanProtocol.def.yaml -o ../firmware/src/canSot/protocol_generated/ --client
