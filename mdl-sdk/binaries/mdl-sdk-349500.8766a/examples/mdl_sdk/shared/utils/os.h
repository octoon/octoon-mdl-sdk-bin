/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

 // examples/mdl_sdk/shared/utils/os.h
 //
 // Code shared by all examples

#ifndef EXAMPLE_SHARED_UTILS_OS_H
#define EXAMPLE_SHARED_UTILS_OS_H

#include <fstream>
#include <algorithm>
#include <sstream>

#include <mi/base/config.h>
#include "strings.h"

#ifdef MI_PLATFORM_WINDOWS
    #include <windows.h>
    #include <commdlg.h>
    #include <direct.h>
    #include <Shlobj.h>
#else
    #include <dlfcn.h>
    #include <unistd.h>
    #include <dirent.h>
#endif

namespace mi { namespace examples { namespace os
{
    // Returns the value of the given environment variable.
    //
    // \param env_var   environment variable name
    // \return          the value of the environment variable or an empty string
    //                  if that variable does not exist or does not have a value.
    inline std::string get_environment(const char* env_var)
    {
        std::string value = "";
        #ifdef MI_PLATFORM_WINDOWS
            char* buf = nullptr;
            size_t sz = 0;
            if (_dupenv_s(&buf, &sz, env_var) == 0 && buf != nullptr) {
                value = buf;
                free(buf);
            }
        #else
            const char* v = getenv(env_var);
            if (v)
                value = v;
        #endif
            return value;
    }

    //---------------------------------------------------------------------------------------------

    // Sets the value of the given environment variable.
    //
    // \param env_var   environment variable name
    // \param value     the new value to set.
    inline bool set_environment(const char* env_var, const char* value)
    {
        #ifdef MI_PLATFORM_WINDOWS
            return 0 == _putenv_s(env_var, value);
        #else
            return 0 == setenv(env_var, value, 1);
        #endif
    }

}}}
#endif
