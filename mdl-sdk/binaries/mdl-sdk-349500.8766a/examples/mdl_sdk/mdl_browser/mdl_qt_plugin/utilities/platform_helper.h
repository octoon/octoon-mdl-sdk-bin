/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief Helper class for handling platform specific operations.


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_PLATFORM_HELPER_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_PLATFORM_HELPER_H

#include <string>
#include <functional>
#include <vector>

class Platform_helper
{
public:
    // get date of the last modification of a file
    static time_t get_file_change_time(const std::string& path);

    // get the current point in time for measuring time differences in seconds
    static double get_time();

    // measure the time required to run some function
    static double tic_toc(const std::function<void()>& action);

    // measure the time required to run some function and print it to the log
    static double tic_toc_log(const std::string& name, const std::function<void()>& action);
};

#endif
