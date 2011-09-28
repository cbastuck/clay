######################################
#       clay-core cmake-file         #
######################################

cmake_minimum_required(VERSION 2.8)

SET(CLAY_TARGET clay-core)

cmake_policy(SET CMP0015 OLD)

PROJECT(${CLAY_TARGET})

MESSAGE("Invoke cmake for: ${CLAY_TARGET} with build type: ${CMAKE_BUILD_TYPE} ...")

SET(CLAY_SRC_FOLDER_BASE   base/ClayDefines.h
						   base/ClayExecutable.cpp base/ClayExecutable.h
                           base/ClayRuntime.cpp base/ClayRuntime.h
                           base/Module.cpp base/Module.h
                           base/ClayShaper.cpp base/ClayShaper.h
                           base/ModuleDependencies.cpp base/ModuleDependencies.h
						   base/ModuleIOBase.h
						   base/ModuleInputBase.h base/ModuleInputBase.cpp
						   base/ModuleOutputBase.h base/ModuleOutputBase.cpp
						   base/ModuleIO.h 
						   base/ModuleInput.h
						   base/ModuleOutput.h
						   base/TypedModuleOutput.h
						   base/TypedModuleInput.h
						   base/ModuleOutputProxy.h
						   base/ModuleInputProxy.h
                           base/ModuleDescriptor.h
                           base/ModuleUIDescriptor.h
						   base/ClayAllocator.h base/ClayAllocator.cpp)

SET(CLAY_SRC_FOLDER_HELPER helper/FileHelper.cpp    helper/FileHelper.h
                           helper/SharedLibrary.cpp helper/SharedLibrary.h
                           helper/XMLHelper.cpp     helper/XMLHelper.h
                           helper/ClayLogger.cpp    helper/ClayLogger.h)
						 
SET(CLAY_SRC_FOLDER_EVENT  event/SignalBase.h 
					       event/Signal.h
						   event/SignalReceiver.h event/SignalReceiver.cpp)
						

SET(CLAY_SRC_FOLDER_HOST   host/Host.cpp host/Host.h)

SET(CLAY_SRC_FOLDER_REG    registry/ModuleRegistry.h   registry/ModuleRegistry.cpp
						   registry/ModuleUIRegistry.h registry/ModuleUIRegistry.cpp)

SET(CLAY_SRC_FOLDER_NUGGET nugget/IClayNugget.h
					       nugget/ClayNugget.h
					       nugget/ClayNuggetLoader.h   nugget/ClayNuggetLoader.cpp
						   nugget/NuggetModuleRegistry.h
						   nugget/NuggetModuleUIRegistry.h)

SET(CLAY_SRC_FOLDER_XML    xml/ClayXML.h               xml/ClayXML.cpp
                           xml/XMLNodeChildIterator.h  xml/XMLNodeChildIterator.cpp
                           xml/XMLNode.h               xml/XMLNode.cpp
                           xml/XMLDocument.h           xml/XMLDocument.cpp
                           xml/XMLBuffer.h             xml/XMLBuffer.cpp)

#platform specific sources
IF(WIN32)
  SET(CLAY_PLATFORM_SOURCES helper/SharedLibraryImpl_WIN32.cpp helper/SharedLibraryImpl_WIN32.h)
ELSEIF(UNIX)
  SET(CLAY_PLATFORM_SOURCES helper/SharedLibraryImpl_UNIX.cpp helper/SharedLibraryImpl_UNIX.h)
ENDIF()  
                       
SOURCE_GROUP(base     FILES ${CLAY_SRC_FOLDER_BASE})
SOURCE_GROUP(helper   FILES ${CLAY_SRC_FOLDER_HELPER})
SOURCE_GROUP(host     FILES ${CLAY_SRC_FOLDER_HOST})
SOURCE_GROUP(registry FILES ${CLAY_SRC_FOLDER_REG})
SOURCE_GROUP(nugget   FILES ${CLAY_SRC_FOLDER_NUGGET})
SOURCE_GROUP(event    FILES ${CLAY_SRC_FOLDER_EVENT})
SOURCE_GROUP(xml      FILES ${CLAY_SRC_FOLDER_XML})

SET(CLAY_SOURCES ${CLAY_SRC_FOLDER_BASE}
                 ${CLAY_SRC_FOLDER_HELPER}
                 ${CLAY_SRC_FOLDER_HOST}
                 ${CLAY_SRC_FOLDER_REG}
                 ${CLAY_SRC_FOLDER_NUGGET}
                 ${CLAY_SRC_FOLDER_XML}
			     ${CLAY_SRC_FOLDER_EVENT}
                 ${CLAY_PLATFORM_SOURCES})

INCLUDE_DIRECTORIES(${CLAY_DEFAULT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CLAY_BOOST_INCLUDE} 
                    ${CLAY_XERCES_INCLUDE} 
                    . 
                    ..)                
					
LINK_DIRECTORIES(${CLAY_XERCES_LINK_PATH})
                    
SET(CLAY_LIBS ${CLAY_XERCES_LIB})  
			
ADD_LIBRARY(${CLAY_TARGET} SHARED ${CLAY_SOURCES})

TARGET_LINK_LIBRARIES(${CLAY_TARGET} ${CLAY_LIBS})  