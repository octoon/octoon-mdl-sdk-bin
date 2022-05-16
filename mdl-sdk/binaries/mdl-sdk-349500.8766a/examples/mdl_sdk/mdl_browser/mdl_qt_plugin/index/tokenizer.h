/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief Spits a text string into single words and applies certain normalization rules to limit 
///        the size of the index and thereby to increase search performance


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_TOKENIZER_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_TOKENIZER_H

#include <vector>
#include <string>
#include <set>

// spits a text string into single words and applies certain normalization rules to limit the size
// of the index and thereby to increase search performance
class Tokenizer
{
public:
    explicit Tokenizer();
    virtual ~Tokenizer() = default;

    // normalization applied to the entire text
    // this typically splits concatenated words, e.g., by replacing underscores with spaces
    virtual std::string normalize_sentence(const std::string& input) const;

    // normalization applied to single words
    // removes invalid characters, to lower case, ..., potentially stemming
    virtual std::string normalize_word(const std::string& input, 
                                       bool allow_operators = false) const;

    // splits an input text into tokens that appear in the index
    // runs normalization and excludes very common words using black listing
    virtual std::vector<std::string> tokenize(const std::string& input, 
                                              bool allow_operators = false) const;

    // some characters, e.g., the minus, are important to interpret the search query
    // this simply checks the a character if it is such a character
    bool is_operator(char character) const;

private:
    std::set<std::string> m_word_blacklist;
    std::vector<char> m_operators;
    size_t m_min_token_length;
};

#endif