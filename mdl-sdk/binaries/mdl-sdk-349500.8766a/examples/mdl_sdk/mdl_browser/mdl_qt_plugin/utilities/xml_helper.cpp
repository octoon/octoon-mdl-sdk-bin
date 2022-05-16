/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/


#include "xml_helper.h"
#include <sstream>
#include <vector>

using namespace tinyxml2;

std::string Xml_helper::query_text(XMLElement* element, const std::string& defaultValue)
{
    if (!element) return defaultValue;
    if (element->FirstChild() && element->FirstChild()->ToText())
        return std::string(element->FirstChild()->Value());
    return "";
}


std::string Xml_helper::query_text_element(XMLElement* element, const std::string& element_name, 
                                           const std::string& defaultValue)
{
    if (!element) return defaultValue;
    XMLElement* child = element->FirstChildElement(element_name.c_str());

    if (child && child->FirstChild())
        return std::string(child->FirstChild()->Value());

    return defaultValue;
}

std::string Xml_helper::query_text_attribute(const XMLElement* element, 
                                             const std::string& attribute_name, 
                                             const std::string& defaultValue)
{
    if (!element) return defaultValue;
    const char* value = element->Attribute(attribute_name.c_str());
    if (value)
        return value;

    return defaultValue;
}

XMLDocument* Xml_helper::create_document()
{
    XMLDocument* doc = new XMLDocument();
    doc->InsertFirstChild(doc->NewDeclaration());
    return doc;
}

XMLElement* Xml_helper::create_element(XMLNode* parent, const std::string& name)
{
    XMLDocument* doc = parent->GetDocument();
    XMLElement* element = doc->NewElement(name.c_str());
    return element;
}

void Xml_helper::attach_element(XMLNode* parent, XMLElement* element)
{
    parent->InsertEndChild(element);
}


XMLElement* Xml_helper::attach_element(XMLDocument* doc, const std::string& name)
{
    XMLElement* element = doc->NewElement(name.c_str());
    doc->InsertEndChild(element);
    return element;
}


XMLElement* Xml_helper::attach_element(XMLNode* parent, const std::string& name)
{
    XMLDocument* doc = parent->GetDocument();
    XMLElement* element = doc->NewElement(name.c_str());
    parent->InsertEndChild(element);
    return element;
}


XMLElement* Xml_helper::attach_element(XMLNode* parent, const std::string& name, 
                                       const std::string& text)
{
    XMLElement* element = create_element(parent, name);
    element->SetText(text.c_str());
    attach_element(parent, element);
    return element;
}

