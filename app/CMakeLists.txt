# Copyright: 2015 Massimo Regoli, iCub Facility - Istituto Italiano di Tecnologia
# Author: Massimo Regoli
# CopyPolicy: Released under the terms of the GNU GPL v2.0.
# 

# Search for application files
file(GLOB APP_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/applications/*.xml)
# Search for ini files
file(GLOB APP_CONF ${CMAKE_CURRENT_SOURCE_DIR}/contexts/${PROJECTNAME}/*.ini)


yarp_install(FILES ${APP_SCRIPTS} DESTINATION ${ICUBCONTRIB_APPLICATIONS_INSTALL_DIR})
yarp_install(FILES ${APP_CONF} DESTINATION ${ICUBCONTRIB_CONTEXTS_INSTALL_DIR}/${PROJECTNAME})

icubcontrib_add_uninstall_target()
