/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief Helper class for handling string operations.


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_STRING_HELPER_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_STRING_HELPER_H

#include <string>
#include <vector>

class String_helper
{
public:

    // replaces all occurrences of a substrings by another one
    std::string static replace(const std::string& input,
                               const std::string& old, 
                               const std::string& with);

    // replaces all occurrences of a character by another one
    static std::string replace(const std::string& input,
                               char old, char with);

    // splits a string into chunks based on a separator character 
    static std::vector<std::string> split(const std::string& input, char sep);

    // splits a string into chunks based on a separator substring 
    static std::vector<std::string> split(const std::string& input, const std::string& sep);
};

#endif