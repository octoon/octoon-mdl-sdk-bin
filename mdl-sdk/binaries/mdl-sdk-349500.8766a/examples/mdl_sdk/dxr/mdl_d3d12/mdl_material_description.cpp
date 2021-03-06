/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

#include "mdl_material_description.h"
#include "mdl_material.h"
#include "mdl_sdk.h"
#include "base_application.h"

#include "example_shared.h"
#include "utils.h"

#include <mi/mdl_sdk.h>
#include <mi/neuraylib/definition_wrapper.h>

#include <cassert>

namespace mi { namespace examples { namespace mdl_d3d12
{

namespace
{
    std::atomic<uint64_t> s_material_desc_id_counter(0);
    std::atomic<uint64_t> s_texture_lookup_call_counter(0);
} // anonymous

// ------------------------------------------------------------------------------------------------

const std::string Mdl_material_description::Invalid_material_identifier = "::dxr::not_available";

// ------------------------------------------------------------------------------------------------

Mdl_material_description::Mdl_material_description(const std::string& unique_material_identifier)
    : m_parameters()
    , m_is_loaded(false)
    , m_is_fallback(false)
    , m_supports_reloading(false)
    , m_loader(nullptr)
    , m_parameter_list(nullptr)
    , m_module_db_name("")
    , m_material_definition_db_name("")
    , m_qualified_module_name("")
    , m_material_name("")
    , m_name_in_scene("")
    , m_source_code("")
    , m_flags(IMaterial::Flags::None)
    , m_unique_id(s_material_desc_id_counter.fetch_add(1))
{
    m_parameters.name = unique_material_identifier;
}

// ------------------------------------------------------------------------------------------------

Mdl_material_description::Mdl_material_description()
    : Mdl_material_description(Invalid_material_identifier)
{
}

// ------------------------------------------------------------------------------------------------

Mdl_material_description::Mdl_material_description(const IScene_loader::Material& description)
    : Mdl_material_description()
{
    m_parameters = description;
}

// ------------------------------------------------------------------------------------------------

Mdl_material_description::~Mdl_material_description()
{
    m_parameter_list = nullptr;
}

// ------------------------------------------------------------------------------------------------

const IScene_loader::Material& Mdl_material_description::get_material_parameters() const
{
    return m_parameters;
}

// ------------------------------------------------------------------------------------------------

bool Mdl_material_description::load_material_definition(
    Mdl_sdk& sdk,
    const std::string& scene_directory,
    mi::neuraylib::IMdl_execution_context* context)
{
    // loaded before
    if (m_is_loaded)
        return true;

    // special case: the invalid material is requested directly
    if (m_parameters.name == Invalid_material_identifier)
    {
        if (!load_material_definition_fallback(sdk, scene_directory , context))
        {
            log_error("[FATAL] Invalid (fall-back) material can not be loaded.", SRC);
            return false;
        }

        m_is_loaded = true;
    }

    // depending on the type of material override (simple name convention)
    // it can make sense to use a gltf material in case of loading failure
    bool skip_glft_material_as_first_fall_back = false;

    // handle mdl (by convention when the name starts with '::')
    if (!m_is_loaded && mi::examples::strings::starts_with(m_parameters.name, "::"))
    {
        log_info("Material name matches the convention for overriding by an MDL material: " +
            m_parameters.name);

        m_is_loaded = load_material_definition_mdl(sdk, scene_directory, context);
        skip_glft_material_as_first_fall_back = true;
    }

    // handle mdle
    if (!m_is_loaded && (
        mi::examples::strings::ends_with(m_parameters.name, ".mdle") ||
        mi::examples::strings::ends_with(m_parameters.name, ".mdle::main")))
    {
        log_info("Material name matches the convention for overriding by an MDLE material: " +
            m_parameters.name);

        m_is_loaded = load_material_definition_mdle(sdk, scene_directory, context);
        skip_glft_material_as_first_fall_back = true;
    }

    // handle loaders
    if (!m_is_loaded)
    {
        sdk.get_library()->visit_material_description_loaders(
            [&](const IMdl_material_description_loader* loader)
            {
                // load if there is a match
                if (loader->match_gltf_name(m_parameters.name))
                {
                    log_info("Material name matches the convention for overriding: " +
                        m_parameters.name);

                    m_loader = loader; // keep the loader for reloading
                    m_is_loaded = load_material_definition_loader(
                        sdk, scene_directory, context, loader);

                    // do not use the gltf support as fall-back (pink instead)
                    skip_glft_material_as_first_fall_back = true;
                    return false; // stop visits, even in case of failure
                }
                return true; // continue visits
            });
    }

    // fall-back to gltf support materials
    if (!skip_glft_material_as_first_fall_back && !m_is_loaded)
    {
        m_is_loaded = load_material_definition_gltf_support(sdk, scene_directory, context);
        log_info("Material name does not match any convention, "
            "so the GLTF support material is used: " + m_parameters.name);
    }

    // fall-back
    if (!m_is_loaded)
        m_is_loaded = load_material_definition_fallback(sdk, scene_directory, context);

    if (m_is_loaded)
    {
        m_material_definition_db_name = m_module_db_name + "::" + m_material_name;

        // reflect changed imports
        sdk.get_library()->update_module_dependencies(m_module_db_name);

        return true;
    }

    log_error("[FATAL] Material definition can't be loaded and fall-back solutions failed for: "
        + m_parameters.name, SRC);
    return false;
}

// ------------------------------------------------------------------------------------------------

bool Mdl_material_description::is_loaded() const
{
    return m_is_loaded;
}

// ------------------------------------------------------------------------------------------------

const mi::neuraylib::IExpression_list* Mdl_material_description::get_parameters() const
{
    if (!m_parameter_list)
        return nullptr;

    m_parameter_list->retain();
    return m_parameter_list.get();
}

// ------------------------------------------------------------------------------------------------

const char* Mdl_material_description::get_qualified_module_name() const
{
    return m_is_loaded ? m_qualified_module_name.c_str() : nullptr;
}

// ------------------------------------------------------------------------------------------------

const char* Mdl_material_description::get_material_name() const
{
    return m_is_loaded ? m_material_name.c_str() : nullptr;
}

// ------------------------------------------------------------------------------------------------

const char* Mdl_material_description::get_scene_name() const
{
    return m_is_loaded ? m_name_in_scene.c_str() : nullptr;
}

// ------------------------------------------------------------------------------------------------

IMaterial::Flags Mdl_material_description::get_flags() const
{
    return m_is_loaded ? m_flags : IMaterial::Flags::None;
}

// ------------------------------------------------------------------------------------------------

bool Mdl_material_description::is_fallback() const
{
    return m_is_loaded ? m_is_fallback : false;
}

// ------------------------------------------------------------------------------------------------

bool Mdl_material_description::supports_reloading() const
{
    return m_is_loaded ? m_supports_reloading : false;
}

// ------------------------------------------------------------------------------------------------

const char* Mdl_material_description::get_module_db_name() const
{
    return m_is_loaded ? m_module_db_name.c_str() : nullptr;
}

// ------------------------------------------------------------------------------------------------

const char* Mdl_material_description::get_material_definition_db_name() const
{
    return m_is_loaded ? m_material_definition_db_name.c_str() : nullptr;
}

// ------------------------------------------------------------------------------------------------

const char* Mdl_material_description::regenerate_source_code(
    Mdl_sdk& sdk,
    const std::string& scene_directory,
    mi::neuraylib::IMdl_execution_context* context)
{
    // not supported or no module that was loaded from source
    if (!m_is_loaded || !m_supports_reloading || m_source_code.empty())
        return nullptr;

    // materials with generated code
    std::string generated_code = "";

    // handle loader
    if (m_loader)
        generated_code = m_loader->generate_mdl_source_code(m_parameters.name, scene_directory);

    // code was generated successfully
    if (!generated_code.empty())
    {
        m_source_code = generated_code;
        return m_source_code.c_str();
    }

    // code generation failed, or no generator for this type of material
    load_material_definition_fallback(sdk, scene_directory, context);
    return nullptr;
}

// ------------------------------------------------------------------------------------------------

bool Mdl_material_description::load_material_definition_mdle(
    Mdl_sdk& sdk,
    const std::string& scene_directory,
    mi::neuraylib::IMdl_execution_context* context)
{
    // drop the optional module name
    if (mi::examples::strings::ends_with(m_parameters.name, ".mdle::main"))
        m_parameters.name = m_parameters.name.substr(0, m_parameters.name.length() - 6);

    // resolve relative paths within the scene directory
    if (mi::examples::io::is_absolute_path(m_parameters.name))
        m_qualified_module_name = m_parameters.name;
    else
        m_qualified_module_name = scene_directory + "/" + m_parameters.name;

    // check if the file exists
    if (!mi::examples::io::file_exists(m_qualified_module_name))
    {
        log_warning(
            "The referenced MDLE file does not exist: " + m_parameters.name +
            "\nUsing a fall-back material instead.");
        return false;
    }

    m_material_name = "main";

    // name only for display
    std::string name = m_qualified_module_name.substr(
        m_qualified_module_name.find_last_of('/') + 1);
    name = name.substr(0, name.length() - 4);
    m_name_in_scene = "[mdle] " + name + " (" + std::to_string(m_unique_id) + ")";

    // load the module
    if (!load_mdl_module(sdk, scene_directory, context))
        return false;

    // check if the mdle contains a material
    std::string material_db_name = m_module_db_name + "::" + m_material_name;
    mi::base::Handle<const mi::neuraylib::IFunction_definition> material_definition(
        sdk.get_transaction().access<const mi::neuraylib::IFunction_definition>(
            material_db_name.c_str()));
    if (!material_definition)
    {
        log_warning(
            "The referenced MDLE file does not contain a material: " +
            m_qualified_module_name + "\nUsing a fall-back material instead.");
        return false;
    }

    // mdle have defaults for all parameters
    // TODO: they can be overridden, possibly by data from the loaded scene
    m_parameter_list = nullptr;
    m_supports_reloading = true;
    m_is_fallback = false;
    return true;
}

// ------------------------------------------------------------------------------------------------

bool Mdl_material_description::load_material_definition_mdl(
    Mdl_sdk& sdk,
    const std::string& scene_directory,
    mi::neuraylib::IMdl_execution_context* context)
{
    // split module and material name
    // [::<package>]::<module>::<material>
    if (!mi::examples::mdl::parse_cmd_argument_material_name(
        m_parameters.name, m_qualified_module_name, m_material_name, false))
    {
        log_warning(
            "Material name is not a fully qualified material name: " + m_parameters.name +
            "\nUsing a fall-back material instead.");
        return false;
    }

    // name only for display
    m_name_in_scene = "[mdl] " + m_material_name + " (" + std::to_string(m_unique_id) + ")";

    // load the module
    if (!load_mdl_module(sdk, scene_directory, context))
        return false;

    // database name of the material
    std::string material_definition_db_name = m_module_db_name + "::" + m_material_name;

    // check if the module contains the requested material
    mi::base::Handle<const mi::neuraylib::IFunction_definition> definition(
        sdk.get_transaction().access<const mi::neuraylib::IFunction_definition>(
            material_definition_db_name.c_str()));
    if (!definition)
    {
        m_module_db_name = "";
        return false;
    }

    // create a parameter list for all parameters (even without default)
    // for that the Definition_wrapper can also be used when creating an instance
    sdk.get_transaction().execute<void>([&](mi::neuraylib::ITransaction* t)
    {
        mi::base::Handle<const  mi::neuraylib::IType_list> parameter_types(
            definition->get_parameter_types());
        mi::base::Handle<const  mi::neuraylib::IExpression_list> defaults(
            definition->get_defaults());

        mi::base::Handle<mi::neuraylib::IValue_factory> vf(
            sdk.get_factory().create_value_factory(t));
        mi::base::Handle<mi::neuraylib::IExpression_factory> ef(
            sdk.get_factory().create_expression_factory(t));

        m_parameter_list = ef->create_expression_list();

        mi::Size count = definition->get_parameter_count();
        for (mi::Size i = 0; i < count; ++i) {
            const char* name = definition->get_parameter_name(i);
            mi::base::Handle<const mi::neuraylib::IExpression> default_(
                defaults->get_expression(name));
            if (!default_) {
                mi::base::Handle<const mi::neuraylib::IType> type(parameter_types->get_type(i));
                mi::base::Handle<mi::neuraylib::IValue> value(vf->create(type.get()));
                mi::base::Handle<mi::neuraylib::IExpression> expr(ef->create_constant(value.get()));
                m_parameter_list->add_expression(name, expr.get());
            }
        }
    });

    m_supports_reloading = true;
    m_is_fallback = false;
    return true;
}

// ------------------------------------------------------------------------------------------------

bool Mdl_material_description::load_material_definition_gltf_support(
    Mdl_sdk& sdk,
    const std::string& scene_directory,
    mi::neuraylib::IMdl_execution_context* context)
{
    m_qualified_module_name = "::nvidia::sdk_examples::gltf_support";

    // .. but it will be disabled for opaque material instances
    if (m_parameters.alpha_mode == IScene_loader::Material::Alpha_mode::Opaque)
        m_flags = mi::examples::enums::set_flag(m_flags, IMaterial::Flags::Opaque);

    if (m_parameters.single_sided == true)
        m_flags = mi::examples::enums::set_flag(m_flags, IMaterial::Flags::SingleSided);

    // model dependent support material name
    switch (m_parameters.pbr_model)
    {
        case IScene_loader::Material::Pbr_model::Khr_specular_glossiness:
            m_material_name = "gltf_material_khr_specular_glossiness";
            break;

        case IScene_loader::Material::Pbr_model::Metallic_roughness:
        default:
            m_material_name = "gltf_material";
            break;
    }

    // name only for display
    m_name_in_scene = "[gltf] " + m_parameters.name + " (" + std::to_string(m_unique_id) + ")";

    // load the module
    if (!load_mdl_module(sdk, scene_directory, context))
        return false;

    // setup the parameter list based on the gltf scene data
    parameterize_gltf_support_material(sdk, scene_directory, context);

    m_supports_reloading = true;
    m_is_fallback = false;
    return true;
}

// ------------------------------------------------------------------------------------------------

bool Mdl_material_description::load_mdl_module(
    Mdl_sdk& sdk,
    const std::string& scene_directory,
    mi::neuraylib::IMdl_execution_context* context)
{
    // expected database name of the module to load
    mi::base::Handle<const mi::IString> module_db_name(
        sdk.get_factory().get_db_module_name(m_qualified_module_name.c_str()));

    // check if the module and thereby the material definition is already loaded
    mi::base::Handle<const mi::neuraylib::IModule> module(
        sdk.get_transaction().access<mi::neuraylib::IModule>(module_db_name->get_c_str()));

    // if not, load it
    if (!module)
    {
        // Load the module that contains the material.
        // This functions supports multi-threading. It blocks when the requested module
        // or a dependency is loaded by a different thread.
        sdk.get_impexp_api().load_module(
            sdk.get_transaction().get(), m_qualified_module_name.c_str(), context);

        // loading failed
        if (!sdk.log_messages(
            "Loading the module failed: " + m_qualified_module_name, context))
                return false;
    }

    m_module_db_name = module_db_name->get_c_str();
    return true;
}

// ------------------------------------------------------------------------------------------------

bool Mdl_material_description::load_material_definition_fallback(
    Mdl_sdk& sdk,
    const std::string& scene_directory,
    mi::neuraylib::IMdl_execution_context* context)
{
    // default module
    m_qualified_module_name = "::dxr";
    m_material_name = "not_available";

    static const char* default_module_src =
        "mdl 1.2;\n"
        "import df::*;\n"
        "export material not_available() = material(\n"
        "    surface: material_surface(\n"
        "        df::diffuse_reflection_bsdf(\n"
        "            tint: color(0.8, 0.0, 0.8)\n"
        "        )\n"
        "    )"
        ");";

    // expected database name of the module to load
    mi::base::Handle<const mi::IString> module_db_name(
        sdk.get_factory().get_db_module_name(m_qualified_module_name.c_str()));

    return sdk.get_transaction().execute<bool>([&](auto t)
    {
        // check if the module and thereby the material definition is already loaded
        mi::base::Handle<const mi::neuraylib::IModule> module(
            t->access<mi::neuraylib::IModule>(module_db_name->get_c_str()));

        // module is not loaded already
        if (!module)
        {
            // load it
            sdk.get_impexp_api().load_module_from_string(
                t, m_qualified_module_name.c_str(), default_module_src, context);

            // loading failed?
            if (!sdk.log_messages(
                "Loading the module failed: " + m_qualified_module_name, context, SRC))
                    return false;
        }

        // module is loaded now
        m_module_db_name = module_db_name->get_c_str();
        m_name_in_scene = "[mdl] invalid material (" + std::to_string(m_unique_id) + ")";

        m_supports_reloading = false;
        m_is_fallback = true;
        m_parameter_list = nullptr;
        m_flags = IMaterial::Flags::None;
        return true;
    });
}

// ------------------------------------------------------------------------------------------------

void Mdl_material_description::parameterize_gltf_support_material(
    Mdl_sdk& sdk,
    const std::string& scene_directory,
    mi::neuraylib::IMdl_execution_context* context)
{
    // this creates elements in the database, therefore it has to be locked when using threads
    sdk.get_transaction().execute<void>([&](mi::neuraylib::ITransaction* t)
    {
        // create material parameters
        mi::base::Handle<mi::neuraylib::IMdl_factory> mdl_factory(
            sdk.get_neuray().get_api_component<mi::neuraylib::IMdl_factory>());
        mi::base::Handle<mi::neuraylib::IValue_factory> vf(
            mdl_factory->create_value_factory(t));
        mi::base::Handle<mi::neuraylib::IExpression_factory> ef(
            mdl_factory->create_expression_factory(t));
        mi::base::Handle<mi::neuraylib::IType_factory> tf(
            mdl_factory->create_type_factory(t));

        mi::base::Handle<const mi::IString> gltf_support_module_db_name(
            mdl_factory->get_db_module_name("::nvidia::sdk_examples::gltf_support"));
        mi::base::Handle<const mi::neuraylib::IModule> gltf_support_module(
            t->access<mi::neuraylib::IModule>(gltf_support_module_db_name->get_c_str()));

        // create a new parameter list
        m_parameter_list = ef->create_expression_list();

        // helper to add a texture if it is available
        auto add_texture_resource = [&](
            mi::neuraylib::IExpression_list* expr_list,
            const std::string& expression_name,
            const std::string& releative_texture_path, float gamma)
        {
            if (releative_texture_path.empty()) return;

            std::string gamma_str = std::to_string(int(gamma * 100));
            std::string image_name = "mdl::" + releative_texture_path + "_image";
            std::string texture_name = "mdl::" + releative_texture_path + "_texture2d_" + gamma_str;

            // if the image is not loaded yet, do so
            mi::base::Handle<const mi::neuraylib::IImage> image(
                t->access<mi::neuraylib::IImage>(image_name.c_str()));
            if (!image)
            {
                mi::base::Handle<mi::neuraylib::IImage> new_image(
                    t->create<mi::neuraylib::IImage>("Image"));
                std::string file_path = scene_directory + "/" + releative_texture_path;
                new_image->reset_file(file_path.c_str());
                t->store(new_image.get(), image_name.c_str());
            }

            // if the texture does not exist yet, create it
            mi::base::Handle<const mi::neuraylib::ITexture> texture(
                t->access<mi::neuraylib::ITexture>(texture_name.c_str()));
            if (!texture)
            {
                mi::base::Handle<mi::neuraylib::ITexture> new_texture(
                    t->create<mi::neuraylib::ITexture>("Texture"));
                new_texture->set_image(image_name.c_str());
                new_texture->set_gamma(gamma);
                t->store(new_texture.get(), texture_name.c_str());
            }

            // Mark the texture for removing right away.
            // Note, this will not delete the data immediately. Instead it will be deleted
            // with the next transaction::commit(). Until then, we will have copied to resources
            // to the GPU.
            t->remove(texture_name.c_str());
            t->remove(image_name.c_str());

            mi::base::Handle<const mi::neuraylib::IType_texture> type(
                tf->create_texture(mi::neuraylib::IType_texture::TS_2D));
            mi::base::Handle<mi::neuraylib::IValue_texture> value(
                vf->create_texture(type.get(), texture_name.c_str()));
            mi::base::Handle<mi::neuraylib::IExpression> expr(
                ef->create_constant(value.get()));

            if (expr_list->add_expression(expression_name.c_str(), expr.get()) != 0)
                log_error("Failed to set glTF parameter '" + expression_name + "'.", SRC);
        };

        // helper to add a color parameter
        auto add_color = [&](
            mi::neuraylib::IExpression_list* expr_list,
            const std::string& expression_name,
            float r, float g, float b)
        {
            mi::base::Handle<mi::neuraylib::IValue> value(vf->create_color(r, g, b));
            mi::base::Handle<mi::neuraylib::IExpression> expr(
                ef->create_constant(value.get()));

            if (expr_list->add_expression(expression_name.c_str(), expr.get()) != 0)
                log_error("Failed to set glTF parameter '" + expression_name + "'.", SRC);
        };

        // helper to add a bool
        auto add_bool = [&](
            mi::neuraylib::IExpression_list* expr_list,
            const std::string& expression_name,
            bool v)
        {
            mi::base::Handle<mi::neuraylib::IValue> value(vf->create_bool(v));
            mi::base::Handle<mi::neuraylib::IExpression> expr(
                ef->create_constant(value.get()));

            if (expr_list->add_expression(expression_name.c_str(), expr.get()) != 0)
                log_error("Failed to set glTF parameter '" + expression_name + "'.", SRC);
        };

        // helper to add a float
        auto add_float = [&](
            mi::neuraylib::IExpression_list* expr_list,
            const std::string& expression_name,
            float x)
        {
            mi::base::Handle<mi::neuraylib::IValue> value(vf->create_float(x));
            mi::base::Handle<mi::neuraylib::IExpression> expr(
                ef->create_constant(value.get()));

            if (expr_list->add_expression(expression_name.c_str(), expr.get()) != 0)
                log_error("Failed to set glTF parameter '" + expression_name + "'.", SRC);
        };

        // helper to add a int
        auto add_int = [&](
            mi::neuraylib::IExpression_list* expr_list,
            const std::string& expression_name,
            int32_t x)
        {
            mi::base::Handle<mi::neuraylib::IValue> value(vf->create_int(x));
            mi::base::Handle<mi::neuraylib::IExpression> expr(
                ef->create_constant(value.get()));

            if (expr_list->add_expression(expression_name.c_str(), expr.get()) != 0)
                log_error("Failed to set glTF parameter '" + expression_name + "'.", SRC);
        };

        // helper to add a float2
        auto add_float2 = [&](
            mi::neuraylib::IExpression_list* expr_list,
            const std::string& expression_name,
            float x, float y)
        {
            mi::base::Handle<const mi::neuraylib::IType_float> float_type(tf->create_float());
            mi::base::Handle<const mi::neuraylib::IType_vector> vec_type(
                tf->create_vector(float_type.get(), 2));

            mi::base::Handle<mi::neuraylib::IValue_vector> value(vf->create_vector(vec_type.get()));
            mi::base::Handle<mi::neuraylib::IValue_float> value_x(vf->create_float(x));
            mi::base::Handle<mi::neuraylib::IValue_float> value_y(vf->create_float(y));
            value->set_value(0, value_x.get());
            value->set_value(1, value_y.get());
            mi::base::Handle<mi::neuraylib::IExpression> expr(
                ef->create_constant(value.get()));

            if (expr_list->add_expression(expression_name.c_str(), expr.get()) != 0)
                log_error("Failed to set glTF parameter '" + expression_name + "'.", SRC);
        };

        // helper to add a enum
        auto add_enum = [&](
            mi::neuraylib::IExpression_list* expr_list,
            const std::string& expression_name,
            const std::string& enum_type_name,
            mi::Sint32 enum_value)
        {
            mi::base::Handle<const mi::neuraylib::IType_enum> type(
                tf->create_enum(enum_type_name.c_str()));

            mi::base::Handle<mi::neuraylib::IValue_enum> value(vf->create_enum(type.get()));
            value->set_value(enum_value);
            mi::base::Handle<mi::neuraylib::IExpression> expr(
                ef->create_constant(value.get()));

            if (expr_list->add_expression(expression_name.c_str(), expr.get()) != 0)
                log_error("Failed to set glTF parameter '" + expression_name + "'.", SRC);
        };

        // get definitions for texture lookup functions
        std::string db_name_without_param_list =
            gltf_support_module_db_name->get_c_str() + std::string("::gltf_texture_lookup");
        mi::base::Handle<const mi::IArray> overloads(
            gltf_support_module->get_function_overloads(db_name_without_param_list.c_str()));
        assert(overloads->get_length() == 1 && "Unexpected overload for 'gltf_texture_lookup'");
        mi::base::Handle<const mi::IString> gltf_texture_lookup_db_name(
            overloads->get_element<const mi::IString>(0));

        db_name_without_param_list =
            gltf_support_module_db_name->get_c_str() + std::string("::gltf_normal_texture_lookup");
        overloads = gltf_support_module->get_function_overloads(db_name_without_param_list.c_str());
        assert(overloads->get_length() == 1 &&
            "Unexpected overload for 'gltf_normal_texture_lookup'");
        mi::base::Handle<const mi::IString> gltf_normal_texture_lookup_db_name(
            overloads->get_element<const mi::IString>(0));

        // helper to add a texture lookup function
        auto add_texture = [&](
            mi::neuraylib::IExpression_list* expr_list,
            const std::string& expression_name,
            const mdl_d3d12::IScene_loader::Material::Texture_info& texture_info,
            float gamma,
            bool normal_texture = false,
            float normal_factor = 1.0f,
            int tex_tangent_index = 0)
        {
            if (texture_info.resource_identifier.empty()) return;

            mi::base::Handle<mi::neuraylib::IExpression_list> call_parameter_list(
                ef->create_expression_list());

            add_texture_resource(
                call_parameter_list.get(), "texture", texture_info.resource_identifier, gamma);
            add_int(call_parameter_list.get(), "tex_coord_index", texture_info.texCoord);
            add_float2(
                call_parameter_list.get(), "offset", texture_info.offset.x, texture_info.offset.y);
            add_float(call_parameter_list.get(), "rotation", texture_info.rotation);
            add_float2(
                call_parameter_list.get(), "scale", texture_info.scale.x, texture_info.scale.y);

            add_enum(call_parameter_list.get(), "wrap_s",
                "::nvidia::sdk_examples::gltf_support::gltf_wrapping_mode",
                static_cast<mi::Sint32>(texture_info.wrap_s));
            add_enum(call_parameter_list.get(), "wrap_t",
                "::nvidia::sdk_examples::gltf_support::gltf_wrapping_mode",
                static_cast<mi::Sint32>(texture_info.wrap_s));

            if (normal_texture)
            {
                add_float(call_parameter_list.get(), "normal_scale_factor", normal_factor);
                add_int(call_parameter_list.get(), "tex_tangent_index", tex_tangent_index);
            }

            mi::neuraylib::Definition_wrapper dw(
                t, normal_texture ? gltf_normal_texture_lookup_db_name->get_c_str()
                                  : gltf_texture_lookup_db_name->get_c_str(),
                mdl_factory.get());

            mi::Sint32 res;
            mi::base::Handle<mi::neuraylib::IFunction_call> lookup_call(
                dw.create_instance<mi::neuraylib::IFunction_call>(call_parameter_list.get(), &res));

            if (res != 0)
            {
                log_error("Failed to create glTF texture lookup call", SRC);
                return;
            }

            std::string call_db_name = "mdl::gltf_support::texture_lookup_call_" +
                std::to_string(s_texture_lookup_call_counter++);
            t->store(lookup_call.get(), call_db_name.c_str());

            mi::base::Handle<mi::neuraylib::IExpression> expr(
                ef->create_call(call_db_name.c_str()));

            if (expr_list->add_expression(expression_name.c_str(), expr.get()) != 0)
                log_error("Failed to set glTF texture lookup call '" + expression_name + "'.", SRC);
        };

        // helper to add transmission
        auto add_transmission = [&](
            const mdl_d3d12::IScene_loader::Material::Model_data_materials_transmission& trans)
        {
            add_float(m_parameter_list.get(), "transmission_factor", trans.transmission_factor);
            add_texture(m_parameter_list.get(), "transmission_texture",
                trans.transmission_texture, 1.0f);
        };

        // helper to add clear-coat
        auto add_clearcoat = [&](
            const mdl_d3d12::IScene_loader::Material::Model_data_materials_clearcoat& clearcoat)
        {
            add_float(m_parameter_list.get(), "clearcoat_factor", clearcoat.clearcoat_factor);
            add_texture(m_parameter_list.get(), "clearcoat_texture",
                clearcoat.clearcoat_texture, 1.0f);
            add_float(m_parameter_list.get(), "clearcoat_roughness_factor",
                clearcoat.clearcoat_roughness_factor);
            add_texture(m_parameter_list.get(), "clearcoat_roughness_texture",
                clearcoat.clearcoat_roughness_texture, 1.0f);
            add_texture(m_parameter_list.get(), "clearcoat_normal_texture",
                clearcoat.clearcoat_normal_texture, 1.0f, true, 1.0f);
        };

        // helper to add sheen
        auto add_sheen = [&](
            const mdl_d3d12::IScene_loader::Material::Model_data_materials_sheen& sheen)
        {
            add_color(m_parameter_list.get(), "sheen_color_factor",
                sheen.sheen_color_factor.x, sheen.sheen_color_factor.y, sheen.sheen_color_factor.z);
            add_texture(m_parameter_list.get(), "sheen_color_texture",
                sheen.sheen_color_texture, 2.2f);
            add_float(m_parameter_list.get(), "sheen_roughness_factor",
                sheen.sheen_roughness_factor);
            add_texture(m_parameter_list.get(), "sheen_roughness_texture",
                sheen.sheen_roughness_texture, 2.2f); // alpha channel is not affected by gamma
                                                      // chance to reuse the sheen color texture
        };

        // helper to add specular
        auto add_specular = [&](
            const mdl_d3d12::IScene_loader::Material::Model_data_materials_specular& specular)
        {
            add_float(m_parameter_list.get(), "specular_factor",
                specular.specular_factor);
            add_texture(m_parameter_list.get(), "specular_texture",
                specular.specular_texture, 2.2f);   // alpha channel is not affected by gamma
                                                    // chance to reuse the sheen color texture
            add_color(m_parameter_list.get(), "specular_color_factor",
                specular.specular_color_factor.x,
                specular.specular_color_factor.y,
                specular.specular_color_factor.z);
            add_texture(m_parameter_list.get(), "specular_color_texture",
                specular.specular_color_texture, 2.2f);
        };

        // helper to add volume
        auto add_volume = [&](
            const mdl_d3d12::IScene_loader::Material::Model_data_materials_volume& volume)
        {
            add_bool(m_parameter_list.get(), "thin_walled", volume.thin_walled);
            add_float(m_parameter_list.get(), "attenuation_distance", volume.attenuation_distance);
            add_color(m_parameter_list.get(), "attenuation_color",
                volume.attenuation_color.x,
                volume.attenuation_color.y,
                volume.attenuation_color.z);
        };

        // add the actual parameters to the parameter list
        add_texture(m_parameter_list.get(), "normal_texture",
            m_parameters.normal_texture, 1.0f, true, m_parameters.normal_scale_factor);

        add_texture(m_parameter_list.get(), "occlusion_texture",
            m_parameters.occlusion_texture, 1.0f);
        add_float(m_parameter_list.get(), "occlusion_strength", m_parameters.occlusion_strength);

        add_texture(m_parameter_list.get(), "emissive_texture",
            m_parameters.emissive_texture, 2.2f);
        add_color(m_parameter_list.get(), "emissive_factor", m_parameters.emissive_factor.x,
            m_parameters.emissive_factor.y, m_parameters.emissive_factor.z);

        add_enum(m_parameter_list.get(), "alpha_mode",
            "::nvidia::sdk_examples::gltf_support::gltf_alpha_mode",
            static_cast<mi::Sint32>(m_parameters.alpha_mode));
        add_float(m_parameter_list.get(), "alpha_cutoff", m_parameters.alpha_cutoff);

        // general extensions
        add_float(m_parameter_list.get(), "emissive_strength",
            m_parameters.emissive_strength.emissive_strength);

        // model dependent parameters
        switch (m_parameters.pbr_model)
            {
            case IScene_loader::Material::Pbr_model::Khr_specular_glossiness:
            {
                add_texture(m_parameter_list.get(), "diffuse_texture",
                            m_parameters.khr_specular_glossiness.diffuse_texture, 2.2f);

                add_color(m_parameter_list.get(), "diffuse_factor",
                            m_parameters.khr_specular_glossiness.diffuse_factor.x,
                            m_parameters.khr_specular_glossiness.diffuse_factor.y,
                            m_parameters.khr_specular_glossiness.diffuse_factor.z);

                add_float(m_parameter_list.get(), "base_alpha",
                            m_parameters.khr_specular_glossiness.diffuse_factor.w);

                add_texture(m_parameter_list.get(), "specular_glossiness_texture",
                            m_parameters.khr_specular_glossiness.specular_glossiness_texture, 2.2f);

                add_color(m_parameter_list.get(), "specular_factor",
                            m_parameters.khr_specular_glossiness.specular_factor.x,
                            m_parameters.khr_specular_glossiness.specular_factor.y,
                            m_parameters.khr_specular_glossiness.specular_factor.z);
                add_float(m_parameter_list.get(), "glossiness_factor",
                            m_parameters.khr_specular_glossiness.glossiness_factor);
                return;
            }

            case IScene_loader::Material::Pbr_model::Metallic_roughness:
            default:
            {
                add_texture(m_parameter_list.get(), "base_color_texture",
                            m_parameters.metallic_roughness.base_color_texture, 2.2f);

                add_color(m_parameter_list.get(), "base_color_factor",
                            m_parameters.metallic_roughness.base_color_factor.x,
                            m_parameters.metallic_roughness.base_color_factor.y,
                            m_parameters.metallic_roughness.base_color_factor.z);

                add_float(m_parameter_list.get(), "base_alpha",
                            m_parameters.metallic_roughness.base_color_factor.w);

                add_texture(m_parameter_list.get(), "metallic_roughness_texture",
                            m_parameters.metallic_roughness.metallic_roughness_texture, 1.0f);

                add_float(m_parameter_list.get(), "metallic_factor",
                            m_parameters.metallic_roughness.metallic_factor);

                add_float(m_parameter_list.get(), "roughness_factor",
                            m_parameters.metallic_roughness.roughness_factor);

                add_transmission(m_parameters.metallic_roughness.transmission);
                add_clearcoat(m_parameters.metallic_roughness.clearcoat);
                add_sheen(m_parameters.metallic_roughness.sheen);
                add_specular(m_parameters.metallic_roughness.specular);
                add_float(m_parameter_list.get(), "ior", m_parameters.metallic_roughness.ior.ior);
                add_volume(m_parameters.metallic_roughness.volume);
                return;
            }
        }
    });
}

// -------------------------------------------------------------------------------------------------

bool Mdl_material_description::load_material_definition_loader(
    Mdl_sdk& sdk,
    const std::string& scene_directory,
    mi::neuraylib::IMdl_execution_context* context,
    const IMdl_material_description_loader* loader)
{
    // naming convention using the gltf material name attribute
    std::string gltf_name = m_parameters.name;

    // generate the source code
    m_source_code = loader->generate_mdl_source_code(gltf_name, scene_directory);
    if (m_source_code.empty())
        return false;

    // compute a full qualified module name
    std::string module_name = gltf_name;
    std::replace(module_name.begin(), module_name.end(), '\\', '_');
    std::replace(module_name.begin(), module_name.end(), '/', '_');
    std::replace(module_name.begin(), module_name.end(), ':', '_');
    std::replace(module_name.begin(), module_name.end(), '.', '_');
    if (module_name.length() < 2 || module_name[0] != ':' || module_name[1] == ':')
        module_name = "::" + module_name;
    m_qualified_module_name = module_name;

    // load the actual module, sequentially for now
    sdk.get_transaction().execute<void>([&](mi::neuraylib::ITransaction* t)
        {
            auto& mdl_impexp = sdk.get_impexp_api();
            mdl_impexp.load_module_from_string(
                t, m_qualified_module_name.c_str(),
                m_source_code.c_str(),
                context);
        });
    if (!sdk.log_messages(
        "Loading generated material (from materialX) failed: " + m_parameters.name,
        context, SRC))
            return false;

    // expected database name of the module to load
    mi::base::Handle<const mi::IString> module_db_name(
        sdk.get_factory().get_db_module_name(m_qualified_module_name.c_str()));
    m_module_db_name = module_db_name->get_c_str();

    // get the loaded module
    mi::base::Handle<const mi::neuraylib::IModule> module(
        sdk.get_transaction().access<const mi::neuraylib::IModule>(
            module_db_name->get_c_str()));

    if (module->get_material_count() == 0)
    {
        log_error("Generated MDL exports no material: " + m_parameters.name, SRC);
        return false;
    }

    // get the first material in the module
    mi::base::Handle<const mi::neuraylib::IFunction_definition> material_definition(
        sdk.get_transaction().access<const mi::neuraylib::IFunction_definition>(module->get_material(0)));
    m_material_name = material_definition->get_mdl_simple_name();

    // set the name in the scene graph and on the GUI
    m_name_in_scene = loader->get_scene_name_prefix() + " " + m_material_name +
        " (" + std::to_string(m_unique_id) + ")";

    m_supports_reloading = loader->supports_reload();
    m_parameter_list = nullptr;         // defaults will be used
    m_flags = IMaterial::Flags::None;   // no hard coded assumptions
    return true;
}

}}} // mi::examples::mdl_d3d12
