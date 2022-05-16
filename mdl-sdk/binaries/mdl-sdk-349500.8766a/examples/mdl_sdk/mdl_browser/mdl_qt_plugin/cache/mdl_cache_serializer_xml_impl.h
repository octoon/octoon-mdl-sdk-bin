/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief Implementation of the IMdl_cache_serializer_xml interface.


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_MDL_CACHE_SERIALIZER_XML_IMPL_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_MDL_CACHE_SERIALIZER_XML_IMPL_H

#include "imdl_cache.h"

namespace tinyxml2 {
    class XMLElement;
}

class Mdl_cache_serializer_xml_impl : public IMdl_cache_serializer_xml
{
public:
    explicit Mdl_cache_serializer_xml_impl();
    virtual ~Mdl_cache_serializer_xml_impl() = default;

    bool serialize(const IMdl_cache* cache, const char* file_path) const override;
    IMdl_cache_package* deserialize(IMdl_cache* cache, const char* file_path) const override;


protected:
     virtual tinyxml2::XMLElement* serialize(tinyxml2::XMLElement* parent, 
                                             const IMdl_cache_item* item) const;

     virtual IMdl_cache_item* deserialize(IMdl_cache* cache, 
                                          const IMdl_cache_node* parent, 
                                          const tinyxml2::XMLElement* item) const;
};

#endif