#*****************************************************************************
# Copyright 2022 NVIDIA Corporation. All rights reserved.
#*****************************************************************************

function(FIND_OPTIX7_EXT)

    if(NOT MDL_ENABLE_OPTIX7_EXAMPLES)
        message(WARNING "Examples that require OPTIX7 are disabled. Enable the option 'MDL_ENABLE_OPTIX7_EXAMPLES' and resolve the required dependencies to re-enable them.")
        return()
    endif()

    set(OPTIX7_DIR "NOT-SPECIFIED" CACHE PATH "Directory that contains the OptiX 7 include folder.")

    if(EXISTS ${OPTIX7_DIR}/include/optix.h)
        set(MDL_OPTIX7_FOUND ON CACHE INTERNAL "")
    else()
        message(STATUS "OPTIX7_DIR: ${OPTIX7_DIR}")
        message(FATAL_ERROR "The dependency \"OptiX7\" could not be resolved. Please specify 'OPTIX7_DIR' or disbale 'MDL_ENABLE_OPTIX7_EXAMPLES'.")
    endif()

    if(MDL_LOG_DEPENDENCIES)
        message(STATUS "[INFO] OPTIX7_DIR:                         ${OPTIX7_DIR}")
    endif()

endfunction()
