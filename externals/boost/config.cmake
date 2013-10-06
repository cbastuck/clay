#
# 1. try to find boost automatically
# 2. try to read boost location from environment variable: BOOSTDIR
# 3. use the default directory specified in this configuration file (s.b.)
# the environment variables override the automatic and default configuration in this file
#

#find_package( Boost 1.41.0 ) 

IF(Boost_FOUND)
    SET(CLAY_BOOST_INCLUDE ${Boost_INCLUDE_DIRS}) #(1)
    SET(CLAY_BOOST_LIBPATH ${Boost_LIBRARIES})
ELSE()
    SET(CLAY_BOOST_INCLUDE /home/cb/Development/boost_1_54_0) #(3)
    SET(CLAY_BOOST_LIBPATH /home/cb/Development/boost_1_54_0/stage/lib)
ENDIF()

IF(EXISTS "$ENV{CLAY_BOOST_INCLUDE}")
    SET(CLAY_BOOST_INCLUDE $ENV{CLAY_BOOST_INCLUDE}) #(2)
ENDIF()

IF(EXISTS "$ENV{CLAY_BOOST_LIBPATH}")
    SET(CLAY_BOOST_LIBPATH $ENV{CLAY_BOOST_LIBPATH}) #(2)
ENDIF()

#if library was found export variables to parent scope
IF(EXISTS "${CLAY_BOOST_INCLUDE}")
    MESSAGE("Using boost at location: ${CLAY_BOOST_INCLUDE}")
    SET(CLAY_BOOST_INCLUDE ${CLAY_BOOST_INCLUDE} PARENT_SCOPE) #export location to parent scope
    SET(CLAY_BOOST_LIBPATH ${CLAY_BOOST_LIBPATH} PARENT_SCOPE)
ELSE()
    MESSAGE("Set CLAY_BOOST_INCLUDE and CLAY_BOOST_LIBPATH environment variable")
ENDIF()

LINK_DIRECTORIES(${LINK_DIRECTORIES} ${CLAY_BOOST_LIBPATH})
