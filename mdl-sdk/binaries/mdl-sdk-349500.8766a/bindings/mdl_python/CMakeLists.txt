#*****************************************************************************
# Copyright 2022 NVIDIA Corporation. All rights reserved.
#*****************************************************************************

# name of the target and the resulting library
set(PROJECT_NAME prod-lib-mdl_python)

# collect sources
set(PROJECT_SOURCES
    "generated/mdl_python_swig.cpp"
    "mdl_python.cpp"
    "mdl_python.h"
    )

# create target from template
create_from_base_preset(
    TARGET ${PROJECT_NAME}
    TYPE SHARED
    DYNAMIC_MSVC_RUNTIME
    SOURCES
        ${PROJECT_SOURCES}
    )

# customize name
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "_pymdlsdk")
if(WINDOWS)
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".pyd")
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".so")
endif()

# target alias for the custom name
add_library(mdl_python ALIAS ${PROJECT_NAME})
add_library(mdl::mdl_python ALIAS ${PROJECT_NAME})

# add dependencies
target_add_dependencies(TARGET ${PROJECT_NAME}
    DEPENDS
        mdl::mdl_sdk
        mdl_sdk::shared
        python_dev
    )

# disable some warnings in generated code
if(UNIX AND NOT APPLE)
    target_compile_options(${PROJECT_NAME}
        PUBLIC
            -Wno-delete-non-virtual-dtor
    )
endif()

# copy content
# the generated python stubs
target_add_content(TARGET ${PROJECT_NAME}
    FILE_BASE "generated/"
    FILES
        "pymdlsdk.py"
    )
source_group("generated" FILES "generated/pymdlsdk.py")

# add tests if available
add_tests(POST)
