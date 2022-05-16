/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/


#include "index_document.h"

void Index_document::insert(std::vector<std::pair<std::string, float>>& destination, 
                            const std::vector<std::string>& source, float ranking)
{
    for (const auto& w : source)
        destination.push_back({w, ranking});
}

uint32_t Index_document::s_id_counter = 0;

Index_document::Index_document() : 
    m_id(++s_id_counter)
{
}


uint32_t Index_document::get_document_id() const
{
    return m_id;
}