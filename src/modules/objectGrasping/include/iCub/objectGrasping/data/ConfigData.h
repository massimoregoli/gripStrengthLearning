#ifndef __ICUB_OBJECTGRASPING_CONFIGDATA_H__
#define __ICUB_OBJECTGRASPING_CONFIGDATA_H__

#include "iCub/objectGrasping/ObjectGraspingEnums.h"

#include <yarp/os/BufferedPort.h>
#include <yarp/os/ResourceFinder.h>
#include <yarp/sig/Vector.h>

#include <vector>

namespace iCub {
    namespace objectGrasping {

        class ConfigData {

            private:
                         
                /* ****** Debug attributes                              ****** */
                std::string dbgTag;

            public:

				double pidKp;
				double pidKi;
				double pidKd;
				double targetPressure;
				
				bool cartesianMode;

				ConfigData(yarp::os::ResourceFinder &rf);
		
        };
    } //namespace objectGrasping
} //namespace iCub

#endif

