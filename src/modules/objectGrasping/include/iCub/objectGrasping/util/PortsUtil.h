#ifndef __ICUB_OBJECTGRASPING_PORTSUTIL_H__
#define __ICUB_OBJECTGRASPING_PORTSUTIL_H__

#include "iCub/objectGrasping/util/RPCCommandsUtil.h";

#include <yarp/os/BufferedPort.h>
#include <yarp/os/ResourceFinder.h>
#include <yarp/os/Bottle.h>
#include <yarp/sig/Vector.h>
#include <yarp/os/RpcClient.h>

namespace iCub {
    namespace objectGrasping {

        class PortsUtil {

        private:

            /* ******* Ports			                ******* */
            yarp::os::BufferedPort<yarp::sig::Vector> portSkinCompIn;
            yarp::os::BufferedPort<yarp::os::Bottle> portLogDataOut;

            yarp::os::RpcClient portOutgoingCommandsRPC;
            yarp::os::RpcClient portOTFROutRPC;
            yarp::os::RpcClient portAREOutRPC;

            iCub::objectGrasping::RPCCommandsUtil *rpcCmdUtil;

            /* ******* Debug attributes.                ******* */
            std::string dbgTag;

        public:

            PortsUtil();

            bool init(yarp::os::ResourceFinder &rf, iCub::objectGrasping::RPCCommandsUtil *rpcCmdUtil);

            bool interrupt();

            bool release();

            void sendCommand(iCub::objectGrasping::YarpPort yarpPort, std::string command);
            void sendCommand(iCub::objectGrasping::YarpPort yarpPort, std::string command, double value);
            void askWhichObject(yarp::os::Bottle &returnMessage);

        private:

            void writeMessage(iCub::objectGrasping::YarpPort yarpPort, yarp::os::Bottle &message);

        };
    } //namespace objectGrasping
} //namespace iCub

#endif

