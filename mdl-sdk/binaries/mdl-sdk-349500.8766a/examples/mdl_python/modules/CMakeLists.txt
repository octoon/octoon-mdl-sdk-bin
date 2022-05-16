#*****************************************************************************
# Copyright 2022 NVIDIA Corporation. All rights reserved.
#*****************************************************************************

set(PROJECT_NAME examples-mdl_python-modules)

# create target from template
create_from_python_preset(
    TARGET ${PROJECT_NAME}
    MAIN "example_modules.py"
    SOURCES
        "example_modules.py"
    )
