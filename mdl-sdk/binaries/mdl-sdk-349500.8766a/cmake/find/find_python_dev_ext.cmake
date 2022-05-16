#*****************************************************************************
# Copyright 2022 NVIDIA Corporation. All rights reserved.
#*****************************************************************************

function(FIND_PYTHON_DEV_EXT)
    if(NOT PYTHON_DIR)
        set(PYTHON_DIR "PYTHON_DIR-NOTFOUND" CACHE PATH "Directory that contains the python dev library and the corresponding headers.")
    endif()
    #-----------------------------------------------------------------------------------------------

    # use the cmake build-in find script (requires 3.12)
    if(EXISTS ${PYTHON_DIR})
        # message(STATUS " Try forcing PYTHON_DIR: ${PYTHON_DIR}")
        set(Python_ROOT_DIR ${PYTHON_DIR})
    endif()
    find_package (Python COMPONENTS Interpreter Development)

    # use the found interpreter as the default python interpreter tool
    if(TARGET Python::Interpreter)
        # message(STATUS " Found Python::Interpreter")
        # message(STATUS " Python_EXECUTABLE: ${Python_EXECUTABLE}")
        # message(STATUS " Python_VERSION: ${Python_VERSION}")
        if (NOT python_PATH)
            set(python_PATH ${Python_EXECUTABLE} CACHE FILEPATH "Path of the Python 2.7+ binary." FORCE)
        endif()
    else()
        message(WARNING " Failed to find Python::Interpreter")
    endif()

    # python dev
    if(TARGET Python::Python)
        # message(STATUS " Found Python::Python")
        # message(STATUS " Python_INCLUDE_DIRS: ${Python_INCLUDE_DIRS}")
        # message(STATUS " Python_LIBRARY_DIRS: ${Python_LIBRARY_DIRS}")
        # message(STATUS " Python_LIBRARIES: ${Python_LIBRARIES}")
        # message(STATUS " Python_LIBRARY_RELEASE: ${Python_LIBRARY_RELEASE}")
        # message(STATUS " Python_RUNTIME_LIBRARY_DIRS: ${Python_RUNTIME_LIBRARY_DIRS}")
        get_filename_component(_PYTHON_DIR ${Python_INCLUDE_DIRS} DIRECTORY)
        set(PYTHON_DIR ${_PYTHON_DIR} CACHE PATH "Directory that contains the python dev library and the corresponding headers." FORCE)

    else()
        if(LINUX OR MACOSX)
            set(_OS_MESSAGE " install the 'python3-dev' (or 'python2-dev') package or")
        endif()
        message(FATAL_ERROR "The dependency \"python\" could not be resolved. Please${_OS_MESSAGE} specify 'Python_DIR'.")
    endif()

    # store paths that are later used in the add_python.cmake
    set(MDL_DEPENDENCY_PYTHON_DEV_INCLUDE ${Python_INCLUDE_DIRS} CACHE INTERNAL "python headers")
    set(MDL_DEPENDENCY_PYTHON_DEV_LIBS ${Python_LIBRARY_RELEASE} CACHE INTERNAL "python libs")
    set(MDL_DEPENDENCY_PYTHON_DEV_EXE ${Python_EXECUTABLE} CACHE INTERNAL "python interpreter")
    #set(MDL_DEPENDENCY_PYTHON_DEV_SHARED ${Python_SHARED} CACHE INTERNAL "python shared libs")
    set(MDL_PYTHON_DEV_FOUND ON CACHE INTERNAL "")

    if(MDL_LOG_DEPENDENCIES)
        message(STATUS "[INFO] MDL_DEPENDENCY_PYTHON_DEV_INCLUDE:  ${MDL_DEPENDENCY_PYTHON_DEV_INCLUDE}")
        message(STATUS "[INFO] MDL_DEPENDENCY_PYTHON_DEV_LIBS:     ${MDL_DEPENDENCY_PYTHON_DEV_LIBS}")
        message(STATUS "[INFO] MDL_DEPENDENCY_PYTHON_DEV_SHARED:   ${MDL_DEPENDENCY_PYTHON_DEV_SHARED}")
        message(STATUS "[INFO] MDL_DEPENDENCY_PYTHON_DEV_EXE:      ${MDL_DEPENDENCY_PYTHON_DEV_EXE}")
    endif()
endfunction()
