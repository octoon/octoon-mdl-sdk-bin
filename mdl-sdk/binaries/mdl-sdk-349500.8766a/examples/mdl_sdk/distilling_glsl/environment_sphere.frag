/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/
#version 330 core

// varying inputs
in vec3 world_position;

// uniforms
uniform sampler2D env_tex;
uniform float exposure_scale = 1.0f;

// outputs
out vec4 FragColor;

const float ONE_OVER_PI = 0.3183099;

vec2 get_spherical_uv(vec3 v)
{
    float gamma = asin(v.y);
    float theta = atan(v.z, v.x);

    return vec2(theta * ONE_OVER_PI * 0.5, gamma * ONE_OVER_PI) + 0.5;
}

// A simple Reinhard tonemapper.
vec3 display(vec3 val, float tonemap_scale)
{
    val *= tonemap_scale;
    float burn_out = 0.1;
    val.x *= (1.0 + val.x * burn_out) / (1.0 + val.x);
    val.y *= (1.0 + val.y * burn_out) / (1.0 + val.y);
    val.z *= (1.0 + val.z * burn_out) / (1.0 + val.z);
    
    float gamma = 1.0/2.2;
    float r = min(pow(max(val.x, 0.0), gamma), 1.0);
    float g = min(pow(max(val.y, 0.0), gamma), 1.0);
    float b = min(pow(max(val.z, 0.0), gamma), 1.0);
    
    return vec3(r, g, b);
}


void main() {

    vec2 uv = get_spherical_uv(normalize(world_position));
    vec3 color = texture(env_tex, uv).rgb;
    FragColor = vec4(display(color, exposure_scale), 1.0);
}
