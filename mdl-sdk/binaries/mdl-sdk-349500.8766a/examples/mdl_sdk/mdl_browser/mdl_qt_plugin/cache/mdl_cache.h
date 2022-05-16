/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief Implementation of the IMdl_cache interface.


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_MDL_CACHE_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_MDL_CACHE_H

#include "example_shared.h"
#include "imdl_cache.h"

#include <string>
#include <unordered_map>

namespace mi
{
    namespace base
    {
        class IInterface;
    }

    namespace neuraylib
    {
        class IMdl_module_info;
        class IMdl_package_info;
        class INeuray;
        class ITransaction;
    }
}
namespace tinyxml2
{
    class XMLElement;
}

class Mdl_cache_package;
class Index_cache_elements;

class Mdl_cache : public IMdl_cache
{
public:

    // child map, indexed by kind and unique identifier
    typedef std::unordered_map<IMdl_cache_node::Child_map_key,
                               IMdl_cache_item*,
                               IMdl_cache_node::Child_map_key_hash> Child_map;

    explicit Mdl_cache();
    virtual ~Mdl_cache();

    IMdl_cache_item* create(
        IMdl_cache_item::Kind kind,
        const char* entity_name,
        const char* simple_name,
        const char* qualified_name) override;
    bool erase(IMdl_cache_item* item) override;

    const char* get_locale() const override {return m_locale.empty() ? nullptr : m_locale.c_str();}
    void set_locale(const char* locale) override { m_locale = locale ? locale : ""; }

    const IMdl_cache_item* get_cache_item(
        const IMdl_cache_node::Child_map_key& key) const override;

    const IMdl_cache_package* get_cache_root() const override;
    const mi::neuraylib::IMdl_discovery_result* get_discovery_result() const override;

    bool save_to_disk(const IMdl_cache_serializer& serializer, const std::string& path) const;
    bool load_from_disk(const IMdl_cache_serializer& serializer, const std::string& path);

    // Updates the cache structure with the info from all search paths.
    // Note, this fails when no valid search path was found.
    bool update(mi::neuraylib::INeuray* neuray, mi::neuraylib::ITransaction* transaction);
    const Index_cache_elements* get_search_index() const { return m_index; }

private:
    mi::base::Handle<const mi::neuraylib::IMdl_discovery_result> m_discovery_result;
    IMdl_cache_package* m_cache_root;
    Index_cache_elements* m_index;
    Child_map m_item_map;
    std::string m_locale;
};


#endif



