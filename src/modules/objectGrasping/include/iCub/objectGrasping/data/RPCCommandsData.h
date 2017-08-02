#ifndef __ICUB_OBJECTGRASPING_RPCCOMMANDSDATA_H__
#define __ICUB_OBJECTGRASPING_RPCCOMMANDSDATA_H__

#include "iCub/objectGrasping/ObjectGraspingEnums.h"

#include <map>
#include <string>

namespace iCub {
    namespace objectGrasping {

        class RPCCommandsData {

            private:

				/* ****** Debug attributes                              ****** */
                std::string dbgTag;

            public:

				std::map<iCub::objectGrasping::RPCMainCmdName,std::string> mainCmdMap;
				std::map<iCub::objectGrasping::RPCMainCmdName,std::string> mainCmdDescMap;
				std::map<std::string,iCub::objectGrasping::RPCMainCmdName> mainCmdRevMap;

				std::map<iCub::objectGrasping::RPCSetCmdArgName,std::string> setCmdArgMap;
				std::map<iCub::objectGrasping::RPCSetCmdArgName,std::string> setCmdArgDescMap;
				std::map<std::string,iCub::objectGrasping::RPCSetCmdArgName> setCmdArgRevMap;

				std::map<iCub::objectGrasping::RPCTaskCmdArgName,std::string> taskCmdArgMap;
				std::map<iCub::objectGrasping::RPCTaskCmdArgName,std::string> taskCmdArgDescMap;
				std::map<std::string,iCub::objectGrasping::RPCTaskCmdArgName> taskCmdArgRevMap;

				std::map<iCub::objectGrasping::RPCViewCmdArgName,std::string> viewCmdArgMap;
				std::map<iCub::objectGrasping::RPCViewCmdArgName,std::string> viewCmdArgDescMap;
				std::map<std::string,iCub::objectGrasping::RPCViewCmdArgName> viewCmdArgRevMap;

				std::map<iCub::objectGrasping::TaskName,std::string> taskMap;
				std::map<iCub::objectGrasping::TaskName,std::string> taskDescMap;
				std::map<std::string,iCub::objectGrasping::TaskName> taskRevMap;


				RPCCommandsData();

				std::string getFullDescription(iCub::objectGrasping::RPCSetCmdArgName setCmdArgName);

				std::string getFullDescription(iCub::objectGrasping::RPCMainCmdName mainCmdName);

			private:

				void add(std::string mapKey,iCub::objectGrasping::RPCMainCmdName mapValue,std::string description);

				void add(std::string mapKey,iCub::objectGrasping::RPCSetCmdArgName mapValue,std::string description);

				void add(std::string mapKey,iCub::objectGrasping::RPCTaskCmdArgName mapValue,std::string description);

				void add(std::string mapKey,iCub::objectGrasping::RPCViewCmdArgName mapValue,std::string description);

				void add(std::string mapKey,iCub::objectGrasping::TaskName mapValue,std::string description);

				
};
    } //namespace objectGrasping
} //namespace iCub

#endif

