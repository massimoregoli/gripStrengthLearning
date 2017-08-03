# Copyright: 2015 iCub Facility, Istituto Italiano di Tecnologia
# Authors: Massimo Regoli
# email:   massimo.regoli@iit.it
# CopyPolicy: Released under the terms of the GNU GPL v2.0.
# 

cmake_minimum_required(VERSION 2.6)

set(PROJECTNAME gripStrengthLearning)
project(${PROJECTNAME})


# Check build type
if(NOT CMAKE_BUILD_TYPE)
    MESSAGE(STATUS "No build type specified. Defaulting to Release.")
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)
# Add flags
MESSAGE(STATUS "Compiling in ${CMAKE_BUILD_TYPE} mode.")
if(CMAKE_BUILD_TYPE MATCHES Debug)
    MESSAGE(STATUS "Adding debug flags to compiler.")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
else(CMAKE_BUILD_TYPE MATCHES Debug)
    MESSAGE(STATUS "Adding release flags to compiler.")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
endif(CMAKE_BUILD_TYPE MATCHES Debug)


# Find packages
find_package(YARP)
find_package(ICUB)
list(APPEND CMAKE_MODULE_PATH ${YARP_MODULE_PATH})
list(APPEND CMAKE_MODULE_PATH ${ICUB_MODULE_PATH})
include(YarpInstallationHelpers)
include(YarpIDL)

find_package(ICUBcontrib)
list(APPEND CMAKE_MODULE_PATH ${ICUBCONTRIB_MODULE_PATH})
include(ICUBcontribHelpers)
include(ICUBcontribOptions)
icubcontrib_set_default_prefix()

add_definitions(${YARP_DEFINES}) # add yarp definitions - this contains also -D_REENTRANT



# Include directories
include_directories(${YARP_INCLUDE_DIRS})
include_directories(${ICUB_INCLUDE_DIRS})


# Add project directories
subdirs(app)
subdirs(src)
