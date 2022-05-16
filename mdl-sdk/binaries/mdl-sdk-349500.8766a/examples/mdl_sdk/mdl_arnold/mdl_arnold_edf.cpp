/***************************************************************************************************
 * Copyright 2021 NVIDIA Corporation. All rights reserved.
 **************************************************************************************************/

#include "mdl_arnold_bsdf.h"
#include <mi/mdl_sdk.h>

AtClosure MdlEDFCreate(
    const AtShaderGlobals* sg,
    const MdlShaderNodeDataCPU* shader_data)
{
    // there is no emission
    if (shader_data->surface_edf_function_index == ~0 &&
        shader_data->backface_edf_function_index == ~0)
        return AiClosureEmission(sg, AI_RGB_BLACK);

    // setup the mdl state
    Mdl_extended_state ext_state;
    setup_mdl_state(sg, ext_state);

    // get function indices for front or back faces
    uint64_t edf_function_index = shader_data->surface_edf_function_index;
    uint64_t intensity_function_index = shader_data->surface_emission_intensity_function_index;
    if (ext_state.is_inside)
    {
        edf_function_index = shader_data->backface_edf_function_index;
        intensity_function_index = shader_data->backface_emission_intensity_function_index;
    }
    if (edf_function_index == ~0)
        return AiClosureEmission(sg, AI_RGB_BLACK);

    // here, potentially redundant texture accesses in sample/eval are pre-fetched (text_results).
    // even if sample is not called here, it is required since `eval` expects the data to be there.
    shader_data->target_code->execute_bsdf_init(
        edf_function_index + 0,  // edf_function_index corresponds to 'init'
            reinterpret_cast<mi::neuraylib::Shading_state_material&>(ext_state.state),
            /*texture_handler*/ nullptr,    // allows to provide a custom texturing runtime
            /*arg_block_data=*/ nullptr);   // only relevant when using class-compilation mode


    // input/output data for evaluate
    mi::neuraylib::Edf_evaluate_data<mi::neuraylib::DF_HSM_NONE> eval_data;
    eval_data.k1 = ext_state.outgoing;  // outgoing direction

    // evaluate the EDF
    shader_data->target_code->execute_edf_evaluate(
        edf_function_index + 2,  // edf_function_index corresponds to 'init'
                                              // edf_function_index+2 to 'evaluate'
        &eval_data,
        reinterpret_cast<mi::neuraylib::Shading_state_material&>(ext_state.state),
        /*texture_handler=*/ nullptr,
        /*arg_block_data=*/ nullptr);

    // evaluate intensity
    if (intensity_function_index != ~0)
    {
        float3 intensity{ 1.0f, 1.0f, 1.0f };
        shader_data->target_code->execute(
            intensity_function_index,
            reinterpret_cast<mi::neuraylib::Shading_state_material&>(ext_state.state),
            /*texture_handler=*/ nullptr,
            /*arg_block_data=*/ nullptr,
            &intensity);

        // apply intensity
        eval_data.edf.x *= intensity.x;
        eval_data.edf.y *= intensity.y;
        eval_data.edf.z *= intensity.z;
    }

    return AiClosureEmission(sg, { eval_data.edf.x, eval_data.edf.y, eval_data.edf.z });
}
