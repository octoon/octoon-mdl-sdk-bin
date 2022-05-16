#*****************************************************************************
# Copyright 2022 NVIDIA Corporation. All rights reserved.
#*****************************************************************************

# name of the target and the resulting example
set(PROJECT_NAME examples-mdl_sdk-optix7)

# -------------------------------------------------------------------------------------------------
# Main Target
# -------------------------------------------------------------------------------------------------

# collect sources
set(PROJECT_SOURCES
    "optix7_mdl.cpp"
    "optix7_mdl.h"
    "optix7_mdl_helper.h"
    "material_info_helper.h"
    )

set(_GENERATED_SOURCES
    "optix7_mdl_closest_hit_radiance.bc"
    )

# mark files as generated to disable the check for existence during configure
set_source_files_properties(${_GENERATED_SOURCES} PROPERTIES GENERATED TRUE)

# create target from template
create_from_base_preset(
    TARGET ${PROJECT_NAME}
    TYPE EXECUTABLE
    NAMESPACE mdl_sdk
    OUTPUT_NAME "optix7"
    SOURCES
        ${PROJECT_SOURCES}
        ${_GENERATED_SOURCES}
    EXAMPLE
)

# add dependencies
target_add_dependencies(TARGET ${PROJECT_NAME}
    DEPENDS
        mdl::mdl_sdk
        mdl_sdk::shared
        opengl
        imgui
        cuda
        optix7
    )

# creates a user settings file to setup the debugger (visual studio only, otherwise this is a no-op)
target_create_vs_user_settings(TARGET ${PROJECT_NAME})


# -------------------------------------------------------------------------------------------------
# CLANG Build Step
# -------------------------------------------------------------------------------------------------

# get clang
target_add_tool_dependency(TARGET ${PROJECT_NAME} TOOL clang)

# on Linux, we need to set C++11
if(LINUX)
    set(_CLANG_FLAGS --std=c++11)
endif()

# Clang 7 needs CUDA 8 to compile the closest hit shader
set(CUDA8_PATH "NOT-SPECIFIED" CACHE PATH "Directory that contains the CUDA 8 include folder")
if(EXISTS ${CUDA8_PATH}/include/cuda.h)
    set(_CLANG_FLAGS ${_CLANG_FLAGS} --cuda-path=${CUDA8_PATH})
else()
    message(FATAL_ERROR "Please specify CUDA8_PATH which contains \"include/cuda.h\"")
endif()

# generate bytecode for closest hit radiance shader
# Note: the _ALLOW_COMPILER_AND_STL_VERSION_MISMATCH define is needed to allow
#       compilation with Clang 7 and VS2019 headers
add_custom_command(
    OUTPUT
        optix7_mdl_closest_hit_radiance.d
        optix7_mdl_closest_hit_radiance.bc
    COMMAND ${CMAKE_COMMAND} -E echo "Compile optix7_mdl_closest_hit_radiance bytecode using clang..."
    COMMAND ${clang_PATH}
        -I ${OPTIX7_DIR}/include -I ${MDL_INCLUDE_FOLDER}
        -I ${CMAKE_CURRENT_SOURCE_DIR}/../shared
        ${_CLANG_FLAGS}
        -D_ALLOW_COMPILER_AND_STL_VERSION_MISMATCH
        -emit-llvm -c -O3 -ffast-math -fcuda-flush-denormals-to-zero -fno-vectorize
        --cuda-device-only ${CMAKE_CURRENT_SOURCE_DIR}/optix7_mdl_closest_hit_radiance.cu
        -o optix7_mdl_closest_hit_radiance.bc
        -MD -MT optix7_mdl_closest_hit_radiance.bc
        -MP -MF optix7_mdl_closest_hit_radiance.d
    WORKING_DIRECTORY
        ${CMAKE_CFG_INTDIR}
    DEPENDS
        "optix7_mdl_closest_hit_radiance.cu"
        ${PROJECT_SOURCES}
    VERBATIM
)

# -------------------------------------------------------------------------------------------------
# PTX Target
# -------------------------------------------------------------------------------------------------

target_add_cuda_ptx_rule(TARGET ${PROJECT_NAME}
    DEPENDS
        mdl::mdl_sdk
        mdl_sdk::shared
        optix7
    CUDA_SOURCES
        "optix7_mdl.cu"
        "optix7_mdl_closest_hit_radiance.cu"
    FAST_MATH
    )

# -------------------------------------------------------------------------------------------------
# Create installation rules to copy the build directory
# -------------------------------------------------------------------------------------------------
add_target_install(
    TARGET ${PROJECT_NAME}
    DESTINATION "examples/mdl_sdk/optix7"
    )

# -------------------------------------------------------------------------------------------------
# Add tests if available
# -------------------------------------------------------------------------------------------------
add_tests()
