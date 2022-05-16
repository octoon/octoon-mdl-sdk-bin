#*****************************************************************************
# Copyright 2022 NVIDIA Corporation. All rights reserved.
#*****************************************************************************

# -------------------------------------------------------------------------------------------------
# Create installion rules to copy the example directory
# -------------------------------------------------------------------------------------------------

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/nvidia/sdk_examples
    DESTINATION "examples/mdl/nvidia"
    USE_SOURCE_PERMISSIONS
    FILES_MATCHING
    PATTERN "*"
    )