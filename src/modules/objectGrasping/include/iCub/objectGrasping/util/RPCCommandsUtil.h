#ifndef __ICUB_OBJECTGRASPING_RPCCOMMANDSUTIL_H__
#define __ICUB_OBJECTGRASPING_RPCCOMMANDSUTIL_H__

#include "iCub/objectGrasping/ObjectGraspingEnums.h"
#include "iCub/objectGrasping/data/RPCCommandsData.h"

#include <yarp/os/Bottle.h>
#include <yarp/os/Value.h>

#include <map>
#include <string>

namespace iCub {
    namespace objectGrasping {

        class RPCCommandsUtil {

            private:

				iCub::objectGrasping::RPCCommandsData *rpcData;

				/* ****** Debug attributes                              ****** */
                std::string dbgTag;

            public:

				iCub::objectGrasping::RPCMainCmdName mainCmd;
				iCub::objectGrasping::RPCSetCmdArgName setCmdArg;
				iCub::objectGrasping::RPCTaskCmdArgName taskCmdArg;
				iCub::objectGrasping::RPCViewCmdArgName viewCmdArg;
				iCub::objectGrasping::TaskName task;

				yarp::os::Value argValue;


				RPCCommandsUtil();

				void init(iCub::objectGrasping::RPCCommandsData *rpcData);

				void processCommand(const yarp::os::Bottle &rpcCmdBottle);

				void createBottleMessage(std::string command,yarp::os::Bottle &message);

				void createBottleMessage(std::string command,double value,yarp::os::Bottle &message);

			private:

				void processTaskCommand(const yarp::os::Bottle &rpcCmdBottle);

};
    } //namespace objectGrasping
} //namespace iCub

#endif

