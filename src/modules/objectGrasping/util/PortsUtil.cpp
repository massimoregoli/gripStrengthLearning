#include "iCub/objectGrasping/util/PortsUtil.h"

#include <yarp/os/Network.h>
#include <yarp/os/Value.h>

using std::string;

using yarp::os::Value;

using iCub::objectGrasping::PortsUtil;

PortsUtil::PortsUtil(){

    dbgTag = "PortsUtil: ";
}

bool PortsUtil::init(yarp::os::ResourceFinder &rf, iCub::objectGrasping::RPCCommandsUtil *rpcCmdUtil){
    using yarp::os::Network;
    using std::cout;

    this->rpcCmdUtil = rpcCmdUtil;

//    string whichHand = rf.check("whichHand", Value("right")).asString().c_str();
//    string moduleSkinCompPortName = "/ObjectGrasping/skin/" + whichHand + "_hand_comp:i";
//    string icubSkinCompPortName = "/icub/skin/" + whichHand + "_hand_comp";
//    string logDataPortName = "/ObjectGrasping/log:o";

    string outgoingCommandsRPCPortName = "/objectGrasping/graspTaskCmd:o";
    string otfrOutRPCPortName = "/objectGrasping/otfrCommand:o";
    string areOutRPCPortName = "/objectGrasping/areCommand:o";

    string stableGraspInRPCPortName = "/tactileControlWrapper/cmd:i"; // TO VERIFY!
    string otfrInRPCPortName = "/onthefly_recognition/human:io";
    string areInRPCPortName = "/actionsRenderingEngine/cmd:io";

    // opening ports
//    if (!portSkinCompIn.open(moduleSkinCompPortName)){
//        cout << dbgTag << "could not open " << moduleSkinCompPortName << " port \n";
//        return false;
//    }
//    if (!portLogDataOut.open(logDataPortName)){
//        cout << dbgTag << "could not open " << logDataPortName << " port \n";
//        return false;
//    }
    if (!portOutgoingCommandsRPC.open(outgoingCommandsRPCPortName)){
        cout << dbgTag << "could not open " << outgoingCommandsRPCPortName << " port \n";
        return false;
    }
    if (!portOTFROutRPC.open(otfrOutRPCPortName)){
        cout << dbgTag << "could not open " << otfrOutRPCPortName << " port \n";
        return false;
    }
    if (!portAREOutRPC.open(areOutRPCPortName)){
        cout << dbgTag << "could not open " << areOutRPCPortName << " port \n";
        return false;
    }

    // connecting ports
//    if (!Network::connect(icubSkinCompPortName, moduleSkinCompPortName)){
//        cout << dbgTag << "could not connect ports: " << icubSkinCompPortName << " -> " << moduleSkinCompPortName << "\n";
//        return false;
//    }
    if (!Network::connect(areOutRPCPortName, areInRPCPortName)){
        cout << dbgTag << "could not connect ports: " << areOutRPCPortName << " -> " << areInRPCPortName << "\n";
        return false;
    }
    if (!Network::connect(otfrOutRPCPortName, otfrInRPCPortName)){
        cout << dbgTag << "could not connect ports: " << otfrOutRPCPortName << " -> " << otfrInRPCPortName << "\n";
        return false;
    }
    if (!Network::connect(outgoingCommandsRPCPortName, stableGraspInRPCPortName)){
        cout << dbgTag << "could not connect ports: " << outgoingCommandsRPCPortName << " -> " << stableGraspInRPCPortName << "\n";
        return false;
    }


    return true;
}

bool PortsUtil::release(){

    portOutgoingCommandsRPC.close();
    portOTFROutRPC.close();
    portAREOutRPC.close();

    return true;
}

bool PortsUtil::interrupt(){

    portOutgoingCommandsRPC.interrupt();
    portOTFROutRPC.interrupt();
    portAREOutRPC.interrupt();

    return true;
}

void PortsUtil::sendCommand(iCub::objectGrasping::YarpPort yarpPort, std::string command){

    yarp::os::Bottle message;

    rpcCmdUtil->createBottleMessage(command, message);

    writeMessage(yarpPort, message);

}

void PortsUtil::sendCommand(iCub::objectGrasping::YarpPort yarpPort, std::string command, double value){

    yarp::os::Bottle message;

    rpcCmdUtil->createBottleMessage(command, value, message);

    writeMessage(yarpPort, message);

}

void PortsUtil::writeMessage(iCub::objectGrasping::YarpPort yarpPort, yarp::os::Bottle &message){


    switch (yarpPort){

    case STABLE_GRASP:
        portOutgoingCommandsRPC.write(message);
        break;

    case OTFR:
        portOTFROutRPC.write(message);
        break;

    case ARE:
        portAREOutRPC.write(message);
        break;

    }

}

void PortsUtil::askWhichObject(yarp::os::Bottle &returnMessage){

    yarp::os::Bottle message;

    rpcCmdUtil->createBottleMessage("what 0", message);

    portOTFROutRPC.write(message,returnMessage);

}