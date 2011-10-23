#
# 1. use the specified default locations in this configuration file (s.b.)
# 2. try to read boost location from environment variables: 
#	CLAY_XERCES_INCLUDE
#	CLAY_XERCES_LIB
# 	CLAY_XERCES_LIB_D
# the environment variables override the default locations!
#

if(MSVC) #(1)
	SET(CLAY_XERCES_LIB    	${CLAY_EXTERNALS_DIR}/xerces-c-3.1.0/Build/Win32/VC9/Release/xerces-c_3.lib)
	SET(CLAY_XERCES_LIB_D  	${CLAY_EXTERNALS_DIR}/xerces-c-3.1.0/Build/Win32/VC9/Debug/xerces-c_3D.lib)
	SET(CLAY_XERCES_INCLUDE ${CLAY_EXTERNALS_DIR}/xerces-c-3.1.0/src) 
endif(MSVC)

if(EXISTS "$ENV{CLAY_XERCES_INCLUDE}") #(2)
	message("Using CLAY_XERCES_INCLUDE environment variable: $ENV{CLAY_XERCES_INCLUDE}")
	SET(CLAY_XERCES_INCLUDE $ENV{CLAY_XERCES_INCLUDE})
endif()

if(EXISTS "$ENV{CLAY_XERCES_LIB}") 
	message("Using CLAY_XERCES_LIB environment variable: $ENV{CLAY_XERCES_LIB}")
	SET(CLAY_XERCES_LIB $ENV{CLAY_XERCES_LIB})
endif()

if(EXISTS "$ENV{CLAY_XERCES_LIB_D}") 
	message("Using CLAY_XERCES_LIB_D environment variable: $ENV{CLAY_XERCES_LIB_D}")
	SET(CLAY_XERCES_LIB_D $ENV{CLAY_XERCES_LIB_D})	
elseif(NOT EXISTS "${CLAY_XERCES_LIB_D}") #use the release for debug if the default does not fit
	message("Using CLAY_XERCES_LIB as CLAY_XERCES_LIB_D")
	SET(CLAY_XERCES_LIB_D $ENV{CLAY_XERCES_LIB}) #use the release version if the debug is not available
endif()


#if library was found export variables to parent scope
if(EXISTS "${CLAY_XERCES_INCLUDE}" AND EXISTS "${CLAY_XERCES_LIB}")
	message("Using Xerces at location ${CLAY_XERCES_INCLUDE}")
	SET(CLAY_XERCES_LIB 	 ${CLAY_XERCES_LIB}     PARENT_SCOPE)
	SET(CLAY_XERCES_LIB_D    ${CLAY_XERCES_LIB_D}   PARENT_SCOPE)
	SET(CLAY_XERCES_INCLUDE  ${CLAY_XERCES_INCLUDE} PARENT_SCOPE)
else()
	message("Xerces was not found on your system - clay will not compile")
	message("Set the following environment variables: CLAY_XERCES_LIB, CLAY_XERCES_LIB_D and 	CLAY_XERCES_INCLUDE")
	
endif()