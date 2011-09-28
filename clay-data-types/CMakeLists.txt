######################################
### clay-data-types cmake-file     ###
######################################

cmake_minimum_required(VERSION 2.8)

SET(CLAY_TARGET clay-data-types)

cmake_policy(SET CMP0015 OLD)

PROJECT(${CLAY_TARGET})

MESSAGE("Invoke cmake for: ${CLAY_TARGET} with build type: ${CMAKE_BUILD_TYPE} ...")

SET(CLAY_BASE_SOURCES    base/ClayTypeChecking.h base/ClayTypeChecking.cpp
					     base/ClayTypeConversion.h base/ClayTypeConversion.cpp
					     base/ClayTypeMapping.h 
                         base/ClayTypeMappingDetail.h
					     base/MatrixModel.h
                         base/MemoryOwner.cpp base/MemoryOwner.h
					     base/Matrix.h
					     base/Vector.h
					     base/Const.h
					     base/MatrixOp.h
					     base/VectorOp.h)

SOURCE_GROUP(base      FILES ${CLAY_BASE_SOURCES})

SET(CLAY_SOURCES ${CLAY_BASE_SOURCES})

INCLUDE_DIRECTORIES(${CLAY_DEFAULT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CLAY_BOOST_INCLUDE} 
                    ${CLAY_XERCES_INCLUDE}
                    ..)                    
                    
ADD_LIBRARY(${CLAY_TARGET} ${CLAY_SOURCES})
