cmake_minimum_required(VERSION 2.8)

SET(CLAY_TARGET clay-test-matrix)

cmake_policy(SET CMP0015 OLD)

PROJECT(${CLAY_TARGET})

MESSAGE("Invoke cmake for: ${CLAY_TARGET} with build type: ${CMAKE_BUILD_TYPE} ...")

INCLUDE(${CLAY_EXTERNALS_DIR}/boost/config.cmake)
INCLUDE(${CLAY_EXTERNALS_DIR}/xercesc/config.cmake)

SET(CLAY_TEST_MATRIX_SRC test-matrix.cpp)

#------- the include section ------------
INCLUDE_DIRECTORIES(${CLAY_DEFAULT_INCLUDES} 
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_BINARY_DIR} 
                    ${CLAY_EXTERNAL_INC} 
                    ${CLAY_PLATFORM_INCLUDES}
					${CLAY_XERCES_INCLUDE}
					${CLAY_BOOST_INCLUDE}
                    ..)
            
#------- the target section ------------
ADD_EXECUTABLE(${CLAY_TARGET} ${CLAY_TEST_MATRIX_SRC})

TARGET_LINK_LIBRARIES(${CLAY_TARGET}
                      clay-core
					  clay-data-types
					  clay-modules
					  optimized ${CLAY_XERCES_LIB}
					  debug ${CLAY_XERCES_LIB_D})