/***************************************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 **************************************************************************************************/

#ifndef MDL_ARNOLD_INTERFACE_H
#define MDL_ARNOLD_INTERFACE_H

#include <mutex>

#include <ai.h>
#include <mi/base/handle.h>
#include <mi/mdl_sdk.h>

namespace mi
{
    namespace neuraylib
    {
        class INeuray;
        class IMdl_compiler;
        class ITransaction;
    }
}

enum class EMdl_sdk_state
{
    undefined = 0,
    loaded = 1,
    error_libmdl_not_found = -1,
    error_freeimage_not_found = -2,
    error_dds_not_found = -3,
    error_default_module_invalid = -4,
};

class Mdl_sdk_interface
{
public:
    explicit Mdl_sdk_interface();
    virtual ~Mdl_sdk_interface();

    EMdl_sdk_state get_sdk_state() const { return m_state; }

    mi::neuraylib::ITransaction& get_transaction() const { return *m_transaction.get(); }
    mi::neuraylib::IMdl_configuration& get_config() const { return *m_mdl_config.get(); }
    mi::neuraylib::IMdl_impexp_api& get_impexp_api() { return *m_mdl_impexp_api.get(); }


    mi::neuraylib::IMdl_factory& get_factory() { return *m_factory.get(); }
    mi::neuraylib::IType_factory& get_type_factory() { return *m_tf.get(); }
    mi::neuraylib::IValue_factory& get_value_factory() { return *m_vf.get(); }
    mi::neuraylib::IExpression_factory& get_expr_factory() { return *m_ef.get(); }

    mi::neuraylib::IMdl_backend* create_native_backend();
    mi::neuraylib::IMdl_execution_context* create_context();

    bool log_messages(const mi::neuraylib::IMdl_execution_context* context);

    const AtString& get_default_mdl_module_name() const {  return m_default_mdl_module_name; }
    const AtString& get_default_mdl_function_name() const { return m_default_mdl_function_name; }
    const std::string& get_default_material_db_name() const { return m_default_material_db_name; }

    void set_search_paths();

    // The mutex should be used if data is written to the database
    // and to avoid duplicate loading of modules and textures.
    std::mutex& get_loading_mutex() { return m_loading_mutex; }

private:
    void* m_so_handle;
    EMdl_sdk_state m_state;
    AtString m_default_mdl_module_name;
    AtString m_default_mdl_function_name;
    std::string m_default_material_db_name;
    std::mutex m_loading_mutex;

    mi::base::Handle<mi::neuraylib::INeuray> m_mdl_sdk;
    mi::base::Handle<mi::neuraylib::IMdl_configuration> m_mdl_config;
    mi::base::Handle<mi::neuraylib::IMdl_backend> m_native_backend;
    mi::base::Handle<mi::neuraylib::IMdl_impexp_api> m_mdl_impexp_api;
    mi::base::Handle<mi::neuraylib::ITransaction> m_transaction;

    mi::base::Handle<mi::neuraylib::IMdl_factory> m_factory;
    mi::base::Handle<mi::neuraylib::IType_factory> m_tf;
    mi::base::Handle<mi::neuraylib::IValue_factory> m_vf;
    mi::base::Handle<mi::neuraylib::IExpression_factory> m_ef;

};
#endif
