#include "iCub/objectGrasping/ObjectGraspingModule.h" 

#include <yarp/os/Network.h>
#include <yarp/dev/Drivers.h>

using yarp::os::Network;
using yarp::os::ResourceFinder;

//YARP_DECLARE_DEVICES(icubmod);

int main(int argc, char * argv[])
{
    /*initializing device driver list */
//    YARP_REGISTER_DEVICES(icubmod);
    
    /* initializing yarp network */ 
    Network yarp;
    if (!yarp.checkNetwork()) {
        std::cerr << "Error: yarp server is not available. \n";
        return -1;
    }

    iCub::objectGrasping::ObjectGraspingModule objectGrasping;

    /* preparing and configuring the resource finder */
    ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultConfigFile("confObjectGrasping.ini");
    rf.setDefaultContext("objectGrasping");
    rf.configure(argc, argv);

    /* starting module */
    objectGrasping.runModule(rf);

    return 0;
}
