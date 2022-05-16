#*****************************************************************************
# Copyright 2022 NVIDIA Corporation. All rights reserved.
#*****************************************************************************

# create Visual Studio code launch.json
get_property(BINDING_MODULE_PATH TARGET "mdl::mdl_python" PROPERTY BINARY_DIR)

# Platform depended symbols and keywords
if(WINDOWS)
    set(ENV_SEP ";")
    set(ENV_LIB_PATH "PATH")
elseif(MACOSX)
    set(ENV_SEP ":")
    set(ENV_LIB_PATH "DYLD_LIBRARY_PATH")
else() # LINUX
    set(ENV_SEP ":")
    set(ENV_LIB_PATH "LD_LIBRARY_PATH")
endif()

# Depending on the product and it's distribution we need set paths and enviroment variables slightly different
if("MDL_BINARY_RELEASE" IN_LIST MDL_ADDITIONAL_COMPILER_DEFINES)
    if(MDL_INTERNAL_BUILD)
        set(_VS_CODE_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/cmake_templates/mdl_python_examples.examples_internal.code-workspace)
        set(_SETUP_ENV_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/cmake_templates/setup_env.examples_internal.sh)
    else()
        set(_VS_CODE_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/cmake_templates/mdl_python_examples.binary_release.code-workspace)
        set(_SETUP_ENV_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/cmake_templates/setup_env.binary_release.sh)
    endif()
elseif ("MDL_SOURCE_RELEASE" IN_LIST MDL_ADDITIONAL_COMPILER_DEFINES)
    get_property(MDL_SDK_PATH TARGET "mdl::mdl_sdk" PROPERTY BINARY_DIR)
    get_property(FREEIMAGE_PATH TARGET "mdl::nv_freeimage" PROPERTY BINARY_DIR)
    set(ENV_MDL_SDK_PATH ${MDL_SDK_PATH})
    set(ENV_FREEIMAGE_PATH ${FREEIMAGE_PATH})
    if(WINDOWS)
        # for running the shell scripts on windows (e.g. using git bash or mingw)
        # we need to adjust the strings that go into the PATH to use a unix-like format
        string(SUBSTRING ${MDL_SDK_PATH} 1 1 _CHAR_AT_1)
        if(_CHAR_AT_1 STREQUAL ":")
            string(SUBSTRING ${MDL_SDK_PATH} 0 1 _CHAR_AT_0)
            string(SUBSTRING ${MDL_SDK_PATH} 2 -1 ENV_MDL_SDK_PATH)
            set(ENV_MDL_SDK_PATH "/${_CHAR_AT_0}${ENV_MDL_SDK_PATH}")
        endif()
        string(SUBSTRING ${FREEIMAGE_PATH} 1 1 _CHAR_AT_1)
        if(_CHAR_AT_1 STREQUAL ":")
            string(SUBSTRING ${FREEIMAGE_PATH} 0 1 _CHAR_AT_0)
            string(SUBSTRING ${FREEIMAGE_PATH} 2 -1 ENV_FREEIMAGE_PATH)
            set(ENV_FREEIMAGE_PATH "/${_CHAR_AT_0}${ENV_FREEIMAGE_PATH}")
        endif()
    endif()
    set(_VS_CODE_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/cmake_templates/mdl_python_examples.open_source.code-workspace)
    set(_SETUP_ENV_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/cmake_templates/setup_env.open_source.sh)
endif()

# create the IDE or shell support files
get_property(_GENERATOR_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(_GENERATOR_IS_MULTI_CONFIG)
    foreach(_CONFIG ${CMAKE_CONFIGURATION_TYPES})
        set(_MI_OBJ ${MI_OBJ_${_CONFIG}})
        make_directory(${CMAKE_CURRENT_BINARY_DIR}/env/${_CONFIG})
        configure_file(${_SETUP_ENV_FILE_IN} ${CMAKE_CURRENT_BINARY_DIR}/env/${_CONFIG}/setup_env.sh @ONLY)
    endforeach()
else()
    set(_CONFIG ${CMAKE_BUILD_TYPE})
    set(_MI_OBJ ${MI_OBJ_${_CONFIG}})
    make_directory(${CMAKE_CURRENT_BINARY_DIR}/env/${_CONFIG})
    configure_file(${_SETUP_ENV_FILE_IN} ${CMAKE_CURRENT_BINARY_DIR}/env/${_CONFIG}/setup_env.sh @ONLY)
endif()
configure_file(${_VS_CODE_FILE_IN} ${CMAKE_BINARY_DIR}/mdl_python_examples.code-workspace @ONLY)
