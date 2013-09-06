cmake_minimum_required(VERSION 2.8)

SET(CLAY_TARGET clay-externals)

cmake_policy(SET CMP0015 OLD)

PROJECT(${CLAY_TARGET})

INCLUDE(boost/config.cmake)
INCLUDE(xercesc/config.cmake)
INCLUDE(mad/config.cmake)
INCLUDE(asio/config.cmake)
#INCLUDE(portaudio/config.cmake)