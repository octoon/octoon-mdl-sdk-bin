/***************************************************************************************************
 * Copyright 2021 NVIDIA Corporation. All rights reserved.
 **************************************************************************************************/

#include "mdl_arnold_bsdf.h"
#include <mi/mdl_sdk.h>

float MdlOpacityCreate(const AtShaderGlobals* sg, const MdlShaderNodeDataCPU* shader_data)
{
    // there is no cutout opacity function, use the constant
    if (shader_data->cutout_opacity_function_index == ~0)
        return shader_data->cutout_opacity_constant;

    // setup the mdl state
    Mdl_extended_state ext_state;
    setup_mdl_state(sg, ext_state);

    // call the generated code
    float opacity = 1.0f;
    shader_data->target_code->execute(
        shader_data->cutout_opacity_function_index,
        reinterpret_cast<mi::neuraylib::Shading_state_material&>(ext_state.state),
        /*texture_handler=*/ nullptr,
        /*arg_block_data=*/ nullptr,
        &opacity);

    return opacity;
}
