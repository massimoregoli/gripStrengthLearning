#ifndef __OBJECTGRASPING_MODULE_H__
#define __OBJECTGRASPING_MODULE_H__

#include "iCub/objectGrasping/util/ControllersUtil.h"
#include "iCub/objectGrasping/util/RPCCommandsUtil.h"
#include "iCub/objectGrasping/util/PortsUtil.h"
#include "iCub/objectGrasping/data/RPCCommandsData.h"
#include "iCub/objectGrasping/data/ConfigData.h"
#include "iCub/objectGrasping/ObjectGraspingEnums.h"

#include <string>

#include <yarp/os/RFModule.h>
#include <yarp/os/RpcServer.h>
#include <yarp/os/RpcClient.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Value.h>

namespace iCub {
    namespace objectGrasping {

        class ObjectGraspingModule : public yarp::os::RFModule {

        private:

            /* ****** Module attributes                             ****** */
            double period;
            std::string moduleName;
            std::string robotName;
            bool closing;
            TaskState taskState;
            int stepCounter;

            /* ****** RPC Ports                                     ****** */
            yarp::os::RpcServer portIncomingCommandsRPC;

            iCub::objectGrasping::RPCCommandsUtil rpcCmdUtil;
            iCub::objectGrasping::RPCCommandsData rpcCmdData;

            /* ******* Controllers utility                          ******* */
            iCub::objectGrasping::ControllersUtil *controllersUtil;

            /* ******* Ports utility                          ******* */
            iCub::objectGrasping::PortsUtil *portsUtil;

            /* ******* Config Data                            ******* */
            ConfigData *configData;

            /* ****** Debug attributes                              ****** */
            std::string dbgTag;

        public:

            ObjectGraspingModule();
            virtual ~ObjectGraspingModule();
            virtual double getPeriod();
            virtual bool configure(yarp::os::ResourceFinder &rf);
            virtual bool updateModule();
            virtual bool interruptModule();
            virtual bool respond(const yarp::os::Bottle &command, yarp::os::Bottle &reply);
            virtual bool close();

            /* ****** RPC Methods                                  ****** */
            bool start();
            bool demo();
            bool home();
            bool stop();
            bool quit();
            void set(iCub::objectGrasping::RPCSetCmdArgName paramName, yarp::os::Value paramValue);
            void task(iCub::objectGrasping::RPCTaskCmdArgName paramName, iCub::objectGrasping::TaskName taskName, yarp::os::Value paramValue);
            void view(iCub::objectGrasping::RPCViewCmdArgName paramName);
            void help();

        };
    }
}

#endif

