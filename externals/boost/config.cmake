#
# 1. try to find boost automatically
# 2. try to read boost location from environment variable: BOOSTDIR
# 3. use the default directory specified in this configuration file (s.b.)
# the environment variables override the automatic and default configuration in this file
#

find_package( Boost 1.41.0 ) 

if(Boost_FOUND)
	SET(CLAY_BOOST_INCLUDE ${Boost_INCLUDE_DIRS}) #(1)	
	SET(CLAY_BOOST_LIBPATH ${Boost_LIBRARIES})
else() 
	SET(CLAY_BOOST_INCLUDE C:/clay/externals/boost-1.41.0.cmake) #(3)
	SET(CLAY_BOOST_LIBPATH C:/clay/externals/boost-1.41.0.cmake/build/lib)
endif()

if(EXISTS "$ENV{CLAY_BOOST_INCLUDE}") 
	SET(CLAY_BOOST_INCLUDE $ENV{CLAY_BOOST_INCLUDE}) #(2)
endif()

if(EXISTS "$ENV{CLAY_BOOST_LIBPATH}") 
	SET(CLAY_BOOST_LIBPATH $ENV{CLAY_BOOST_LIBPATH}) #(2)
endif()

#if library was found export variables to parent scope
if(EXISTS "${CLAY_BOOST_INCLUDE}")
	message("Using boost at location: ${CLAY_BOOST_INCLUDE}")
	SET(CLAY_BOOST_INCLUDE ${CLAY_BOOST_INCLUDE} PARENT_SCOPE) #export location to parent scope
	SET(CLAY_BOOST_LIBPATH ${CLAY_BOOST_LIBPATH} PARENT_SCOPE)
else()
	message("Could not find boost on your system - Clay will not compile with this configuration")
	message("Set CLAY_BOOST_INCLUDE and CLAY_BOOST_LIBPATH environment variable")
endif()