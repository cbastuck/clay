find_package(PythonLibs)

if(NOT PYTHONLIBS_FOUND)
    set(PYTHON_INCLUDE_DIR ${CLAY_EXTERNALS_DIR}/python/Python-2.7.5/Include)
    set(PYTHON_LIBRARY ${CLAY_EXTERNALS_DIR}/python/Python-2.7.5/PC)
    find_package(PythonLibs REQUIRED)
endif()

if(PYTHONLIBS_FOUND)
	SET(CLAY_PYTHON_INCLUDE ${PYTHON_INCLUDE_DIRS}) 
	SET(CLAY_PYTHON_LIBPATH ${PYTHON_LIBRARIES})
    MESSAGE("Python found at: " ${CLAY_PYTHON_INCLUDE} " and "  ${CLAY_PYTHON_LIBPATH})
else() 
    MESSAGE("Could not find Python")
endif()

LINK_DIRECTORIES(${LINK_DIRECTORIES} ${CLAY_PYTHON_LIBPATH}) 