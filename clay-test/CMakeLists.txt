cmake_minimum_required(VERSION 2.8)

SET(CLAY_TARGET clay-test)

cmake_policy(SET CMP0015 OLD)

PROJECT(${CLAY_TARGET})

MESSAGE("Invoke cmake for: ${CLAY_TARGET} ...")

ADD_SUBDIRECTORY(test-basics)