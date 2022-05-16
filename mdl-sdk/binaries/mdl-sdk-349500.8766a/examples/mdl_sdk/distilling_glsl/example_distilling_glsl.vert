/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/
#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec3 Tangent;
layout (location = 3) in vec3 Binormal;
layout (location = 4) in vec2 TexCoord;

out vec3 world_position;
out vec3 world_normal;
out vec3 world_tangent;
out vec3 world_binormal;
out vec3 texture_coordinate;

uniform mat4 m_model = mat4(1.0);
uniform mat4 m_view;
uniform mat4 m_projection;

void main()
{
    mat3 m = mat3(m_model); // transpose ?
    world_position = vec3(m_model * vec4(Position, 1.0));
    world_normal =   m * Normal;
    world_tangent =  m * Tangent;
    world_binormal = m * Binormal;
    texture_coordinate = vec3(TexCoord.x, TexCoord.y, 0.0);
    
    gl_Position =  m_projection * m_view * vec4(world_position, 1.0);
}
