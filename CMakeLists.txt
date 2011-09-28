cmake_minimum_required(VERSION 2.8)

SET(CLAY_TARGET clay)

MESSAGE("Invoke cmake for: ${CLAY_TARGET} with build type: ${CMAKE_BUILD_TYPE} ...")

#--------------- set the output path -------------------------
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH    ${CMAKE_CURRENT_BINARY_DIR}/bin)

#--------------- set the externals dir -------------------------
SET(CLAY_ROOT 		   ${CMAKE_SOURCE_DIR})
SET(CLAY_EXTERNALS_DIR ${CLAY_ROOT}/externals)

#------------ enable static libray linkage under WIN32 ---------
SET(CMAKE_USER_MAKE_RULES_OVERRIDE     ${CLAY_ROOT}/c_flag_overrides.cmake)
SET(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX ${CLAY_ROOT}/cxx_flag_overrides.cmake)

#------- set the Qt root dir and enable cmake finding it -------
SET(CMAKE_MODULE_PATH       ${CMAKE_MODULE_PATH} ${CLAY_ROOT})
IF(WIN32)
  SET(CLAY_QT_PATH          ${CLAY_EXTERNALS_DIR}/qt-everywhere-opensource-src-4.6.2)
  SET(QT_QMAKE_EXECUTABLE   ${CLAY_QT_PATH}/bin/qmake.exe)
ENDIF(WIN32)

PROJECT(${CLAY_TARGET}) 

IF(WIN32)
  SET(QT_RCC_EXECUTABLE   C:/Clay/externals/qt-everywhere-opensource-src-4.6.2/bin/rcc.exe)
  #ADD_DEFINITIONS(/D "_UNICODE" /D "UNICODE" /D "XERCES_STATIC_LIBRARY")
  ADD_DEFINITIONS(/D "_UNICODE" /D "UNICODE")
ENDIF(WIN32)

#--------------- INCLUDE PATH VARIABLES ------------------------
SET(CLAY_BOOST_INCLUDE    ${CLAY_EXTERNALS_DIR}/boost-1.41.0.cmake)
SET(CLAY_LUA_INCLUDE      ${CLAY_EXTERNALS_DIR}/lua/5.1.4/include) 
SET(CLAY_VST_SDK_INCLUDE  ${CLAY_EXTERNALS_DIR}/vstsdk2.4)
SET(CLAY_ASIO_SDK_INCLUDE ${CLAY_EXTERNALS_DIR}/ASIOSDK2)
SET(CLAY_CURL_INCLUDE     ${CLAY_EXTERNALS_DIR}/curl-7.20.0/include)
SET(CLAY_JUCE_INCLUDE     ${CLAY_EXTERNALS_DIR}/juce_1_50/juce)
SET(CLAY_LIB_PNG_INCLUDE  ${CLAY_EXTERNALS_DIR}/libpng)
SET(CLAY_ZLIB_INCLUDE     ${CLAY_EXTERNALS_DIR}/zlib)
SET(CLAY_LIBMAD_INCLUDE   ${CLAY_EXTERNALS_DIR}/libmad-0.15.1b)
SET(CLAY_FFTW_INCLUDE     ${CLAY_EXTERNALS_DIR}/fftw-3.2.2.pl1-dll32)
SET(CLAY_XMLRPC_INCLUDE   ${CLAY_EXTERNALS_DIR}/xmlrpc-c/bin/include)
SET(CLAY_XERCES_INCLUDE   ${CLAY_EXTERNALS_DIR}/xerces-c-3.1.0/src)
SET(CLAY_XQILLA_INCLUDE   ${CLAY_EXTERNALS_DIR}/XQilla-2.2.4/bin/include)

#platform specific include paths
IF(WIN32)
  
  SET(CLAY_COIN_INCLUDE   ${CLAY_EXTERNALS_DIR}/Coin-3.1.3/include)
ELSEIF(UNIX)
  #SET(CLAY_XERCES_INCLUDE /usr/include/)
  SET(CLAY_COIN_INCLUDE   /usr/include/Coin2)
ENDIF()  

SET(CLAY_DEFAULT_INCLUDES ${CLAY_ROOT} 
						  ${CMAKE_BINARY_DIR})   
     
#------------------------------------------------------------

#---------------- LINK PATH VARIABLES -----------------------
IF(WIN32)
  SET(CLAY_XERCES_LINK_PATH  ${CLAY_EXTERNALS_DIR}/xerces-c-3.1.0/Build/Win32/VC9/Debug)
  SET(CLAY_QT_LINK_PATH      ${CLAY_EXTERNALS_DIR}/qt-all-opensource-src-4.5.2/lib)
  SET(CLAY_LUA_LINK_PATH     ${CLAY_EXTERNALS_DIR}/lua/5.1.4/lib/static)
  #SET(CLAY_BOOST_LINK_PATH  ${CLAY_EXTERNALS_DIR}/boost_1_41_0/build)
  SET(CLAY_BOOST_LINK_PATH   ${CLAY_EXTERNALS_DIR}/boost-1.41.0.cmake/build/lib/Debug)
  SET(CLAY_COIN_LINK_PATH    ${CLAY_EXTERNALS_DIR}/Coin-3.1.3/lib)
  SET(CLAY_CURL_LINK_PATH    ${CLAY_EXTERNALS_DIR}/curl-7.20.0/build/lib)
  SET(CLAY_JUCE_LINK_PATH    ${CLAY_EXTERNALS_DIR}/juce_1_50/juce/bin)
  SET(CLAY_LIB_PNG_LINK_PATH ${CLAY_EXTERNALS_DIR}/libpng/projects/visualc71/Win32_LIB_Release)
  SET(CLAY_ZLIB_LINK_PATH    ${CLAY_EXTERNALS_DIR}/libpng/projects/visualc71/Win32_LIB_Release/ZLib)
  SET(CLAY_ASIO_LINK_PATH    ${CLAY_EXTERNALS_DIR}/ASIOSDK2/ASIOLib/Debug)
  SET(CLAY_LIBMAD_LINK_PATH  ${CLAY_EXTERNALS_DIR}/libmad-0.15.1b/msvc++10/libmad/Debug)
  SET(CLAY_FFTW_LINK_PATH    ${CLAY_EXTERNALS_DIR}/fftw-3.2.2.pl1-dll32)
ELSEIF(UNIX)
  SET(CLAY_XQILLA_LINK_PATH  ${CLAY_EXTERNALS_DIR}/XQilla-2.2.4/bin/lib)
  SET(CLAY_XMLRPC_LINK_PATH  ${CLAY_EXTERNALS_DIR}/xmlrpc-c/bin/lib)
  SET(CLAY_XERCES_LINK_PATH  ${CLAY_EXTERNALS_DIR}/xerces-c-3.1.0/Build)
  SET(CLAY_QT_LINK_PATH      /usr/local/lib)
  SET(CLAY_LUA_LINK_PATH     /usr/local/lib)
ENDIF()
#------------------------------------------------------------

#---------------------- LIBRARY VARIABLES -------------------
IF(WIN32)
    SET(CLAY_QT_CORE_LIB   )
    SET(CLAY_QT_GUI_LIB    )
    SET(CLAY_XERCES_LIB    xerces-c_3D)
    SET(CLAY_LUA_LIB       lua5.1)
    SET(CLAY_PLATFORM_LIBS imm32
                           winmm
                           ws2_32)
	SET(CLAY_COIN_LIB      coin3sd)                         
	SET(CLAY_SOQT_LIB      soqt1sd)
	SET(CLAY_CURL_LIB      libcurld)
	SET(CLAY_OPENGL_LIB    opengl32)
	SET(CLAY_JUCE_LIB      jucelib_static_Win32_debug)
	SET(CLAY_LIBPNG_LIB    libpng.lib)
	SET(CLAY_ZLIB_LIB      zlib.lib)
	SET(CLAY_ASIO_LIB      ASIOLib.lib)
	SET(CLAY_LIBMAD_LIB    libmad.lib)
	SET(CLAY_FFTW_LIB      libfftw3f-3.lib)
ELSEIF(UNIX)
    SET(CLAY_QT_CORE_LIB   )
    SET(CLAY_QT_GUI_LIB    )
    SET(CLAY_XERCES_LIB    xerces-c-3.1)
    SET(CLAY_LUA_LIB       lua-5.1)
    SET(CLAY_PLATFORM_LIBS )
    SET(CLAY_XMLRPC_LIB    xmlrpc_client++ xmlrpc_server++ xmlrpc++ xmlrpc_server_abyss++)
    SET(CLAY_XQILLA_LIB    xqilla)
ENDIF()
#------------------------------------------------------------

ADD_SUBDIRECTORY(clay-core)
ADD_SUBDIRECTORY(clay-modules)
ADD_SUBDIRECTORY(clay-data-types)
ADD_SUBDIRECTORY(clay-gui-qt)
ADD_SUBDIRECTORY(clay-designer)
ADD_SUBDIRECTORY(clay-nuggets)