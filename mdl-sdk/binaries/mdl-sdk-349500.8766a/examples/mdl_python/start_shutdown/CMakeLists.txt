#*****************************************************************************
# Copyright 2022 NVIDIA Corporation. All rights reserved.
#*****************************************************************************

set(PROJECT_NAME examples-mdl_python-start_shutdown)

# create target from template
create_from_python_preset(
    TARGET ${PROJECT_NAME}
    MAIN "example_start_shutdown.py"
    SOURCES
        "example_start_shutdown.py"
    )
