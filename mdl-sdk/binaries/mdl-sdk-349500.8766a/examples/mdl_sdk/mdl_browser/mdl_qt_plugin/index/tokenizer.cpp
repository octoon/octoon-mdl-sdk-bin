/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/


#include "tokenizer.h"
#include <algorithm>
#include <string>
#include "../utilities/string_helper.h"


Tokenizer::Tokenizer() : 
    m_min_token_length(3)
{
    // example list.. 
    m_word_blacklist.insert("a");
    m_word_blacklist.insert("be");
    m_word_blacklist.insert("and");
    m_word_blacklist.insert("are");
    m_word_blacklist.insert("by");
    m_word_blacklist.insert("for");
    m_word_blacklist.insert("in");
    m_word_blacklist.insert("is");
    m_word_blacklist.insert("it");
    m_word_blacklist.insert("of");
    m_word_blacklist.insert("on");
    m_word_blacklist.insert("so");
    m_word_blacklist.insert("the");
    m_word_blacklist.insert("that");
    m_word_blacklist.insert("this");
    m_word_blacklist.insert("to");
    m_word_blacklist.insert("with");

    m_operators.push_back('+');
    m_operators.push_back('-');
    m_operators.push_back('&');
    m_operators.push_back('|');
}


std::string Tokenizer::normalize_sentence(const std::string& input) const
{
    
    std::string sentence = String_helper::replace(input, "::", " "); // treat separator as space
    sentence = String_helper::replace(sentence, '_', ' '); // treat underscore as space
    return sentence;
}


std::string Tokenizer::normalize_word(const std::string& input, bool allow_operators) const
{
    // to lower
    std::string word(input);
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);

    // filter characters
    for (int i = static_cast<int>(word.size()) - 1; i >= 0; --i)
    {
        if(allow_operators && i==0 && is_operator(word[0]))
            continue;

        if ((word[i] < 'a' || word[i] > 'z') &&   // allow a-z
            (word[i] < '0' || word[i] > '9'))     // allow digits
                word.erase(i, 1);
    }

    // here you could add stemming
    return word;
}

std::vector<std::string> Tokenizer::tokenize(const std::string& input, bool allow_operators) const
{
    std::vector<std::string> words;

    // normalize sentence
    std::string sentence = normalize_sentence(input);

    size_t offset(0);
    size_t pos(0);
    while (pos != std::string::npos)
    {
        pos = sentence.find(' ', offset); // split at space

        std::string token = sentence.substr(offset, pos - offset);
        token = normalize_word(token, allow_operators);

        // filter words
        if ((token.length() >= m_min_token_length) &&                   // skip to short words
            (m_word_blacklist.find(token) == m_word_blacklist.end()))   // skip words on black list
                words.push_back(token);
        
        offset = pos + 1;
    }

    return words;
}


bool Tokenizer::is_operator(char character) const
{
    return std::find(m_operators.begin(), m_operators.end(), character) != m_operators.end();
}


