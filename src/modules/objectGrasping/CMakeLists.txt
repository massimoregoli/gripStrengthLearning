# Copyright: 2015 iCub Facility, Istituto Italiano di Tecnologia
# Authors: Massimo Regoli
# email:   massimo.regoli@iit.it
# CopyPolicy: Released under the terms of the GNU GPL v2.0.
# 


set(MODULENAME objectGrasping)

# Search for source code.
set(INC_HEADERS
    include/iCub/objectGrasping/ObjectGraspingModule.h
    include/iCub/objectGrasping/ObjectGraspingEnums.h

    include/iCub/objectGrasping/data/ConfigData.h
    include/iCub/objectGrasping/data/RPCCommandsData.h

    include/iCub/objectGrasping/util/ControllersUtil.h
    include/iCub/objectGrasping/util/RPCCommandsUtil.h
    include/iCub/objectGrasping/util/PortsUtil.h

)

set(INC_SOURCES

    main.cpp

    ObjectGraspingModule.cpp

    data/ConfigData.cpp
    data/RPCCommandsData.cpp

    util/ControllersUtil.cpp
    util/RPCCommandsUtil.cpp
    util/PortsUtil.cpp
)

source_group("Source Files" FILES ${INC_SOURCES})
source_group("Header Files" FILES ${INC_HEADERS})

# Include directories
include_Directories(${skinDynLib_INCLUDE_DIRS})
include_directories(include)

add_executable(${MODULENAME} ${INC_SOURCES} ${INC_HEADERS})
target_link_libraries(${MODULENAME} ${YARP_LIBRARIES} skinDynLib)

if(WIN32)
    install(TARGETS ${PROJECT_NAME} DESTINATION bin/${CMAKE_BUILD_TYPE})
else(WIN32)
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)
endif(WIN32)
