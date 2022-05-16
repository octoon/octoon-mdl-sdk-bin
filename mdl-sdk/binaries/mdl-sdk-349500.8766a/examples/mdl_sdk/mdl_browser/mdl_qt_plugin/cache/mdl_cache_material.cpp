/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/


#include "mdl_cache_material.h"
#include <mi/mdl_sdk.h>

bool Mdl_cache_material::update(mi::neuraylib::INeuray* neuray, 
                                mi::neuraylib::ITransaction* transaction,
                                const mi::base::IInterface* module)
{
    // Access the material definition
    const mi::base::Handle<const mi::neuraylib::IFunction_definition> material_definition(
        transaction->access<mi::neuraylib::IFunction_definition>(
            (std::string("mdl") + get_qualified_name()).c_str()));

    if (!material_definition)
    {
        std::cerr << "[Mdl_cache_material] update: Failed to get material definition: "
                  << get_qualified_name() << "\n";
        return false;
    }

    // get infos from annotations
    const mi::base::Handle<const mi::neuraylib::IAnnotation_block> anno_block(
        material_definition->get_annotations());

    // are there annotations?
    if (anno_block)
    {
        const mi::neuraylib::Annotation_wrapper annotations(anno_block.get());
        const char* value = nullptr;

        if (annotations.get_annotation_index("::anno::hidden()") != static_cast<mi::Size>(-1))
            set_is_hidden(true);

        if (0 == annotations.get_annotation_param_value_by_name<const char*>(
            "::anno::author(string)", 0, value))
            set_cache_data("Author", value);

        if (0 == annotations.get_annotation_param_value_by_name<const char*>(
            "::anno::display_name(string)", 0, value))
            set_cache_data("DisplayName", value);

        if (0 == annotations.get_annotation_param_value_by_name<const char*>(
            "::anno::description(string)", 0, value))
            set_cache_data("Description", value);

        const mi::Size ai = annotations.get_annotation_index("::anno::key_words(string[N])");
        if (ai != static_cast<mi::Size>(-1))
        {
            const mi::base::Handle<const mi::neuraylib::IValue> ivalue(
                annotations.get_annotation_param_value(ai, 0));

            const mi::base::Handle<const mi::neuraylib::IValue_array> ivalue_array(
                ivalue->get_interface<const mi::neuraylib::IValue_array>());

            const mi::Size keyword_count = ivalue_array->get_size();
            std::stringstream s;
            for (mi::Size i = 0; i < keyword_count; ++i)
            {
                if (i > 0) s << ", ";

                const mi::base::Handle<const mi::neuraylib::IValue> keyword_value(
                    ivalue_array->get_value<const mi::neuraylib::IValue>(i));

                const mi::base::Handle<const mi::neuraylib::IValue_string> keyword(
                    keyword_value->get_interface<const mi::neuraylib::IValue_string>());

                s << keyword->get_value();
            }

            if (keyword_count > 0)
            {
                std::string keywords = s.str();
                set_cache_data("Keywords", keywords.c_str());
            }
        }
    }

    // would be nice to not use the cache in order to get updates
    // however, loading the material is no option during runtime
    const char* path = material_definition->get_thumbnail();
    if (path)
        set_cache_data("Thumbnail", path);
    
    return true;
}