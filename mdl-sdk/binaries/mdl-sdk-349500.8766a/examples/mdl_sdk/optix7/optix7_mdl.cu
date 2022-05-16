/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

#include <optix.h>

#include "optix7_mdl.h"

#include <mi/neuraylib/target_code_types.h>


static __forceinline__ __device__ void traceRadiance(
    OptixTraversableHandle handle,
    float3                 ray_origin,
    float3                 ray_direction,
    float                  tmin,
    float                  tmax,
    RadiancePRD*           prd,
    RayFlags&              depth
#ifdef CONTRIB_IN_PAYLOAD
    , float3&              contrib
#endif
)
{
    uint32_t u0, u1;
    pack_pointer(prd, u0, u1);
    optixTrace(
        handle,
        ray_origin,
        ray_direction,
        tmin,
        tmax,
        0.0f,                     // rayTime
        OptixVisibilityMask(255),
        OPTIX_RAY_FLAG_NONE,
        RAY_TYPE_RADIANCE,        // SBT offset
        RAY_TYPE_COUNT,           // SBT stride
        RAY_TYPE_RADIANCE,        // missSBTIndex
        u0, u1,
        reinterpret_cast<uint32_t&>(depth)
#ifdef CONTRIB_IN_PAYLOAD
        , reinterpret_cast<uint32_t&>(contrib.x),
        reinterpret_cast<uint32_t&>(contrib.y),
        reinterpret_cast<uint32_t&>(contrib.z)
#endif
    );
}


// simple Reinhard tonemapper + gamma + quantize
__forceinline__ __device__ uchar4 make_color(float3 const &c)
{
    const float burn_out = 0.1f;
    const float gamma = 2.2f;

    float3 val;
    val.x = c.x * (1.0f + c.x * burn_out) / (1.0f + c.x);
    val.y = c.y * (1.0f + c.y * burn_out) / (1.0f + c.y);
    val.z = c.z * (1.0f + c.z * burn_out) / (1.0f + c.z);

    return make_uchar4(
        static_cast<uint8_t>(powf(saturate(val.x), 1.0 / gamma)*255.0f),
        static_cast<uint8_t>(powf(saturate(val.y), 1.0 / gamma)*255.0f),
        static_cast<uint8_t>(powf(saturate(val.z), 1.0 / gamma)*255.0f),
        255u
    );
}


//------------------------------------------------------------------------------
//
// Ray generation function
//
//------------------------------------------------------------------------------

extern "C" __global__ void __raygen__rg()
{
    const int    w   = params.width;
    const int    h   = params.height;
    const float3 eye = params.eye;
    const float3 U   = params.U;
    const float3 V   = params.V;
    const float3 W   = params.W;
    const uint3  idx = optixGetLaunchIndex();
    const int    subframe_index = params.subframe_index;

    uint32_t seed = tea<4>(idx.y*w + idx.x, subframe_index);

    float3 result = make_float3(0.0f);
    int i = params.samples_per_launch;

    do
    {
        const float2 subpixel_jitter = make_float2(rnd(seed), rnd(seed));

        const float2 d = 2.0f * make_float2(
                (static_cast<float>(idx.x) + subpixel_jitter.x) / static_cast<float>(w),
                (static_cast<float>(idx.y) + subpixel_jitter.y) / static_cast<float>(h)
            ) - 1.0f;
        float3 ray_direction = normalize(d.x*U + d.y*V + W);
        float3 ray_origin    = eye;

        RadiancePRD prd;
#ifndef CONTRIB_IN_PAYLOAD
        prd.contribution = make_float3(0.f);
#endif
        prd.weight       = make_float3(1.f);
        prd.seed         = seed;
        prd.last_pdf     = -1.0f;

        RayFlags ray_flags = RAY_FLAGS_NONE;
        for (;; )
        {
            traceRadiance(
                params.handle,
                ray_origin,
                ray_direction,
                0.01f,  // tmin       // TODO: smarter offset
                1e16f,  // tmax
                &prd,
                ray_flags
#ifdef CONTRIB_IN_PAYLOAD
                , result
#endif
            );

            if ((ray_flags & RAY_FLAGS_DEPTH_MASK) >= MAX_DEPTH)
                break;

            ray_origin = prd.origin;
            ray_direction = prd.direction;

            ray_flags = RayFlags(int(ray_flags) + 1);
        }

#ifndef CONTRIB_IN_PAYLOAD
        result += prd.contribution;
#endif
    } while (--i > 0);

    const uint3    launch_index = optixGetLaunchIndex();
    const uint32_t image_index  = launch_index.y * params.width + launch_index.x;
    float3         accum_color  = result / static_cast<float>(params.samples_per_launch);

    if (subframe_index > 0)
    {
        const float                 a = 1.0f / static_cast<float>(subframe_index + 1);
        const float3 accum_color_prev = params.accum_buffer[image_index];
        accum_color = lerp(accum_color_prev, accum_color, a);
    }
    params.accum_buffer[image_index] = accum_color;
    if (params.frame_buffer)
        params.frame_buffer[image_index] = make_color(accum_color);
}


//------------------------------------------------------------------------------
//
// Miss function of radiance ray
//
//------------------------------------------------------------------------------

extern "C" __global__ void __miss__radiance()
{
    RadiancePRD* prd = get_radiance_prd();

    const float3 ray_dir = optixGetWorldRayDirection();

    float pdf;
    float3 radiance = environment_eval(pdf, ray_dir);

    // to incorporate the point light selection probability
    if (params.light.emission.x > 0.0f || params.light.emission.y > 0.0f || params.light.emission.z > 0.0f)
        pdf *= 0.5f;

    if (prd->last_pdf > 0.0f)
    {
        float mis_weight = prd->last_pdf / (prd->last_pdf + pdf);
        radiance *= mis_weight;
    }

#ifdef CONTRIB_IN_PAYLOAD
    set_radiance_payload_contrib(get_radiance_payload_contrib() + radiance * prd->weight);
#else
    prd->contribution += radiance * prd->weight;
#endif

    set_radiance_payload_depth(MAX_DEPTH);
}


//------------------------------------------------------------------------------
//
// Closest-hit function of occlusion ray
//
//------------------------------------------------------------------------------

extern "C" __global__ void __closesthit__occlusion()
{
    set_occlusion_payload(true);
}
