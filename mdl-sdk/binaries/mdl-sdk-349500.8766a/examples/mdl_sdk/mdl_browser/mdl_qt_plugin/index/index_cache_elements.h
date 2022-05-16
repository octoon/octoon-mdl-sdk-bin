/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief inverse index build of cache elements.
///        allows to search for materials in modules based on available info, 
///        e.g., from annotations


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_INDEX_MDL_CACHE_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_INDEX_MDL_CACHE_H

#include "index.h"
#include "index_document_cache_element.h"

class Mdl_cache;

// inverse index build of cache elements.
// allows to search for materials in modules based on available info, e.g., from annotations
class Index_cache_elements : public Index<Index_document_cache_element>
{
public:
    // constructs the cache index from the cache.
    // therefore, each element in the cache is treated as document
    bool build(const Mdl_cache* cache);
};

#endif