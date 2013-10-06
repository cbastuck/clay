#
# 1. use the specified default locations in this configuration file (s.b.)
# 2. try to read boost location from environment variables: 
#	CLAY_XERCES_INCLUDE
#	CLAY_XERCES_LIB
# 	CLAY_XERCES_LIB_D
# the environment variables override the default locations!
#

IF(MSVC) #(1)
	SET(CLAY_XERCES_LIB    	C:/ClayGithub/externals/bin/x86/Release/xerces-c_3.lib)
	SET(CLAY_XERCES_LIB_D  	C:/ClayGithub/externals/bin/x86/Debug/xerces-c_3D.lib)
	SET(CLAY_XERCES_INCLUDE C:/ClayGithub/externals/xercesc/xerces-c-3.1.1/src) 
ENDIF(MSVC)

IF(EXISTS "$ENV{CLAY_XERCES_INCLUDE}") #(2)
        MESSAGE("Using CLAY_XERCES_INCLUDE environment variable: $ENV{CLAY_XERCES_INCLUDE}")
	SET(CLAY_XERCES_INCLUDE $ENV{CLAY_XERCES_INCLUDE})
ENDIF()

IF(EXISTS "$ENV{CLAY_XERCES_LIB}")
        MESSAGE("Using CLAY_XERCES_LIB environment variable: $ENV{CLAY_XERCES_LIB}")
        SET(CLAY_XERCES_LIB $ENV{CLAY_XERCES_LIB})
ENDIF()

IF(EXISTS "$ENV{CLAY_XERCES_LIB_D}")
        MESSAGE("Using CLAY_XERCES_LIB_D environment variable: $ENV{CLAY_XERCES_LIB_D}")
	SET(CLAY_XERCES_LIB_D $ENV{CLAY_XERCES_LIB_D})	
ELSEIF(NOT EXISTS "${CLAY_XERCES_LIB_D}") #use the release for debug if the default does not fit
        MESSAGE("Using CLAY_XERCES_LIB as CLAY_XERCES_LIB_D")
	SET(CLAY_XERCES_LIB_D $ENV{CLAY_XERCES_LIB}) #use the release version if the debug is not available
ENDIF()

#if library was found export variables to parent scope
IF(EXISTS "${CLAY_XERCES_INCLUDE}" AND EXISTS "${CLAY_XERCES_LIB}")
        MESSAGE("Using Xerces at location ${CLAY_XERCES_INCLUDE}")
	SET(CLAY_XERCES_LIB 	 ${CLAY_XERCES_LIB}     PARENT_SCOPE)
	SET(CLAY_XERCES_LIB_D    ${CLAY_XERCES_LIB_D}   PARENT_SCOPE)
	SET(CLAY_XERCES_INCLUDE  ${CLAY_XERCES_INCLUDE} PARENT_SCOPE)
ELSE()
    SET(CLAY_XERCES_LIB 	 /usr/lib/libxerces-c.so)
    SET(CLAY_XERCES_LIB_D 	 /usr/lib/libxerces-c.so)
    SET(CLAY_XERCES_INCLUDE	 /usr/include/xercesc)
ENDIF()
