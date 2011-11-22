cmake_minimum_required(VERSION 2.8)

SET(CLAY_TARGET xmlrpc-c-nugget)

cmake_policy(SET CMP0015 OLD)

PROJECT(${CLAY_TARGET})

MESSAGE("Invoke cmake for: ${CLAY_TARGET} with build type: ${CMAKE_BUILD_TYPE} ...")

#---- use Qt
FIND_PACKAGE(ClayQt4 COMPONENTS QtCore QtGui REQUIRED)
INCLUDE(${QT_USE_FILE})

#---- vst module nugget
SET(CLAY_XMLRPC_C_NUGGET_TARGET ${CLAY_TARGET})

SET(CLAY_XMLRPC_C_NUGGET_SRC    XMLRPCNugget.cpp
						        ${CLAY_ROOT}/clay-modules/rpc/xmlrpc-c/XMLRPCClient.h   ${CLAY_ROOT}/clay-modules/rpc/xmlrpc-c/XMLRPCClient.cpp
						        ${CLAY_ROOT}/clay-modules/rpc/xmlrpc-c/XMLRPCServer.h   ${CLAY_ROOT}/clay-modules/rpc/xmlrpc-c/XMLRPCServer.cpp
						        ${CLAY_ROOT}/clay-modules/xml/XMLProcessor.h            ${CLAY_ROOT}/clay-modules//xml/XMLProcessor.cpp)
SOURCE_GROUP(${CLAY_XMLRPC_C_NUGGET_TARGET} FILES ${CLAY_XMLRPC_C_NUGGET_SRC})

#------- the include section ------------
INCLUDE_DIRECTORIES(${CLAY_DEFAULT_INCLUDES} 
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CLAY_BOOST_INCLUDE} 
                    ${CLAY_XERCES_INCLUDE}
                    ${CLAY_JUCE_INCLUDE}
                    ${CMAKE_BINARY_DIR} 
                    ${CLAY_EXTERNAL_INC} 
                    ${CLAY_PLATFORM_INCLUDES}
                    ${CLAY_XMLRPC_INCLUDE}
                    ${CLAY_XQILLA_INCLUDE}
                    ..)
                                                   			 
SET(CLAY_LIBS clay-core
              clay-data-types
              ${CLAY_XERCES_LIB})
            
#------- the target section ------------

ADD_LIBRARY(${CLAY_XMLRPC_C_NUGGET_TARGET} SHARED ${CLAY_XMLRPC_C_NUGGET_SRC})

TARGET_LINK_LIBRARIES(${CLAY_XMLRPC_C_NUGGET_TARGET}
                      ${CLAY_LIBS}
                      ${CLAY_JUCE_LIB}
                      ${CLAY_LIBPNG_LIB}
                      ${QT_LIBRARIES}
                      ${CLAY_XMLRPC_LIB}
                      ${CLAY_XQILLA_LIB}
                      clay-gui-qt)     