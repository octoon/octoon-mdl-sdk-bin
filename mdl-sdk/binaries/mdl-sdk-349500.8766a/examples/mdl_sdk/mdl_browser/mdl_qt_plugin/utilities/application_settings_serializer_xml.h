/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief Serializer for persistent settings


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_APPLICATION_SETTINGS_SERIALIZER_XML_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_APPLICATION_SETTINGS_SERIALIZER_XML_H

#include "application_settings.h"

class Application_settings_serializer_xml
    : public Application_settings_base::Application_settings_serializer_base
{
public:
    explicit Application_settings_serializer_xml() = default;
    virtual ~Application_settings_serializer_xml() = default;

protected:
    bool serialize(const Application_settings_base& settings, 
                   const std::string& file_path) const override;

    bool deserialize(Application_settings_base& settings, 
                     const std::string& file_path) const override;
};

#endif