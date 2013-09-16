cmake_minimum_required(VERSION 2.8)

SET(CLAY_TARGET clay)

MESSAGE("Invoke cmake for: ${CLAY_TARGET} ...")

#---------------- set the output path --------------------------
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH    ${CMAKE_CURRENT_BINARY_DIR}/bin)

#--------------- set the externals dir -------------------------
SET(CLAY_ROOT 		   ${CMAKE_SOURCE_DIR})
SET(CLAY_EXTERNALS_DIR ${CLAY_ROOT}/externals)

#------------ enable static libray linkage under WIN32 ---------
SET(CMAKE_USER_MAKE_RULES_OVERRIDE     ${CLAY_ROOT}/c_flag_overrides.cmake)
SET(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX ${CLAY_ROOT}/cxx_flag_overrides.cmake)

#--------- look in the root directory for cmake modules --------
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CLAY_ROOT})

PROJECT(${CLAY_TARGET}) 

IF(WIN32)
  ADD_DEFINITIONS(/D "_UNICODE" /D "UNICODE")
ENDIF(WIN32)

#------------------ check for clay dependencies ---------------
FIND_PACKAGE(ClayQt4 COMPONENTS QtCore QtGui)

#--------------------- include search path ---------------------
SET(CLAY_DEFAULT_INCLUDES ${CLAY_ROOT} 
						  ${CMAKE_BINARY_DIR})   

#--------------------- library dependencies --------------------
IF(WIN32)
    SET(CLAY_PLATFORM_LIBS imm32 winmm ws2_32)
ELSEIF(UNIX)
    SET(CLAY_PLATFORM_LIBS )    
ENDIF()

IF(QT4_FOUND)
	INCLUDE(${QT_USE_FILE})
endif()					
#---------------------------------------------------------------

ADD_SUBDIRECTORY(clay-core)
ADD_SUBDIRECTORY(clay-modules)
ADD_SUBDIRECTORY(clay-data-types)
ADD_SUBDIRECTORY(clay-test)

#---------------- gui-related clay projects -------------------
IF(QT4_FOUND)
	ADD_SUBDIRECTORY(clay-gui-qt)
	ADD_SUBDIRECTORY(clay-designer)
	ADD_SUBDIRECTORY(clay-nuggets)
ELSE()
	MESSAGE("WARNING: Qt not found, all GUI related projects will be ommitted during build")
ENDIF()

#------------------------------------------------------------