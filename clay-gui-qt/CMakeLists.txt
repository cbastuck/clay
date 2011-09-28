cmake_minimum_required(VERSION 2.8)

SET(CLAY_TARGET clay-gui-qt)

cmake_policy(SET CMP0015 OLD)

PROJECT(${CLAY_TARGET})

MESSAGE("Invoke cmake for: ${CLAY_TARGET} with build type: ${CMAKE_BUILD_TYPE} ...")

FIND_PACKAGE(ClayQt4 COMPONENTS QtCore QtGui REQUIRED)
INCLUDE(${QT_USE_FILE})

SET(CLAY_SRC_FOLDER_BASE base/QtModuleUIRegistry.cpp      base/QtModuleUIRegistry.h
                         base/ModuleWidget.cpp            base/ModuleWidget.h 
                         base/ModuleWidgetHelper.h
                         base/QtIOWidget.cpp              base/QtIOWidget.h
                         base/QtListDialog.cpp            base/QtListDialog.h
                         base/QtViewer.cpp                base/QtViewer.h
                         base/QtViewerMainWindow.cpp      base/QtViewerMainWindow.h
                         base/QtViewerWorkspace.cpp       base/QtViewerWorkspace.h)

SET(CLAY_SRC_FOLDER_HOST modules/HostModuleUI/HostModuleUI.cpp 
                         modules/HostModuleUI/HostModuleUI.h
                         modules/HostModuleUI/HostModuleUIInternalView.h
					     modules/HostModuleUI/HostModuleUIInternalView.cpp)					   
					                                           
SET(CLAY_SRC_FOLDER_MISC modules/MatrixModelViewer/MatrixViewer.h)
				   
SET(CLAY_MOC_FILES base/ModuleWidget.h
                   base/QtViewerMainWindow.h
                   modules/HostModuleUI/HostModuleUI.h
				   modules/HostModuleUI/HostModuleUIInternalView.h)

QT4_WRAP_CPP(CLAY_MOC_OUTFILES ${CLAY_MOC_FILES})

SET(CLAY_UIC_FILES base/ModuleWidgetUI.ui
                   base/QListDialog.ui
                   base/QtViewerMainWindow.ui
                   base/ModuleWidgetUIConnector.ui
                   modules/HostModuleUI/HostModuleUIInternalViewQt.ui)

SOURCE_GROUP(base  FILES ${CLAY_SRC_FOLDER_BASE})
SOURCE_GROUP(host  FILES ${CLAY_SRC_FOLDER_HOST})
SOURCE_GROUP(moc   FILES ${CLAY_MOC_FILES})
SOURCE_GROUP(uic   FILES ${CLAY_UIC_FILES})
SOURCE_GROUP(misc  FILES ${CLAY_SRC_FOLDER_MISC})

QT4_WRAP_UI(CLAY_UI_OUTFILES ${CLAY_UIC_FILES})
               
INCLUDE_DIRECTORIES(${CLAY_DEFAULT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CLAY_BOOST_INCLUDE}
                    ${CLAY_VST_SDK_INCLUDE}
                    ${CLAY_LUA_INCLUDE}
                    ${CLAY_XERCES_INCLUDE}
                    ${CLAY_COIN_INCLUDE}
                    ${QT_INCLUDE_DIR}
                    ..)    
                   
SET(CLAY_SOURCES ${CLAY_MOC_OUTFILES} 
                 ${CLAY_UIC_OUTFILES}
                 ${CLAY_SRC_FOLDER_BASE}
                 ${CLAY_SRC_FOLDER_HOST}
			     ${CLAY_SRC_FOLDER_MISC})

QT4_ADD_RESOURCES(CLAY_COMPILED_RESOURCES resources/gui.qrc)

ADD_LIBRARY(${CLAY_TARGET} 
			${CLAY_UI_OUTFILES}
            ${CLAY_SOURCES}
            ${CLAY_COMPILED_RESOURCES})

