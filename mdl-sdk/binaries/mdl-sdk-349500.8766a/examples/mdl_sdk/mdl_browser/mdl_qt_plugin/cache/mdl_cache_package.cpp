/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/


#include "mdl_cache_package.h"
#include <mi/mdl_sdk.h>
#include "mdl_cache_module.h"
#include <set>

bool Mdl_cache_package::update(mi::neuraylib::INeuray* neuray, 
                               mi::neuraylib::ITransaction* transaction, 
                               const mi::base::IInterface* node)
{
    const mi::base::Handle<const mi::neuraylib::IMdl_package_info> graph_node(
        node->get_interface<const mi::neuraylib::IMdl_package_info>());
    if (!graph_node)
        return false;

    // TODO check content changed.. for folder(s)?
    // return true here if nothing changed

    // TODO get hidden annotation for packages (specified, but not implemented yet)

    // keep track of the existence of children to allow clean-up
    std::set<Child_map_key> not_present_children;
    for (const auto& c : get_children())
        not_present_children.insert(c.first);

    bool changed = false;

    // iterate over all sub-packages and modules
    for (mi::Size c = 0, n = graph_node->get_child_count(); c < n; c++)
    {
        mi::base::Handle<const mi::neuraylib::IMdl_info> c_discovery(graph_node->get_child(c));

        const char* simple_name(c_discovery->get_simple_name());

        switch (c_discovery->get_kind())
        {
            case mi::neuraylib::IMdl_info::DK_PACKAGE:
            {
                const Child_map_key key{IMdl_cache_item::CK_PACKAGE, simple_name};

                // "mark" as present by removing from not_present_children
                const std::set<Child_map_key>::iterator pos =  not_present_children.find(key);
                if (pos != not_present_children.end())
                    not_present_children.erase(pos);

                // insert new child if not already available
                IMdl_cache_item* child = get_child(key);
                if (!child)
                {
                    std::string qualified_name = c_discovery->get_qualified_name();
                    std::string simple_name = c_discovery->get_simple_name();
                    child = get_cache()->create(
                        CK_PACKAGE, simple_name.c_str(), simple_name.c_str(), qualified_name.c_str());
                    add_child(child);
                }

                // update recursively
                changed |= dynamic_cast<Mdl_cache_package*>(child)->update(
                    neuray, transaction, c_discovery.get());

                break;
            }

            case mi::neuraylib::IMdl_info::DK_MODULE:
            {
                const Child_map_key key{IMdl_cache_item::CK_MODULE, simple_name};

                // "mark" as present by removing from not_present_children
                const std::set<Child_map_key>::iterator pos = not_present_children.find(key);
                if (pos != not_present_children.end())
                    not_present_children.erase(pos);

                // insert new child if not already available
                IMdl_cache_item* child = get_child(key);
                if(!child)
                {
                    std::string qualified_name = c_discovery->get_qualified_name();
                    std::string simple_name = c_discovery->get_simple_name();
                    child = get_cache()->create(
                        CK_MODULE, simple_name.c_str(), simple_name.c_str(), qualified_name.c_str());
                    add_child(child);
                }

                // update recursively
                changed |= dynamic_cast<Mdl_cache_module*>(child)->update(
                    neuray, transaction, c_discovery.get());

                break;
            }

            default:
                std::cerr << "[Mdl_cache_package] update: discovery info kind no handled: "
                    << c_discovery->get_simple_name() << " .\n";
                break;
        }
    }

    // remove all cached modules that are not present anymore
    for (const auto& c : not_present_children)
    {
        IMdl_cache_item* item = remove_child(c);
        get_cache()->erase(item);
    }
    
    return changed;
}
