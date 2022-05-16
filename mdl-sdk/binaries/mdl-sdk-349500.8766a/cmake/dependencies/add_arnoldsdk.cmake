#*****************************************************************************
# Copyright 2022 NVIDIA Corporation. All rights reserved.
#*****************************************************************************

# -------------------------------------------------------------------------------------------------
# script expects the following variables:
# - __TARGET_ADD_DEPENDENCY_TARGET
# - __TARGET_ADD_DEPENDENCY_DEPENDS
# - __TARGET_ADD_DEPENDENCY_COMPONENTS
# - __TARGET_ADD_DEPENDENCY_NO_RUNTIME_COPY
# - __TARGET_ADD_DEPENDENCIES_NO_RUNTIME
# - __TARGET_ADD_DEPENDENCIES_NO_LINKING
# - __TARGET_ADD_DEPENDENCIES_NO_INCLUDE
# -------------------------------------------------------------------------------------------------

# the dependency consists of a set of packages

# assuming the find_* script was successful
# if not, this is an error case. The corresponding project should not have been selected for build.
if(NOT MDL_BUILD_ARNOLD_PLUGIN)
    message(FATAL_ERROR "The dependency \"${__TARGET_ADD_DEPENDENCY_DEPENDS}\" for target \"${__TARGET_ADD_DEPENDENCY_TARGET}\" could not be resolved.")
    return()
endif()

# headers
if(NOT __TARGET_ADD_DEPENDENCIES_NO_INCLUDE)
    target_include_directories(${__TARGET_ADD_DEPENDENCY_TARGET} 
        PRIVATE
            ${MDL_DEPENDENCY_ARNOLDSDK_INCLUDE}
        )
endif()

# static library
if(NOT __TARGET_ADD_DEPENDENCIES_NO_LINKING AND MDL_DEPENDENCY_ARNOLDSDK_LIBS)
    target_link_libraries(${__TARGET_ADD_DEPENDENCY_TARGET} 
        PRIVATE
            ${MDL_DEPENDENCY_ARNOLDSDK_LIBS}
        )
endif()

# runtime dependencies
if(NOT __TARGET_ADD_DEPENDENCY_NO_RUNTIME AND MDL_DEPENDENCY_ARNOLDSDK_BIN)

    if(WINDOWS)
        target_add_vs_debugger_env_path(TARGET ${__TARGET_ADD_DEPENDENCY_TARGET}
            PATHS 
                ${MDL_DEPENDENCY_ARNOLDSDK_BIN}
            )
    endif()

    # on linux, the user has to setup the LD_LIBRARY_PATH when running examples
    # on mac, DYLD_LIBRARY_PATH, respectively.
endif()
