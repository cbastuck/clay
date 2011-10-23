cmake_minimum_required(VERSION 2.8)

SET(CLAY_TARGET common-nugget)

cmake_policy(SET CMP0015 OLD)

PROJECT(${CLAY_TARGET})

MESSAGE("Invoke cmake for: ${CLAY_TARGET} ...")

SET(CLAY_COMMON_NUGGET_SRC    CommonNugget.cpp
							   ${CLAY_ROOT}/clay-modules/common/math/AdderModule.h
							   ${CLAY_ROOT}/clay-modules/common/math/AdderModule.cpp
							   ${CLAY_ROOT}/clay-modules/common/output/ConsoleOutput.h
							   ${CLAY_ROOT}/clay-modules/common/output/ConsoleOutput.cpp)

SOURCE_GROUP(${CLAY_TARGET} FILES ${CLAY_COMMON_NUGGET_SRC})

QT4_WRAP_CPP(CLAY_MOC_OUTFILES ${CLAY_ROOT}/clay-gui-qt/modules/value/ValueModuleUI.h)

INCLUDE_DIRECTORIES(${CLAY_DEFAULT_INCLUDES} 
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CLAY_BOOST_INCLUDE} 
                    ${CLAY_XERCES_INCLUDE}
                    ..)

ADD_LIBRARY(${CLAY_TARGET} SHARED ${CLAY_COMMON_NUGGET_SRC} ${CLAY_MOC_OUTFILES})

TARGET_LINK_LIBRARIES(${CLAY_TARGET}
					  clay-gui-qt
					  clay-core
					  clay-data-types
					  ${CLAY_XERCES_LIB}
                      ${QT_LIBRARIES})     