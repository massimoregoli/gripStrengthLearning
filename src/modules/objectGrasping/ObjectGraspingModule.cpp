#include "iCub/objectGrasping/ObjectGraspingModule.h"

#include <yarp/os/Time.h>

#include <sstream>

using iCub::objectGrasping::ObjectGraspingModule;

using std::cout;

using yarp::os::ResourceFinder;
using yarp::os::Value;


/* *********************************************************************************************************************** */
/* ******* Constructor                                                      ********************************************** */
ObjectGraspingModule::ObjectGraspingModule()
    : RFModule() {
    closing = false;

    dbgTag = "ObjectGraspingModule: ";
}
/* *********************************************************************************************************************** */


/* *********************************************************************************************************************** */
/* ******* Destructor                                                       ********************************************** */
ObjectGraspingModule::~ObjectGraspingModule() {}
/* *********************************************************************************************************************** */


/* *********************************************************************************************************************** */
/* ******* Get Period                                                       ********************************************** */
double ObjectGraspingModule::getPeriod() { return period; }
/* *********************************************************************************************************************** */


/* *********************************************************************************************************************** */
/* ******* Configure module                                                 ********************************************** */
bool ObjectGraspingModule::configure(ResourceFinder &rf) {
    using std::string;
    using yarp::os::Property;

    cout << dbgTag << "Starting. \n";

    /* ****** Configure the Module                            ****** */
    moduleName = rf.check("name", Value("objectGrasping")).asString().c_str();
    period = rf.check("period", 1.0).asDouble();

    /* ******* Open ports                                       ******* */
    portIncomingCommandsRPC.open("/objectGrasping/incomingCmd:i");
    portOutgoingCommandsRPC.open("/objectGrasping/graspTaskCmd:o");
    attach(portIncomingCommandsRPC);

    rpcCmdUtil.init(&rpcCmdData);

    configData = new ConfigData(rf);

    // initialize controllers
    controllersUtil = new ControllersUtil();
    if (!controllersUtil->init(rf)) {
        cout << dbgTag << "failed to initialize controllers utility\n";
        return false;
    }

    // save current arm position, to be restored when the thread ends
    if (!controllersUtil->saveCurrentArmPosition()) {
        cout << dbgTag << "failed to store current arm position\n";
        return false;
    }


    complianceEnabled = true;
    taskState = SET_ARM_IN_START_POSITION;
    stepCounter = 0;
    maxXStep = 1;
    maxYStep = 9;
    maxCounter = maxXStep * maxYStep;

    if (complianceEnabled) controllersUtil->saveCurrentStiffness();

    if (complianceEnabled) controllersUtil->setStiffness();

    //controllersUtil->enableTorsoJoints();

    cout << dbgTag << "Started correctly. \n";

    return true;
}
/* *********************************************************************************************************************** */


/* *********************************************************************************************************************** */
/* ******* Update    module                                                 ********************************************** */
bool ObjectGraspingModule::updateModule() {

    std::stringstream cmd;

    switch (taskState){

    case SET_ARM_IN_START_POSITION:
        if (controllersUtil->setArmInStartPosition(configData->cartesianMode)) {
            taskState = WAIT;
        }
        else{
            cout << dbgTag << "failed to set the arm in start position\n";
            return false;
        }
        break;

    case WAIT:
        // do nothing
        break;

    case EXECUTE_EXPLORATION:

        if (stepCounter == 0){
            controllersUtil->saveCurrentPose();
        }

        int yStep = stepCounter%maxYStep;
        int xStep = stepCounter / maxYStep;
        if (xStep % 2 == 1){
            yStep = maxYStep - 1 - yStep;
        }



        std::cout << "step " << stepCounter << " - going to (" << yStep << " " << xStep << ")" << std::endl;
        
        controllersUtil->goToXY(xStep, yStep);

        controllersUtil->goDown();

        controllersUtil->goToXY(xStep, yStep);

        // total waiting time = goToXYTrajTime + 1 + goDownTrajTime + waitDown = 7

        stepCounter++;

        if (stepCounter == maxCounter){

            std::cout << "EXPLORATION COMPLETED" << std::endl;

            taskState = WAIT;
            stepCounter = 0;
        }

        break;

    }

    return !closing;
}
/* *********************************************************************************************************************** */


/* *********************************************************************************************************************** */
/* ******* Interrupt module                                                 ********************************************** */
bool ObjectGraspingModule::interruptModule() {
    cout << dbgTag << "Interrupting. \n";

    // Interrupt port
    portIncomingCommandsRPC.interrupt();
    portOutgoingCommandsRPC.interrupt();

    cout << dbgTag << "Interrupted correctly. \n";

    return true;
}
/* *********************************************************************************************************************** */

/* *********************************************************************************************************************** */
/* ******* Manage commands coming from RPC Port                             ********************************************** */
bool ObjectGraspingModule::respond(const yarp::os::Bottle& command, yarp::os::Bottle& reply){

    rpcCmdUtil.processCommand(command);

    switch (rpcCmdUtil.mainCmd){

    case DEMO:
        demo();
        break;
    case HOME:
        home();
        break;
    case HELP:
        help();
        break;
    case SET:
        set(rpcCmdUtil.setCmdArg, rpcCmdUtil.argValue);
        break;
    case TASK:
        task(rpcCmdUtil.taskCmdArg, rpcCmdUtil.task, rpcCmdUtil.argValue);
        break;
    case VIEW:
        view(rpcCmdUtil.viewCmdArg);
        break;
    case START:
        start();
        break;
    case STOP:
        stop();
        break;
    case QUIT:
        quit();
        break;
    }

    return true;
}

/* *********************************************************************************************************************** */
/* ******* Close module                                                     ********************************************** */
bool ObjectGraspingModule::close() {
    cout << dbgTag << "Closing. \n";

    //controllersUtil->disableTorsoJoints();
    if (complianceEnabled) controllersUtil->restoreStiffness();
    controllersUtil->restorePreviousArmPosition();

    controllersUtil->release();

    delete(controllersUtil);

    // Close port
    portIncomingCommandsRPC.close();
    portOutgoingCommandsRPC.close();

    cout << dbgTag << "Closed. \n";

    return true;
}
/* *********************************************************************************************************************** */


/* *********************************************************************************************************************** */
/* ******* RPC Open hand                                                    ********************************************** */
bool ObjectGraspingModule::stop() {

    return true;
}
/* *********************************************************************************************************************** */


/* *********************************************************************************************************************** */
/* ******* RPC Grasp object                                                 ********************************************** */
bool ObjectGraspingModule::start() {

    return true;
}
/* *********************************************************************************************************************** */

bool ObjectGraspingModule::demo() {

    yarp::os::Time::delay(3);
    taskState = EXECUTE_EXPLORATION;

    return true;
}

bool ObjectGraspingModule::home() {

    taskState = SET_ARM_IN_START_POSITION;

    return true;
}

/* *********************************************************************************************************************** */
/* ******* RPC Quit module                                                  ********************************************** */
bool ObjectGraspingModule::quit() {

    return closing = true;
}
/* *********************************************************************************************************************** */


void ObjectGraspingModule::set(iCub::objectGrasping::RPCSetCmdArgName paramName, Value paramValue){
    //	taskThread->set(paramName,paramValue,rpcCmdData);
    //	view(SETTINGS);
}

void ObjectGraspingModule::task(iCub::objectGrasping::RPCTaskCmdArgName paramName, iCub::objectGrasping::TaskName taskName, Value paramValue){
    //	taskThread->task(paramName,taskName,paramValue,rpcCmdData);
    //	view(TASKS);
}

void ObjectGraspingModule::view(iCub::objectGrasping::RPCViewCmdArgName paramName){
    //	taskThread->view(paramName,rpcCmdData);
}

void ObjectGraspingModule::help(){
    //	taskThread->help(rpcCmdData);
}



void ObjectGraspingModule::sendCommand(std::string command){

    yarp::os::Bottle message;

    rpcCmdUtil.createBottleMessage(command, message);

    portOutgoingCommandsRPC.write(message);

}

void ObjectGraspingModule::sendCommand(std::string command, double value){

    yarp::os::Bottle message;

    rpcCmdUtil.createBottleMessage(command, value, message);

    portOutgoingCommandsRPC.write(message);

}
