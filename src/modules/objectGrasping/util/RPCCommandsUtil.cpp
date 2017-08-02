#include "iCub/objectGrasping/util/RPCCommandsUtil.h"

#include <string.h>

#include <vector>

using iCub::objectGrasping::RPCCommandsUtil;
using iCub::objectGrasping::RPCCommandsData;
using iCub::objectGrasping::RPCMainCmdName;
using iCub::objectGrasping::RPCSetCmdArgName;
using iCub::objectGrasping::RPCTaskCmdArgName;
using iCub::objectGrasping::RPCViewCmdArgName;
using iCub::objectGrasping::TaskName;

using yarp::os::Bottle;

using std::string;
using std::pair;

RPCCommandsUtil::RPCCommandsUtil(){}

void RPCCommandsUtil::init(RPCCommandsData *rpcData){

    this->rpcData = rpcData;

    dbgTag = "RPCCommandsUtil: ";
}

void RPCCommandsUtil::processCommand(const Bottle &rpcCmdBottle){

    mainCmd = rpcData->mainCmdRevMap[rpcCmdBottle.get(0).asString()];

    switch (mainCmd){

    case DEMO: // do nothing
        break;
    case HOME: // do nothing
        break;
    case HELP: // do nothing
        break;
    case SET:
        setCmdArg = rpcData->setCmdArgRevMap[rpcCmdBottle.get(1).asString()];
        argValue = rpcCmdBottle.get(2);
        break;
    case TASK:
        processTaskCommand(rpcCmdBottle);
        break;
    case VIEW:
        viewCmdArg = rpcData->viewCmdArgRevMap[rpcCmdBottle.get(1).asString()];
        break;
    case START: // do nothing
        break;
    case STOP: // do nothing
        break;
    case QUIT: // do nothing
        break;

    };

}

void RPCCommandsUtil::processTaskCommand(const Bottle &rpcCmdBottle){

    taskCmdArg = rpcData->taskCmdArgRevMap[rpcCmdBottle.get(1).asString()];
    switch (taskCmdArg){

    case ADD:
        task = rpcData->taskRevMap[rpcCmdBottle.get(2).asString()];
        argValue = rpcCmdBottle.get(3);
        break;
    case EMPTY: // do nothing
        break;
    case POP: // do nothing
        break;
    }
}

void RPCCommandsUtil::createBottleMessage(string command, Bottle &message){

    message.clear();

    if (!command.empty()){

        char *commandChar = new char[command.length() + 1];
        strcpy(commandChar, command.c_str());
        std::vector<string> wordList;
        char *target;

        target = strtok(commandChar, " ");
        while (target != NULL){
            wordList.push_back(target);
            target = strtok(NULL, " ");
        }

        for (size_t i = 0; i < wordList.size(); i++){
            message.add(yarp::os::Value(wordList[i]));
        }

    }
}

void RPCCommandsUtil::createBottleMessage(string command, double value, Bottle &message){

    createBottleMessage(command, message);

    message.add(yarp::os::Value(value));

}


