#include "iCub/objectGrasping/data/ConfigData.h"

#include "iCub/objectGrasping/ObjectGraspingEnums.h"

#include <yarp/os/Value.h>

using yarp::os::Value;

using iCub::objectGrasping::ConfigData;


ConfigData::ConfigData(yarp::os::ResourceFinder &rf) {
	using iCub::objectGrasping::ControlTaskOpMode;
	using yarp::os::Bottle;

	dbgTag = "ConfigData: ";

	targetPressure = rf.check("targetPressure",Value(1.0)).asDouble();
	
	pidKp = rf.check("pidKp",Value(1.0)).asDouble();
	pidKi = rf.check("pidKi",Value(1.0)).asDouble();
	pidKd = rf.check("pidKd",Value(0.0)).asDouble();
	
	cartesianMode = rf.check("cartesianMode",Value(0)).asInt() != 0;
}

