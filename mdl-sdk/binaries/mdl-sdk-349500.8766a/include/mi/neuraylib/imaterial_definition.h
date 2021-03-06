/***************************************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 **************************************************************************************************/
/// \file
/// \brief      Scene element Material_definition

#ifndef MI_NEURAYLIB_IMATERIAL_DEFINITION_H
#define MI_NEURAYLIB_IMATERIAL_DEFINITION_H

#include <mi/neuraylib/iexpression.h>
#include <mi/neuraylib/iscene_element.h>
#include <mi/neuraylib/ifunction_definition.h>

namespace mi {

namespace neuraylib {

/** \addtogroup mi_neuray_mdl_elements
@{
*/

class IMaterial_instance;
class IMdl_execution_context;

/// This interface represents a material definition.
///
/// This interface is obsolete. See \ref mi_mdl_materials_are_functions and
/// #mi::neuraylib::IFunction_definition.
class IMaterial_definition : public
    mi::base::Interface_declare<0x73753e3d,0x62e4,0x41a7,0xa8,0xf5,0x37,0xeb,0xda,0xd9,0x01,0xd9,
                                neuraylib::IScene_element>
{
public:
    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_module().
    virtual const char* get_module() const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_mdl_name().
    virtual const char* get_mdl_name() const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_mdl_module_name().
    virtual const char* get_mdl_module_name() const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_mdl_simple_name().
    virtual const char* get_mdl_simple_name() const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_mdl_parameter_type_name().
    virtual const char* get_mdl_parameter_type_name( Size index) const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_prototype().
    virtual const char* get_prototype() const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_mdl_version().
    virtual void get_mdl_version( Mdl_version& since, Mdl_version& removed) const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_semantic().
    virtual IFunction_definition::Semantics get_semantic() const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::is_exported().
    virtual bool is_exported() const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_return_type().
    virtual const IType* get_return_type() const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_parameter_count().
    virtual Size get_parameter_count() const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_parameter_name().
    virtual const char* get_parameter_name( Size index) const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_parameter_index().
    virtual Size get_parameter_index( const char* name) const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_parameter_types().
    virtual const IType_list* get_parameter_types() const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_defaults().
    virtual const IExpression_list* get_defaults() const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_enable_if_conditions().
    virtual const IExpression_list* get_enable_if_conditions() const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_enable_if_users().
    virtual Size get_enable_if_users( Size index) const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_enable_if_user().
    virtual Size get_enable_if_user( Size index, Size u_index) const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_annotations().
    virtual const IAnnotation_block* get_annotations() const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_return_annotations().
    virtual const IAnnotation_block* get_return_annotations() const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_parameter_annotations().
    virtual const IAnnotation_list* get_parameter_annotations() const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_thumbnail().
    virtual const char* get_thumbnail() const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::is_valid().
    virtual bool is_valid( IMdl_execution_context* context) const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_body().
    virtual const IExpression_direct_call* get_body() const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_temporary_count().
    virtual Size get_temporary_count() const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_temporary().
    virtual const IExpression* get_temporary( Size index) const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_temporary_name().
    virtual const char* get_temporary_name( Size index) const = 0;

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::get_temporary_name().
    template<class T>
    const T* get_temporary( Size index) const
    {
        const IExpression* ptr_iexpression = get_temporary( index);
        if ( !ptr_iexpression)
            return 0;
        const T* ptr_T = static_cast<const T*>( ptr_iexpression->get_interface( typename T::IID()));
        ptr_iexpression->release();
        return ptr_T;
    }

    /// See \ref mi_mdl_materials_are_functions and
    /// #mi::neuraylib::IFunction_definition::create_function_call().
    virtual IMaterial_instance* create_material_instance(
        const IExpression_list* arguments, Sint32* errors = 0) const = 0;
};

/*@}*/ // end group mi_neuray_mdl_elements

} // namespace neuraylib

} // namespace mi

#endif // MI_NEURAYLIB_IMATERIAL_DEFINITION_H
