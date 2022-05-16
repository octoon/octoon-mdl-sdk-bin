/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/


#include "index_cache_elements.h"
#include "../cache/imdl_cache.h"
#include "../cache/mdl_cache.h"
#include <iostream>


bool build_recursively(Index_cache_elements* index, const IMdl_cache_node* node)
{
    bool success = true;


    for (mi::Size i = 0, n = node->get_child_count(); i<n; ++i)
    {
        // nodes that can have "grand" children  (packages only)
        const IMdl_cache_item* child_item = node->get_child(*node->get_child_key(i));
        const auto child_node = dynamic_cast<const IMdl_cache_node*>(child_item);
        if (child_node)
        {
            build_recursively(index, child_node);
            continue;
        }

        // elements that can be selected
        const auto child_element = dynamic_cast<const IMdl_cache_element*>(child_item);
        if (child_element)
        {
            index->add_document(new Index_document_cache_element(child_element));
            continue;
        }

        std::cerr << "[Index_mdl_cache] build_recursively: missing case.\n";
        success = false;
        break;
    }
    return success;
}

bool Index_cache_elements::build(const Mdl_cache* cache)
{
    const IMdl_cache_package* root = cache->get_cache_root();
    return build_recursively(this, root);
}
