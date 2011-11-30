cmake_minimum_required(VERSION 2.8)

SET(CLAY_TARGET clay-nuggets)

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

#search for nuggets
SUBDIRLIST(SUBDIRS ${CLAY_ROOT}/clay-nuggets)

FOREACH(subdir ${SUBDIRS})
    ADD_SUBDIRECTORY(${subdir})
ENDFOREACH()

#search for tests inside nuggets
SUBDIRLIST(SUBDIRS ${CLAY_ROOT}/clay-nuggets)

FOREACH(subdir ${SUBDIRS})
	IF(EXISTS "${CLAY_ROOT}/clay-nuggets/${subdir}/${subdir}-test")
		ADD_SUBDIRECTORY(${CLAY_ROOT}/clay-nuggets/${subdir}/${subdir}-test)
	ELSE()
		MESSAGE("Warning: no tests found for nugget: ${subdir}")
		MESSAGE("  -> ${CLAY_ROOT}/clay-nuggets/${subdir}/${subdir}-test")
	ENDIF()
ENDFOREACH()