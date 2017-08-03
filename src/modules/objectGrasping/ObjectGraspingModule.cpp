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
    period = rf.check("period", 0.5).asDouble();

    /* ******* Open ports                                       ******* */
    portIncomingCommandsRPC.open("/objectGrasping/incomingCmd:i");

    attach(portIncomingCommandsRPC);

    rpcCmdUtil.init(&rpcCmdData);

    configData = new ConfigData(rf);

    // initialize controllers
    controllersUtil = new ControllersUtil();
    if (!controllersUtil->init(rf)) {
        cout << dbgTag << "failed to initialize controllers utility\n";
        return false;
    }

    // initialize ports
    portsUtil = new PortsUtil();
    if (!portsUtil->init(rf,&rpcCmdUtil)) {
        cout << dbgTag << "failed to initialize ports utility\n";
        return false;
    }

    // save current arm position, to be restored when the thread ends
    if (!controllersUtil->saveCurrentArmPosition()) {
        cout << dbgTag << "failed to store current arm position\n";
        return false;
    }

    taskState = WAIT;
    stepCounter = 0;

    cout << dbgTag << "Started correctly. \n";

    return true;
}
/* *********************************************************************************************************************** */


/* *********************************************************************************************************************** */
/* ******* Update    module                                                 ********************************************** */
bool ObjectGraspingModule::updateModule() {

    std::stringstream cmd;
    std::stringstream gripStrengthCommand("");
    yarp::os::Bottle returnMessage;
    std::string objectName;

    switch (taskState){

    case WAIT:

        // do nothing

        break;

    case EXECUTE:

        // wait for the object to be ready
        yarp::os::Time::delay(1);

        // start tracking
        portsUtil->sendCommand(ARE, "track motion no_sacc");

        // wait for the classifier to stabilize the response
        yarp::os::Time::delay(3);

        // ask which is the object
        portsUtil->askWhichObject(returnMessage);
        objectName = returnMessage.get(1).asString();

        // retrieve grip_strength associated to <what>
        double gripStrength = 30;
        if (objectName == "book"){
            gripStrength = 95;
        }
        else if (objectName == "papercup"){
            gripStrength = 25;
        }
        gripStrengthCommand << "set control.high.grip_strength " << gripStrength;

        // set the grip strength
        portsUtil->sendCommand(STABLE_GRASP, gripStrengthCommand.str());

        // TODO say "ok, I will grasp the <what>" (optional)

        // look at the hand
//        portsUtil->sendCommand(ARE, "idle"); // CHECK IF IT IS NECESSARY
        portsUtil->sendCommand(ARE, "look hand left");

        // set the robot in task position
        controllersUtil->setArmInStartPosition(); // COPY THE HAND JOINTS

        // grasp the object
        portsUtil->sendCommand(STABLE_GRASP, "grasp");

        // wait a little while holding the object
        yarp::os::Time::delay(5);

        portsUtil->sendCommand(STABLE_GRASP, "open false");

        // wait a little while opening the hand
        yarp::os::Time::delay(1);

        portsUtil->sendCommand(ARE, "look (\"cartesian\" 2 0 0.4)");

        controllersUtil->restorePreviousArmPosition();

        taskState = TRANSITION;

        stepCounter++;

        break;


    case TRANSITION:

        if (stepCounter == 2){
            taskState = WAIT;
        }
        else {
            taskState = EXECUTE;
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
    portsUtil->interrupt();

    cout << dbgTag << "Interrupted correctly. \n";

    return true;
}
/* *********************************************************************************************************************** */

/* *********************************************************************************************************************** */
/* ******* Manage commands coming from RPC Port                             ********************************************** */
bool ObjectGraspingModule::respond(const yarp::os::Bottle &command, yarp::os::Bottle &reply){

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
    controllersUtil->restorePreviousArmPosition();

    controllersUtil->release();

    delete(controllersUtil);

    // Close port
    portIncomingCommandsRPC.close();
    portsUtil->release();

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

    taskState = EXECUTE;

    return true;
}

bool ObjectGraspingModule::home() {

//    taskState = SET_ARM_IN_START_POSITION;

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



