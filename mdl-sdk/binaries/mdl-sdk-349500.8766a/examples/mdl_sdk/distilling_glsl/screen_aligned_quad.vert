/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/
#version 330 core

layout (location = 0) in vec3 Position;

out vec3 world_position;

uniform mat4 inv_mv;
uniform mat4 inv_proj;

void main()
{
    gl_Position = vec4(Position, 1.0);
    
    mat4 m = inv_mv;
    m[3][0] = 0.0;
    m[3][1] = 0.0;
    m[3][2] = 0.0;
    
    world_position = vec3(m * inv_proj * gl_Position).xyz;
}
