cmake_minimum_required(VERSION 2.8)

SET(CLAY_TARGET clay-designer)

cmake_policy(SET CMP0015 OLD)

PROJECT(${CLAY_TARGET})

MESSAGE("Invoke cmake for: ${CLAY_TARGET} ...")

#dependencies
INCLUDE(${CLAY_EXTERNALS_DIR}/boost/config.cmake)
INCLUDE(${CLAY_EXTERNALS_DIR}/xercesc/config.cmake)

SET(CLAY_SOURCES clay-designer.cpp
			     ../clay-modules/host/HostModule.cpp  ../clay-modules/host/HostModule.h
			     ../clay-modules/StandardRegistry.cpp ../clay-modules/StandardRegistry.h)
	   
INCLUDE_DIRECTORIES(${CLAY_DEFAULT_INCLUDES}
                    ${CLAY_BOOST_INCLUDE} 
                    ${CLAY_XERCES_INCLUDE} 
                    ${CMAKE_CURRENT_BINARY_DIR}
					${CMAKE_CURRENT_BINARY_DIR}/../clay-gui-qt #hack otherwise we do not find resources
                    ..)          
                                          
SET(CLAY_LIBS clay-core 
              clay-gui-qt
			  clay-data-types
              optimized ${CLAY_XERCES_LIB}
			  debug ${CLAY_XERCES_LIB_D}
              ${CLAY_PLATFORM_LIBS})                  

LINK_DIRECTORIES(${CLAY_BOOST_LIBPATH}) 
              
ADD_EXECUTABLE(${CLAY_TARGET} 
			   ${CLAY_SOURCES})
               
TARGET_LINK_LIBRARIES(${CLAY_TARGET}
                      ${CLAY_LIBS}
					  ${QT_LIBRARIES})   