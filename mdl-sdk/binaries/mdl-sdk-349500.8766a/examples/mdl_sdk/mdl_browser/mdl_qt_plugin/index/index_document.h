/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief Document for an inverse index search structure.


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_DOCUMENT_BASE_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_DOCUMENT_BASE_H

#include <vector>
#include <string>

class Tokenizer;

class Index_document
{
public:
    // word-rating pair to be returned by get_words and in the index class.
    typedef std::vector<std::pair<std::string, float>> word_list;

    explicit Index_document();
    virtual ~Index_document() = default;

    // the words to be indexed for this document
    virtual word_list get_words(const Tokenizer* tokenizer) const = 0;

    // identifier of this document that is used in the posting list of the index.
    virtual uint32_t get_document_id() const;

protected:

    // helper function to be used in get_words.
    // allows to add multiple words with a certain ranking to the word_list.
    static void insert(std::vector<std::pair<std::string, float>>& destination,
                       const std::vector<std::string>& source, 
                       float ranking);

private:
    // As we build up the index on startup, we can use a simple running index
    // to identify documents. In case the index should be saved to disk, you
    // need to something more sophisticated or swtich to a real data base.
    static uint32_t s_id_counter;

    const uint32_t m_id;
};

#endif