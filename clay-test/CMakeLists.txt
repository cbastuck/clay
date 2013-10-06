cmake_minimum_required(VERSION 2.8)

SET(CLAY_TARGET clay-test)

cmake_policy(SET CMP0015 OLD)

PROJECT(${CLAY_TARGET})

MESSAGE("Invoke cmake for: ${CLAY_TARGET} ...")

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        SET(dirlist ${dirlist} ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(SUBDIRS ${CLAY_ROOT}/clay-test)

FOREACH(subdir ${SUBDIRS})
    IF(EXISTS "${CLAY_ROOT}/clay-test/${subdir}/CMakeLists.txt")
        MESSAGE("Building tests for: ${subdir}")
        ADD_SUBDIRECTORY(${subdir})
    ELSE()
        MESSAGE("Skipping tests for: ${subdir}")
    ENDIF()
ENDFOREACH()
