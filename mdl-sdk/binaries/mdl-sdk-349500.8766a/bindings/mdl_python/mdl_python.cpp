/***************************************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 **************************************************************************************************/

#include "example_shared.h"
#include "mdl_python.h"

mi::neuraylib::INeuray* load_and_get_ineuray(const char* filename)
{
    if (!filename || strlen(filename) == 0)
        return mi::examples::mdl::load_and_get_ineuray();

    std::string lib = filename;
    if (!mi::examples::strings::ends_with(lib, MI_BASE_DLL_FILE_EXT))
        lib += MI_BASE_DLL_FILE_EXT;

    return mi::examples::mdl::load_and_get_ineuray(lib.c_str());
}

bool load_plugin(mi::neuraylib::INeuray* neuray, const char* filename)
{
    if (!neuray || !filename)
        return false;

    std::string lib = filename;
    if (!mi::examples::strings::ends_with(lib, MI_BASE_DLL_FILE_EXT))
        lib += MI_BASE_DLL_FILE_EXT;

    return mi::examples::mdl::load_plugin(neuray, lib.c_str()) == 0;
}

bool unload()
{
    return mi::examples::mdl::unload();
}
