#!/bin/bash

# Example script to test the Arnold MDL plugin on Linux.
#
# Assumes a Makefile-based build, the script has not yet been adapted to also
# handle a cmake-based build.

# Default to MI_OBJ_G if MI_OBJ is not set
if [ -z $MI_OBJ ]; then
    MI_OBJ=$MI_OBJ_G
fi

# Check MI_SRC
if [ -z $MI_SRC ]; then
    echo Error: MI_SRC not set
    exit 1
fi
# Check MI_OBJ/MI_OBJ_G
if [ -z $MI_OBJ ]; then
    echo Error: MI_OBJ and MI_OBJ_G not set
    exit 1
fi

# Path to the kick binary
export PATH=/h/misrc/neuray/thirdparty/arnoldsdk/6.2.0.1/linux-x86-64/bin:$PATH

# Path to libai.so
export LD_LIBRARY_PATH=/h/misrc/neuray/thirdparty/arnoldsdk/6.2.0.1/linux-x86-64/bin:$LD_LIBRARY_PATH

# Path to libmdl_arnold.so
export ARNOLD_PLUGIN_PATH=$MI_OBJ/prod/lib/mdl_arnold

# MDL search path
export MDL_PATHS=$MI_SRC/prod/lib/mdl_arnold/example_scene/mdl

# Create symlink for libmdl_sdk.so and add to LD_LIBRARY_PATH
cd $MI_OBJ/prod/lib/mdl_sdk
ln -sf libmdl_sdk.so libmdl_sdk_ai.so
export LD_LIBRARY_PATH=$PWD:$LD_LIBRARY_PATH

# Create symlink for FreeImage plugin and add to LD_LIBRARY_PATH
cd $MI_OBJ/shaders/plugin/freeimage
ln -sf nv_freeimage.so nv_freeimage_ai.so
export LD_LIBRARY_PATH=$PWD:$LD_LIBRARY_PATH

# Create symlink for DDS plugin and add to LD_LIBRARY_PATH
cd $MI_OBJ/shaders/plugin/dds
ln -sf dds.so dds_ai.so
export LD_LIBRARY_PATH=$PWD:$LD_LIBRARY_PATH

# Options
# -device %s          Rendering device (cpu|gpu)
# -dw                 Disable render window (recommended for batch rendering)
# -dp                 Disable progressive rendering (recommended for batch rendering)
# -nokeypress         Disable wait for ESC keypress after rendering to display window

# Run
kick -t 0 -v 4 -ipr q -nostdin -i $MI_SRC/prod/lib/mdl_arnold/example_scene/cornell_single.ass
