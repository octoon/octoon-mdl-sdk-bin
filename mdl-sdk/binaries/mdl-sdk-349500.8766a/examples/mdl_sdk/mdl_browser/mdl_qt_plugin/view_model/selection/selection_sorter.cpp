/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/


#include "selection_sorter.h"
#include "../../cache/imdl_cache.h"

Selection_sorter_base::Selection_sorter_base(const std::string& name, bool ascending)
    : m_name(name)
    , m_ascending(ascending)
{
}

float Selection_sorter_ranking::compare(const VM_sel_element* left, 
                                        const VM_sel_element* right) const
{
    return left->get_search_ranking() - right->get_search_ranking();
}

float Selection_sorter_name::compare(const VM_sel_element* left, 
                                     const VM_sel_element* right) const
{
    QString left_name = left->get_display_name();
    if (left_name.isEmpty()) left_name = left->get_name();

    QString right_name = right->get_display_name();
    if (right_name.isEmpty()) right_name = right->get_name();

    return static_cast<float>(QString::compare(left_name, right_name, Qt::CaseInsensitive));
}

float Selection_sorter_date::compare(const VM_sel_element* left, const VM_sel_element* right) const
{
    const auto parent_module_left = left->get_parent_module_cache_element();
    const auto parent_module_right = right->get_parent_module_cache_element();

    return static_cast<float>(static_cast<int64_t>(parent_module_left->get_timestamp())
           - static_cast<int64_t>(parent_module_right->get_timestamp()));
}
