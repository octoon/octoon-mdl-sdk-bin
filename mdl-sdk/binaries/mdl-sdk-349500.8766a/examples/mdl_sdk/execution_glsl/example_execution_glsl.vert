/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/
#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;

out vec3 vPosition;
out vec3[1] texture_coordinate;

void main()
{
    gl_Position = vec4(Position, 1.0);
    vPosition = Position;
    texture_coordinate = vec3[1](vec3(TexCoord.x, TexCoord.y, 0.0));
}
