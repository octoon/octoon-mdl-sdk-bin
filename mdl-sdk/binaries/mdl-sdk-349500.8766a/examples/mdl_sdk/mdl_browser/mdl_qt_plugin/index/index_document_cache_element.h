/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief indexable document that is created from an element of the cache that stores 
///        informations about materials, functions, ...


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_DOCUMENT_MDL_ELEMENT_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_DOCUMENT_MDL_ELEMENT_H

#include "index_document.h"

class IMdl_cache_element;
class Tokenizer;

// indexable document that is created from an element of the cache that stores informations about 
// materials, functions, ...
class Index_document_cache_element : public Index_document
{
public:
    explicit Index_document_cache_element(const IMdl_cache_element* cache_item);
    virtual ~Index_document_cache_element() = default;

    // the words to be indexed for this document
    word_list get_words(const Tokenizer* tokenizer) const override;

    // the cache element this documented is created from
    const IMdl_cache_element* get_cache_element() const { return m_cache_element; }

private:
    const IMdl_cache_element* m_cache_element;
};

#endif
