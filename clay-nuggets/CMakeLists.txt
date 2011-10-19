cmake_minimum_required(VERSION 2.8)

SET(CLAY_TARGET clay-nuggets)

cmake_policy(SET CMP0015 OLD)

PROJECT(${CLAY_TARGET})

MESSAGE("Invoke cmake for: ${CLAY_TARGET} with build type: ${CMAKE_BUILD_TYPE} ...")

ADD_SUBDIRECTORY(xmlrpc-c-nugget)