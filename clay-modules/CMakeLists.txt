cmake_minimum_required(VERSION 2.8)

SET(CLAY_TARGET clay-modules)

cmake_policy(SET CMP0015 OLD)

PROJECT(${CLAY_TARGET})

MESSAGE("Invoke cmake for: ${CLAY_TARGET} with build type: ${CMAKE_BUILD_TYPE} ...")  
                          
SET(CLAY_SRC_FOLDER_HOST    host/HostModule.cpp                    host/HostModule.h)

SOURCE_GROUP(host      FILES ${CLAY_SRC_FOLDER_HOST})


SET(CLAY_SOURCES StandardRegistry.cpp
                 ${CLAY_SRC_FOLDER_HOST})               

INCLUDE_DIRECTORIES(${CLAY_DEFAULT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CLAY_BOOST_INCLUDE}
                    ${CLAY_XERCES_INCLUDE}
                    ..)                    
                    
ADD_LIBRARY(${CLAY_TARGET} ${CLAY_SOURCES})
