/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/


#include "string_helper.h"
#include <algorithm>

std::string String_helper::replace(const std::string& input,
    const std::string& old, const std::string& with)
{
    if (input.empty()) return input;

    std::string sentance(input);
    size_t offset(0);
    size_t pos(0);
    while (pos != std::string::npos)
    {
        pos = sentance.find(old, offset);
        if (pos == std::string::npos)
            break;

        sentance.replace(pos, old.length(), with);
        offset = pos + with.length();
    }
    return sentance;
}

std::string String_helper::replace(const std::string& input,
    char old, char with)
{
    std::string output(input);
    std::replace(output.begin(), output.end(), old, with);
    return output;
}

std::vector<std::string> String_helper::split(const std::string& input, char sep)
{
    std::vector<std::string> chunks;

    size_t offset(0);
    size_t pos(0);
    while (pos != std::string::npos)
    {
        pos = input.find(sep, offset);

        if (pos == std::string::npos)
        {
            chunks.push_back(input.substr(offset));
            break;
        }

        chunks.push_back(input.substr(offset, pos - offset));
        offset = pos + 1;
    }
    return chunks;
}

std::vector<std::string> String_helper::split(const std::string& input, const std::string& sep)
{
    std::vector<std::string> chunks;

    size_t offset(0);
    size_t pos(0);
    while (pos != std::string::npos)
    {
        pos = input.find(sep, offset);

        if (pos == std::string::npos)
        {
            chunks.push_back(input.substr(offset));
            break;
        }

        chunks.push_back(input.substr(offset, pos - offset));
        offset = pos + sep.length();
    }
    return chunks;
}
