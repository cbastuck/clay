cmake_minimum_required(VERSION 2.8)

SET(CLAY_TARGET claython)

cmake_policy(SET CMP0015 OLD)

PROJECT(${CLAY_TARGET})

MESSAGE("Invoke cmake for: ${CLAY_TARGET} ...")

#check for dependencies
INCLUDE(${CLAY_EXTERNALS_DIR}/python/config.cmake)
INCLUDE(${CLAY_EXTERNALS_DIR}/xercesc/config.cmake)
INCLUDE(${CLAY_EXTERNALS_DIR}/boost/config.cmake)

SET(CLAY_SOURCES main.cpp PythonHost.h PythonHost.cpp)
	   
INCLUDE_DIRECTORIES(${CLAY_DEFAULT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CLAY_PYTHON_INCLUDE}
                    ${CLAY_BOOST_INCLUDE})
                                          
SET(CLAY_LIBS clay-core 
              clay-data-types
              optimized ${CLAY_XERCES_LIB}
              debug ${CLAY_XERCES_LIB_D}
              ${CLAY_PLATFORM_LIBS})                  
                                       
ADD_LIBRARY(${CLAY_TARGET} SHARED ${CLAY_SOURCES})

TARGET_LINK_LIBRARIES(${CLAY_TARGET}
                      ${CLAY_LIBS}
                      boost_python)

#on windows python extensions must have .pyd extension
IF(WIN32)
    SET_TARGET_PROPERTIES( ${CLAY_TARGET}
         PROPERTIES
         PREFIX ""
         SUFFIX ".pyd"
     )
ENDIF()
