cmake_minimum_required(VERSION 2.8)

SET(CLAY_TARGET clay-designer)

cmake_policy(SET CMP0015 OLD)

PROJECT(${CLAY_TARGET})

MESSAGE("Invoke cmake for: ${CLAY_TARGET} with build type: ${CMAKE_BUILD_TYPE} ...")

FIND_PACKAGE(ClayQt4 COMPONENTS QtCore QtGui REQUIRED)
INCLUDE(${QT_USE_FILE})

SET(CLAY_SOURCES clay-designer.cpp
			     ../clay-modules/host/HostModule.cpp  ../clay-modules/host/HostModule.h
			     ../clay-modules/StandardRegistry.cpp ../clay-modules/StandardRegistry.h)
	   
INCLUDE_DIRECTORIES(${CLAY_DEFAULT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR}
					${CMAKE_CURRENT_BINARY_DIR}/../clay-gui-qt #hack otherwise we do not find resources
                    ${CLAY_BOOST_INCLUDE}
                    ${CLAY_XERCES_INCLUDE}
                    ..)          
                                          
SET(CLAY_LIBS clay-core 
              clay-gui-qt
			  clay-data-types
              ${CLAY_XERCES_LIB}
              ${CLAY_PLATFORM_LIBS})                  
                                       
ADD_EXECUTABLE(${CLAY_TARGET} 
			   ${CLAY_SOURCES})

TARGET_LINK_LIBRARIES(${CLAY_TARGET}
                      ${CLAY_LIBS}
					  ${QT_LIBRARIES})   