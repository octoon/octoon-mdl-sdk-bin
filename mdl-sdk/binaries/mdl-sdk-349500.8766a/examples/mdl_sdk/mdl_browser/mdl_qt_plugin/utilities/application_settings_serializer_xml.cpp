/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/


#include "application_settings_serializer_xml.h"
#include "xml_helper.h"
#include <fstream>

using namespace tinyxml2;

bool Application_settings_serializer_xml::serialize(
    const Application_settings_base& settings, const std::string& file_path) const
{
    XMLDocument* doc = Xml_helper::create_document();
    XMLElement* settings_node = Xml_helper::attach_element(doc, "Settings");

    for (const auto& kv : get_key_value_storage(settings))
        Xml_helper::attach_element(settings_node, kv.first, kv.second);    

    XMLPrinter printer;
    doc->Print(&printer);

    std::ofstream file_stream;
    file_stream.open(file_path);
    if (file_stream)
    {
        file_stream << printer.CStr();
        file_stream.close();
        return true;
    }
    return false;
}

bool Application_settings_serializer_xml::deserialize(
    Application_settings_base& settings, const std::string& file_path) const
{
    XMLDocument* document = new XMLDocument();
    if (XML_SUCCESS != document->LoadFile(file_path.c_str()))
        return false;

    XMLElement* settings_node = document->FirstChildElement("Settings");

    auto& kv_map = get_key_value_storage(settings);
    for (const XMLElement* data = settings_node->FirstChildElement(); 
         data != nullptr; 
         data = data->NextSiblingElement())
    {
        if (data && data->FirstChild())
        {
            const char* key = data->Value();
            const char* value = data->FirstChild()->Value();
            kv_map[key] = value;
        }
    }

    return true;
}
