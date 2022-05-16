/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

// examples/mdl_sdk/shared/utils/profiling.h
//
// Code shared by all examples

#ifndef MDL_D3D12_PROFILING_H
#define MDL_D3D12_PROFILING_H

#include <chrono>
#include <string>


namespace mi { namespace examples { namespace profiling
{
    struct Timing
    {
        explicit Timing(std::string operation)
            : m_operation(operation)
        {
            m_start = std::chrono::steady_clock::now();
        }

        ~Timing()
        {
            auto stop = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed_seconds = stop - m_start;
            printf("Finished '%s' after %f seconds.\n",
                m_operation.c_str(), elapsed_seconds.count());
        }

    private:
        std::string m_operation;
        std::chrono::steady_clock::time_point m_start;
    };
} } } // mi::examples::profiling

#endif  // MDL_D3D12_PROFILING_H
