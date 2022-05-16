/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/


#include "application_settings.h"


template<>
bool Application_settings_base::get_value(const std::string& key, std::string& out_value) const
{
    const auto it = m_settings.find(key);
    if (it == m_settings.end())
        return false;

    out_value = it->second;
    return true;
}

template<>
bool Application_settings_base::get_value(const std::string& key, size_t& out_value) const
{
    std::string string_value = "";
    if (!get_value<std::string>(key, string_value))
        return false;

    out_value = static_cast<size_t>(std::stoll(string_value, nullptr, 0));
    return true;
}

template<>
bool Application_settings_base::get_value(const std::string& key, bool& out_value) const
{
    std::string string_value = "";
    if (!get_value<std::string>(key, string_value))
        return false;

    out_value = string_value == "true";
    return true;
}

template<>
bool Application_settings_base::set_value(const std::string& key, const std::string& value)
{
    if (m_settings[key] == value)
        return false;

    m_settings[key] = value;
    if (!m_auto_file_path.empty()) save(m_auto_file_path);
    return true;
}

template<>
bool Application_settings_base::set_value(const std::string& key, const bool& value)
{
    std::string s = value ? "true" : "false";
    if (m_settings[key] == s)
        return false;

    m_settings[key] = s;
    if (!m_auto_file_path.empty()) save(m_auto_file_path);
    return true;
}