/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

// Code shared by CPU and GPU baking

#ifndef EXAMPLE_DISTILLING_UNITY_H
#define EXAMPLE_DISTILLING_UNITY_H

// Type of expression being baked
enum Expression_type
{
    EXT_UNKNOWN = 0
    , EXT_BASE_COLOR
    , EXT_TRANSPARENCY
    , EXT_METALLIC
    , EXT_ROUGHNESS
    , EXT_OPACITY
};

#endif // EXAMPLE_DISTILLING_UNITY_H

