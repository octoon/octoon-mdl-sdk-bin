/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief Helper class for handling xml operations.


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_XML_HELPER_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_XML_HELPER_H

#include <tinyxml2.h>
#include <string>
#include <vector>


class Xml_helper
{
public:
    // reading
    //---------------------------------------------------------------------
    static std::string query_text(tinyxml2::XMLElement* element, 
                                  const std::string& defaultValue = "");

    static std::string query_text_element(tinyxml2::XMLElement* element, 
                                          const std::string& element_name, 
                                          const std::string& defaultValue = "");

    static std::string query_text_attribute(const tinyxml2::XMLElement* element, 
                                            const std::string& attribute_name, 
                                            const std::string& defaultValue = "");


    // writing
    //---------------------------------------------------------------------
    static tinyxml2::XMLDocument* create_document();
    static tinyxml2::XMLElement* attach_element(tinyxml2::XMLDocument* doc, 
                                                const std::string& name);

    static tinyxml2::XMLElement* attach_element(tinyxml2::XMLNode* parent, 
                                                const std::string& name);

    static tinyxml2::XMLElement* attach_element(tinyxml2::XMLNode* parent, 
                                                const std::string& name, 
                                                const std::string& text);


private:
    static tinyxml2::XMLElement* create_element(tinyxml2::XMLNode* parent, 
                                                const std::string& name);

    static void attach_element(tinyxml2::XMLNode* parent, 
                               tinyxml2::XMLElement* element);
};

#endif