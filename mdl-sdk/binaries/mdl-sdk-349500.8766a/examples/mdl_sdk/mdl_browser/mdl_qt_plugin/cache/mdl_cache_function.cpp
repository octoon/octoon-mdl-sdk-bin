/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/


#include "mdl_cache_function.h"
#include <mi/mdl_sdk.h>

bool Mdl_cache_function::update(mi::neuraylib::INeuray* neuray, 
                                mi::neuraylib::ITransaction* transaction, 
                                const mi::base::IInterface* module)
{
    // Access the material definition
    const mi::base::Handle<const mi::neuraylib::IFunction_definition> function_definition(
        transaction->access<mi::neuraylib::IFunction_definition>(
            (std::string("mdl") + get_qualified_name()).c_str()));

    if (!function_definition)
    {
        std::cerr << "[Mdl_cache_function] update: Failed to get function definition: "
                  << get_qualified_name() << "\n";
        return false;
    }

    // get infos from annotations
    const mi::base::Handle<const mi::neuraylib::IAnnotation_block> anno_block(
        function_definition->get_annotations());

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
    }

    return true;
}